/**************************************************************
*
* File: kernelMod.h
*
* Description: Header file for kernelMod.c
**************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/ioctl.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);
static long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static int dev_fib(int n);