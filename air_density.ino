#include <Wire.h>  
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#include <SoftwareSerial.h>
#define RxD 2
#define TxD 3
SoftwareSerial blueToothSerial(RxD,TxD); 
char                 databuffer[35];
double               temp;

void getBuffer()                                                                    
{
  int index;
  for (index = 0;index < 35;index ++)
  {if(blueToothSerial.available())
    {databuffer[index] = blueToothSerial.read();
      if (databuffer[0] != 'c')
      {index = -1;
      }
    }
    else
    {index --;
    }
  }
  char c;
  while(blueToothSerial.available()>0)c=blueToothSerial.read();
}

int transCharToInt(char *_buffer,int _start,int _stop)                            
{
  int _index;
  int result = 0;
  int num = _stop - _start + 1;
  int _temp[num];
  for (_index = _start;_index <= _stop;_index ++)
  {_temp[_index - _start] = _buffer[_index] - '0';
    result = 10*result + _temp[_index - _start];
  }
  return result;
}

float WindDirection()                                                                 
{
  return transCharToInt(databuffer,1,3);
}

float WindSpeedAverage()                                                            
{
  temp = 3600/1000*0.44704 * transCharToInt(databuffer,5,7);
  return temp;
}

float WindSpeedMax()                                                                
{
  temp = 3.6*0.44704 * transCharToInt(databuffer,9,11);
  return temp;
}

float Temperature()                                                         
  temp = (transCharToInt(databuffer,13,15) - 32.00) * 5.00 / 9.00;
  return temp;
}

float BarPressure()                                                         
{
  temp = transCharToInt(databuffer,28,32);
  return temp / 10.00;
}

void setup() {
  Serial.begin(9600); 
  blueToothSerial.begin(9600);  
  lcd.begin(16, 2);      
  for(
    int i = 0; i < 3; i++) {
    lcd.backlight(); 
    delay(250);
    lcd.noBacklight(); 
    delay(250);
  }
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello, user!");
  delay(8000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Please type in");
  lcd.setCursor(0, 1);
  lcd.print("distance, time");
}

void loop() {
float p=1.293*BarPressure()/1013.25*273.15/(Temperature()+273.15);
  if (Serial.avaiable())                        
 {
    delay(100);
    lcd.clear();
    double a[3]={0};
      for(int i=0;i<3;i++){
while(true){
char tmp;
if(Serial.available())tmp=Serial.read();
else break;
     if(tmp==' '||tmp=='\n')break;
     a[i]=a[i]*10+tmp-48;}}
    lcd.setCursor(0, 0);
    lcd.print(a[0]/(a[1]-a[2]));
    lcd.setCursor(0, 1);
    lcd.print("km/hr");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("the amount of extra gas:");
    lcd.setCursor(0, 1);
lcd.print(((1/2*p*0.35*182*148*(a[0]/(a[1]-a[2]))*(a[0]/(a[1]-a[2]))*a[1])/3600)
-1/2*p*0.35*182*148*((a[0]/(a[1]-a[2]))+5)*((a[0]/(a[1]-a[2])+5))*a[1])/3600)/7300);               
}
}
