
void PWM_init();

void PWM_init()
{
    Initialize_MCU();
    Delay_ms(50);
    
    GPIOE->MODER &= 0xF3FFFFFF;         // PE14 = alternate function mode
    GPIOE->MODER |= 0x08000000;
    GPIOE->AFR[1] &= 0xFF0FFFFF;         // PE14 = TIM1_CH4
    GPIOE->AFR[1] |= 0x00100000;

    RCC->APB2ENR |= 0x00000001;         // enable TIM1 clock

    TIM1->PSC = 2159;            // 108MHz/(107+1) = 1MHz
    TIM1->ARR = 999;            // 1MHz / (999+1) = 1kHz
    TIM1->CCR3 = 30;
    TIM1->CNT = 0;            // clear counter
    TIM1->CCMR2 = 0x00000060;         // OC4M = 0110(PWM mode 1), CC4S = 00(output)
    TIM1->CCER = 0x00000100;         // CC4E = 1(enable OC4 output)
    TIM1->BDTR = 0x00008000;         // MOE = 1
    TIM1->CR1 = 0x0005;            // edge-aligned, up-counter, enable TIM1
}

void servo_selector(char servo) {
    switch (servo)
    {
    case 0:
        GPIOD->BSRR = 0x00580000; //모터0 x0x0 0xxx
        break;
    case 1:
        GPIOD->BSRR = 0x00500008; //모터1 x0x0 1xxx
        break;
    case 2:
        GPIOD->BSRR = 0x00480010; //모터2 x0x1 0xxx
        break;
    case 3:
        GPIOD->BSRR = 0x00400018; //모터3 x0x1 1xxx
        break;
    }
}