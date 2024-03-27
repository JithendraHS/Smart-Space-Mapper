/*****************************************************************************
* | File        :   oled.c
* | Author      :   Waveshare team
* | Function    :   1.5inch OLED Module (B) Drive function
* | Info        :
*----------------
* | This version:   V1.0
* | Date        :   2023-05-20
* | Info        :
* -----------------------------------------------------------------------------
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "oled.h"
#include "spi_code.h"
#include "pico/stdlib.h"
#include "stdio.h"
/**
 * Image attributes
**/
typedef struct {
    UBYTE *Image;
    UWORD Width;
    UWORD Height;
    UWORD WidthMemory;
    UWORD HeightMemory;
    UWORD Color;
    UWORD Rotate;
    UWORD Mirror;
    UWORD WidthByte;
    UWORD HeightByte;
    UWORD Scale;
} PAINT;
PAINT Paint;

static void OLED_Reset(void)
{
    gpio_put(SPI_RESET_PIN, 1);
    sleep_ms(100);
    gpio_put(SPI_RESET_PIN, 0);
    sleep_ms(100);
    gpio_put(SPI_RESET_PIN, 1);
    sleep_ms(100);
}

void OLED_init() {
    // Initialize and set up SPI_CS_PIN
    gpio_init(SPI_CS_PIN);
    gpio_set_dir(SPI_CS_PIN, GPIO_OUT);
    gpio_put(SPI_CS_PIN, 1);  // Set CS pin to a default state

    // Initialize and set up SPI_DC_PIN
    gpio_init(SPI_DC_PIN);
    gpio_set_dir(SPI_DC_PIN, GPIO_OUT);
    gpio_put(SPI_DC_PIN, 1);  // Set DC pin to a default state

    // Initialize and set up SPI_RESET_PIN
    gpio_init(SPI_RESET_PIN);
    gpio_set_dir(SPI_RESET_PIN, GPIO_OUT);

    // Perform OLED reset
    OLED_Reset();


    SPI_WriteCommand(0xae); /*turn off OLED display*/
    SPI_WriteCommand(0x00); /*set lower column address*/
    SPI_WriteCommand(0x10); /*set higher column address*/

    SPI_WriteCommand(0xB0); /*set page address*/

    SPI_WriteCommand(0xdc); /*set display start line*/
    SPI_WriteCommand(0x20);

    SPI_WriteCommand(0x81); /*contract control*/
    SPI_WriteCommand(0x6f); /*128*/

    SPI_WriteCommand(0x21); /* Set Memory addressing mode (0x20/0x21) */

    SPI_WriteCommand(0xa1); /*set segment remap*/

    SPI_WriteCommand(0xc0); /*Com scan direction*/

    SPI_WriteCommand(0xa4); /*Disable Entire Display On (0xA4/0xA5)*/

    SPI_WriteCommand(0xa6); /*normal / reverse*/

    SPI_WriteCommand(0xa8); /*multiplex ratio*/
    SPI_WriteCommand(0x7f); /*duty = 1/64*/

    SPI_WriteCommand(0xd3); /*set display offset*/
    SPI_WriteCommand(0x60);

    SPI_WriteCommand(0xd5); /*set osc division*/
    SPI_WriteCommand(0x80);

    SPI_WriteCommand(0xd9); /*set pre-charge period*/
    SPI_WriteCommand(0x1d);

    SPI_WriteCommand(0xdb); /*set vcomh*/
    SPI_WriteCommand(0x35);

    SPI_WriteCommand(0xad); /*set charge pump enable*/
    SPI_WriteCommand(0x80); /*Set DC-DC enable (a=0:disable; a=1:enable) */
   
    sleep_ms(200);
    SPI_WriteCommand(0xaf); /*Turn on the OLED display*/
}

void OLED_Clear()
{
    UWORD Width, Height, column;
    Width = (OLED_WIDTH % 8 == 0) ? (OLED_WIDTH / 8) : (OLED_WIDTH / 8 + 1);
    Height = OLED_HEIGHT;

    SPI_WriteCommand(0xb0); // Set the row  start address
    for (uint16_t j = 0; j < Height; j++)
    {
        column = j;
        SPI_WriteCommand(0x00 + (column & 0x0f)); // Set column low start address
        SPI_WriteCommand(0x10 + (column >> 4));   // Set column higt start address
        for (uint16_t i = 0; i < Width; i++)
        {
            SPI_WriteData(0x00);
        }
    }
}
/********************************************************************************
function:   
            reverse a byte data
********************************************************************************/
UBYTE reverse(UBYTE temp)
{
    temp = ((temp & 0x55) << 1) | ((temp & 0xaa) >> 1);
    temp = ((temp & 0x33) << 2) | ((temp & 0xcc) >> 2);
    temp = ((temp & 0x0f) << 4) | ((temp & 0xf0) >> 4);  
    return temp;
}
/********************************************************************************
function:	
    Update all memory to OLED
********************************************************************************/
void OLED_Display(const UBYTE *Image)
{       
	UWORD Width, Height, column, temp;
	Width = (OLED_WIDTH % 8 == 0)? (OLED_WIDTH / 8 ): (OLED_WIDTH / 8 + 1);
	Height = OLED_HEIGHT;   
	SPI_WriteCommand(0xb0); 	//Set the row  start address
	for (UWORD j = 0; j < Height; j++) {
		// column = 63 - j;
        column=j;
		SPI_WriteCommand(0x00 + (column & 0x0f));  //Set column low start address
		SPI_WriteCommand(0x10 + (column >> 4));  //Set column higt start address
		for (UWORD i = 0; i < Width; i++) {
			temp = (Image[i + j * (Width)]);
            temp = reverse(temp);
			SPI_WriteData(temp);
		 }
	}   
}

/********************************************************************************
function:
        OLED display test animation
********************************************************************************/
void OLED_Display_Test(void)
{
    UWORD Width, Height, column;

    Width = (OLED_WIDTH % 8 == 0) ? (OLED_WIDTH / 8) : (OLED_WIDTH / 8 + 1);
    Height = OLED_HEIGHT;

    for (UWORD m = 0; m < Height; m++)
    {
        SPI_WriteCommand(0xb0); // Set the row  start address
        for (UWORD j = 0; j < Height; j++)
        {
            column=j;
            SPI_WriteCommand(0x00 + (column & 0x0f)); // Set column low start address
            SPI_WriteCommand(0x10 + (column >> 4));   // Set column higt start address
            for (UWORD i = 0; i < Width; i++)
            {
                if (j == m)
                {
                    SPI_WriteData(0xff);
                }
                else
                {
                    SPI_WriteData(0x00);
                }
            }
        }
        sleep_ms(20);
    }

    for (UWORD m = 0; m < Width; m++)
    {
        SPI_WriteCommand(0xb0); // Set the row  start address
        for (UWORD j = 0; j < Height; j++)
        {
            column=j;
            SPI_WriteCommand(0x00 + (column & 0x0f)); // Set column low start address
            SPI_WriteCommand(0x10 + (column >> 4));   // Set column higt start address
            for (UWORD i = 0; i < Width; i++)
            {
                if (i == m)
                {
                    SPI_WriteData(0xff);
                }
                else
                {
                    SPI_WriteData(0x00);
                }
            }
        }
        sleep_ms(100);
    }
}

/******************************************************************************
function: Select Image
parameter:
    image : Pointer to the image cache
******************************************************************************/
void Paint_SelectImage(UBYTE *image)
{
    Paint.Image = image;
}

/******************************************************************************
function: Draw Pixels
parameter:
    Xpoint : At point X
    Ypoint : At point Y
    Color  : Painted colors
******************************************************************************/
void Paint_SetPixel(UWORD Xpoint, UWORD Ypoint, UWORD Color)
{
    if(Xpoint > Paint.Width || Ypoint > Paint.Height){
        printf("Exceeding display boundaries\r\n");
        return;
    }      
    UWORD X, Y;

    switch(Paint.Rotate) {
    case 0:
        X = Xpoint;
        Y = Ypoint;  
        break;
    case 90:
        X = Paint.WidthMemory - Ypoint - 1;
        Y = Xpoint;
        break;
    case 180:
        X = Paint.WidthMemory - Xpoint - 1;
        Y = Paint.HeightMemory - Ypoint - 1;
        break;
    case 270:
        X = Ypoint;
        Y = Paint.HeightMemory - Xpoint - 1;
        break;
    default:
        return;
    }
    
    switch(Paint.Mirror) {
    case MIRROR_NONE:
        break;
    case MIRROR_HORIZONTAL:
        X = Paint.WidthMemory - X - 1;
        break;
    case MIRROR_VERTICAL:
        Y = Paint.HeightMemory - Y - 1;
        break;
    case MIRROR_ORIGIN:
        X = Paint.WidthMemory - X - 1;
        Y = Paint.HeightMemory - Y - 1;
        break;
    default:
        return;
    }

    if(X > Paint.WidthMemory || Y > Paint.HeightMemory){
        printf("Exceeding display boundaries\r\n");
        return;
    }
    
    if(Paint.Scale == 2){
        UDOUBLE Addr = X / 8 + Y * Paint.WidthByte;
        UBYTE Rdata = Paint.Image[Addr];
        if(Color == BLACK)
            Paint.Image[Addr] = Rdata & ~(0x80 >> (X % 8));
        else
            Paint.Image[Addr] = Rdata | (0x80 >> (X % 8));
    }else if(Paint.Scale == 4){
        UDOUBLE Addr = X / 4 + Y * Paint.WidthByte;
        Color = Color % 4;//Guaranteed color scale is 4  --- 0~3
        UBYTE Rdata = Paint.Image[Addr];
        
        Rdata = Rdata & (~(0xC0 >> ((X % 4)*2)));
        Paint.Image[Addr] = Rdata | ((Color << 6) >> ((X % 4)*2));
    }else if(Paint.Scale == 16) {
        UDOUBLE Addr = X / 2 + Y * Paint.WidthByte;
        UBYTE Rdata = Paint.Image[Addr];
        Color = Color % 16;
        Rdata = Rdata & (~(0xf0 >> ((X % 2)*4)));
        Paint.Image[Addr] = Rdata | ((Color << 4) >> ((X % 2)*4));
    }else if(Paint.Scale == 65) {
        UDOUBLE Addr = X*2 + Y*Paint.WidthByte;
        Paint.Image[Addr] = 0xff & (Color>>8);
        Paint.Image[Addr+1] = 0xff & Color;
    }

}

/******************************************************************************
function: Show English characters
parameter:
    Xpoint           ：X coordinate
    Ypoint           ：Y coordinate
    Acsii_Char       ：To display the English characters
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void Paint_DrawChar(UWORD Xpoint, UWORD Ypoint, const char Acsii_Char,
                    sFONT* Font, UWORD Color_Foreground, UWORD Color_Background)
{
    UWORD Page, Column;

    if (Xpoint > Paint.Width || Ypoint > Paint.Height) {
        printf("Paint_DrawChar Input exceeds the normal display range\r\n");
        return;
    }

    uint32_t Char_Offset = (Acsii_Char - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
    const unsigned char *ptr = &Font->table[Char_Offset];

    for (Page = 0; Page < Font->Height; Page ++ ) {
        for (Column = 0; Column < Font->Width; Column ++ ) {

            //To determine whether the font background color and screen background color is consistent
            if (FONT_BACKGROUND == Color_Background) { //this process is to speed up the scan
                if (*ptr & (0x80 >> (Column % 8)))
                    Paint_SetPixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
                    // Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
            } else {
                if (*ptr & (0x80 >> (Column % 8))) {
                    Paint_SetPixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
                    // Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                } else {
                    Paint_SetPixel(Xpoint + Column, Ypoint + Page, Color_Background);
                    // Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Background, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                }
            }
            //One pixel is 8 bits
            if (Column % 8 == 7)
                ptr++;
        }// Write a line
        if (Font->Width % 8 != 0)
            ptr++;
    }// Write all
}

/******************************************************************************
function:	Display the string
parameter:
    Xstart           ：X coordinate
    Ystart           ：Y coordinate
    pString          ：The first address of the English string to be displayed
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void Paint_DrawString_EN(UWORD Xstart, UWORD Ystart, const char * pString,
                         sFONT* Font, UWORD Color_Foreground, UWORD Color_Background)
{
    UWORD Xpoint = Xstart;
    UWORD Ypoint = Ystart;

    if (Xstart > Paint.Width || Ystart > Paint.Height) {
        printf("Paint_DrawString_EN Input exceeds the normal display range\r\n");
        return;
    }

    while (* pString != '\0') {
        //if X direction filled , reposition to(Xstart,Ypoint),Ypoint is Y direction plus the Height of the character
        if ((Xpoint + Font->Width ) > Paint.Width ) {
            Xpoint = Xstart;
            Ypoint += Font->Height;
        }

        // If the Y direction is full, reposition to(Xstart, Ystart)
        if ((Ypoint  + Font->Height ) > Paint.Height ) {
            Xpoint = Xstart;
            Ypoint = Ystart;
        }
        Paint_DrawChar(Xpoint, Ypoint, * pString, Font, Color_Foreground, Color_Background);

        //The next character of the address
        pString ++;

        //The next word of the abscissa increases the font of the broadband
        Xpoint += Font->Width;
    }
}
void Paint_SetScale(UBYTE scale)
{
    if(scale == 2){
        Paint.Scale = scale;
        Paint.WidthByte = (Paint.WidthMemory % 8 == 0)? (Paint.WidthMemory / 8 ): (Paint.WidthMemory / 8 + 1);
    }else if(scale == 4){
        Paint.Scale = scale;
        Paint.WidthByte = (Paint.WidthMemory % 4 == 0)? (Paint.WidthMemory / 4 ): (Paint.WidthMemory / 4 + 1);
    }else if(scale ==16) {
        Paint.Scale = scale;
        Paint.WidthByte = (Paint.WidthMemory%2==0) ? (Paint.WidthMemory/2) : (Paint.WidthMemory/2+1); 
    }else if(scale ==65) {
        Paint.Scale = scale;
        Paint.WidthByte = Paint.WidthMemory*2; 
    }else{
        printf("Set Scale Input parameter error\r\n");
        printf("Scale Only support: 2 4 16 65\r\n");
    }
}

/******************************************************************************
function: Create Image
parameter:
    image   :   Pointer to the image cache
    width   :   The width of the picture
    Height  :   The height of the picture
    Color   :   Whether the picture is inverted
******************************************************************************/
void Paint_NewImage(UBYTE *image, UWORD Width, UWORD Height, UWORD Rotate, UWORD Color)
{
    Paint.Image = NULL;
    Paint.Image = image;

    Paint.WidthMemory = Width;
    Paint.HeightMemory = Height;
    Paint.Color = Color;    
	Paint.Scale = 2;
		
    Paint.WidthByte = (Width % 8 == 0)? (Width / 8 ): (Width / 8 + 1);
    Paint.HeightByte = Height;    
//    printf("WidthByte = %d, HeightByte = %d\r\n", Paint.WidthByte, Paint.HeightByte);
//    printf(" EPD_WIDTH / 8 = %d\r\n",  122 / 8);
   
    Paint.Rotate = Rotate;
    Paint.Mirror = MIRROR_NONE;
    
    if(Rotate == ROTATE_0 || Rotate == ROTATE_180) {
        Paint.Width = Width;
        Paint.Height = Height;
    } else {
        Paint.Width = Height;
        Paint.Height = Width;
    }
}

/******************************************************************************
function: Clear the color of the picture
parameter:
    Color : Painted colors
******************************************************************************/
void Paint_Clear(UWORD Color)
{
    if(Paint.Scale == 2 || Paint.Scale == 4) {
        for (UWORD Y = 0; Y < Paint.HeightByte; Y++) {
            for (UWORD X = 0; X < Paint.WidthByte; X++ ) {//8 pixel =  1 byte
                UDOUBLE Addr = X + Y*Paint.WidthByte;
                Paint.Image[Addr] = Color;
            }
        }
    }else if(Paint.Scale == 16) {
        for (UWORD Y = 0; Y < Paint.HeightByte; Y++) {
            for (UWORD X = 0; X < Paint.WidthByte; X++ ) {//8 pixel =  1 byte
                UDOUBLE Addr = X + Y*Paint.WidthByte;
                Color = Color & 0x0f;
                Paint.Image[Addr] = (Color<<4) | Color;
            }
        }
    }else if(Paint.Scale == 65) {
        for (UWORD Y = 0; Y < Paint.HeightByte; Y++) {
            for (UWORD X = 0; X < Paint.WidthByte; X++ ) {//8 pixel =  1 byte
                UDOUBLE Addr = X*2 + Y*Paint.WidthByte;
                Paint.Image[Addr] = 0x0f & (Color>>8);
                Paint.Image[Addr+1] = 0x0f & Color;
            }
        }
    }
}

/******************************************************************************
function: Draw Point(Xpoint, Ypoint) Fill the color
parameter:
    Xpoint		: The Xpoint coordinate of the point
    Ypoint		: The Ypoint coordinate of the point
    Color		: Painted color
    Dot_Pixel	: point size
    Dot_Style	: point Style
******************************************************************************/
void Paint_DrawPoint(UWORD Xpoint, UWORD Ypoint, UWORD Color,
                     DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_Style)
{
    if (Xpoint > Paint.Width || Ypoint > Paint.Height) {
        printf("Paint_DrawPoint Input exceeds the normal display range\r\n");
				printf("Xpoint = %d , Paint.Width = %d  \r\n ",Xpoint,Paint.Width);
				printf("Ypoint = %d , Paint.Height = %d  \r\n ",Ypoint,Paint.Height);
        return;
    }

    int16_t XDir_Num , YDir_Num;
    if (Dot_Style == DOT_FILL_AROUND) {
        for (XDir_Num = 0; XDir_Num < 2 * Dot_Pixel - 1; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num < 2 * Dot_Pixel - 1; YDir_Num++) {
                if(Xpoint + XDir_Num - Dot_Pixel < 0 || Ypoint + YDir_Num - Dot_Pixel < 0)
                    break;
                // printf("x = %d, y = %d\r\n", Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel);
                Paint_SetPixel(Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel, Color);
            }
        }
    } else {
        for (XDir_Num = 0; XDir_Num <  Dot_Pixel; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num <  Dot_Pixel; YDir_Num++) {
                Paint_SetPixel(Xpoint + XDir_Num - 1, Ypoint + YDir_Num - 1, Color);
            }
        }
    }
}

/******************************************************************************
function: Draw a line of arbitrary slope
parameter:
    Xstart ：Starting Xpoint point coordinates
    Ystart ：Starting Xpoint point coordinates
    Xend   ：End point Xpoint coordinate
    Yend   ：End point Ypoint coordinate
    Color  ：The color of the line segment
    Line_width : Line width
    Line_Style: Solid and dotted lines
******************************************************************************/
void Paint_DrawLine(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend,
                    UWORD Color, DOT_PIXEL Line_width, LINE_STYLE Line_Style)
{
    if (Xstart > Paint.Width || Ystart > Paint.Height ||
        Xend > Paint.Width || Yend > Paint.Height) {
        printf("Paint_DrawLine Input exceeds the normal display range\r\n");
        return;
    }

    UWORD Xpoint = Xstart;
    UWORD Ypoint = Ystart;
    int dx = (int)Xend - (int)Xstart >= 0 ? Xend - Xstart : Xstart - Xend;
    int dy = (int)Yend - (int)Ystart <= 0 ? Yend - Ystart : Ystart - Yend;

    // Increment direction, 1 is positive, -1 is counter;
    int XAddway = Xstart < Xend ? 1 : -1;
    int YAddway = Ystart < Yend ? 1 : -1;

    //Cumulative error
    int Esp = dx + dy;
    char Dotted_Len = 0;

    for (;;) {
        Dotted_Len++;
        //Painted dotted line, 2 point is really virtual
        if (Line_Style == LINE_STYLE_DOTTED && Dotted_Len % 3 == 0) {
            //Debug("LINE_DOTTED\r\n");
						if(Color)
							Paint_DrawPoint(Xpoint, Ypoint, BLACK, Line_width, DOT_STYLE_DFT);
            else
							Paint_DrawPoint(Xpoint, Ypoint, WHITE, Line_width, DOT_STYLE_DFT);
            Dotted_Len = 0;
        } else {
            Paint_DrawPoint(Xpoint, Ypoint, Color, Line_width, DOT_STYLE_DFT);
        }
        if (2 * Esp >= dy) {
            if (Xpoint == Xend)
                break;
            Esp += dy;
            Xpoint += XAddway;
        }
        if (2 * Esp <= dx) {
            if (Ypoint == Yend)
                break;
            Esp += dx;
            Ypoint += YAddway;
        }
    }
}

/******************************************************************************
function: Draw a rectangle
parameter:
    Xstart ：Rectangular  Starting Xpoint point coordinates
    Ystart ：Rectangular  Starting Xpoint point coordinates
    Xend   ：Rectangular  End point Xpoint coordinate
    Yend   ：Rectangular  End point Ypoint coordinate
    Color  ：The color of the Rectangular segment
    Line_width: Line width
    Draw_Fill : Whether to fill the inside of the rectangle
******************************************************************************/
void Paint_DrawRectangle(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend,
                         UWORD Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill)
{
    if (Xstart > Paint.Width || Ystart > Paint.Height ||
        Xend > Paint.Width || Yend > Paint.Height) {
        printf("Input exceeds the normal display range\r\n");
        return;
    }

    if (Draw_Fill) {
        UWORD Ypoint;
        for(Ypoint = Ystart; Ypoint < Yend; Ypoint++) {
            Paint_DrawLine(Xstart, Ypoint, Xend, Ypoint, Color , Line_width, LINE_STYLE_SOLID);
        }
    } else {
        Paint_DrawLine(Xstart, Ystart, Xend, Ystart, Color, Line_width, LINE_STYLE_SOLID);
        Paint_DrawLine(Xstart, Ystart, Xstart, Yend, Color, Line_width, LINE_STYLE_SOLID);
        Paint_DrawLine(Xend, Yend, Xend, Ystart, Color, Line_width, LINE_STYLE_SOLID);
        Paint_DrawLine(Xend, Yend, Xstart, Yend, Color, Line_width, LINE_STYLE_SOLID);
    }
}

/******************************************************************************
function: Use the 8-point method to draw a circle of the
            specified size at the specified position->
parameter:
    X_Center  ：Center X coordinate
    Y_Center  ：Center Y coordinate
    Radius    ：circle Radius
    Color     ：The color of the ：circle segment
    Line_width: Line width
    Draw_Fill : Whether to fill the inside of the Circle
******************************************************************************/
void Paint_DrawCircle(UWORD X_Center, UWORD Y_Center, UWORD Radius,
                      UWORD Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill)
{
    if (X_Center > Paint.Width || Y_Center >= Paint.Height) {
        printf("Paint_DrawCircle Input exceeds the normal display range\r\n");
        return;
    }

    //Draw a circle from(0, R) as a starting point
    int16_t XCurrent, YCurrent;
    XCurrent = 0;
    YCurrent = Radius;

    //Cumulative error,judge the next point of the logo
    int16_t Esp = 3 - (Radius << 1 );

    int16_t sCountY;
    if (Draw_Fill == DRAW_FILL_FULL) {
        while (XCurrent <= YCurrent ) { //Realistic circles
            for (sCountY = XCurrent; sCountY <= YCurrent; sCountY ++ ) {
                Paint_DrawPoint(X_Center + XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//1
                Paint_DrawPoint(X_Center - XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//2
                Paint_DrawPoint(X_Center - sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//3
                Paint_DrawPoint(X_Center - sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//4
                Paint_DrawPoint(X_Center - XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//5
                Paint_DrawPoint(X_Center + XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//6
                Paint_DrawPoint(X_Center + sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//7
                Paint_DrawPoint(X_Center + sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);
            }
            if (Esp < 0 )
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent );
                YCurrent --;
            }
            XCurrent ++;
        }
    } else { //Draw a hollow circle
        while (XCurrent <= YCurrent ) {
            Paint_DrawPoint(X_Center + XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT);//1
            Paint_DrawPoint(X_Center - XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT);//2
            Paint_DrawPoint(X_Center - YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT);//3
            Paint_DrawPoint(X_Center - YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT);//4
            Paint_DrawPoint(X_Center - XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT);//5
            Paint_DrawPoint(X_Center + XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT);//6
            Paint_DrawPoint(X_Center + YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT);//7
            Paint_DrawPoint(X_Center + YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT);//0

            if (Esp < 0 )
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent );
                YCurrent --;
            }
            XCurrent ++;
        }
    }
}

/******************************************************************************
function:	Display nummber
parameter:
    Xstart           ：X coordinate
    Ystart           : Y coordinate
    Nummber          : The number displayed
    Font             ：A structure pointer that displays a character size
	Digit						 : Fractional width
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
#define  ARRAY_LEN 255
void Paint_DrawNum(UWORD Xpoint, UWORD Ypoint, double Nummber,
                   sFONT* Font, UWORD Digit,UWORD Color_Foreground, UWORD Color_Background)
{
    int16_t Num_Bit = 0, Str_Bit = 0;
    uint8_t Str_Array[ARRAY_LEN] = {0}, Num_Array[ARRAY_LEN] = {0};
    uint8_t *pStr = Str_Array;
	int temp = Nummber;
	float decimals;
	uint8_t i;
    if (Xpoint > Paint.Width || Ypoint > Paint.Height) {
        printf("Paint_DisNum Input exceeds the normal display range\r\n");
        return;
    }
   
	if(Digit > 0) {		
		decimals = Nummber - temp;
		for(i=Digit; i > 0; i--) {
			decimals*=10;
		}
		temp = decimals;
		//Converts a number to a string
		for(i=Digit; i>0; i--) {
			Num_Array[Num_Bit] = temp % 10 + '0';
			Num_Bit++;
			temp /= 10;						
		}	
		Num_Array[Num_Bit] = '.';
		Num_Bit++;
	}

	if(Nummber < 0){
        temp = - Nummber;
        Str_Array[Str_Bit] = '-';
        Str_Bit ++;
    }else{
        temp = Nummber;
    }
    if(temp == 0){
        Num_Array[Num_Bit] = temp % 10 + '0';
        Num_Bit++;
    }
    //Converts a number to a string
    while (temp) {
        Num_Array[Num_Bit] = temp % 10 + '0';
        Num_Bit++;
        temp /= 10;
    }
    //The string is inverted
    while (Num_Bit > 0) {
        Str_Array[Str_Bit] = Num_Array[Num_Bit - 1];
        Str_Bit ++;
        Num_Bit --;
    }
    //show
    Paint_DrawString_EN(Xpoint, Ypoint, (const char*)pStr, Font, Color_Foreground, Color_Background);
}