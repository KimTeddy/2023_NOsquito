
#include "stm32f767xx.h"
#include "OK-STM767.h"
#include "OK-STM767_large.h"
#include "nosquito_images.h"
#include "nosquito_bluetooth_serial.h"
#include "nosquito_display.h"
#include "nosquito_adc_fft.h"
#include "nosquito_motor_pwm.h"
#include "nosquito_led.h"

#define DISP_LEFT 0
#define DISP_MIDDLE 1
#define DISP_RIGHT 2

unsigned char mosquito_detected = 0, mosquito_5stack = 0;

void TIM7_IRQHandler(void);         /* TIM7 interrupt(25.6kHz) for ADC input */
void TIM7_IRQHandler(void)         /* TIM7 interrupt(25.6kHz) for ADC input */
{
	unsigned int ADC1_result, ADC2_result;

	TIM7->SR = 0x0000;            // clear pending bit of TIM7 interrupt
	ADC1->CR2 |= 0x40000000;         // start conversion by software
	while (!(ADC->CSR & 0x00000002));      // wait for end of conversion
	ADC1_result = ADC->CDR & 0x0000FFFF;
	ADC2_result = ADC->CDR >> 16;

	FFT_buffer1[FFT_count] = ((float)ADC1_result - 2048.) / 2048.;
	FFT_buffer2[FFT_count] = ((float)ADC2_result - 2048.) / 2048.;

	ADC1->SR = 0x00000000;
	ADC2->SR = 0x00000000;

	FFT_buffer1[FFT_count + 1] = 0.;
	FFT_buffer2[FFT_count + 1] = 0.;

	FFT_count += 1;
	if (FFT_count >= 2048)
	{
		FFT_count = 0;
		FFT_flag = 1;
		ADC_calculation();

		if (FFT_output1[45] > 50 || FFT_output2[45] > 50) {
			ADC_stop();
			mosquito_detected = 1;
		}
		else {
			display_ADC();
		}
	}
}

int main(void)
{
	Initialize_MCU();
	Delay_ms(50);
	Initialize_TFT_LCD();
	LED_on();
	display_starting(); Delay_ms(3000);// 처음 화면 "노스키또"


	ADC_init();

	while (1) {
		if (mosquito_detected == 1)
		{
			int right = 0, left = 0;
			int delta = 0;
			char direction = 0;

			ADC_stop();
			TFT_clear_screen(); //모기가 감지됐어용
			display_detected(); Delay_ms(2000);
			GPIOD->BSRR = 0x00DF0000;//쉴드 사용

			Bluetooth_init();
			TX2_string("Mosquito was detected!\n");
			Initialize_MCU();
			Delay_ms(50);

			for (int i = 20; i < 80; i++)
			{
				if ((FFT_output1[i] > FFT_output2[i]) && (FFT_output1[i] - FFT_output2[i]) > 5) left++;
				else if ((FFT_output1[i] < FFT_output2[i]) && (FFT_output2[i] - FFT_output1[i]) > 5) right++;
				else;
			}
			delta = left - right;

			if (delta > 5) {
				direction = DISP_LEFT;
			}
			else if (delta >= -5 && delta <= 5) {
				direction = DISP_MIDDLE;
			}
			else if (delta < -5) {
				direction = DISP_RIGHT;
			}

			LED_on();
			Initialize_TFT_LCD();
			TFT_clear_screen();
			DisplayMosquitoLocation(direction);
			LED_off();
			Delay_ms(50);
			led_selector(direction);
			Delay_ms(3000);

			PWM_init();
			led_selector(direction);

			TIM1->CCR3 = 30;
			if (direction == DISP_LEFT) {
				servo_selector(0);
				Delay_ms(350); //모터0
				servo_selector(1);
				Delay_ms(350); //모터1
			}
			else if (direction == DISP_MIDDLE) {
				servo_selector(1);
				Delay_ms(350); //모터1
				servo_selector(2);
				Delay_ms(350); //모터2
				servo_selector(0);
				Delay_ms(350); //모터0
				servo_selector(3);
				Delay_ms(350); //모터1
			}
			else if (direction == DISP_RIGHT) {
				servo_selector(2);
				Delay_ms(350); //모터2
				servo_selector(3);
				Delay_ms(350); //모터3
			}

			TIM1->CCR3 = 20;
			if (direction == DISP_LEFT) {
				servo_selector(0);
				Delay_ms(350); //모터0
				servo_selector(1);
				Delay_ms(350); //모터1
			}
			else if (direction == DISP_MIDDLE) {
				servo_selector(1);
				Delay_ms(350); //모터1
				servo_selector(2);
				Delay_ms(350); //모터2
				servo_selector(0);
				Delay_ms(350); //모터0
				servo_selector(3);
				Delay_ms(350); //모터1
			}
			else if (direction == DISP_RIGHT) {
				servo_selector(2);
				Delay_ms(350); //모터2
				servo_selector(3);
				Delay_ms(350); //모터3
			}

			Bluetooth_init();
			TX2_string("Spray!!\n");
			Bluetooth_end();

			display_starting();
			Delay_ms(3000);
			mosquito_detected = 0;
			LED_on();
			ADC_init();
			ADC_start();
		}
	}
}