#include<Servo.h>

int LA1[] = { 34, 35, 8};  // linear act 1       A - B - PWM
int LA2[] = { 36, 37, 9};  // linear act 2
int GRP1[] = { 22, 24, 2}; // gripper 
int GRP2[] = { 27, 25, 3}; // gripper

Servo yaw, roll, pitch;

int CH[] = { A11,  A10,    A9,   A8,   A5,   A6,   A7 };
//           la1 - la2 - pitch - TT - yaw - roll - grp

int rcmap[7], pulse[7];
int y;
float fy, a = 0.5;

int i;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  for(i = 0; i < 7; i++ )
  {
    pinMode(CH[i], INPUT);
  }

  for(i = 0; i < 4; i++)
  {
    pinMode(LA1[i], OUTPUT);
    pinMode(LA2[i], OUTPUT);
    pinMode(GRP1[i], OUTPUT);
    pinMode(GRP2[i], OUTPUT);
  }

  yaw.attach(12);
  pitch.attach(10);
  roll.attach(11);

  Serial3.begin(9600);//tt

}

void loop() {
  // put your main code here, to run repeatedly:
  rc();

  lina(LA1, rcmap[0]);
  lina(LA2, rcmap[1]);
  lina(GRP1, rcmap[6]);
  lina(GRP2, rcmap[6]);

  turntable(rcmap[3]);

  pitch.writeMicroseconds(rcmap[2]);
  yaw.writeMicroseconds(rcmap[4]);
  roll.writeMicroseconds(rcmap[5]);

}

void rc()
{
  for(i = 0; i < 7; i++)
  {
    pulse[i] = pulseIn(CH[i], HIGH);
  }

  rcmap[0] = map(pulse[0], 1133, 1967, -100, 100);       //la1
  rcmap[1] = map(pulse[1], 1126, 1968, -100, 100);       //la2
  rcmap[2] = map(pulse[2], 1076, 1917, 1400, 1600);      //pitch
  rcmap[3] = map(pulse[3], 1126, 1969, -40, 40);         //TT
  rcmap[4] = map(pulse[4], 1050, 1871, 1200, 1500);      //yaw
  rcmap[5] = map(pulse[5], 1046, 1883, 1000, 2000);      //roll
  rcmap[6] = map(pulse[6], 1034, 1890, -80, 80);         //grp 
}

void turntable(int x)
{
  y = map(x, -100, 100, 128, 256);
  fy = fy * a + (1 - a) * y;
  delay(5);
  Serial.println(y);
  Serial3.write((int)fy);
  delay(5);
}

void lina(int p[], int t)
{

  if(t > 20)
  {
    digitalWrite(p[0], HIGH);
    digitalWrite(p[1], LOW);
  }
  else if(t < -20)
  {
    digitalWrite(p[0], LOW);
    digitalWrite(p[1], HIGH);
  }
  else
  {
    digitalWrite(p[0], LOW);
    digitalWrite(p[1], LOW);
  }
  analogWrite(p[2], abs(t) * 2);
}

