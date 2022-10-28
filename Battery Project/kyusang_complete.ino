#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

//wow!

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(2,OUTPUT);
pinMode(3,OUTPUT);
pinMode(4,OUTPUT);
pinMode(5,OUTPUT);
pinMode(6,OUTPUT);

lcd.init();

lcd.backlight();

}


double VOC;

void loop() {
  // put your main code here, to run repeatedly:
 double Rload=1.2;//부하저항(1)+스위치(0.2) 저항
double V1=analogRead(A0);//저항과 스위치사이
double V2=analogRead(A1);//전원과 스위치사이


double V11=V1*0.004882;//개방전압 저장을 위한 도구에 불과
//스위치 오픈 시 0,닫힐 시 부하저항에 걸리는 전압


double Voc=V2*0.004882;//스위치 오픈 시 개방전압 전역변수에 저장

double Vload=V2*0.004882;//스위치와 부하저항에 걸리는 전압
//스위치 닫힐 시 측정

//스위치 오픈 시 V11이 0라는 게 포인트

if(V11+0.5<Voc)//스위치의 저항이 0.2옴
VOC=Voc;//스위치 오픈 시 개방전압 측정 후 전역변수로 저장

double empty=0.90;
double full=1.68;

double reserve=(VOC-empty)/(full-empty)*100;//잔량

double Rin=Rload*(VOC-Vload)/Vload;//내부저항


Serial.println(V11);
Serial.println(Voc);
Serial.println(VOC);
Serial.println(reserve);
Serial.println(Rin);
delay(300);


if(0>reserve)
{
digitalWrite(2,LOW);
digitalWrite(3,LOW);
digitalWrite(4,LOW);
digitalWrite(5,LOW);
digitalWrite(6,LOW);
}

else if(20>reserve){
digitalWrite(2,HIGH);
digitalWrite(3,LOW);
digitalWrite(4,LOW);
digitalWrite(5,LOW);
digitalWrite(6,LOW);
}
else if(40>reserve){
digitalWrite(3,HIGH);
digitalWrite(2,LOW);

digitalWrite(4,LOW);
digitalWrite(5,LOW);
digitalWrite(6,LOW);}

else if(60>reserve){
digitalWrite(4,HIGH);
digitalWrite(2,LOW);
digitalWrite(3,LOW);

digitalWrite(5,LOW);
digitalWrite(6,LOW);}
else if(80>reserve){
digitalWrite(5,HIGH);
digitalWrite(2,LOW);
digitalWrite(3,LOW);
digitalWrite(4,LOW);

digitalWrite(6,LOW);
}
else{
digitalWrite(2,LOW);
digitalWrite(3,LOW);
digitalWrite(4,LOW);
digitalWrite(5,LOW);
digitalWrite(6,HIGH);}


lcd.setCursor(0,0);//1ㅎ1ㅇ//배열과같은 표기
lcd.print("Rin:");
lcd.setCursor(5,0);//6ㅎ1ㅇ//배열과같은 표기

if(V11<0.5)//스위치 오픈 시
lcd.print("pressbtn");
else
lcd.print(Rin);

delay(1000);

lcd.setCursor(0,1);//1ㅎ2ㅇ
lcd.print("rsv:");
lcd.setCursor(5,1);//6ㅎ2ㅇ//배열과같은 표기

if(reserve<0)
lcd.print("no battery");
else
lcd.print(reserve);

delay(1000);
lcd.clear();
delay(1000);


}
