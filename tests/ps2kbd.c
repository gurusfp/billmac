#include <p89v51rd2.h>  
#include <stdint.h>  
   
void
delayms(uint16_t ms)
{   
  unsigned char i;   
  while(ms--)   
    {   
      for(i = 0; i < 120; i++);   
    }   
}   

/*********************************LCD HEADER FILE START*************************************/
#define First_Line  0x80
#define Second_Line 0xc0
#define Curser_On   0x0f
#define Curser_Off  0x0c
#define Clear_Display 0x01
 
#define LCD_rs P2_0
#define LCD_en P2_1
 
void Lcd8_Init();
void Lcd8_Write(unsigned char,unsigned char);
void Lcd8_Display(unsigned char,const unsigned char*,unsigned int);
void Lcd8_Decimal2(unsigned char,unsigned char);
void Lcd8_Decimal3(unsigned char,unsigned char);
void Lcd8_Decimal4(unsigned char,unsigned int);

# define LCD_PORT(val)       \
  P2_2 = (val & 0x1);	     \
  P2_3 = ((val>>1) & 0x1);   \
  P2_4 = ((val>>2) & 0x1);   \
  P2_5 = ((val>>3) & 0x1)

# define LCD_cmd(var)   \
  LCD_PORT(var>>4);	\
  LCD_rs = 0;		\
  LCD_en = 1;		\
  LCD_en = 0;		\
  LCD_PORT(var);	\
  LCD_rs = 0;		\
  LCD_en = 1;		\
  LCD_en = 0;		\
  delayms(10)

# define LCD_wrnib(var)	\
  LCD_PORT(var);	\
  LCD_rs = 1;		\
  LCD_en = 1;		\
  LCD_en = 0

# define LCD_wrchar(var)\
  LCD_wrnib(var>>4);	\
  LCD_wrnib(var);	\
  delayms(10)

void Lcd8_Init()
{
  delayms(100);
  LCD_wrnib(3);
  delayms(100);
  LCD_wrnib(3);
  delayms(10);
  LCD_wrnib(3);
  delayms(10);
  LCD_wrnib(2);
  delayms(10);

  LCD_cmd(0x38); //to select function set
  LCD_cmd(0x06); //entry mode set
  LCD_cmd(0x0c); //display on
  LCD_cmd(0x01); //clear display
}

void Lcd8_Write(unsigned char com,unsigned char lr)
{
  LCD_cmd(com);
  LCD_wrchar(lr);
}
 
void Lcd8_Display(unsigned char com,const unsigned char *word,unsigned int n)
{
  unsigned char Lcd_i;
 
  for(Lcd_i=0;Lcd_i<n;Lcd_i++)
    { 
      Lcd8_Write(com+Lcd_i,word[Lcd_i]);
    }
}
 
void Lcd8_hex(unsigned char com,unsigned char val)
{
  unsigned int Lcd_t, Lcd_o;
 
  Lcd_t = (val>>4) & 0xF;
  Lcd_o = val & 0xF;
 
  Lcd8_Write(com,((Lcd_t>9)?'a'-10:'0')+Lcd_t);
  Lcd8_Write(com+1,((Lcd_o>9)?'a'-10:'0')+Lcd_o);
}
 
void Lcd8_Decimal2(unsigned char com,unsigned char val)
{
  unsigned int Lcd_hr,Lcd_t,Lcd_o;
 
  Lcd_hr=val%100;
  Lcd_t=Lcd_hr/10;
  Lcd_o=Lcd_hr%10;
 
  Lcd8_Write(com,Lcd_t+0x30);
  Lcd8_Write(com+1,Lcd_o+0x30);
}
 
 
void Lcd8_Decimal3(unsigned char com,unsigned char val)
{
  unsigned int Lcd_h,Lcd_hr,Lcd_t,Lcd_o;
 
  Lcd_h=val/100;
  Lcd_hr=val%100;
  Lcd_t=Lcd_hr/10;
  Lcd_o=Lcd_hr%10;
 
  Lcd8_Write(com,Lcd_h+0x30);
  Lcd8_Write(com+1,Lcd_t+0x30);
  Lcd8_Write(com+2,Lcd_o+0x30);
}
 
void Lcd8_Decimal4(unsigned char com,unsigned int val) 
{
  unsigned int Lcd_th,Lcd_thr,Lcd_h,Lcd_hr,Lcd_t,Lcd_o;
 
  val = val%10000;
  Lcd_th=val/1000;
  Lcd_thr=val%1000;
  Lcd_h=Lcd_thr/100;
  Lcd_hr=Lcd_thr%100;
  Lcd_t=Lcd_hr/10;
  Lcd_o=Lcd_hr%10;
 
  Lcd8_Write(com,Lcd_th+0x30);
  Lcd8_Write(com+1,Lcd_h+0x30);
 
  Lcd8_Write(com+2,Lcd_t+0x30);
  Lcd8_Write(com+3,Lcd_o+0x30);
}
 
/*********************************LCD HEADER FILE END*************************************/
 
//PS/2- Keyboard   Pins
#define PS2_DATA   P3_6
#define PS2_CLK    P3_2

//---------------------
//  Scan keyboard
//---------------------
uint16_t
Scan_Data()
{
  uint8_t Data;
  uint8_t temp, ui1;

  while(PS2_CLK==0);

  Data = 0;
  for (ui1=0; ui1<11; ui1++) {
    while(PS2_CLK==1);
    if ((ui1 > 0) && (ui1 < 9)) {
      temp=PS2_DATA;
      Data |= (temp<<(ui1-1));
    }
    while(PS2_CLK==0);
  }
  PS2_CLK = 0;

  return(Data);
}

/***************************/
/*Main Program Starts Here */
/***************************/
void main()
{
  uint8_t KeyVal, idx;

  Lcd8_Init();
  Lcd8_Display(0x80,"PS2 TESTING:    ",16);
  delayms(100);delayms(100);
  LCD_cmd(0x01);

  delayms(50);
  PS2_CLK = 1;
  PS2_DATA = 1;
  delayms(50);

  idx = 0;
  Lcd8_Display(First_Line,"Scan Code: ", 11);
  while (1) {
    PS2_CLK = 1;
    KeyVal = Scan_Data();
    if (idx > 15) idx = 0;
    Lcd8_hex(Second_Line+idx, KeyVal);
    idx += 2;
  }
}
