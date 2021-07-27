#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/cred.h>

#define DEVICE_NAME "overflow"
#define CLASS_NAME "overflowme"
#define MAXSIZE 0x100

int major_num;
struct class *module_class = NULL;
struct device *module_device = NULL;
void *buffer = NULL;
spinlock_t spin;
long long offset;

int __init kernel_module_init(void);
void __exit kernel_module_exit(void);
int module_open(struct inode *, struct file *);
ssize_t module_read(struct file *, char __user *, size_t, loff_t *);
ssize_t __internal_module_read(struct file *, char __user *, size_t, loff_t *);
ssize_t module_write(struct file *, const char __user *, size_t, loff_t *);
int module_release(struct inode *, struct file *);
long module_ioctl(struct file *, unsigned int, unsigned long);
long __internal_module_ioctl(struct file *__file, unsigned int cmd, unsigned long param);
int backdoor(void);

struct file_operations module_fo =
    {
        .owner = THIS_MODULE,
        .unlocked_ioctl = module_ioctl,
        .open = module_open,
        .read = module_read,
        .write = module_write,
        .release = module_release,
};

module_init(kernel_module_init);
module_exit(kernel_module_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("mrh929");


// int backdoor(void){
//     commit_creds(prepare_kernel_cred(0));
// }

int __init kernel_module_init(void)
{
    spin_lock_init(&spin);
    printk(KERN_INFO "[KernelModule:] Module loaded. Start to register device...\n");
    major_num = register_chrdev(0, DEVICE_NAME, &module_fo);
    if (major_num < 0)
    {
        printk(KERN_INFO "[KernelModule:] Failed to register a major number.\n");
        return major_num;
    }
    printk(KERN_INFO "[KernelModule:] Register complete, major number: %d\n", major_num);
    module_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(module_class))
    {
        unregister_chrdev(major_num, DEVICE_NAME);
        printk(KERN_INFO "[KernelModule:] Failed to register class device!\n");
        return PTR_ERR(module_class);
    }
    printk(KERN_INFO "[KernelModule:] Class device register complete.\n");
    module_device = device_create(module_class, NULL, MKDEV(major_num, 0), NULL, DEVICE_NAME);
    if (IS_ERR(module_device))
    {
        class_destroy(module_class);
        unregister_chrdev(major_num, DEVICE_NAME);
        printk(KERN_INFO "[KernelModule:] Failed to create the device!\n");
        return PTR_ERR(module_device);
    }
    printk(KERN_INFO "[KernelModule:] Module register complete.\n");
    return 0;
}

void __exit kernel_module_exit(void)
{
    printk(KERN_INFO "[KernelModule:] Start to clean up the module.\n");
    device_destroy(module_class, MKDEV(major_num, 0));
    class_destroy(module_class);
    unregister_chrdev(major_num, DEVICE_NAME);
    printk(KERN_INFO "[KernelModule:] Module clean up complete.\n");
}

long module_ioctl(struct file *__file, unsigned int cmd, unsigned long param)
{
    long ret;
    spin_lock(&spin);
    ret = __internal_module_ioctl(__file, cmd, param);
    spin_unlock(&spin);
    return ret;
}

long __internal_module_ioctl(struct file *__file, unsigned int cmd, unsigned long param)
{
    char message[0x30];
    printk(KERN_INFO "[KernelModule:] Received operation code: %d\n", cmd);
    switch(cmd){
        case 0x114514:
            printk(KERN_INFO "[KernelModule:] Change offset to %d", param);
            offset = param;
            break;
        case 0x1919810:
            memcpy(message, buffer, param);
            printk(KERN_INFO "[KernelModule:] Received message: %s", message);
            break;
        default:
            printk(KERN_INFO "[KernelModule:] Unknown operation code. \n");
            break;
    }

    return 0;
}

int module_open(struct inode *__inode, struct file *__file)
{
    spin_lock(&spin);

    if (buffer == NULL)
    {
        buffer = kmalloc(MAXSIZE, GFP_ATOMIC);
        if (buffer == NULL)
        {
            printk(KERN_INFO "[KernelModule:] Unable to initialize the buffer. Kernel malloc error.\n");
            return -1;
        }
        memset(buffer, 0, MAXSIZE);
        printk(KERN_INFO "[KernelModule:] Device open, buffer initialized successfully.\n");
    }
    else
    {
        printk(KERN_INFO "[KernelModule:] Warning: reopen the device may cause unexpected error in kernel.\n");
    }
    spin_unlock(&spin);
    return 0;
}

int module_release(struct inode *__inode, struct file *__file)
{
    spin_lock(&spin);
    if (buffer)
    {
        kfree(buffer);
        buffer = NULL;
    }
    printk(KERN_INFO "[KernelModule:] Device closed.\n");
    spin_unlock(&spin);
    return 0;
}


ssize_t __internal_module_read(struct file *__file, char __user *user_buf, size_t size, loff_t *__loff){
    const char *const buf = (char *)buffer;
    char banner[0x30];
    strcpy(banner, "welcome to ctf challenge!");
    copy_to_user(user_buf, &banner[offset], 1);
}

ssize_t module_read(struct file *__file, char __user *user_buf, size_t size, loff_t *__loff)
{
    spin_lock(&spin);
    __internal_module_read(__file, user_buf, size, __loff);
    spin_unlock(&spin);
    return 0;
}

ssize_t module_write(struct file *__file, const char __user *user_buf, size_t size, loff_t *__loff)
{
    char *const buf = (char *)buffer;
    spin_lock(&spin);
    copy_from_user(buf, user_buf, size > MAXSIZE ? MAXSIZE : size);
    spin_unlock(&spin);
    return 0;
}