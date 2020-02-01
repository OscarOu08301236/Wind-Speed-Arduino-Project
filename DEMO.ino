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
  {
    if(blueToothSerial.available())
    {
      databuffer[index] = blueToothSerial.read();
      if (databuffer[0] != 'c')
      {
        index = -1;
      }
    }
    else
    {
      index --;
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
  {
    _temp[_index - _start] = _buffer[_index] - '0';
    result = 10*result + _temp[_index - _start];
  }
  return result;
}

int WindDirection()                                                                 
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

void setup()
{
  Serial.begin(9600);
  blueToothSerial.begin(9600); 
}
void loop()
{ 
  double a[5]={0}; 
  int flag=0;
   if (Serial.available())
   {
    delay(100);
    for(int i=0;i<5;i++)
    { 
    while(true)
    {
    char tmp;
    if(Serial.available())tmp=Serial.read();
    else continue;
     if(tmp==' '||tmp=='\n')break;
     a[i]=a[i]*10+tmp-48;
     }
     if(i==4)flag=1;
    }
   }
  if(flag)
  {
  getBuffer(); //Begin!
  /*Serial.write(27,BYTE);
  Serial.print("[2J");
  Serial.write(27,BYTE);
  Serial.print("[H");*/
  Serial.println("\n\n\n");
  Serial.print("Average Wind Speed (One Minute): ");
  Serial.print(WindSpeedAverage());
  Serial.println("km/hr  ");
  Serial.print("Best velocity:");
  Serial.print(a[0]/(a[1]-a[2]));
  Serial.println("km/hr");
  Serial.print("Extra gas:");
  Serial.print((1/2*1.1691*a[3]*a[4]*a[0]/(a[1]-a[2])*a[0]/(a[1]-a[2])*a[1]/3600)-(1/2*1.1691*a[3]*a[4]*(a[0]/(a[1]-a[2])+5)*(a[0]/(a[1]-a[2])+5)*a[1]/3600/7300));
  Serial.println("l");
  Serial.flush();
  delay(2000);
  }
}


