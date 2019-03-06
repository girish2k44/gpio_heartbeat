/*
 * Copyright (C) 2019 Girish Sharma <girish.sharma@einfochips.com, girish2k44@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 */

/*
===============================================================================
Driver Name		:		gpio_heartbeat
Author			:		GIRISH SHARMA
License			:		GPL
Description		:		Sends a heartbeat signal to GPIO pin
===============================================================================
*/

#include"gpio-heartbeat.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GIRISH SHARMA");
MODULE_PARM_DESC(gpioPIN, " GPIO PIN number (default=50)");
MODULE_DESCRIPTION("Sends a heartbeat signal to GPIO pin");

static uint gpioPIN = 50;
static struct task_struct *task;
static bool gpioState = false;

#if defined(CONFIG_OF)
static struct of_device_id platform_device_match_table[] = {
	{ .compatible = "gpio-heartbeat" },
	{}
};
MODULE_DEVICE_TABLE(of, platform_device_match_table);
#endif

static int heartbeat(void *arg)
{
	int count = 0;
	int timeout = 0;
	PINFO("GPIO Heartbeat: thread started\n");

	while (!kthread_should_stop()) {
		set_current_state(TASK_RUNNING);
		count++;
		gpioState = !gpioState;
		gpio_set_value(gpioPIN, gpioState);
		if (count % 4 == 0) {
			timeout = 1250;
			count = 0;
		} else {
			timeout = 120;
		}
		set_current_state(TASK_INTERRUPTIBLE);
		msleep(timeout);
	}

	PINFO("GPIO Heartbeat: thread complete\n");
	return 0;
}

static int  gpio_heartbeat_init(void)
{
	int res = 0;
	gpio_request(gpioPIN, "sysfs");
	gpio_direction_output(gpioPIN, false); // Turn off by default.
	gpio_export(gpioPIN, false); // Export, but prevent direction change.
	task = kthread_run(heartbeat, NULL, "GPIO_heartbeat_thread");
	if (IS_ERR(task)) {
		PINFO("GPIO Heartbeat: failed to create task\n");
		return PTR_ERR(task);
		}
	PINFO("GPIO Heartbeat: init\n");

	return res;
}

static void gpio_heartbeat_exit(void)
{
	kthread_stop(task);
	gpio_set_value(gpioPIN, 0);
	gpio_unexport(gpioPIN);
	gpio_free(gpioPIN);
	PINFO("GPIO Heartbeat: exit\n");
}

static int platform_device_probe(struct platform_device *pdev)
{
	return gpio_heartbeat_init();
}

static int platform_device_remove(struct platform_device *pdev)
{
	gpio_heartbeat_exit();
	return 0;
}

static struct platform_driver platform_device_platform_driver = {
	.probe = platform_device_probe,
	.remove = platform_device_remove,
	.driver = {
		.name = "gpio-heartbeat",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(platform_device_match_table),
	},
};

module_platform_driver(platform_device_platform_driver);
module_param(gpioPIN, uint, S_IRUGO);
