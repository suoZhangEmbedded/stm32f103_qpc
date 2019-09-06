
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
#include "bsp_adc.h"

#include "math.h"

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
	bsp_adc_init();

	double value = 0;
	double ppm = 0;
	
	uint16_t adc_value = 0;
	
	for(;;)
	{
		adc_value = bsp_get_adc_value( 1 );
		
//		value =((float)adc_value*(5.0/4096))*0.36-1.08;
		value = (float)adc_value/4096*3.3;
		printf( "tick:%u,adc value:%f.\r\n", xTaskGetTickCount(), value );
		
		ppm = pow(11.5428 * 35.904 * value/(25.5-5.1* value),0.6549);
		printf( "tick:%u,ppm value:%f ppm.\r\n", xTaskGetTickCount(), ppm );
		
		bsp_led_toggle( e_led_red_system_status );
		
//		printf( "tick:%u,system heap:%u.\r\n", xTaskGetTickCount(), xPortGetFreeHeapSize() );
		
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		
	}
	
}


















