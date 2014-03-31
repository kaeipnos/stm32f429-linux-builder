/*
 * (C) Copyright 2014
 * Denis Bodor <lefinnois@lefinnois.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <mach/leds.h>
#include <mach/platform.h>

#include <linux/leds.h>
#include <linux/platform_device.h>


static struct gpio_led stm32f429idisco_led_pins[] = {
	{
		.name               = "LD3",
		.gpio               = 109,
		.default_trigger    = "none",
	},
	{
		.name               = "LD4",
		.gpio               = 110,
		.default_trigger    = "heartbeat",
	},
};

static struct gpio_led_platform_data stm32f429idisco_led_data = {
	.num_leds               = ARRAY_SIZE(stm32f429idisco_led_pins),
	.leds                   = stm32f429idisco_led_pins,
};

static struct platform_device stm32f429idisco_leds = {
	.name                   = "leds-gpio",
	.id                     = -1,
	.dev.platform_data      = &stm32f429idisco_led_data,
};

static struct platform_device *stm32f429idisco_devices[] __initdata = {
	&stm32f429idisco_leds,
};

void __init stm32_led_init(void)
{
	int platform;
	platform = stm32_platform_get();
	if(platform == PLATFORM_STM32_STM32429_DISCO)
		platform_add_devices(stm32f429idisco_devices, ARRAY_SIZE(stm32f429idisco_devices));
}






