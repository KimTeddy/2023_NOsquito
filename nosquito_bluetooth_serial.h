void Bluetooth_init();
void Bluetooth_end();
void TX2_char(U08 data); /* transmit a character by USART2 */
void TX2_string(U08* string); /* transmit a string by USART2 */

unsigned char RX2_char(void) /* receive a character by USART2 */
{
    if (USART2->ISR & 0x00000020)
        // if RXNE=1, return with a received character
        return USART2->RDR;
    else // if RXNE=0, return with 0x00
        return 0x00;
}

void TX2_char(U08 data) /* transmit a character by USART2 */
{
    while (!(USART2->ISR & 0x00000080)); // wait until TXE = 1, TDR 레지스터 내용이 송신 시프트 레지스러로 옮겨져셔 현재 송신 버퍼가 비어 있는 상태
    USART2->TDR = data;
}

void TX2_string(U08* string) /* transmit a string by USART2 */
{
    while (*string != '\0')
    {
        TX2_char(*string);
        string++;
    }
}// 0xAB 1바이트

void Bluetooth_init() {
    Initialize_MCU();
    Delay_ms(50);
    GPIOD->MODER &= 0xFFFFFC3FF;         // PD5 = USART2_TX PD6 =USART2_RX
    GPIOD->MODER |= 0x00002800;                   // PD5,PD6 = 부수적인 기능(AF7)
    GPIOD->AFR[0] &= 0xF00FFFFF;
    GPIOD->AFR[0] |= 0x07700000;

    RCC->APB1ENR |= 0x00020000;         // enable USART2 clock

    USART2->CR1 = 0x00000000;  // TE=RE=UE = 1, 8 data bit, oversampling by 16
    USART2->CR2 = 0x00000000;  // asynchronous mode, 1 stop bit
    USART2->CR3 = 0x00000000;  // 3 sampling bit

    USART2->CR1 = 0x0000000D;
    USART2->BRR = 5625;  // 9600 보레이트

    Delay_ms(1);
}
void Bluetooth_end() {
    Initialize_MCU();
    Delay_ms(50);
    Initialize_TFT_LCD();
}
//CR1 -> UE=0
//CR1 -> M[1:0] 1문자 길이 정함
//BRR로 보레이트 설정
//CR2에서 스톱비트의 수를 지정
//CR1 -> UE=1 으로 usart동작 허용, idle frame이 전송되는 상태로 만듦
//CR1 -> TE=1로 하여 송신부 동작 허용
//TDR 레지스터에 송신할ㄹ 데이터를 라이트함, 이때 ISR레지스터에서 TXE=0이 됌
//1문자씩 여러번 전송할 경우에는 TXE =1 이 되는것을 확인
//전송할 마지막 문자를 TDR 레지스터에 라이트하고 ISR 레지스터에서 TC=1이 되면 모든 송신이 완료+ TXE=1을 의미
