/* 
 * File:   Ultrasonic.h
 * Author: Mohamed_Nagy
 * https://github.com/Ged0oo 
 * https://www.linkedin.com/in/mohamednagyofficial/
 * Created on February 23, 2023, 8:41 PM
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
#include "../../MCAL/DIO/DIO.h"
#include "../../MCAL/TIMER1/TIMER_1.h"



typedef struct  
{
	ST_pinCofig_t echo;
	ST_pinCofig_t trigger;
}ultrasonic_t;


Std_ReturnType UltraSonic_Init(const ultrasonic_t* ultra);
uint16 Ultrasonic_GetDistance(const ultrasonic_t* ultra); 
uint8 ULTRASONIC_GetDistanceNoBlock(uint8*Pdistance); 
#endif /* ULTRASONIC_H_ */