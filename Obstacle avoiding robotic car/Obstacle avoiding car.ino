#include<LiquidCrystal.h>
#include <Servo.h>
int triggerpin=3,echopin=2,servopin=A5,servopos=512;
int rpin=A1,gpin=A2,count=0;
float dist;
int mot1in1=5,mot1in2=4,mot1pin=A4;
int mot2in1=7,mot2in2=8,mot2pin=A3;
float mot1speed,mot2speed;
int tiltval, tiltpin=A0;
String m1="%%ALERT%%";
String m2="%%SAFE%%";
String m3="%%WAIT%%";
Servo s1;
LiquidCrystal lcd(6,9,10,11,12,13);
void setup()
{
  pinMode(triggerpin,OUTPUT);
  pinMode(echopin,INPUT);
  pinMode(rpin, OUTPUT);
  pinMode(gpin, OUTPUT);
  pinMode(mot1in1, OUTPUT);
  pinMode(mot1in2, OUTPUT);
  pinMode(mot1pin, OUTPUT);
  pinMode(mot2in2, OUTPUT);
  pinMode(mot2in1, OUTPUT);
  pinMode(mot2pin, OUTPUT);
  pinMode(tiltpin, INPUT);
  s1.attach(servopin);
  lcd.begin(16,2);
  Serial.begin(9600);
}


void loop()
{
  s1.write(90);
  dist= distt(triggerpin, echopin);
  msg(dist);   
  tiltcheck();
  if (dist>=50)
  {
    forward();
  }
  else 
  {
    overcome();
  }
}


void overcome()
{
  stop();
  analogWrite(gpin,1023);
  analogWrite(rpin,1023);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(m3);
  delay(2000);
  s1.write(0);
  tiltcheck();
  delay(4000);
  dist= distt(triggerpin, echopin);
  if(dist>=50)
  {
    tiltcheck();
    right();
    delay(3000);
    forward();
    delay(5000);
    left();
    delay(3000);
    forward();
  }
  else if (dist<50)
  {
    s1.write(180);
    delay(4000);
    dist= distt(triggerpin, echopin);
    if(dist>=50)
  	{
      tiltcheck();
      left();
      delay(3000);
      forward();
      delay(5000);
      right();
      delay(3000);
      forward();
    }
    else
  	{
      while(dist<=50)
      {
      	backward();
        dist= distt(triggerpin, echopin);
      }
    }
  }
  count++;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(m2);
  delay(2000);
  s1.write(90);
  lcd.setCursor(0,1);
  lcd.print("obj overcome- ");
  lcd.println(count);
  tiltcheck();
}

float distt(int a, int b)
{
  digitalWrite(a, LOW);
  delayMicroseconds(20);
  digitalWrite(a, HIGH);
  delayMicroseconds(20);
  digitalWrite(a, LOW);
  int p;
  p=pulseIn(b,HIGH);
  float c;
  c=p*343./20000.;
  return(c);
}

void msg(float a)
{
  if (a<50)
  {
    analogWrite(rpin,1023);
    analogWrite(gpin,0);
  	lcd.clear();
  	lcd.setCursor(0,0);
  	lcd.print(m1);
  	delay(2000);
  }
  if (a>=50)
  {
    analogWrite(rpin,0);
    analogWrite(gpin,1023);
  }
}
void forward()
{ 
  mot1speed=1023;
  mot2speed=1023;    
  digitalWrite(mot2in1,HIGH);
  digitalWrite(mot2in2,LOW);
  analogWrite(mot2pin,mot2speed);
  digitalWrite(mot1in1,HIGH);
  digitalWrite(mot1in2,LOW);
  analogWrite(mot1pin,mot1speed);
  tiltcheck();
}
void backward()
{
  mot1speed=1023;
  mot2speed=1023;
  digitalWrite(mot1in2,HIGH);
  digitalWrite(mot1in1,LOW);
  analogWrite(mot1pin,mot1speed);   
  digitalWrite(mot2in2,HIGH);
  digitalWrite(mot2in1,LOW);
  analogWrite(mot2pin,mot2speed);
  tiltcheck();
}
void stop()
{
  //stop
  mot1speed=mot2speed=0;
  analogWrite(mot1pin,mot1speed);
  analogWrite(mot2pin,mot2speed); 
}
void left()
{ 
  mot1speed=0;
  mot2speed=1023;    
  digitalWrite(mot2in1,HIGH);
  digitalWrite(mot2in2,LOW);
  analogWrite(mot2pin,mot2speed);
  digitalWrite(mot1in1,HIGH);
  digitalWrite(mot1in2,LOW);
  analogWrite(mot1pin,mot1speed);
  tiltcheck();
}
void right()
{ 
  mot1speed=1023;
  mot2speed=0;    
  digitalWrite(mot2in1,HIGH);
  digitalWrite(mot2in2,LOW);
  analogWrite(mot2pin,mot2speed);
  digitalWrite(mot1in1,HIGH);
  digitalWrite(mot1in2,LOW);
  analogWrite(mot1pin,mot1speed);
  tiltcheck();
}
void tiltcheck()
{
  tiltval=digitalRead(tiltpin);
  while(tiltval==0)
    {
      stop();
  	  tiltval=digitalRead(tiltpin);
    }
}