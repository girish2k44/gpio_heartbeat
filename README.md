# gpio_heartbeat
A simple Linux kernel platform driver to send heartbeat signal on GPIO pin

## Working
This driver provides a continuous heartbeat signal on a GPIO pin.
Used as optical signal that the board/HW is working, by placing an LED on GPIO pin.

## Integration
This driver is developed as generic driver.
Can be include in existing Kernel source tree or the corresponding headers.

```
make
```

This builds the module for the currently booted (uname -r) Kernel. To install
run `make install`.

## Device tree support

The module can be used as a platform driver with automated early loading using
a device tree entry like the following.

```
    gpio-heartbeat {
        compatible = "gpio-heartbeat";
        status = "okay";
    };
```

To build with sepearte kernel source consider below changes
Add in your defconfig
```
	CONFIG_LEDS_GPIO_HEARTBEAT=y
```
Add in your Makefile
```
	obj-$(CONFIG_LEDS_GPIO_HEARTBEAT)      += gpio-heartbeat.o
```

Add in your Kconfig file
```
	config LEDS_GPIO_HEARTBEAT
	tristate "LED GPIO Heartbeat"
	help
	 This allows GPIO to send hearbeat signal.
	 If unsure, say Y.
```
