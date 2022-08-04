#include <IRremote.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(32, 16, 2);

// IR VARIABLE
int IRpin = 3;
IRrecv IR(IRpin);

// BUZZER VARIABL
int buzzer=5;

// PIR VARIABLE
int PIR_pin = 12;
int pirVal;

// LED
int LEDpin = 8;
bool State = LOW;
bool pRes_State;

// PHOTORESISTOR
int pRes = A3;
int pRes_Val;

// MILLIS
unsigned long currentms;
unsigned long pastms;
int i = 300;      // interval

bool powerCond = false;

void setup()
{
  Serial.begin(9600);
  IR.enableIRIn();
  
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  lcd.setBacklight(0);
  
  pinMode(buzzer,OUTPUT);
}

void loop()
{
  pirVal = digitalRead(PIR_pin);
  pRes_Val = analogRead(pRes);
  Serial.println(pRes);
  if (pRes_Val == 679)
  {
    pRes_State = true;
  }
  while(pRes_Val <= 12 && pRes_State)
  {
    lcd.setBacklight(1);
    pirVal = digitalRead(PIR_pin);
    pRes = analogRead(pRes_Val);
    
    if(pirVal == 1)
    {
      lcd.setCursor(4,0);
      lcd.print("INTRUDER");
      lcd.setCursor(4,1);
      lcd.print("DETECTED");
      currentms = millis();
      
      remote_Control();
      Serial.println(i);
      if (currentms - pastms >= i)
      {
        pastms = currentms;
        State = !State;
      }
      
      digitalWrite(buzzer,State);
      digitalWrite(LEDpin,State);
      
      if (powerCond)
      {
        lcd.clear();
        State = LOW;
        digitalWrite(buzzer,LOW);
        digitalWrite(LEDpin,LOW);
        lcd.setCursor(4,0);
        lcd.print("Powering");
        lcd.setCursor(6,1);
        lcd.print("Off!");
        delay(1000);
        lcd.clear();
        lcd.setBacklight(0); 
        while(1){}
      }
    }
  }
}

void remote_Control()
{
   while(IR.decode())
  {
     //Serial.println(IR.decodedIRData.decodedRawData, HEX);
    
    if(IR.decodedIRData.decodedRawData == 0xFF00BF00)
    {
      lcd.setBacklight(1);
      Serial.println("Power");
      powerCond = true;
    } 
    else if(IR.decodedIRData.decodedRawData == 0xF30CBF00)
    {
      Serial.println("0");
      i = 0;
    }
    else if(IR.decodedIRData.decodedRawData == 0xEF10BF00)
    {
      Serial.println("1");
      i = 100;
    }
    else if(IR.decodedIRData.decodedRawData == 0xEE11BF00)
    {
      Serial.println("2");
      i = 200;
    }
    else if(IR.decodedIRData.decodedRawData == 0xED12BF00)
    {
      Serial.println("3");
      i = 300;
    }
    else if(IR.decodedIRData.decodedRawData == 0xEB14BF00)
    {
      Serial.println("4");
      i = 400;
    }
    else if(IR.decodedIRData.decodedRawData == 0xEA15BF00)
    {
      Serial.println("5");
      i = 500;
    }
    else if(IR.decodedIRData.decodedRawData == 0xE916BF00)
    {
      Serial.println("6");
      i = 600;
    }
    else if(IR.decodedIRData.decodedRawData == 0xE718BF00)
    {
      Serial.println("7");
      i = 700;
    }
    else if(IR.decodedIRData.decodedRawData == 0xE619BF00)
    {
      Serial.println("8");
      i = 8000;
    }
    else if(IR.decodedIRData.decodedRawData == 0xE51ABF00)
    {
      Serial.println("9");
      i = 900;
    }
    IR.resume();
  }
}
