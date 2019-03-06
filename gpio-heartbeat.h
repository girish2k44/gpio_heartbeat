
#define DRIVER_NAME "gpio_heartbeat"
#define PDEBUG(fmt,args...) printk(KERN_DEBUG"%s:"fmt,DRIVER_NAME, ##args)
#define PERR(fmt,args...) printk(KERN_ERR"%s:"fmt,DRIVER_NAME,##args)
#define PINFO(fmt,args...) printk(KERN_INFO"%s:"fmt,DRIVER_NAME, ##args)
#include<linux/debugfs.h>
#include<linux/init.h>
#include<linux/module.h>
//#include<linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/gpio.h>				// GPIO
#include <linux/kthread.h>			// threads
#include <linux/delay.h>			// msleep()
#include <linux/platform_device.h>	// platform driver
