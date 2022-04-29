/*
 * 보드 - 모스펫 모듈
 * SIG에 디지털 핀(여기선 13번 핀)
 * GND에 GND
 * VCC에 3.3v
 * VIN에 VIN
 * 
 * 모스펫 모듈 -> 워터펌프
 * V+ -> 연결
 * V- -> 연결
 * 
 * 2초 동안 작동하고 2초동안 멈추는 워터펌프를 만듬
 * 실제로 이따구로 만들진 않음.
 */


void setup() {
  pinMode(13,OUTPUT);

}

void loop() {
  digitalWrite(13,HIGH);
  delay(2000);
  digitalWrite(13,LOW);
  delay(2000);

}
