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
    while (!(USART2->ISR & 0x00000080)); // wait until TXE = 1, TDR �������� ������ �۽� ����Ʈ ���������� �Ű����� ���� �۽� ���۰� ��� �ִ� ����
    USART2->TDR = data;
}

void TX2_string(U08* string) /* transmit a string by USART2 */
{
    while (*string != '\0')
    {
        TX2_char(*string);
        string++;
    }
}// 0xAB 1����Ʈ

void Bluetooth_init() {
    Initialize_MCU();
    Delay_ms(50);
    GPIOD->MODER &= 0xFFFFFC3FF;         // PD5 = USART2_TX PD6 =USART2_RX
    GPIOD->MODER |= 0x00002800;                   // PD5,PD6 = �μ����� ���(AF7)
    GPIOD->AFR[0] &= 0xF00FFFFF;
    GPIOD->AFR[0] |= 0x07700000;

    RCC->APB1ENR |= 0x00020000;         // enable USART2 clock

    USART2->CR1 = 0x00000000;  // TE=RE=UE = 1, 8 data bit, oversampling by 16
    USART2->CR2 = 0x00000000;  // asynchronous mode, 1 stop bit
    USART2->CR3 = 0x00000000;  // 3 sampling bit

    USART2->CR1 = 0x0000000D;
    USART2->BRR = 5625;  // 9600 ������Ʈ

    Delay_ms(1);
}
void Bluetooth_end() {
    Initialize_MCU();
    Delay_ms(50);
    Initialize_TFT_LCD();
}
//CR1 -> UE=0
//CR1 -> M[1:0] 1���� ���� ����
//BRR�� ������Ʈ ����
//CR2���� �����Ʈ�� ���� ����
//CR1 -> UE=1 ���� usart���� ���, idle frame�� ���۵Ǵ� ���·� ����
//CR1 -> TE=1�� �Ͽ� �۽ź� ���� ���
//TDR �������Ϳ� �۽��Ҥ� �����͸� ����Ʈ��, �̶� ISR�������Ϳ��� TXE=0�� ��
//1���ھ� ������ ������ ��쿡�� TXE =1 �� �Ǵ°��� Ȯ��
//������ ������ ���ڸ� TDR �������Ϳ� ����Ʈ�ϰ� ISR �������Ϳ��� TC=1�� �Ǹ� ��� �۽��� �Ϸ�+ TXE=1�� �ǹ�
