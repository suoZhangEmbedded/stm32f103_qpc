
/*
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-6-11     suozhang      the first version
 *
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "stm32f10x.h"

#include "SEGGER_RTT.h"
#include "stdio.h"

#include "bsp_led.h"

/**
 * 重定义fputc函数
 *
 * @param 输出的字符
 * @param 目标文件
 *
 * @return ch
 */
int fputc(int ch, FILE *f)
{

	SEGGER_RTT_Write( 0, &ch, 1 );
	
	return ch;

}

void led_task( void *pvParameters );

TaskHandle_t led_task_handle = NULL;

int main(void)
{
	
	/* 优先级分组设置为4 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	xTaskCreate( led_task, "led", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+3, &led_task_handle );

	/* Start the scheduler. */
	vTaskStartScheduler();
	
	while(1);
	
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char * pcTaskName )
{
	
	printf( "vApplicationStackOverflowHook.\r\n" );
		
	while(1);
	
}

void led_task( void *pvParameters )
{
	
	bsp_led_init();

	for(;;)
	{
	
		bsp_led_toggle( e_led_red_system_status );
		
		printf( "tick:%u,system heap:%u.\r\n", xTaskGetTickCount(), xPortGetFreeHeapSize() );
		
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		
	}
	
}


















