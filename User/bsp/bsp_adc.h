
/*
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-6-11     suozhang      the first version
 *
 */


#ifndef _BSP_ADC_H
#define _BSP_ADC_H

#include "stm32f10x.h"

void bsp_adc_init( void );
uint16_t bsp_get_adc_value( uint8_t ch );

#endif /* _BSP_ADC_H */

