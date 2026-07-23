#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "SG90.h"

uint8_t i;

int main (void)
{
	OLED_Init();
	Servo_Init();
	
	Set_Angle(90);
	
while (1)
{

}
}
