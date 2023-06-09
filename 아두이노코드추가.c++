#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

const int limitSwitchPins[] = {2, 3, 4, 5, 6, 7};  // 리미트 스위치의 핀 번호 배열
const int servoPins[] = {8, 9, 10, 11, 12, 13};  // 서보 모터의 핀 번호 배열

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C LCD 객체
Servo servos[6];  // 서보 모터 객체 배열

int score = 0;  // 점수 변수 초기화
int hitCount = 0;  // 리미트 스위치 누적 개수 변수 초기화
unsigned long gameStartTime = 0;  // 게임 시작 시간 변수 초기화
const unsigned long gameDuration = 60000;  // 게임 작동 시간 (1분 = 60000밀리초)

void setup() {
  for (int i = 0; i < 6; i++) {
    pinMode(limitSwitchPins[i], INPUT_PULLUP);  // 리미트 스위치 핀을 입력으로 설정
  }
  pinMode(A0, INPUT_PULLUP);  // 푸시 버튼 핀을 입력으로 설정

  lcd.begin(16, 2);  // LCD 초기화
  lcd.print("Score: ");  // "Score: " 출력
  lcd.setCursor(7, 0);  // 커서 위치 설정
  lcd.print(score);  // 점수 출력

  for (int i = 0; i < 6; i++) {
    servos[i].attach(servoPins[i]);  // 서보 모터 객체에 핀 할당
  }

  gameStartTime = millis();  // 게임 시작 시간 기록
}

void loop() {
  checkLimitSwitches();  // 리미트 스위치 상태 확인
  checkPushButton();  // 푸시 버튼 상태 확인

  unsigned long currentTime = millis();  // 현재 시간 기록
  if (currentTime - gameStartTime >= gameDuration) {
    endGame();  // 게임 종료
  }
}

void incrementScore() {
  score += 10;  // 점수 증가
  lcd.setCursor(7, 0);  // 커서 위치 설정
  lcd.print(score);  // 점수 출력
}

void checkLimitSwitches() {
  for (int i = 0; i < 6; i++) {
    if (digitalRead(limitSwitchPins[i]) == LOW) {
      incrementScore();  // 점수 증가 함수 호출
      hitCount++;  // 리미트 스위치 누적 개수 증가

      delay(100);  // 100밀리초 딜레이

      servos[i].write(20);  // 해당 리미트 스위치와 연결된 서보 모터를 20도로 회전
    }
  }

  if (hitCount >= 6) {
    endGame();  // 게임 종료
  }
}

void checkPushButton() {
  if (digitalRead(A0) == LOW) {
    for (int i = 0; i < 6; i++) {
      servos[i].write(0);  // 푸시 버튼이 눌렸을 때 모든 서보 모터를 0도로 회전
    }
  }
}

void endGame() {
  for (int i = 0; i < 6; i++) {
    servos[i].write(90);  // 게임 종료 시 모든 서보 모터를 90도로 회전
    servos[i].detach();  // 서보 모터 제어 신호 해제
  }
  lcd.clear();  // LCD 화면 지우기
  lcd.print("Game Over");  // "Game Over" 출력
  lcd.setCursor(0, 1);  // 커서 위치 설정
  lcd.print("Final Score: ");  // "Final Score: " 출력
  lcd.print(score);  // 최종 점수 출력
  while (true) {}  // 무한루프로 프로그램 정지
}
