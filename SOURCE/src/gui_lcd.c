/************************** Start of file ***************************/
#include "gui_lcd.h"

void LcdSendCommand(uint16_t reg){
  LCD_REG = reg; 
}

void LcdSendData(uint16_t data){
  LCD_DATA = data;
}

/********************************************************************/
void Gui_Draw_Pixel(uint16_t color, uint16_t size){
  for(uint16_t i = 0x00; i < size; i++){
    LCD_DATA = color;
  }
}

///********************************************************************/
//void Gui_Draw_Line (uint16_t x, uint16_t y, uint16_t x_end, uint16_t y_end, uint8_t size, uint16_t color){
//  int deltaX = abs(x_end - x);
//  int deltaY = abs(y_end - y);
//  int signX = x < x_end ? 0x01 : -1;
//  int signY = y < y_end ? 0x01 : -1;
//  int error = deltaX - deltaY;
//  int error2 = 0x00;
//  for (;;)
//  {
//    Gui_Draw_Fill_Rectangle(x, y, size, size, color);
//    if(x == x_end && y == y_end) break;
//    error2 = error * 0x02;
//    if(error2 > -deltaY)
//    {
//      error -= deltaY;
//      x += signX;
//    }
//    if(error2 < deltaX)
//    {
//      error += deltaX;
//      y += signY;
//    }
//  }
//}

///********************************************************************/
//void Gui_Draw_HLine(uint16_t x, uint16_t y, uint16_t length, uint16_t size, uint16_t color)
//{
//  Hx8347i_Set_Area(x, y, length, size);
//  Gui_Draw_Pixel(color, length * size);
//  HX8347I_CS_HIGHT;
//}

///********************************************************************/
//void Gui_Draw_VLine(uint16_t x, uint16_t y, uint16_t length, uint16_t size, uint16_t color)
//{
//  Hx8347i_Set_Area(x, y, size, length);
//  Gui_Draw_Pixel(color, length * size);
//  HX8347I_CS_HIGHT;
//}

///********************************************************************/
//void Gui_Draw_Rectangle(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint8_t size, uint16_t color)
//{
//  Gui_Draw_HLine(x, y, length, size, color);
//  Gui_Draw_HLine(x, y + width, length, size, color);
//  Gui_Draw_VLine(x, y, width, size, color);
//  Gui_Draw_VLine(x + length - size, y, width, size, color);
//}

///********************************************************************/
void Gui_Draw_Fill_Rectangle(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t color){
  LcdSetWindows(x, y, length, width);
  Gui_Draw_Pixel(color, length * width);
}

///********************************************************************/
//void Gui_Draw_Triangle( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t size, uint16_t color)
//{
//  Gui_Draw_Line( x1, y1, x2, y2, color, size);
//  Gui_Draw_Line( x2, y2, x3, y3, color, size);
//  Gui_Draw_Line( x3, y3, x1, y1, color, size);
//}

///********************************************************************/
//void Gui_Draw_Circle(uint16_t x, uint16_t y, uint8_t radius, uint8_t fill, uint8_t size, uint16_t color)
//{
//  int a_, b_, P;
//  a_ = 0x00;
//  b_ = radius;
//  P = 0x01 - radius;
//  while (a_ <= b_)
//  {
//    if(fill == 0x01)
//    {
//      Gui_Draw_Fill_Rectangle(x - a_, y - b_, 0x02 * a_ + 0x01, 0x02 * b_ + 0x01, color);
//      Gui_Draw_Fill_Rectangle(x - b_, y - a_, 0x02 * b_ + 0x01, 0x02 * a_ + 0x01, color);
//    }
//    else
//    {
//      Gui_Draw_Fill_Rectangle(a_ + x, b_ + y, size, size, color);
//      Gui_Draw_Fill_Rectangle(b_ + x, a_ + y, size, size, color);
//      Gui_Draw_Fill_Rectangle(x - a_, b_ + y, size, size, color);
//      Gui_Draw_Fill_Rectangle(x - b_, a_ + y, size, size, color);
//      Gui_Draw_Fill_Rectangle(b_ + x, y - a_, size, size, color);
//      Gui_Draw_Fill_Rectangle(a_ + x, y - b_, size, size, color);
//      Gui_Draw_Fill_Rectangle(x - a_, y - b_, size, size, color);
//      Gui_Draw_Fill_Rectangle(x - b_, y - a_, size, size, color);
//    }
//    if (P < 0x00 )
//    {
//      P = (P + 0x03) + (0x02 * a_);
//      a_++;
//    }
//    else
//    {
//      P = (P + 0x05) + (0x02 * (a_ - b_));
//      a_++;
//      b_--;
//    }
//  }
//}

///********************************************************************/
//void Gui_Draw_Circle_Helper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint8_t size, uint16_t color)
//{
//  int16_t f = 0x01 - r;
//  int16_t ddF_x = 0x01;
//  int16_t ddF_y = -2 * r;
//  int16_t x = 0x00;
//  int16_t y = r;
//  while (x < y)
//  {
//    if (f >= 0x00)
//    {
//      y--;
//      ddF_y += 0x02;
//      f += ddF_y;
//    }
//    x++;
//    ddF_x += 0x02;
//    f += ddF_x;
//    if (cornername & 0x04)
//    {
//      Gui_Draw_Fill_Rectangle(x0 + x, y0 + y, size, size, color);
//      Gui_Draw_Fill_Rectangle(x0 + y, y0 + x, size, size, color);
//    }
//    if (cornername & 0x02)
//    {
//      Gui_Draw_Fill_Rectangle(x0 + x, y0 - y, size, size, color);
//      Gui_Draw_Fill_Rectangle(x0 + y, y0 - x, size, size, color);
//    }
//    if (cornername & 0x08)
//    {
//      Gui_Draw_Fill_Rectangle(x0 - y, y0 + x, size, size, color);
//      Gui_Draw_Fill_Rectangle(x0 - x, y0 + y, size, size, color);
//    }
//    if (cornername & 0x01)
//    {
//      Gui_Draw_Fill_Rectangle(x0 - y, y0 - x, size, size, color);
//      Gui_Draw_Fill_Rectangle(x0 - x, y0 - y, size, size, color);
//    }
//  }
//}

///********************************************************************/
//void Gui_Draw_Round_Rect(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint8_t size, uint16_t color)
//{

//  Gui_Draw_HLine(x + r, y, length - 0x02 * r, size, color);                 // Top
//  Gui_Draw_HLine(x + r, y + width - 0x01, length - 0x02 * r, size, color);  // Bottom
//  Gui_Draw_VLine(x, y + r, width - 0x02 * r, size, color);                  // Left
//  Gui_Draw_VLine(x + length - 0x01, y + r, width - 0x02 * r, size, color);  // Right

//  Gui_Draw_Circle_Helper(x + r, y + r, r, 0x01, size, color);
//  Gui_Draw_Circle_Helper(x + length - r - 0x01, y + r, r, 0x02, size, color);
//  Gui_Draw_Circle_Helper(x + length - r - 0x01, y + width - r - 0x01, r, 0x04, size, color);
//  Gui_Draw_Circle_Helper(x + r, y + width - r - 0x01, r, 0x08, size, color);
//}

///********************************************************************/
//void Gui_Draw_Fill_Circle_Helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color) 
//{
//  int16_t f = 0x01 - r;
//  int16_t ddF_x = 0x01;
//  int16_t ddF_y = - 0x02 * r;
//  int16_t x = 0x00;
//  int16_t y = r;

//  while (x < y)
//  {
//    if (f >= 0x00)
//    {
//      y--;
//      ddF_y += 0x02;
//      f += ddF_y;
//    }
//    x++;
//    ddF_x += 0x02;
//    f += ddF_x;

//    if (cornername & 0x1)
//    {
//      Gui_Draw_VLine(x0 + x, y0 - y, 0x02 * y + 0x01 + delta, 0x01, color);
//      Gui_Draw_VLine(x0 + y, y0 - x, 0x02 * x + 0x01 + delta, 0x01, color);
//    }
//    if (cornername & 0x2)
//    {
//     Gui_Draw_VLine(x0 - x, y0 - y, 0x02 * y + 0x01 + delta, 0x01, color);
//     Gui_Draw_VLine(x0 - y, y0 - x, 0x02 * x + 0x01 + delta, 0x01, color);
//    }
//  }
//}

///********************************************************************/
//void Gui_Draw_Fill_Round_Rect(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint16_t color)
//{
//  Gui_Draw_Fill_Rectangle(x + r, y, length - 0x02 * r, width, color);
//  Gui_Draw_Fill_Circle_Helper(x + length - r - 0x01, y + r, r, 0x01, width - 0x02 * r - 0x01, color);
//  Gui_Draw_Fill_Circle_Helper(x + r, y + r, r, 0x02, width - 0x02 * r - 0x01, color);
//}

////void Gui_Write_Img(uint8_t type, uint16_t ink, uint16_t paper)
////{
////  switch(type)
////  {
////    case 0x00:
////      SPI1->DR = 0x0000;
////      while(!(SPI1->SR & SPI_SR_RXNE));
////      GPIOC->ODR = SPI1->DR;     
////      HX8347I_WR_LOW;
////      HX8347I_WR_HIGHT;
////    break;
////    case 0x01:
////      SPI1->DR = 0x0000;
////      while(!(SPI1->SR & SPI_SR_RXNE));
////      uint16_t d = SPI1->DR;
////      for(uint8_t z = 0x00; 0x10 < z; z++)
////      {
////        if(d & 0x80)
////        {
////          Gui_Draw_Pixel(ink, 0x01);
////        }else{
////          Gui_Draw_Pixel(paper, 0x01);
////        }
////      d <<= 0x01;
////      }
////    break;
////    default:
////    break;
////  }
////}

///********************************************************************/
//void Gui_Load_Color_Img(uint16_t x, uint16_t y, uint8_t img)
//{
//  uint8_t adr, flesh_h, flesh_l;
//  uint16_t width, height;
//  uint32_t i;
//  
//  W25Qxx_Read_Page(w25qxx.conf_img + (img / 0x10));
//  adr = (img % 0x10) * 0x10;
//  flesh_h = w25qxx.read_buff[adr];
//  flesh_l = w25qxx.read_buff[adr + 0x01];
//  width = w25qxx.read_buff[adr + 0x02] << 0x08;
//  width |= w25qxx.read_buff[adr + 0x03];
//  height = w25qxx.read_buff[adr + 0x04] << 0x08;
//  height |= w25qxx.read_buff[adr + 0x05];
//  i = (width * height);
//  Hx8347i_Set_Area(x, y, width, height);
//  W25Qxx_CS_LOW;
//  W25Qxx_W_R(CMD_FAST_READ);
//  W25Qxx_W_R(flesh_h);
//  W25Qxx_W_R(flesh_l);
//  W25Qxx_W_R(0x00);
//  W25Qxx_W_R(0x00);
//  SPI1->CR1 |= SPI_CR1_DFF;
//  while(i--)
//  {
//    SPI1->DR = 0x0000;
//    while(!(SPI1->SR & SPI_SR_RXNE));
//    GPIOC->ODR = SPI1->DR;     
//    HX8347I_WR_LOW;
//    HX8347I_WR_HIGHT;
//  }
//  SPI1->CR1 &= ~SPI_CR1_DFF;
//  W25Qxx_CS_HIGHT;
//  HX8347I_CS_HIGHT;
//}

///********************************************************************/
//void Gui_Load_Mono_Img(uint16_t x, uint16_t y, uint8_t img, uint16_t ink, uint16_t paper)
//{
//  uint8_t adr, flesh_h, flesh_l;
//  uint16_t width, height;
//  uint32_t i;
//  
//  W25Qxx_Read_Page(w25qxx.conf_img + (img / 0x10));
//  adr = (img % 0x10) * 0x10;
//  flesh_h = w25qxx.read_buff[adr];
//  flesh_l = w25qxx.read_buff[adr + 0x01];
//  width = w25qxx.read_buff[adr + 0x02] << 0x08;
//  width |= w25qxx.read_buff[adr + 0x03];
//  height = w25qxx.read_buff[adr + 0x04] << 0x08;
//  height |= w25qxx.read_buff[adr + 0x05];
//  i = (width * height);
//  Hx8347i_Set_Area(x, y, width, height);
//  W25Qxx_CS_LOW;
//  W25Qxx_W_R(CMD_FAST_READ);
//  W25Qxx_W_R(flesh_h);
//  W25Qxx_W_R(flesh_l);
//  W25Qxx_W_R(0x00);
//  W25Qxx_W_R(0x00);
//  SPI1->CR1 |= SPI_CR1_DFF;
//  while(i--)
//  {
//    SPI1->DR = 0x0000;
//    while(!(SPI1->SR & SPI_SR_RXNE));
//    uint16_t d = SPI1->DR;
//    for(uint8_t z = 0x00; 0x10 < z; z++)
//    {
//      if(d & 0x80)
//      {
//        Gui_Draw_Pixel(ink, 0x01);
//      }else{
//        Gui_Draw_Pixel(paper, 0x01);
//      }
//      d <<= 0x01;
//    }    
//  }
//  SPI1->CR1 &= ~SPI_CR1_DFF;
//  W25Qxx_CS_HIGHT;
//  HX8347I_CS_HIGHT;
//}
//#pragma O1
void LcdSetWindows(uint16_t x, uint16_t y, uint16_t width, uint16_t height){

//  if(hx8347i.width < width) width = hx8347i.width;
//  if(hx8347i.height < height) height = hx8347i.height;
//  if(hx8347i.width < (x + width)) x = hx8347i.width - width;
//  if(hx8347i.height <(y + height)) y = hx8347i.height - height;

  LcdSendCommand(0x2A); 
  LcdSendData(x >> 8);
  LcdSendData(x);
  LcdSendData((x + width - 0x01) >> 8);
  LcdSendData(x + width - 0x01);
  LcdSendCommand(0x2B); 
  LcdSendData(y >> 8);
  LcdSendData(y);
  LcdSendData((y + height - 0x01) >> 8);
  LcdSendData(y + height - 0x01);
  LcdSendCommand(0x2C);
}
/********************************************************************/
void Gui_Print_Char(uint16_t x, uint16_t y, uint16_t ink, uint16_t paper, uint8_t zoom, uint8_t str)
{
  uint8_t ch_width = fonts_Descriptors[(str - 0x21)][0x00];
  uint16_t ch_address = (fonts_Descriptors[(str - 0x21)][0x01]) * CH_HEIGHT;
  uint8_t x1, x2, x1i, x2i, zy, char_c = 0x00;
  uint8_t z = zoom & 0x0F;
  uint8_t zx = (zoom >> 0x04);
  uint8_t char_y;
  zy = z;
  if(ch_width > 0x08)
  {
    x1 = 0x08;
    x2 = ch_width - 0x08;
  }else{
    x1 = ch_width;
    x2 = 0x00;
  }
  LcdSetWindows(x, y, ((ch_width + 0x02) * zx), (CH_HEIGHT * z));
  for(char_y = 0x00; char_y < CH_HEIGHT; char_y++)
  {
    while(zy--)
    {
      char_c = fonts_Bitmaps[ch_address];
      Gui_Draw_Pixel(paper, zx);
      for(x1i = 0x00; x1i < x1; x1i++)
      {
        if(char_c & 0x80)
        {
          Gui_Draw_Pixel(ink, zx);
        }else{
          Gui_Draw_Pixel(paper, zx);
        }
        char_c<<= 0x01;
      }
      if(ch_width > 0x08)
      {
        char_c = fonts_Bitmaps[ch_address+1];
        for(x2i = 0x00; x2i < x2; x2i++)
        {
          if(char_c & 0x80)
          {
            Gui_Draw_Pixel(ink, zx);
          }else{
            Gui_Draw_Pixel(paper, zx);
          }
          char_c <<= 0x01;
        }
      }
      Gui_Draw_Pixel(paper, zx);
    }
    ch_address++;
    if(ch_width > 0x08)ch_address++;
    zy = z;
  }
}

/********************************************************************/
void Gui_Print_Chars(uint16_t x, uint16_t y, uint16_t ink, uint16_t paper, uint8_t zoom, const uint8_t str[])
{   
  uint8_t zy = zoom & 0x0F;
  uint8_t zx = (zoom >> 0x04);
  uint8_t ch_x;
  uint8_t st;
  while (*str != 0x00)
  {
    st = *str;
    if (st == 0x0A) 
    {
      str++;
      y += (CH_HEIGHT * zy);
      x = 0x00;
      st = *str;
    }
    if (st == 0x20) 
    {
      Gui_Draw_Fill_Rectangle(x, y, (0x04 * zx), (CH_HEIGHT * zy), paper);
      str++;
      x += (0x02 * zx) + 0x02;
      st = *str;
    }
    if (st == 0xD0)
    {
      str++;
      st = *str;
      st -= 0x11;
    }
    if (st == 0xD1)
    {
      str++;
      st = *str;
      st += 0x30;
    }
    ch_x = fonts_Descriptors[(st - 0x21)][0x00];
    if ((x + (ch_x * zx)) > 480) 
    {
      y += (CH_HEIGHT * zy);
      x = 0x00;
    }
    Gui_Print_Char(x, y, ink, paper, zoom, st);
    x += (ch_x * zx) + 0x02;
    str++;
  }
}
