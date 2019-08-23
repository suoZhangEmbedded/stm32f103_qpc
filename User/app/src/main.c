
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

#include "qpc.h"

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

/*..........................................................................*/
void QF_onCleanup(void) 
{
	
}

/* QF callbacks ============================================================*/
void QF_onStartup(void) 
{
    /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
    //SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC); // done in FreeRTOS

}

/*..........................................................................*/
void Q_onAssert(char const *module, int loc) 
{
    /*
    * NOTE: add here your application-specific error handling
    */
    (void)module;
    (void)loc;
    QS_ASSERTION(module, loc, (uint32_t)10000U); /* report assertion to QS */

#ifndef NDEBUG
    /* light up LED */
    bsp_led_on(e_led_red_system_status);
    /* for debugging, hang on in an endless loop... */
    for (;;) {
    }
#endif

    NVIC_SystemReset();
}

/*..........................................................................*/
/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must
* provide an implementation of vApplicationGetIdleTaskMemory() to provide
* the memory that is used by the Idle task.
*/
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
    /* If the buffers to be provided to the Idle task are declared inside
    * this function then they must be declared static - otherwise they will
    * be allocated on the stack and so not exists after this function exits.
    */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the
    * Idle task's state will be stored.
    */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    * Note that, as the array is necessarily of type StackType_t,
    * configMINIMAL_STACK_SIZE is specified in words, not bytes.
    */
    *pulIdleTaskStackSize = Q_DIM(uxIdleTaskStack);
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


















