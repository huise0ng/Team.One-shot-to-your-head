#include <Wire.h> // Wire 라이브러리를 포함합니다. I2C 통신을 위해 사용됩니다.
#include <LiquidCrystal_I2C.h> // LiquidCrystal_I2C 라이브러리를 포함합니다. I2C를 통해 LCD 디스플레이를 제어하기 위해 사용됩니다.
#include <Servo.h> // Servo 라이브러리를 포함합니다. 서보 모터를 제어하기 위해 사용됩니다.

// 리미트 스위치 및 서보 모터 핀 번호
const int limitSwitchPins[] = {2, 3, 4, 5, 6, 7}; // 리미트 스위치의 핀 번호를 배열로 정의합니다.
const int servoPins[] = {8, 9, 10, 11, 12, 13}; // 서보 모터의 핀 번호를 배열로 정의합니다.

// LCD 디스플레이 초기화
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD 주소와 크기를 설정하여 LiquidCrystal_I2C 객체를 생성합니다.

// 서보 모터 객체 배열
Servo servos[6]; // 6개의 서보 모터 객체를 배열로 생성합니다.

// 게임 관련 변수
int score = 0; // 점수 변수입니다.
int hitCount = 0; // 리미트 스위치 누적 개수를 저장하는 변수입니다.
unsigned long gameStartTime = 0; // 게임 시작 시간을 기록하는 변수입니다.
const unsigned long gameDuration = 60000; // 게임 작동 시간을 1분 (60초)로 설정합니다.

void setup() {
  // 리미트 스위치와 푸시 버튼 핀을 입력으로 설정합니다.
  for (int i = 0; i < 6; i++) {
    pinMode(limitSwitchPins[i], INPUT_PULLUP); // 리미트 스위치 핀을 입력으로 설정하고 풀업 저항을 사용합니다.
  }
  pinMode(A0, INPUT_PULLUP); // 푸시 버튼 핀을 입력으로 설정하고 풀업 저항을 사용합니다.

  // LCD 디스플레이 초기화
  lcd.begin(16, 2); // LCD 디스플레이 크기를 설정합니다.
  lcd.print("Score: "); // "Score: " 텍스트를 출력합니다.
  lcd.setCursor(7, 0); // 커서 위치를 설정합니다. (첫 번째 줄, 7번째 컬럼)
  lcd.print(score); // 초기 점수를 출력합니다.

  // 서보 모터 객체에 핀을 할당합니다.
  for (int i = 0; i < 6; i++) {
    servos[i].attach(servoPins[i]); // 서보 모터 객체에 핀을 할당합니다.
  }

  gameStartTime = millis(); // 현재 시간을 기록하여 게임 시작 시간으로 설정합니다.
}

void loop() {
  checkLimitSwitches(); // 리미트 스위치 상태를 확인합니다.
  checkPushButton(); // 푸시 버튼 상태를 확인합니다.

  unsigned long currentTime = millis(); // 현재 시간을 저장합니다.
  if (currentTime - gameStartTime >= gameDuration) {
    // 게임 시간이 만료되었을 때 게임 종료
    endGame(); // 게임 종료 함수를 호출합니다.
  }
}

void incrementScore() {
  score += 10; // 점수를 10만큼 증가시킵니다.
  lcd.setCursor(7, 0); // 커서 위치를 설정합니다. (첫 번째 줄, 7번째 컬럼)
  lcd.print(score); // 점수를 LCD에 출력합니다.
}

void checkLimitSwitches() {
  for (int i = 0; i < 6; i++) {
    if (digitalRead(limitSwitchPins[i]) == LOW) {
      // 리미트 스위치가 눌렸을 때 점수 증가
      incrementScore(); // 점수 증가 함수를 호출합니다.
      hitCount++; // 리미트 스위치 누적 개수를 증가시킵니다.
    }
  }
  
  if (hitCount >= 6) {
    // 모든 리미트 스위치가 눌렸을 때 게임 종료
    endGame(); // 게임 종료 함수를 호출합니다.
  }
}

void checkPushButton() {
  if (digitalRead(A0) == LOW) {
    // 푸시 버튼이 눌렸을 때 서보 모터 작동
    for (int i = 0; i < 6; i++) {
      servos[i].write(0); // 서보 모터를 작동시키는 코드입니다. (0도로 회전)
    }
  }
}

void endGame() {
  // 게임 종료 시 서보 모터 중지 및 메시지 표시
  for (int i = 0; i < 6; i++) {
    servos[i].write(90); // 서보 모터를 중앙으로 위치시킵니다. (90도로 회전)
    servos[i].detach(); // 서보 모터를 비활성화합니다.
  }
  lcd.clear(); // LCD 디스플레이를 지웁니다.
  lcd.print("Game Over"); // "Game Over" 메시지를 출력합니다.
  lcd.setCursor(0, 1); // 커서 위치를 설정합니다. (두 번째 줄, 0번째 컬럼)
  lcd.print("Final Score: "); // "Final Score: " 메시지를 출력합니다.
  lcd.print(score); // 최종 점수를 출력합니다.
  while (true) {} // 코드 실행을 종료합니다.
}
