#include <LiquidCrystal.h>
#include <string.h>

LiquidCrystal lcd(0, 1, 8, 9, 10, 11);
boolean STATE = false;
char table[5][15] = {"Normal", 
                      "Mild", 
                      "Moderate", 
                      "MSevere", 
                      "Severe"};

char result[5][15] = {"XXXXXXX", "XXXXXXX", "XXXXXXX", "XXXXXXX", "XXXXXXX"}; 

char emptyRow[16] = "                ";

///SPEAKER
int hzNumber = 125;
int tableCnt = 0;
int SpeakerOutput = 7;
int resCnt = 0;
///BUTTONS
int hzNumberUpBtn = 3;
int hzNumberDownBtn = 2;
int volumeUpBtn = 4;
int volumeDownBtn = 5;
int startStopBtn = 13;
int senseBtn = 12;
///BUTTON STATES
int buttonStateUpHz = 0;
int lastButtonStateUpHz = 0;
int buttonStateDownHz = 0;
int lastButtonStateDownHz = 0;
int buttonStateUpVol = 0;
int lastButtonStateUpVol = 0;
int buttonStateDownVol = 0;
int lastButtonStateDownVol = 0;
//---
int startStopState = 0;
int lastStartStopState = 0;
int senseState = 0;
int lastSenseState = 0;

void setup()
{
  lcd.begin(16, 2);
  pinMode(SpeakerOutput, OUTPUT);
  /// BUTTONS
  //pinMode(hzNumberUpBtn, INPUT);
  //pinMode(hzNumberDownBtn, INPUT);
  //pinMode(volumeUpBtn, INPUT);
  //pinMode(volumeDownBtn, INPUT);
  //pinMode(startStopBtn, INPUT);
  //pinMode(senseBtn, INPUT);
}

void printWhileCheck() {
  lcd.setCursor(2, 0);
  lcd.print(table[tableCnt]);
  lcd.setCursor(0, 1);
  lcd.write("Current hz: ");
  lcd.setCursor(12, 1);
  lcd.print(hzNumber);
  if (hzNumber < 1000) {
    lcd.setCursor(15, 1);
    lcd.print(" ");
  }
}

void printResult(int val) {
  // first row
  lcd.setCursor(0, 0);
  lcd.print(table[val]);
  lcd.setCursor(9, 0);
  lcd.print(result[val]);
  // second row
  if (val < 4) {
    lcd.setCursor(0, 1);
    lcd.print(table[val + 1]);
    lcd.setCursor(9, 1);
    lcd.print(result[val + 1]);
  } else {
    lcd.setCursor(0, 1);
    lcd.write(emptyRow);
  }
}

void clearLCD() {
  lcd.setCursor(0, 0);
  lcd.write(emptyRow);
  lcd.setCursor(0, 1);
  lcd.write(emptyRow);
}

int power(int number) {
  int cnt = 0;
  while (number != 125) {
    cnt++;
    number /= 2;
  }
  return cnt;
}

void resetResultMatrix() {
  for(int i = 0; i < 5; i++) {
    for(int j = 0; j < 7; j++) {
      result[i][j] = 'X';
    }
  }
}

void loop() {
  //the speaker
  if (STATE)
    tone(SpeakerOutput, hzNumber);
  else
    noTone(SpeakerOutput);
  // BUTTONS
  buttonStateDownHz = digitalRead(hzNumberDownBtn);
  buttonStateUpHz = digitalRead(hzNumberUpBtn);
  buttonStateUpVol = digitalRead(volumeUpBtn);
  buttonStateDownVol = digitalRead(volumeDownBtn);
  startStopState = digitalRead(startStopBtn);
  senseState = digitalRead(senseBtn);
  
  // DEBOUNCER BUTTON START - STOP
  if (startStopState != lastStartStopState) {
    if (startStopState == HIGH) {
      if (STATE == true)
        hzNumber = 125;
      else
        resetResultMatrix();
      tableCnt = 0;
      STATE = not(STATE);
      clearLCD();
    }
    delay(50);
  }
  lastStartStopState = startStopState;
  // DEBOUNCER BUTTON UP --- HERTZ
  if (senseState != lastSenseState) {
    if (senseState == HIGH && STATE)
      result[tableCnt][power(hzNumber)] = '0';
    delay(50);
  }
  lastSenseState = senseState;
  // DEBOUNCER BUTTON UP --- HERTZ
  if (buttonStateUpHz != lastButtonStateUpHz) {
    if (buttonStateUpHz == HIGH && hzNumber < 8000 && STATE)
      hzNumber *= 2;
    delay(50);
  }
  lastButtonStateUpHz = buttonStateUpHz;
  // DEBOUNCER BUTTON DOWN --- HERTZ
  if (buttonStateDownHz != lastButtonStateDownHz) {
    if (buttonStateDownHz == HIGH && hzNumber > 125 && STATE)
        hzNumber /= 2;
    delay(50);
  }
  lastButtonStateDownHz = buttonStateDownHz;
  // DEBOUNCER BUTTON UP --- VOLUME
  if (buttonStateUpVol != lastButtonStateUpVol) {
    if (buttonStateUpVol == HIGH && tableCnt < 4) { 
      if (STATE == true) {
        tableCnt++;
      } else {
        tableCnt++;
      }
      lcd.setCursor(0, 0);
      lcd.print(emptyRow);
      lcd.setCursor(0, 1);
      lcd.print(emptyRow);
    }
    delay(50);
  }
  lastButtonStateUpVol = buttonStateUpVol;
  // DEBOUNCER BUTTON DOWN --- VOLUME
  if (buttonStateDownVol != lastButtonStateDownVol) {
    if (buttonStateDownVol == HIGH && tableCnt > 0) {
      if (STATE == true) {
        tableCnt--;
      } else {
        tableCnt--;
      }
      lcd.setCursor(0, 0);
      lcd.print(emptyRow);
      lcd.setCursor(0, 1);
      lcd.print(emptyRow);
    }
    delay(50);
  }
  lastButtonStateDownVol = buttonStateDownVol;
  // LCD
  if (STATE) {
    printWhileCheck();
  }
  else {
    printResult(tableCnt);
  }
  
}
