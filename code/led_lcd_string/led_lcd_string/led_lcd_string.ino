#include <LiquidCrystal.h>
#include <string.h>

LiquidCrystal lcd(0, 1, 8, 9, 10, 11); 

void setup()
{
  lcd.begin(16, 2);
  pinMode(12,OUTPUT);
}

int  string_length(char *a){
  int counter = 0;
  while (a[counter] != '\0')
    counter++;
    return (counter);
}

void loop() {
  char a[] = "alma";
  if (string_length(a) % 2 == 0)
  {
    lcd.setCursor(0, 1);
    digitalWrite(12,HIGH);
    lcd.write(a);
  }
  else{
    lcd.setCursor(0,1);
    lcd.write("not even");
  }
  
}
