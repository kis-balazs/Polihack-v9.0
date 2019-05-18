#include <LiquidCrystal.h>
#include <string.h>

LiquidCrystal lcd(0, 1, 8, 9, 10, 11);
char table[6][15] = {"-10:25-Normal", "25:40-Mild", "40:55-Moderate", "55:70-MSevere", "70:90-Severe", "90-120:Prof"};

int hzNumber = 125;
int SpeakerOutput = 7;
int hzNumberUpBtn = 3;
int hzNumberDownBtn = 2;

int buttonStateUp = 0;         // current state of the buttonUp
int lastButtonStateUp = 0;     // previous state of the buttonUp
int buttonStateDown = 0;       // current state of the buttonDown
int lastButtonStateDown = 0;   // previous state of the buttonDown

void setup()
{
  lcd.begin(16, 2);
  pinMode(SpeakerOutput, OUTPUT);
  /// buttons
  pinMode(hzNumberUpBtn, INPUT);
  pinMode(hzNumberDownBtn, INPUT);
}

void loop() {
  //the speaker
  tone(SpeakerOutput, hzNumber);
  buttonStateDown = digitalRead(hzNumberDownBtn);
  buttonStateUp = digitalRead(hzNumberUpBtn);
  
  // debouncer button UP
  if (buttonStateUp != lastButtonStateUp) {
    if (buttonStateUp == HIGH && hzNumber < 8000) {
      hzNumber *= 2;
      Serial.println(hzNumber);
    }
    delay(50);
  }
  lastButtonStateUp = buttonStateUp;
  // debouncer button DOWN
  if (buttonStateDown != lastButtonStateDown) {
    if (buttonStateDown == HIGH && hzNumber > 125) { 
      hzNumber /= 2;
      Serial.println(hzNumber);
    }
    delay(50);
  }
  lastButtonStateDown = buttonStateDown;
  // LCD
  lcd.setCursor(0,1);
  lcd.write(12);
}
