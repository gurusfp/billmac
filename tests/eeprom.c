#include <p89v51rd2.h>  
#include <stdint.h>  
   
#define EEPROM_CTRL_READ   0xA1
#define EEPROM_CTRL_WRITE  0xA0
#define MAX_ADDR  0x7F
    
unsigned char dis_code[] = {0x7e,0xbd,0xdb,0xe7,0xdb,0xbd,0x7e,0xff};   
  
    
#define SDA P3_5
#define SCL P3_4

void delayms(uint16_t ms);   
    
#define I2C_START_BIT	 \
  SDA = 1;		 \
  SCL = 1;		 \
  SDA = 0;		 \
  SCL = 0

#define I2C_STOP_BIT	 \
  SDA = 0;		 \
  SCL = 1;		 \
  SDA = 1

#define I2C_GET_ACK(a)	   \
  SDA = 1;		   \
  SCL = 1;		   \
  a = SDA;		   \
  SCL = 0


#define I2C_SEND_ACK(a)	   \
  SDA = a;		   \
  SCL = 1;		   \
  SCL = 0

#define I2C_SEND_BYTE(b)    \
  for (ui3=8; ui3>0;) {	    \
    ui3--;		    \
    CY = (b >> ui3) & 0x1;  \
    SDA = CY;		    \
    SCL=1;		    \
    SCL=0;		    \
  }

#define I2C_GET_BYTE(b)     \
  SDA = 1;		    \
  b = 0;		    \
  for (ui3=0; ui3<8;ui3++) {	    \
    SCL=1;		    \
    b <<= 1;		    \
    b |= SDA;		    \
    SCL=0;		    \
  }

void
eepromWriteBytes(uint16_t addr, uint8_t *val, uint8_t n_bytes)
{
  uint8_t ui2, ui3;

  for (; n_bytes>0; n_bytes--, val++, addr++) {
    I2C_START_BIT;
    I2C_SEND_BYTE(EEPROM_CTRL_WRITE);   
    I2C_GET_ACK(CY);

#if EP24C > 2
    ui2 = addr>>8;
    I2C_SEND_BYTE(ui2);
    I2C_GET_ACK(CY);
#endif

    ui2 = (uint8_t)addr;
    I2C_SEND_BYTE(ui2);
    I2C_GET_ACK(CY);

    I2C_SEND_BYTE(val[0]);
    I2C_GET_ACK(CY);
    I2C_STOP_BIT;

    delayms(10);
  }
}   
    
void
eepromReadBytes(uint16_t addr, uint8_t *val, uint8_t n_bytes)
{   
  uint8_t ui2, ui3;

  for (; n_bytes>0; n_bytes--, val++, addr++) {
    I2C_START_BIT;

    I2C_SEND_BYTE(EEPROM_CTRL_WRITE);   
    I2C_GET_ACK(CY);

#if EP24C > 2
    ui2 = addr>>8;
    I2C_SEND_BYTE(ui2);
    I2C_GET_ACK(CY);
#endif

    ui2 = (uint8_t)addr;
    I2C_SEND_BYTE(ui2);
    I2C_GET_ACK(CY);

    I2C_START_BIT;
    I2C_SEND_BYTE(EEPROM_CTRL_READ);   
    I2C_GET_ACK(CY);

    I2C_GET_BYTE(ui2);
    I2C_SEND_ACK(1);
    val[0] = ui2;

    I2C_STOP_BIT;
  }
}   
    
void delayms(uint16_t ms)
{   
  unsigned char i;   
  while(ms--)   
  {   
  for(i = 0; i < 120; i++);   
  }   
}   

/*********************************LCD HEADER FILE START*************************************/
#define First_Line 0x80
#define Second_Line 0xc0
#define Curser_On  0x0f
#define Curser_Off 0x0c
#define Clear_Display 0x01
#define Data_Port P1
 
#define Lcd_rs P2_0
#define Lcd_rw P2_1
#define Lcd_en P2_2
 
void Lcd8_Init();
void Lcd8_Command(unsigned char);
void Lcd8_Write(unsigned char,unsigned char);
void Lcd8_Display(unsigned char,const unsigned char*,unsigned int);
void Lcd8_Decimal2(unsigned char,unsigned char);
void Lcd8_Decimal3(unsigned char,unsigned char);
void Lcd8_Decimal4(unsigned char,unsigned int);
void Delay(unsigned int);
void del();
void Lcd8_Init()
{
  Lcd8_Command(0x38); //to select function set
  Lcd8_Command(0x06); //entry mode set
  Lcd8_Command(0x0c); //display on
  Lcd8_Command(0x01); //clear display
}
 
void Lcd8_Command(unsigned char com)
{
  Data_Port=com;
  Lcd_en=1;
  Lcd_rs=Lcd_rw=0;
  Delay(125);
  Lcd_en=0;
  Delay(125);
}
 
void Lcd8_Write(unsigned char com,unsigned char lr)
{
  Lcd8_Command(com);
 
  Data_Port=lr; // Data 
  Lcd_en=Lcd_rs=1;
  Lcd_rw=0;
  Delay(125);
  Lcd_en=0;
  Delay(125);
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
 
void Delay(unsigned int del)
{
  while(del--);
}
 
void del()
{
  Delay(65000);Delay(65000);
}
 
/*********************************LCD HEADER FILE END*************************************/
 
void
main(void)   
{   
  unsigned char i, ui2, oval[8];   

  Lcd8_Init();
  Lcd8_Display(0x80,"RTC TESTING:    ",16);
  Delay(65000);Delay(65000);
  Lcd8_Command(0x01);
  Lcd8_Display(First_Line,"Exp:",4);
  Lcd8_Display(Second_Line,"Obt:",4);

  SDA = 1;   
  SCL = 1;   
  
  eepromWriteBytes(0, dis_code, 8);

  eepromReadBytes(0, oval, 8);
  for(i = 0 ; i < 8; i++) {
    ui2 = oval[i];
    Lcd8_hex(0xc6, ui2);
    delayms(1000);   
  }

}   
