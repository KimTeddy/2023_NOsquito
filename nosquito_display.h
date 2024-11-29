void Loc(S16 x1, S16 y1, S16 x2, S16 y2, U16 color, U16 fill);
void DisplayMosquitoLocation(int number);
void DisplayRectangles();

void display_starting() {
    unsigned short i, x, y, color;
    LED_on();
    TFT_string(8, 2, Cyan, Black, "____NOsquitto ON____");   // 시작화면

    Rectangle(0, 0, 319, 239, Green);
    i = 0;
    for (y = 0; y < 173; y++)
        for (x = 0; x < 180; x++)
        {
            color = Cortex[i++];
            TFT_pixel(x + 70, y + 45, color);
        }
}

void display_finding() {
    unsigned short i = 0, x, y, color;
    LED_on();
    TFT_string(7, 3, Cyan, Black, "모기를 찾고 있습니다..."); // 모기 탐색
    Rectangle(0, 0, 319, 239, Blue);
    for (y = 0; y < 120; y++)
        for (x = 0; x < 200; x++)
        {
            color = STMicroelectronics[i++];
            TFT_pixel(x + 60, y + 80, color);
        }
}

void display_detected() {
    unsigned short i, x, y, color;
    LED_on();
    TFT_string(7, 3, Red, Black, "모기를 발견했습니다!!"); // 모기발견

    Rectangle(0, 0, 319, 239, Red);
    i = 0;
    for (y = 0; y < 120; y++)
        for (x = 0; x < 200; x++)
        {
            color = WARN[i++];
            TFT_pixel(x + 60, y + 80, color);
        }
}


void Loc(S16 x1, S16 y1, S16 x2, S16 y2, U16 color, U16 fill)
{
    short i;

    //Rectangle(40, 0, 280, 239, Green);  //frame 
    //Rectangle(40, 0, 120, 80, Green); //1
    //Rectangle(120, 0, 200, 80, Green); //2
    //Rectangle(200, 0, 280, 80, Green);  //3
    Rectangle(40, 0, 120, 239, Green);  //left
    //Rectangle(40, 80, 120, 160, Green); //4
    Rectangle(120, 0, 200, 239, Green); //medium
    //Rectangle(200, 80, 280, 160, Green); //6    
    Rectangle(200, 0, 280, 239, Green); //right
    //Rectangle(40, 160, 120, 239, Green); //7
    //Rectangle(120, 160, 200, 239, Green); //8
    //Rectangle(200, 160, 280, 239, Green); //9

    Line(x1, y1, x1, y2, color);         // horizontal line
    Line(x2, y1, x2, y2, color);
    Line(x1, y1, x2, y1, color);         // vertical line
    Line(x1, y2, x2, y2, color);

    if ((y1 < y2) && (x1 != x2))         // fill block
    {
        for (i = y1 + 1; i <= y2 - 1; i++)
            Line(x1 + 1, i, x2 - 1, i, fill);
    }
    else if ((y1 > y2) && (x1 != x2))
    {
        for (i = y2 + 1; i <= y1 - 1; i++)
            Line(x1 + 1, i, x2 - 1, i, fill);
    }
}


void DisplayRectangles()
{
    //Rectangle(40, 0, 280, 239, Green);  //frame 
//Rectangle(40, 0, 120, 80, Green); //1
//Rectangle(120, 0, 200, 80, Green); //2
//Rectangle(200, 0, 280, 80, Green);  //3
    Rectangle(40, 0, 120, 239, Green);  //left
    //Rectangle(40, 80, 120, 160, Green); //4
    Rectangle(120, 0, 200, 239, Green); //medium
    //Rectangle(200, 80, 280, 160, Green); //6    
    Rectangle(200, 0, 280, 239, Green); //right
    //Rectangle(40, 160, 120, 239, Green); //7
    //Rectangle(120, 160, 200, 239, Green); //8
    //Rectangle(200, 160, 280, 239, Green); //9
    Delay_ms(500);
}

void DisplayMosquitoLocation(int number)
{
    LED_on();
    switch (number)
    {
    case 0:
        Loc(40, 0, 120, 239, Green, Red);
        TFT_string_large(2, 5, Cyan, Black, "왼쪽에서 감지했습니다");
        break;
   
    case 1:
        Loc(120, 0, 200, 239, Green, Red);
        TFT_string_large(2, 5, Cyan, Black, "중간에서 감지했습니다.");
        break;

    case 2:
        Loc(200, 0, 280, 239, Green, Red);
        TFT_string_large(2, 5, Cyan, Black, "오른쪽에서 감지했습니다.");
        break;

    default:
        TFT_string_large(2, 5, Cyan, Black, "감지할 수 없습니다.");
        break;
    }
}