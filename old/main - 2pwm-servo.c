/* ============================================================================ */
/*		Exp12_1_PWM(1).c : 타이머1의 1채널 PWM 출력 프로그램		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"

int main(void)
{
  unsigned char key;
  unsigned int PWM;				// pulse width

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization

  PWM = 100;					// initialize PWM = 499 (duty ratio 50%)

  GPIOE->MODER &= 0xCFFFFFFF;			// PE14 = alternate function mode
  GPIOE->MODER |= 0x20000000;
  GPIOE->AFR[1] &= 0xF0FFFFFF;			// PE14 = TIM1_CH4
  GPIOE->AFR[1] |= 0x01000000;

  RCC->APB2ENR |= 0x00000001;			// enable TIM1 clock

  TIM1->PSC = 2159;				// 108MHz/(107+1) = 1MHz
  TIM1->ARR = 999;				// 1MHz / (999+1) = 1kHz
  TIM1->CCR4 = PWM;
  TIM1->CNT = 0;				// clear counter
  TIM1->CCMR2 = 0x00006C00;			// OC4M = 0110(PWM mode 1), CC4S = 00(output)
  TIM1->CCER = 0x00001000;			// CC4E = 1(enable OC4 output)
  TIM1->BDTR = 0x00008000;			// MOE = 1
  TIM1->CR1 = 0x0005;				// edge-aligned, up-counter, enable TIM1

  while(1)
    { 
      key = Key_input_silent();			// key input
      if((key == KEY3) && (PWM < 100))		// if KEY3, increment duty ratio by 10%
        { PWM += 5;
          TIM1->CCR4  = PWM;
        }
      
      else if((key == KEY4) && (PWM > 0))	// if KEY4, decrement duty ratio by 10%
        { PWM -= 5;
          TIM1->CCR4  = PWM;
	}
    }
}
