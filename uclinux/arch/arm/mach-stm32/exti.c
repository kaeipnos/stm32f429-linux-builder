/*
 * (C) Copyright 2012-2014
 * Emcraft Systems, <www.emcraft.com>
 * Alexander Potashev <aspotashev@emcraft.com>
 * Denis Bodor <lefinnois@lefinnois.net>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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
#include <linux/module.h>
#include <mach/exti.h>

/*
 * EXTI register map base
 */
struct stm32_exti_regs {
	u32 imr;	/* Interrupt mask register */
	u32 emr;	/* Event mask register */
	u32 rtsr;	/* Rising trigger selection register */
	u32 ftsr;	/* Falling trigger selection register */
	u32 swier;	/* Software interrupt event register */
	u32 pr;		/* Pending register */
};
#define STM32_EXTI_BASE	0x40013c00
#define STM32_EXTI	((volatile struct stm32_exti_regs *) \
		STM32_EXTI_BASE)

/*
 * SYSCFG register map base
 */
struct stm32_syscfg_reg {
	u32 memrm;
	u32 pmc;
	u32 exticr1;
	u32 exticr2;
	u32 exticr3;
	u32 exticr4;
	u32 cmpcr;
};
#define SYSCFG_BASE	0x40013800
#define SYSCFG		((volatile struct stm32_syscfg_reg *) \
		SYSCFG_BASE)


/*
 * exti0-15 == pin 0-15
 * bits = port A-K
 * SYSCFG_EXTICR1 [----reserved----][exti 3][exti 2][exti 1][exti 0]
 * SYSCFG_EXTICR2 [----reserved----][exti 7][exti 6][exti 5][exti 4]
 * SYSCFG_EXTICR3 [----reserved----][exti11][exti10][exti 9][exti 8]
 * SYSCFG_EXTICR4 [----reserved----][exti15][exti14][exti13][exti12]
 */

int stm32_exti_set_gpio(unsigned int port, unsigned int pin) {
	if (port >= STM32_F429_EXTI_NUM_PORT)
		return -EINVAL;

	if (pin >= STM32_F429_EXTI_NUM_PIN)
		return -EINVAL;

	/*
	 * Compute register
	 * change masqued bits
	 * reg ^= (a ^ b) & mask;
	 */
	if(pin > 11) {
		// r = a ^ ((a ^ b) & mask);
		SYSCFG->exticr4 ^= (SYSCFG->exticr4 ^ (port << 4 * (pin - (11 + 1))))
			& (STM32_SYSCFG_EXTI_MASK << 4 * (pin - (11 + 1)));
		return 0;
	}
	if(pin > 7) {
		SYSCFG->exticr3 ^= (SYSCFG->exticr3 ^ (port << 4 * (pin- (7 + 1))))
			& (STM32_SYSCFG_EXTI_MASK << 4 * (pin - (7 + 1)));
		return 0;
	}
	if(pin > 3) {
		SYSCFG->exticr2 ^= (SYSCFG->exticr2 ^ (port << 4 * (pin - (3 + 1))))
			& (STM32_SYSCFG_EXTI_MASK << 4 * (pin - (3 + 1)));
		return 0;
	}

	SYSCFG->exticr1 ^= (SYSCFG->exticr1 ^ (port << 4 * pin))
		& (STM32_SYSCFG_EXTI_MASK << 4 * pin);

	return 0;
}

/*
 * Enable interrupt on the rising edge of a event line
 */
int stm32_exti_enable_int(unsigned int line, int edge)
{
	if (line >= STM32F2_EXTI_NUM_LINES)
		return -EINVAL;

	stm32_exti_clear_pending(line);

	if (edge & STM32_EXTI_RISING)
		/* Enable trigger on rising edge */
		STM32_EXTI->rtsr |= (1 << line);

	if (edge & STM32_EXTI_FALLING)
		/* Enable trigger on falling edge */
		STM32_EXTI->ftsr |= (1 << line);

	/* Enable interrupt for the event */
	STM32_EXTI->imr |= (1 << line);

	return 0;
}
EXPORT_SYMBOL(stm32_exti_enable_int);

/*
 * Disable interrupt on the rising edge of a event line
 */
int stm32_exti_disable_int(unsigned int line)
{
	if (line >=  STM32F2_EXTI_NUM_LINES)
		return -EINVAL;

	/* Disable interrupt for the event */
	STM32_EXTI->imr &= ~(1 << line);
	/* Disable trigger on rising edge */
	STM32_EXTI->rtsr &= ~(1 << line);
	/* Disable trigger on falling edge */
	STM32_EXTI->ftsr &= ~(1 << line);

	stm32_exti_clear_pending(line);

	return 0;
}
EXPORT_SYMBOL(stm32_exti_disable_int);

/*
 * Clear the pending state of a given event
 */
int stm32_exti_clear_pending(unsigned int line)
{
	if (line >=  STM32F2_EXTI_NUM_LINES)
		return -EINVAL;

	STM32_EXTI->pr = (1 << line);

	return 0;
}
EXPORT_SYMBOL(stm32_exti_clear_pending);
