/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/


// 쉴드에서 디지털 0번 핀이랑 3번 핀을 5초간 켰다 끄는 제어 코드 

/*
전원 인가할 장치가 MOSFET의 V+ V- 영역에, 전원 공급 장치가 GND 영역에서 실행하고

쉴드의 위쪽은 디지털 단자

아래쪽은 아날로그 단자

아두이노 보드의 전원 공급은 컴퓨터 usb micro-b (고속충전 포트 사용 자제) 아니면, 5V micro-b 충전기(9V 12V 지원하는 애들 위험함) 사용 추천 

*/ 

int swPin = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(swPin, OUTPUT);  
}

// the loop function runs over and over again forever
void loop() {
/*  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(10000);                       // wait for a second
  */
  analogWrite(swPin,255);   
   delay(5000);  
  analogWrite(swPin,0);  
   delay(5000);  
   analogWrite(swPin+3,HIGH);   
   delay(5000);  
  analogWrite(swPin+3,LOW);  
   delay(5000);  
}
