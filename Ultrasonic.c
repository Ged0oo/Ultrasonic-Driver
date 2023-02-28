/* 
 * File:   Ultrasonic.c
 * Author: Mohamed_Nagy
 * https://github.com/Ged0oo 
 * https://www.linkedin.com/in/mohamednagyofficial/
 * Created on February 23, 2023, 8:41 PM
 */ 


#include "Ultrasonic.h"

static volatile uint16 t1,t2,flag=0,c=0;
void tmr1_ov_interput();
void interrupt_icu();

ST_timer1_t tmr = 
{
	.mode=TMR1_NORMAL_MODE,
	.prescaler=TMR1_8_PRESCALER,
	.channle=TIMER1_OCR1A,
	.fast_pwm=OC1_DISCONNECTED,
	.TMR1_InterriptOveFlow=tmr1_ov_interput,
	.TMR1_InterriptCAPT=interrupt_icu
};


Std_ReturnType UltraSonic_Init(const ultrasonic_t* ultra)
{
		Std_ReturnType ret=E_OK;
		if(NULL == ultra)
		{
			ret = E_NOT_OK; 
		}
		else
		{
			ret = gpio_pin_init(&(ultra->trigger));
			ret = gpio_pin_init(&(ultra->echo));
			ret = TIMER1_Init(&tmr);
			ret = TIMER1_Interrupt_CAPT_enable(interrupt_icu);
		}
		return ret;
}


uint16 Ultrasonic_GetDistance(const ultrasonic_t* ultra)
{
		Std_ReturnType ret=E_OK;
		if(NULL == ultra)
		{
			ret = E_NOT_OK;
		}
		else
		{
			uint16 t=0;
			uint16 time;
			uint16 distance;
			
			ret = TIMER1_voidSetTCNT1(0);
			c=0;
			flag=0;
			
			ret = gpio_write_logic(&(ultra->trigger),GPIO_HIGH);
			_delay_us(10);
			ret = gpio_write_logic(&(ultra->trigger),GPIO_LOW);
		
			ret = TIMER1_Set_CaptureEdge(ICU_RISING_EDGE);
			ret = TIMER1_Interrupt_CAPT_enable(interrupt_icu);	
			ret = TIMER1_Interrupt_Over_Flow_enable(&tmr);
			
			while(flag <2);
			
			time = t2-t1 + ((uint32)(c*65535));
			
			distance= time/58;
	
			ret = TIMER1_Interrupt_CAPT_Disable();
			return distance;
		}
}


void interrupt_icu()
{
	if(flag == 0)
	{
		c=0;
		t1=ICR1;
		flag=1;		
		TIMER1_Set_Edge(FALLING_EDGE);
	}
	else if(flag == 1)
	{
		t2=ICR1;
		flag=2;
		_delay_ms(100);
		TIMER1_Set_Edge(RISING_EDGE);
		TMR1_INTERRUPT_CAPT_DISABLE();
		TMR1_INTERRUPT_OVER_FLOW_DISABLE();
	}
}


void tmr1_ov_interput()
{
	c++;
}


Std_ReturnType TIMER1_Set_Edge(EdgeSelect_t _Edge)
{
	if(FALLING_EDGE == _Edge)
	{
		CLEAR_BIT(TCCR1B, ICES1);
	}
	else if(RISING_EDGE == _Edge)
	{
		SET_BIT(TCCR1B, ICES1);
	}
}