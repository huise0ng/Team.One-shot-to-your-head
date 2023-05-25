#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define RST 0
#define COIN 1
#define BTN1 2
#define BTN2 4
#define BTN3 7
#define BTN4 8
#define BTN5 12
#define BTN6 13

#define SV1 3
#define SV2 5
#define SV3 6
#define SV4 9
#define SV5 10
#define SV6 11

Servo svoArray[6];
LiquidCrystal_I2C lcd(0x27, 16, 2);

int dataArray[6] = {BTN1, BTN2, BTN3, BTN4, BTN5, BTN6};
int scoreArray[6] = {100, 50, 30, 200, 90, 180};
int svoPinArray[6] = {SV1, SV2, SV3, SV4, SV5, SV6};
int score = 0;

void reset();
void lcdprint(int scoreOrNum, int mode);
void playGame();

void setup() {
  Serial.begin(9600);
  Serial.println("Hello!");
  lcd.begin();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game Start!");

  pinMode(RST, INPUT_PULLUP);
  pinMode(COIN, INPUT_PULLUP);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT_PULLUP);
  pinMode(BTN5, INPUT_PULLUP);
  pinMode(BTN6, INPUT_PULLUP);

  for(int i = 0; i < 6; i++){
    svoArray[i].attach(svoPinArray[i]);
  }
  reset();
}



void loop() {
  Serial.println("Insert Coin!");
  lcd.setCursor(0,0);
  lcd.print("Insert Coin!    ");
  while (analogRead(A0) < 500){
    continue;
  }
  lcd.setCursor(0,0);
  lcd.print("Press SRT Button");
  Serial.println("Press Start Button");
  while (digitalRead(RST) ){
    continue;
  }
  playGame();
}

void playGame(){
  int centinal = 0;
  Serial.println("Game Start!");
  lcd.setCursor(0,0);
  lcd.print("Game Start!      ");
  delay(1000);
  while (true){
    centinal = 0;
    int standback;
    lcdprint(score, 1);
    while ( ! centinal ){
      if ( ! digitalRead(RST) ){
        reset();
        return;
      }
      for( int i = 0; i < 6; i++){
        if ( !digitalRead( dataArray[i] ) ){
          standback = i;
          centinal = 1;
          score += scoreArray[i];
          break;
        }
      }
      if ( centinal ){
        break;
      }
    }
    
    for( int i = 0; i < 6; i++){
      Serial.print( ! digitalRead( dataArray[i] ) );
      Serial.print("  ");
    }
    Serial.println();
    lcdprint(scoreArray[standback], 0);
    delay(1000);
    svoArray[standback].write(3);
    delay(1000);
    svoArray[standback].write(130);
  }
}

void reset(){
  score = 0;
  Serial.println("\nRestart!");
  lcd.setCursor(0,0);
  lcd.print("Restart!        ");
  for(int i = 0; i < 6; i++){
    svoArray[i].write(3);
  }
  delay(1000);
  for(int i = 0; i < 6; i++){
    svoArray[i].write(130);
  }
  lcdprint(score,1);
}

void lcdprint(int scoreOrNum, int mode){
  lcd.setCursor(0, 0);
  if (mode){
    lcd.print("Score : ");
    lcd.print(scoreOrNum);
    Serial.print("Score : ");
    Serial.println(scoreOrNum);
    Serial.println();
  }
  else{
    lcd.print("You Hit : ");
    lcd.print(scoreOrNum);
    Serial.print("You Hit : ");
    Serial.println(scoreOrNum);
  }
  lcd.print("           ");
}