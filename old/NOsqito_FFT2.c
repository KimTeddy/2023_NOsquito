/* ============================================================================ */
/*	Exp18_1_FFT(1).c : 1kHz의 정현파/삼각파/구형파에 대한 FFT 분석		*/
/* ============================================================================ */
/*					Programmed by Duck-Yong Yoon in 2016.   */

#include "stm32f767xx.h"
#include "OK-STM767.h"
#include <arm_math.h>				// 반드시 마지막에 인클루드할 것.

void SysTick_Handler(void);			/* SysTick interrupt(100kHz) for DAC output */
void TIM7_IRQHandler(void);			/* TIM7 interrupt(25.6kHz) for ADC input */
void Display_waveform_screen(void);		/* display waveform screen */
void Draw_waveform_axis(void);			/* draw axis of waveform screen */
void Clear_waveform(void);			/* clear graphic screen */
void Display_FFT_screen(void);			/* display FFT screen */
void Draw_FFT(U16 index, float value);		/* draw bar of FFT result */

unsigned char FFT_flag;
unsigned short FFT_count=0;
unsigned int time_1sec = 0;

float FFT_buffer1[2048], FFT_input1[2048], FFT_output1[1024], FFT_buffer2[2048], FFT_input2[2048], FFT_output2[1024],max_value;
unsigned int max_index;

arm_cfft_radix4_instance_f32 S1,S2;

/* ----- 인터럽트 처리 프로그램 ----------------------------------------------- */

void TIM7_IRQHandler(void)			/* TIM7 interrupt(25.6kHz) for ADC input */
{
  unsigned int r1, r2;
  
  TIM7->SR = 0x0000;				// clear pending bit of TIM7 interrupt

    ADC1->CR2 |= 0x40000000;			// start conversion by software
      while(!(ADC1->SR & 0x00000002));		// wait for end of conversion
      r1=ADC1->DR;

      while (!(ADC1->SR & 0x00000002));
      r2=ADC1->DR;
      
      //FFT_buffer1[FFT_count] = ((float)r1 - 2048.)/2048.;
      FFT_buffer2[FFT_count] = ((float)r2 - 2048.) / 2048.;
      
      //FFT_buffer1[FFT_count+1] = 0.;
      FFT_buffer2[FFT_count + 1] = 0.;

      FFT_count += 1;
      if(FFT_count >= 2048)
        { 
	  FFT_count = 0;
          FFT_flag = 1;
        }
    
}

/* ----- 메인 프로그램 -------------------------------------------------------- */

int main(void)
{
  unsigned short i;	// graphic variable

  Initialize_MCU();				// initialize MCU and kit
  Delay_ms(50);					// wait for system stabilization
 
  Initialize_TFT_LCD();				// initialize TFT-LCD modul
  Display_FFT_screen();
  
  //arm_cfft_radix4_init_f32(&S1, 1024, 0, 1);      // initialize radix-4 CFFT function
  arm_cfft_radix4_init_f32(&S2, 1024, 0, 1);      // initialize radix-4 CFFT function

  GPIOA->MODER |= 0x00003000;			// PA6 = analog mode(ADC12_IN6)
  RCC->APB2ENR |= 0x00000100;			// enable ADC1 clock
  ADC->CCR = 0x00000000;			// ADCCLK = 54MHz/2 = 27MHz
  ADC1->SMPR2 = 0x00040000;			// sampling time of channel 6 = 15 cycle
  ADC1->CR1 = 0x00000000;			// 12-bit resolution
  ADC1->CR2 = 0x00000001;			// right alignment, single conversion, ADON = 1
  ADC1->SQR1 = 0x00000000;			// total regular channel number = 1
  ADC1->SQR3 = 0x00000006;			// channel 6

  RCC->APB1ENR |= 0x00000020;			// enable TIM7 clock
  TIM7->PSC = 9;				// 108MHz/(9+1) = 10.8MHz
  TIM7->ARR = 421;				// 10.8MHz/(421+1) = 25.6kHz
  TIM7->CNT = 0;				// clear counter
  TIM7->DIER = 0x0001;				// enable update interrupt
  TIM7->CR1 = 0x0085;				// enable TIM7, update event, and preload
  NVIC->ISER[1] |= 0x00800000;			// enable (55)TIM7 interrupt

  
  while(1)
    {
      if(FFT_flag == 1) // if FFT mode, display FFT result
        { FFT_flag = 0;

	  for (i = 0; i < 2048; i++)
      {
          //FFT_input1[i] = FFT_buffer1[i];
          FFT_input2[i] = FFT_buffer2[i];
      }

         // arm_cfft_radix4_f32(&S1, FFT_input1);	// calculate radix-4 CFFT
          arm_cfft_radix4_f32(&S2, FFT_input2);	// calculate radix-4 CFFT
         // arm_cmplx_mag_f32(FFT_input1, FFT_output1, 1024);
          arm_cmplx_mag_f32(FFT_input2, FFT_output2, 1024);
         // arm_max_f32(FFT_output1, 1024, &max_value, &max_index);
          arm_max_f32(FFT_output2, 1024, &max_value, &max_index);

          for (i = 1; i < 128; i++)		// draw FFT result
            Draw_FFT(i, FFT_output2[i+5 ]*2);
        }
    }
}

/* ----- 사용자 정의 함수 ----------------------------------------------------- */

void Display_FFT_screen(void)			/* display FFT screen */
{
  unsigned short x, y;

  TFT_clear_screen();				// clear screen

  TFT_color(White,Black);
  TFT_English_pixel(18,213, '0');               // 0
  TFT_English_pixel(10,195, '1');               // 10
  TFT_English_pixel(18,195, '0');
  TFT_English_pixel(10,177, '2');               // 20
  TFT_English_pixel(18,177, '0');
  TFT_English_pixel(10,159, '3');               // 30
  TFT_English_pixel(18,159, '0');
  TFT_English_pixel(10,141, '4');               // 40
  TFT_English_pixel(18,141, '0');
  TFT_English_pixel(10,123, '5');               // 50
  TFT_English_pixel(18,123, '0');
  TFT_English_pixel(10,105, '6');               // 60
  TFT_English_pixel(18,105, '0');
  TFT_English_pixel(10, 87, '7');               // 70
  TFT_English_pixel(18, 87, '0');
  TFT_English_pixel(10, 69, '8');               // 80
  TFT_English_pixel(18, 69, '0');
  TFT_English_pixel(10, 51, '9');               // 90
  TFT_English_pixel(18, 51, '0');
  TFT_English_pixel(2,  33, '1');		// 100
  TFT_English_pixel(10, 33, '0');
  TFT_English_pixel(18, 33, '0');
  TFT_color(Magenta,Black);
  TFT_English_pixel(2,  16, '[');               // [%]
  TFT_English_pixel(10, 16, '%');
  TFT_English_pixel(18, 16, ']');

  TFT_color(White,Black);
  TFT_English_pixel(26, 222, '0');		// 0
  TFT_English_pixel(46, 222, '1');		// 1
  TFT_English_pixel(66, 222, '2');		// 2
  TFT_English_pixel(86, 222, '3');		// 3
  TFT_English_pixel(106,222, '4');		// 4
  TFT_English_pixel(126,222, '5');		// 5
  TFT_English_pixel(146,222, '6');		// 6
  TFT_English_pixel(166,222, '7');		// 7
  TFT_English_pixel(186,222, '8');		// 8
  TFT_English_pixel(206,222, '9');		// 9
  TFT_English_pixel(222,222, '1');		// 10
  TFT_English_pixel(230,222, '0');
  TFT_English_pixel(242,222, '1');		// 11
  TFT_English_pixel(250,222, '1');
  TFT_English_pixel(262,222, '1');		// 12
  TFT_English_pixel(270,222, '2');
  TFT_color(Magenta,Black);
  TFT_English_pixel(280,223, '[');		// [kHz]
  TFT_English_pixel(288,223, 'c');
  TFT_English_pixel(296,223, 'H');
  TFT_English_pixel(304,223, 'z');
  TFT_English_pixel(312,223, ']');

  Line(30, 220, 310, 220, White);		// x-axis line
  Line(305,215, 310, 220, White);
  Line(305,225, 310, 220, White);
  for(x = 50; x <= 309; x += 20)		// x-axis scale
    Line(x,218, x,222, White);

  Line(30,  28,  30, 220, White);		// y-axis line
  Line(35,  33,  30,  28, White);
  Line(25,  33,  30,  28, White);
  for(y = 40; y <= 202; y += 18)		// y-axis scale
    Line(28,y, 32,y, White);
}

void Draw_FFT(U16 index, float value)		/* draw bar of FFT result */
{
  unsigned short height;

  height = (unsigned short)(180.*value*2/max_value + 0.5);
  if(height >= 180.) height = 180.;

  Line(30+5*index, 219, 30+5*index, 219 - 180, Black);	    // delete old bar
  Line(30+5*index, 219, 30+5*index, 219 - height, Magenta); // draw new bar
}
