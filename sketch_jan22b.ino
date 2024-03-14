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
bool click = false;

MicroDS18B20<3> sensor1;
float b;
void setup() {
   Serial.begin(9600);
   pinMode(12, INPUT_PULLUP);
   pinMode(10, OUTPUT);
   encoder.setPosition(10 / STEPS);
   lcd.begin(16,2);
   lcd.setCursor(0,0);
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
   
   if (lastPos != newPos) {
      Serial.println(newPos);
      Serial.println(click);
      lcd.print(temp);
      lcd.setCursor(2,0);
      lcd.print("C");
      lcd.setCursor(0,0);
      lastPos = newPos;
   }
   
    if(encoderButton.pressed()) click = !click;
    
    if(click){
      lcd.setCursor(11, 0);
      lcd.print("On ");
      digitalWrite(10, HIGH);
      lcd.setCursor(0, 0);
    }else{
       lcd.setCursor(11, 0);
       lcd.print("Off");
       digitalWrite(10, LOW);
       lcd.setCursor(0, 0);
    }
    
    if (sensor1.readTemp()) b = sensor1.getTemp();
   Serial.println(b);
   lcd.setCursor(6, 0);
   lcd.print(b);
   sensor1.requestTemp();
}
