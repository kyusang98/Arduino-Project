#include <SimpleTimer.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);// I2C LCD 객체 선언

SimpleTimer timer;

boolean CountOverV=0;
boolean MosfetG=1;


float BVoltage;//배터리 전압상태
float BCurrent1;//배터리 전류상태 요소하나
//float BCurrent=0;//배터리 전류상태, 평균값
float CValue;

void DetectOverV()
{
  if(CountOverV>4)
{
  exit();
  }
  else
  {
    if(BVoltage>4.2)
    {
    CountOverV++;
    delay(10000);

    }

  }
  }

void LCDWork()
{
lcd.setCursor(0,0);//1열1행//배열과 반대 표기
lcd.print("volt:");
lcd.setCursor(6,0);//7열1행//배열과 반대 표기
lcd.print(BVoltage);

lcd.setCursor(0,1);//1ㅇ2ㅎ
lcd.print("curr:");
lcd.setCursor(5,1);//6ㅇ2ㅎ
lcd.print(BCurrent1);


}
void LCDc()
{lcd.clear();}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
pinMode(A0,INPUT);//배터리 전류측정용,ACS712에서 정보받음
pinMode(A1,INPUT);//배터리 전압측정용,배터리의 플러스단자쪽
pinMode(A2,INPUT);//배터리 전압측정용,배터리 마이너스 단자측



timer.setInterval(1000,DetectOverV);
timer.setInterval(1000,LCDWork);
timer.setInterval(2000,LCDc);

// initialize the LCD
  lcd.begin(); // lcd를 사용을 시작합니다.
  lcd.backlight(); // backlight를 On 시킵니다.

}




void loop() {
  // put your main code here, to run repeatedly:
//0.004882는 5V를 1024로 나눈 값!,analogRead값은 입력된 값을 
//1024칸으로 나누고 칸 수로 입력값을 표현한다.
//digitalWrite(12,MosfetG);//모스펫의 G에는 항시 신호를 쏴준다.
BVoltage=(analogRead(A1)-analogRead(A2))*0.004882;
CValue=(analogRead(A0)-511)*0.004882;//ACS712에서 받은 값
BCurrent1=((CValue)/(-0.185))*1000;//2.5는 5V의 절반,5는 5A짜리ACS712
//float BCurrent2=0;//배터리 전류상태, 평균값


timer.run();
}
