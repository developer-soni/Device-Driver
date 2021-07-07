/**************************************************************
* Project: Assignment 6 - Device Driver
*
* File: kernelMod.c
*
* Description: The Linux Kernel Module connects with the 
* testProgram to return nth number in Fibonacci Series.
*
* Template followed from: 
* http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/
**************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/ioctl.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include "kernelMod.h"

#define DEVICE_NAME "kernelmod"
#define CLASS_NAME "kmod"
#define WR_VALUE _IOW('a', 'a', int)
#define RD_VALUE _IOR('a', 'b', int)

MODULE_LICENSE("GPL");

static int major;
static char msg[256] = {0};
static short sizeofmsg;
static int value = 0;
static int fibb = 0;
static struct class *kernelClass = NULL;
static struct device *kernelDevice = NULL;

static struct file_operations fops =
    {
        .open = dev_open,
        .read = dev_read,
        .write = dev_write,
        .unlocked_ioctl = dev_ioctl,
        .release = dev_release,
};

// Initializing kernel mod
static int __init kernelmod_init(void)
{
   //registering class
   major = register_chrdev(0, DEVICE_NAME, &fops);
   if (major < 0)
      return major;

   kernelClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(kernelClass))
   {
      unregister_chrdev(major, DEVICE_NAME);
      return PTR_ERR(kernelClass);
   }

   kernelDevice = device_create(kernelClass, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
   if (IS_ERR(kernelDevice))
   {
      class_destroy(kernelClass);
      unregister_chrdev(major, DEVICE_NAME);
      return PTR_ERR(kernelDevice);
   }
   return 0;
}

// Exit class, destroy class, unregister class
static void __exit kernelmod_exit(void)
{
   device_destroy(kernelClass, MKDEV(major, 0));
   class_unregister(kernelClass);
   class_destroy(kernelClass);
   unregister_chrdev(major, DEVICE_NAME);
}

//printing to kernel log
static int dev_open(struct inode *inodep, struct file *filep)
{
   return 0;
}

//sending message to user buffer
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
   int error_count = 0;
   error_count = copy_to_user(buffer, msg, sizeofmsg);
   if (error_count == 0)
      return (sizeofmsg = 0);
   else
      return -EFAULT;
}

//receiving message from user buffer
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
   sprintf(msg, "%s", buffer);
   sizeofmsg = strlen(msg);
   return len;
}

//write receives from user, read sends to user
static long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
   switch (cmd)
   {
   case WR_VALUE:
      copy_from_user(&value, (int32_t *)arg, sizeof(value));
      break;
   case RD_VALUE:
      fibb = dev_fib(value);
      copy_to_user((int32_t *)arg, &fibb, sizeof(fibb));
      break;
   }
   return 0;
}

//calculating nth Fibonacci number
static int dev_fib(int n)
{
   if (n <= 1)
      return n;
   return dev_fib(n - 1) + dev_fib(n - 2);
}

//printing to kernel log
static int dev_release(struct inode *inodep, struct file *filep)
{
   return 0;
}

//cleanup
module_init(kernelmod_init);
module_exit(kernelmod_exit);