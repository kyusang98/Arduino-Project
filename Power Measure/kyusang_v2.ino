//////////////////////////////////////////////////////라이브러리 및 객체 선언
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include<SimpleTimer.h>
SimpleTimer timer;
LiquidCrystal_I2C lcd(0x27, 20, 4);
//////////////////////////////////////////////////////////변수선언
//cal check
float start=0;
float finish=0;
float span=finish-start;

//timer
int i=0;
float Cal_T=1;//몇 ms당 한 번씩 측정할 것인가
int Sampling=3000;// 몇 번 씩 측정해서 평균값을 낼 것인가
float Sampling_rate=1000/Cal_T;


float ACP_sum=0;
float DCP_sum=0;
float ACP_mean=0;
float DCP_mean=0;
float P_ACP_mean=0;
float P_DCP_mean=0;

//ACV
const int ACV_analogIn = A0; //아날로그 입력 PIN
int ACV_RawValue=0;
float AC_Voltage=0;
float Scaled=0;

//DCV
const int DCV_analogIn = A1; //아날로그 입력 PIN
int DCV_RawValue = 0;
float DC_Voltage1 = 0.0; 
float DC_Voltage2 = 0.0;
float DC_Voltage = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;

//Current
const int AC_analogIn = A2; //아날로그 입력 PIN
const int DC_analogIn = A3; //아날로그 입력 PIN  
int mVperAmp = 185;//현재 20A 
                   // 아날로그 입력 mV당 전류 값  
                   // 5A 짜리는 185  
                   // 20A 짜리는 100  
                   // 30A 짜리는 66  

                   
int AC_RawValue= 0;   // 아날로그 값 저장 변수  
int DC_RawValue= 0;   // 아날로그 값 저장 변수  


int ACSoffset = 2500;//mV // 기준 값 0A일때 아날로그 값은 2500mV 이다.  


double ACC_Voltage = 0;   // 계산된 아날로그 값  
double AC_Amps = 0;      // 실제 측정된 전류 값  
double DCC_Voltage = 0;   // 계산된 아날로그 값  
double DC_Amps = 0;      // 실제 측정된 전류 값  

//power
double ACP =0;
double DCP =0;

//AC Apparent power
double AC_Apparent=0;


//Efficiency
double Eff=0;
double EFFP=0;

////////////////////////////////////////////////////////////타이머로 발동시킬 함수
//Functions

//Print
void PrintWork()
{
  Serial.print("Span:");   
  Serial.print(span);//V
  Serial.print("\t"); 
  Serial.print("Sampling:");   
  Serial.print(i);//V
  Serial.print("\t");  
  Serial.print("AC_Power:");   
  Serial.print(P_ACP_mean);//V
  Serial.print("\t");  
  Serial.print("DC_power:");   
  Serial.print(P_DCP_mean);//V
  Serial.print("\t");
  Serial.print("EFFP:");   
  Serial.println(EFFP);//V
  
  }

//LCD  
void LCDWork()
{
  
  lcd.setCursor(0,0);
  lcd.print("AC Power:");
  lcd.print(P_ACP_mean);
  lcd.print("W");
  lcd.setCursor(0,1);
  lcd.print("DC Power:");
  lcd.print(P_DCP_mean); 
  lcd.print("W");
  lcd.setCursor(0,2);
  lcd.print("Efficiency:");
  lcd.print(EFFP);//A 
  lcd.print("%");
  lcd.setCursor(0,3);
  lcd.print("S rate:");
  lcd.println(Sampling_rate);//A 
  lcd.print("Hz");
  
  
  }

 
void calculate()
{
  start=millis();
 //Voltage
  ACV_RawValue = analogRead(ACV_analogIn);  
  AC_Voltage = (ACV_RawValue / 1024.0) * 250;//V
  Scaled=(AC_Voltage-125)*10;

  DCV_RawValue = analogRead(A1);
  DC_Voltage1 = (DCV_RawValue * 5.0) / 1024.0;  //전압값을 계산해주는 공식입니다.
  DC_Voltage = DC_Voltage1 / ( R2 / ( R1 + R2) );

  //Current
  AC_RawValue = analogRead(AC_analogIn);  
  ACC_Voltage = (AC_RawValue / 1024.0) * 5000;//mV 
  AC_Amps =((ACC_Voltage-ACSoffset) / mVperAmp)*0.055;//A 0.055=12/220

  DC_RawValue = analogRead(DC_analogIn);  
  DCC_Voltage = (DC_RawValue / 1024.0) * 5000;//mV 
  DC_Amps = ((DCC_Voltage-ACSoffset) / mVperAmp);//A 

  //power

  ACP=(float)Scaled*AC_Amps;
  DCP=(float)DC_Voltage*DC_Amps;
  
  ACP_sum+=ACP;
  DCP_sum+=DCP;
  ACP_mean=(float)ACP_sum/Sampling;
  DCP_mean=(float)DCP_sum/Sampling;  
  Eff=(float)P_DCP_mean/P_ACP_mean;
  if (Eff<0)
  {
    Eff=0;
  }
  EFFP=Eff*100;


  //calculate함수가 발동될 때마다 i가 1씩 증가하며 Sampling 개수를 
  //넘어가면 i와 ACP_sum,DCP_sum를 0으로 초기화시킨다.
  i++;
  if(i>Sampling)
  {
  i=0;
  ACP_sum=0;
  DCP_sum=0;
  }
  //샘플링갯수 만큼 연산했을 때의 평균을 저장한다
  //이것을 출력한다.
  if(i==Sampling)
  {
  P_ACP_mean=ACP_mean;
  P_DCP_mean=DCP_mean; 
  }
  finish=millis();
  span=finish-start;
  }
  
//////////////////////////////////////////////////////////


void setup() {
  // put your setup code here, to run once:
Serial.begin (9600);

lcd.begin(); 
lcd.backlight();

timer.setInterval(100,PrintWork);
timer.setInterval(1000,LCDWork);
timer.setInterval(Cal_T,calculate);

}

////////////////////////////////////////////////////////////////


void loop() {
  // put your main code here, to run repeatedly:

  timer.run();
}
