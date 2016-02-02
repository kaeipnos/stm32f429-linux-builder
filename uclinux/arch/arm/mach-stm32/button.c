#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <mach/stm32.h>
#include <mach/platform.h>
#include <mach/exti.h>
#include <mach/button.h>

#define STM32429_DISCO_BUTTON_INT	6
#define STM32429_DISCO_BUTTON_PORT	0
#define STM32429_DISCO_BUTTON_PIN	0
#define STM32429_DISCO_BUTTON_EXTILINE	0

#if defined(CONFIG_STM32F429IDISCO_BLUEBUTTON)
static irqreturn_t stm32_bouton_isr(int irq, void *dev_id)
{
	printk("INFO: irq on blue button !\n");
	
	/* clear irq */
	stm32_exti_clear_pending(STM32429_DISCO_BUTTON_EXTILINE);

	return IRQ_HANDLED;
}

void __init stm32_button_init(void)
{
	int rv;
	/* Enable PA0 event line in the event controller (EXTI0) */
	printk("INFO: enabling interrupt on PA0 (%d/%d)\n",
			STM32429_DISCO_BUTTON_EXTILINE,
			STM32F2_EXTI_NUM_LINES);

	/* EXTI[3:0]=0000 -> PA selected for pin 0 */
	stm32_exti_enable_int(STM32429_DISCO_BUTTON_EXTILINE, STM32_EXTI_RISING);
	rv = request_irq(STM32429_DISCO_BUTTON_INT,
			stm32_bouton_isr,
			IRQF_TRIGGER_RISING,
			"STM32 TEXI0/PA0 Button",
			(void *)stm32_bouton_isr);
	if(rv) {
		printk("INFO: %s: request_irq(%d) failed (%d)\n",
				__func__, STM32429_DISCO_BUTTON_INT, rv);
	} else {
		printk("INFO: BOUTON interrupt handler installed \n");
	}
}
#endif
