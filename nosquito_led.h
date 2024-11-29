
void led_selector(char led_num){
  switch(led_num)
  {
  case 0:
      GPIOD->BSRR = 0x00070000; //LED0 xxxx x000
      break;
  case 1:
      GPIOD->BSRR = 0x00060001; //LED1 xxxx x001
      break;
  case 2:
      GPIOD->BSRR = 0x00050002; //LED2 xxxx x010
      break;
  case 3:
      GPIOD->BSRR = 0x00040003; //LED3 xxxx x011
      break;
  }
}