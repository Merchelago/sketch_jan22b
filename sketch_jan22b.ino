#include <DS_raw.h>
#include "microDS18B20.h"
#include <microOneWire.h>
#include <Button.h>
#include "RotaryEncoder.h"  // библиотека для энкодера
#include <LiquidCrystal.h>         
RotaryEncoder encoder(2, 11);  // пины подключение энкодера (DT, CLK)

// задаем шаг энкодера и макс./мин. значение
#define STEPS  5
#define POSMIN 20
#define POSMAX 80

Button encoderButton(12);
const int rs=8, en=9, d4=4,d5=5,d6=6,d7=7;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
int lastPos, newPos;
int temp = 20;
int heat;
bool click = false;
bool switchClick = false;


MicroDS18B20<3> sensor1;
float b;

byte customChar[] = {
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

void handleEncoderPinA();
void handleEncoderPinB();
void handleEncoderButtonPress();

void setup() {
    Serial.begin(9600);
    pinMode(12, INPUT_PULLUP);
    pinMode(10, OUTPUT);
    encoder.setPosition(10 / STEPS);
    lcd.begin(16,2);
    lcd.setCursor(0,0);
    lcd.createChar(0, customChar);
    attachInterrupt(digitalPinToInterrupt(2), handleEncoderPinA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(11), handleEncoderPinB, CHANGE);
    attachInterrupt(digitalPinToInterrupt(12), handleEncoderButtonPress, RISING);
}

void loop() {
   lcd.setCursor(0,0);
   encoder.tick();
   newPos = encoder.getPosition() * STEPS;

   if (newPos < POSMIN) { 
   encoder.setPosition(POSMIN / STEPS); 
   newPos = POSMIN;
   }
   else if (newPos > POSMAX) { 
   encoder.setPosition(POSMAX / STEPS); 
   newPos = POSMAX; 
   }
   temp = encoder.getPosition()* STEPS;
   heat = sensor1.getTemp();
   if (lastPos != newPos) {
      Serial.println(newPos);
      Serial.println(click);
      lcd.print(temp);
      lcd.setCursor(2,0);
      lcd.print(char(0));
      lcd.print("C");
      lcd.setCursor(0,0);
      lastPos = newPos;
   }
   
    if(encoderButton.pressed()){ click = !click; switchClick = !switchClick;}
    if(heat >= temp){
      switchClick = false; lcd.setCursor(9, 0);
      lcd.print("       ");
      lcd.setCursor(9, 0);
      lcd.print("On Off");
      digitalWrite(10, LOW);
      }
    if(click && switchClick && heat <= temp){
      lcd.setCursor(9, 0);
      lcd.print("       ");
      lcd.setCursor(9, 0);
      lcd.print("On On");
      digitalWrite(10, HIGH);
      //lcd.setCursor(0, 0);
    }else if(click && heat >= temp){
      switchClick = false;
      lcd.setCursor(9, 0);
      lcd.print("       ");
      lcd.setCursor(9, 0);
      lcd.print("On Off");
      digitalWrite(10, LOW);
    }else if(click && heat <= temp){
      switchClick = true;
      lcd.setCursor(9, 0);
      lcd.print("       ");
      lcd.setCursor(9, 0);
      lcd.print("On On");
      digitalWrite(10, HIGH);
    }
    else{
       lcd.setCursor(9, 0);
       lcd.print("       ");
       lcd.setCursor(9, 0);
       lcd.print("Off Off");
       digitalWrite(10, LOW);
       //lcd.setCursor(0, 0);
    }
    
    if (sensor1.readTemp()) b = sensor1.getTemp();
   Serial.println(b);
   lcd.setCursor(0, 1);
   lcd.print(b);
   lcd.setCursor(5,1);
   lcd.print(char(0));
   lcd.print("C");
   sensor1.requestTemp();
}

void handleEncoderPinA() {
    encoder.tick();
}

void handleEncoderPinB() {
    encoder.tick();
}

// Обработчик прерывания для кнопки энкодера
void handleEncoderButtonPress() {
    click = !click;
    switchClick = !switchClick;
}
