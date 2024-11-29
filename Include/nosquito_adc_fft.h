
#include <arm_math.h>// 반드시 마지막에 인클루드할 것.

void Display_FFT_screen(void);         /* display FFT screen */
void Draw_FFT(U16 index, float value, int a);      /* draw bar of FFT result */

void ADC_init();
void ADC_calculation();
void display_ADC();

unsigned char FFT_flag;
unsigned short FFT_count=0;
unsigned int time_1sec = 0;

float FFT_buffer1[2048], FFT_input1[2048], FFT_output1[1024], FFT_buffer2[2048], FFT_input2[2048], FFT_output2[1024], max_value;
unsigned int max_index;

arm_cfft_radix4_instance_f32 S1,S2;

void ADC_init() {
    Initialize_MCU();
    Delay_ms(50);
    arm_cfft_radix4_init_f32(&S1, 1024, 0, 1);      // initialize radix-4 CFFT function
    arm_cfft_radix4_init_f32(&S2, 1024, 0, 1);      // initialize radix-4 CFFT function

    GPIOA->MODER |= 0x0000F000;         // PA6 = analog mode(ADC12_IN6)
    RCC->APB2ENR |= 0x00000300;         // enable ADC1 clock
    ADC->CCR = 0x00008006;         // ADCCLK = 54MHz/2 = 27MHz
    ADC1->SMPR2 = 0x00040000;         // sampling time of channel 6 = 15 cycle
    ADC2->SMPR2 = 0x00200000;         // sampling time of channel 7 = 15 cycle

    ADC1->CR1 = 0x00000100;         // 12-bit resolution
    ADC2->CR1 = 0x00000100;         // 12-bit resolution

    ADC1->CR2 = 0x00000401;         // right alignment, single conversion, ADON = 1 , EOC 안돠면 1쓰자
    ADC2->CR2 = 0x00000401;         // right alignment, single conversion, ADON = 1

    ADC1->SQR1 = 0x00000000;         // total regular channel number = 1
    ADC2->SQR1 = 0x00000000;

    ADC1->SQR3 = 0x00000006;         // channel 6
    ADC2->SQR3 = 0x00000007;         // channel 7
    
    
    RCC->APB1ENR |= 0x00000020;         // enable TIM7 clock
    TIM7->PSC = 9;            // 108MHz/(9+1) = 10.8MHz
    TIM7->ARR = 421;            // 10.8MHz/(421+1) = 25.6kHz
    TIM7->CNT = 0;            // clear counter
    TIM7->DIER = 0x0001;            // enable update interrupt
    TIM7->CR1 = 0x0085;            // enable TIM7, update event, and preload
    NVIC->ISER[1] |= 0x00800000;         // enable (55)TIM7 interrupt
    
}
void ADC_start() {
    //TIM7->ARR = 421;            // 10.8MHz/(421+1) = 25.6kHz
  TIM7->CR1 |= 0x00000001;            // 10.8MHz/(421+1) = 25.6kHz

}

void ADC_stop() { 
  TIM7->CR1 &= 0xFFFFFFFE;
    //TIM7->ARR = 0;
}

void ADC_calculation()
{
      for (unsigned short i = 0; i < 2048; i++)
      {
	  FFT_input1[i] = FFT_buffer1[i];
	  FFT_input2[i] = FFT_buffer2[i];
      }
      arm_cfft_radix4_f32(&S1, FFT_input1);// calculate radix-4 CFFT
      arm_cfft_radix4_f32(&S2, FFT_input2);
      arm_cmplx_mag_f32(FFT_input1, FFT_output1, 1024);
      arm_cmplx_mag_f32(FFT_input2, FFT_output2, 1024);
      arm_max_f32(FFT_output1, 1024, &max_value, &max_index);
      arm_max_f32(FFT_output2, 1024, &max_value, &max_index);
}

void display_ADC() {
    LED_on();
    TFT_color_screen(Black);
    Display_FFT_screen();
    TFT_string(7, 3, Cyan, Black, "모기를 찾고 있습니다..."); // 모기 탐색
    Rectangle(0, 0, 319, 239, Blue);

    for (int i = 1; i < 128; i++)      // draw FFT result
        Draw_FFT(i, FFT_output1[i + 8] * 2, 0);
    for (int i = 1; i < 128; i++)      // draw FFT result
        Draw_FFT(i, FFT_output2[i + 8] * 2, 1);
}

void Display_FFT_screen(void)         /* display FFT screen */
{
  unsigned short x, y;

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
  TFT_English_pixel(2,  33, '1');      // 100
  TFT_English_pixel(10, 33, '0');
  TFT_English_pixel(18, 33, '0');
  TFT_color(Magenta,Black);
  TFT_English_pixel(2,  16, '[');               // [%]
  TFT_English_pixel(10, 16, '%');
  TFT_English_pixel(18, 16, ']');

  TFT_color(White,Black);
  TFT_English_pixel(26, 222, '1');      // 1
  TFT_English_pixel(34, 222, '0');      // 
  TFT_English_pixel(42, 222, '0');      // 
  
  TFT_English_pixel(58, 222, '2');      // 
  TFT_English_pixel(66, 222, '0');      // 2
  TFT_English_pixel(74, 222, '0');      // 
  
  TFT_English_pixel(98, 222, '3');      // 
  TFT_English_pixel(106,222, '0');      // 3
  TFT_English_pixel(114,222, '0');      // 
  
  TFT_English_pixel(138,222, '4');      // 
  TFT_English_pixel(146,222, '0');      // 4
  TFT_English_pixel(154,222, '0');      // 
  
  TFT_English_pixel(178,222, '5');      // 
  TFT_English_pixel(186,222, '0');      // 5
  TFT_English_pixel(194,222, '0');      // 
  
  TFT_English_pixel(218,222, '6');      // 
  TFT_English_pixel(226,222, '0');      // 6
  TFT_English_pixel(234,222, '0');      // 
  
  TFT_English_pixel(258,222, '7');      // 
  TFT_English_pixel(266,222, '0');      // 7
  TFT_English_pixel(274,222, '0');      // 
  
  TFT_color(Magenta,Black);
  TFT_English_pixel(288,223, '[');// [Hz]
  TFT_English_pixel(296,223, 'H');
  TFT_English_pixel(304,223, 'z');
  TFT_English_pixel(312,223, ']');

  Line(30, 220, 310, 220, White);      // x-axis line
  Line(305,215, 310, 220, White);
  Line(305,225, 310, 220, White);
  for(x = 50; x <= 309; x += 20)      // x-axis scale
    Line(x,218, x,222, White);

  Line(30,  28,  30, 220, White);      // y-axis line
  Line(35,  33,  30,  28, White);
  Line(25,  33,  30,  28, White);
  for(y = 40; y <= 202; y += 18)      // y-axis scale
    Line(28,y, 32,y, White);
}

void Draw_FFT(U16 index, float value, int a)      /* draw bar of FFT result */
{
    unsigned short height;

    height = (unsigned short)(180. * value * 2 / max_value + 0.5);
    if (height >= 180.) height = 180.;

    if (a == 0) {
        Line(30 + 5 * index, 219, 30 + 5 * index, 219 - 180, Black);       // delete old bar
        Line(30 + 5 * index, 219, 30 + 5 * index, 219 - height, Magenta); // draw new bar
    }
    else {
        Line(30 + 5 * index + 1, 219, 30 + 5 * index + 1, 219 - 180, Black);       // delete old bar
        Line(30 + 5 * index + 1, 219, 30 + 5 * index + 1, 219 - height, Cyan); // draw new bar
    }
}