#include <p89v51rd2.h>

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
 
/*********************************RTC HEADER FILE START*********************************/
#define DS1307_ID 0xD0
#define SEC   0x00
#define MIN   0x01
#define HOUR  0x02
#define DATE  0x04
#define MONTH 0x05
#define YEAR  0x06
 
#define  SDA P3_5
#define  SCL P3_4
 
unsigned char DS1307_get(unsigned char);
void timerDateSet(unsigned char, unsigned char, unsigned char);
void timerTimeSet(unsigned char, unsigned char);
unsigned char Send2lcd(unsigned char);
void Rtc_Write(unsigned char,unsigned char);
unsigned char Rtc_Read(unsigned char);
void Rtc_Init();
 
#define TIMER_CTRL_WRITE  0xD0
#define TIMER_CTRL_READ   0xD1

unsigned char d_rtc,datain_rtc,in_rtc,temp_rtc,dat_rtc,flag_rtc;

#define I2C_START_BIT	 \
  SDA = 1;		 \
  SCL = 1;		 \
  SDA = 0;		 \
  SCL = 0;

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
  for (ui1=8; ui1>0;) {	    \
    ui1--;		    \
    CY = (b >> ui1) & 0x1;  \
    SDA = CY;		    \
    SCL=1;		    \
    SCL=0;		    \
  }

#define I2C_GET_BYTE(b)     \
  SDA = 1;		    \
  b = 0;		    \
  for (ui1=0; ui1<8;ui1++) {	    \
    SCL=1;		    \
    b <<= 1;		    \
    b |= SDA;		    \
    SCL=0;		    \
  }

unsigned char DS1307_get(unsigned char addr)
{
  unsigned char ret;
 
//  I2C_START_BIT;
  ret = Rtc_Read(addr);
//  I2C_STOP_BIT;
 
  return ret;
}
 
void
timerTimeSet(unsigned char hour, unsigned char min)
{
//  I2C_START_BIT;

  Rtc_Write(0x01, min);
  Rtc_Write(0x02, hour);
  Rtc_Write(0x00, 0);

//  I2C_STOP_BIT;
}
 
 
void
timerDateSet(unsigned char year, unsigned char month, unsigned char date)
{
//  I2C_START_BIT;

  Rtc_Write(0x04, date);
  Rtc_Write(0x05, month);
  Rtc_Write(0x06, year);

//  I2C_STOP_BIT;
}
 
 
unsigned char
Send2lcd(unsigned char value)
{
  unsigned char buf1,buf2,buf = 0;
 
  buf1 = value & 0xF0; /* Filter for high byte */
  buf1 = (buf1>>4); /* Convert  to ascii code */
 
  buf2 = value & 0x0F; /* Filter for low byte */
 
  buf=(buf1*10)+buf2;
 
  return buf;
}
 
 
void Rtc_Init()//lower order 256 bytes of the chip
{
}
 
 
void
Rtc_Write(unsigned char zig,unsigned char zag)// program to write to EEPROM
{
  signed char ui1;

  I2C_START_BIT;

  do {
    I2C_SEND_BYTE(TIMER_CTRL_WRITE);
    I2C_GET_ACK(CY);
  } while (CY==1);

  do {
    I2C_SEND_BYTE(zig);
    I2C_GET_ACK(CY);
  } while (CY==1);

  do {
    I2C_SEND_BYTE(zag);
    I2C_GET_ACK(CY);
  } while (CY==1);

  CY=0;
  I2C_STOP_BIT;
}

 
unsigned char
Rtc_Read(unsigned char zig)// program to read from EEPROM
{
  signed char ui1;

  I2C_START_BIT;

  do {
    I2C_SEND_BYTE(TIMER_CTRL_WRITE);
    I2C_GET_ACK(CY);
  } while (CY==1);

  do {
    I2C_SEND_BYTE(zig);
    I2C_GET_ACK(CY);
  } while (CY==1);

  I2C_START_BIT;

  do {
    I2C_SEND_BYTE(TIMER_CTRL_READ);
    I2C_GET_ACK(CY);
  } while (CY==1);

  I2C_GET_BYTE(datain_rtc);
  I2C_SEND_ACK(1);
  CY=0;
  I2C_STOP_BIT;

  return(datain_rtc);
}
 
/*********************************RTC HEADER FILE END*********************************/


/********************************* MAIN PROGRAM START*************************************/
unsigned char i,j,a[10],cur=0;
unsigned char sec,hour,min,date,month,year,sec1,hour1,min1,sec2,hour2,min2,hh,mm,ss,dd,mn,yy;
unsigned char count[6]={0x86,0x89,0x8c,0xc6,0xc9,0xcc};

#define set P0_0
#define mov P0_1
#define inc P0_2
#define dec P0_3
#define ent P0_4

void all_disp();
void time_set();

unsigned char dec_hex(unsigned char tt)
{
  if(tt>59)tt+=36;
  else if(tt>49)tt+=30;
  else if(tt>39)tt+=24;
  else if(tt>29)tt+=18;
  else if(tt>19)tt+=12;
  else if(tt>9)tt+=6;
  return(tt);
}
void all_disp()
{
  sec   = DS1307_get(SEC);
  min   = DS1307_get(MIN);
  hour  = DS1307_get(HOUR);
  date  = DS1307_get(DATE);
  month = DS1307_get(MONTH);
  year  = DS1307_get(YEAR);
 
  sec=Send2lcd(sec);
  min=Send2lcd(min);
  hour=Send2lcd(hour);
  date=Send2lcd(date);
  month=Send2lcd(month);
  year=Send2lcd(year);
 
  Lcd8_Decimal2(0x86,date);
  Lcd8_Write(0x88,'-');
  Lcd8_Decimal2(0x89,month);
  Lcd8_Write(0x8b,'-');
  Lcd8_Decimal2(0x8c,year);
 
  Lcd8_Decimal2(0xc6,hour);
  Lcd8_Write(0xc8,'.');
  Lcd8_Decimal2(0xc9,min);
  Lcd8_Write(0xcb,'.');
  Lcd8_Decimal2(0xcc,sec);
}
void main()
{
  Lcd8_Init();
  Lcd8_Display(0x80,"RTC TESTING:    ",16);
  Delay(65000);Delay(65000);
  Lcd8_Command(0x01);
  Lcd8_Display(First_Line,"Date:",5);
  Lcd8_Display(Second_Line,"Time:",5);
  Rtc_Init();
   
  timerDateSet(0x04,0x11,0x11);
  timerTimeSet(0x2,0x3);
 
  while(1)
    {
      all_disp();
      Delay(65000);
//      time_set();
    }
}
void time_set()
{
/*   Lcd8_Display(0x80,"    TIME SET    ",16); */
/*   Lcd8_Display(0xc0,"      MODE      ",16); */
/*   Delay(65000); */
/*   Lcd8_Command(0x01); */
/*   Lcd8_Display(First_Line,"Date:",5); */
/*   Lcd8_Display(Second_Line,"Time:",5); */
/*   all_disp(); */
/*   cur=0; */
/*   while(ent) */
/*     { */
/*       Lcd8_Command(0x0f);Lcd8_Command(count[cur]); */
/*       if(mov==0){while(!mov);cur++;if(cur==6)cur=0;} */
/*       else if(count[cur]==0x86) */
/* 	{ */
/* 	  if(inc==0){while(!inc);date++;if(date>=32)date=1;Lcd8_Decimal2(count[cur],date);} */
/* 	  else if(dec==0){while(!dec);date--;if(date==0xff)date=31;Lcd8_Decimal2(count[cur],date);} */
/* 	} */
 
/*       else if(count[cur]==0x89) */
/* 	{ */
/* 	  if(inc==0){while(!inc);month++;if(month>=13)month=1;Lcd8_Decimal2(count[cur],month);} */
/* 	  else if(dec==0){while(!dec);month--;if(month==0xff)month=12;Lcd8_Decimal2(count[cur],month);} */
/* 	} */
 
/*       else if(count[cur]==0x8c) */
/* 	{ */
/* 	  if(inc==0){while(!inc);year++;if(year>99)year=0;Lcd8_Decimal2(count[cur],year);} */
/* 	  else if(dec==0){while(!dec);year--;if(year==0xff)year=99;Lcd8_Decimal2(count[cur],year);} */
/* 	} */
 
/*       else if(count[cur]==0xc6) */
/* 	{ */
/* 	  if(inc==0){while(inc==0);hour++;if(hour>=24)hour=0;Lcd8_Decimal2(count[cur],hour);} */
/* 	  else if(dec==0){while(dec==0);hour--;if(hour==0)hour=23;Lcd8_Decimal2(count[cur],hour);} */
/* 	} */
 
/*       else if(count[cur]==0xc9) */
/* 	{ */
/* 	  if(inc==0){while(inc==0);min++;if(min>=60)min=0;Lcd8_Decimal2(count[cur],min);} */
/* 	  else if(dec==0){while(dec==0);min--;if(min==0)min=59;Lcd8_Decimal2(count[cur],min);} */
/* 	} */
 
/*       else if(count[cur]==0xcc) */
/* 	{ */
/* 	  if(inc==0){while(inc==0);Lcd8_Command(0x0c);sec++;if(sec>=60)sec=0;Lcd8_Decimal2(count[cur],sec);} */
/* 	  else if(dec==0){while(dec==0);Lcd8_Command(0x0c);sec--;if(sec==0xff)sec=59;Lcd8_Decimal2(count[cur],sec);} */
/* 	} */
/*     } */
 
/*   Lcd8_Command(0x0c); */
/*   Lcd8_Display(0x80,"   TIME  IS     ",16); */
/*   Lcd8_Display(0xc0,"    STORED      ",16); */
/*   Delay(65000);Delay(65000); */

  dd=dec_hex(date);
  mn=dec_hex(month);
  yy=dec_hex(year);
  hh=dec_hex(hour);
  mm=dec_hex(min);
  ss=dec_hex(sec);
 
//  DS1307_settime(hh,mm,ss);
//  DS1307_setdate(dd,mn,yy); 
  Lcd8_Command(0x01);
  Lcd8_Display(First_Line,"Date:",5);
  Lcd8_Display(Second_Line,"Time:",5);
}
 
/********************************* MAIN PROGRAM END*************************************/
