#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <mach/stm32.h>
#include <mach/platform.h>
#include <mach/exti.h>
#include <mach/button.h>

#define STM32429_DISCO_BUTTON_INT 6
#define STM32429_DISCO_BUTTON_PORT 0
#define STM32429_DISCO_BUTTON_PIN 0

static irqreturn_t stm32_bouton_isr(int irq, void *dev_id) {
  printk("INFO: irq on blue button !\n");

  /* clear irq */
  stm32_exti_clear_pending(STM32_GPIO_PORTPIN2NUM(
        STM32429_DISCO_BUTTON_PORT,STM32429_DISCO_BUTTON_PIN));

  return IRQ_HANDLED;
}

void __init stm32_button_init(void) {
  int rv;
   /* Enable PA0 event line in the event controller (EXTI0) */
  printk("INFO: enabling interrupt on PA0 (%d/%d)\n",
      STM32_GPIO_PORTPIN2NUM(STM32429_DISCO_BUTTON_PORT,STM32429_DISCO_BUTTON_PIN), 
      STM32F2_EXTI_NUM_LINES);
  
  stm32_exti_enable_int(STM32_GPIO_PORTPIN2NUM(STM32429_DISCO_BUTTON_PORT,
        STM32429_DISCO_BUTTON_PIN),1);
  rv = request_irq(STM32429_DISCO_BUTTON_INT,
      stm32_bouton_isr,
      IRQF_TRIGGER_FALLING,
      "STM32 TEXI0/PA0 Button",
      (void *)stm32_bouton_isr);
  if(rv) {
    printk("INFO: %s: request_irq(%d) failed (%d)\n",
        __func__, STM32429_DISCO_BUTTON_INT, rv);
  } else {
    printk("INFO: BOUTON interrupt handler installed \n");
  }
}
