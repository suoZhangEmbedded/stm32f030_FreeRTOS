#include <stm32f0xx.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "SEGGER_RTT.h"

void v_led_task( void *pvParameters );

int main()
{

	xTaskCreate( v_led_task, "led", configMINIMAL_STACK_SIZE, NULL, 3, NULL );	
	
	/* Start the kernel.  From here on, only tasks and interrupts will run. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then there
	was	insufficient FreeRTOS heap memory available for the idle and/or timer
	tasks to be created.  See the memory management section on the FreeRTOS web
	site, or the FreeRTOS tutorial books for more details. */
	for( ;; );
}

void bsp_led_toggle( void )
{
		GPIOA->ODR ^= GPIO_Pin_15;
}

extern uint32_t SystemCoreClock;

void v_led_task( void *pvParameters )
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_15);
	
	while(1)
	{
		
		bsp_led_toggle();
		
		SystemCoreClockUpdate();
		
		SEGGER_RTT_printf( 0, "SystemCoreClock:%u.\r\n", SystemCoreClock );
		
		SEGGER_RTT_printf( 0, "system heap:%u, tick:%u.\r\n", xPortGetFreeHeapSize(), xTaskGetTickCount() );
		
		vTaskDelay( 1000 );
		
	}
}

















