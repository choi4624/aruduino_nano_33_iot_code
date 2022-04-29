/*
 * 보드 - 모스펫 모듈
 * SIG에 디지털 핀(여기선 13번 핀)
 * GND에 GND
 * VCC에 3.3v
 * VIN에 VIN
 * 
 * 모스펫 모듈 -> 팬
 * V+ -> 빨간선
 * V- -> 검은선
 * 
 * 5초 동안 작동하고 5초동안 멈추는 환기 팬을 만듦.
 * 실제로 이따구로 만들진 않음.
 */


void setup() {
  pinMode(12,OUTPUT);

}

void loop() {
  digitalWrite(12,HIGH);
  delay(5000);
  digitalWrite(12,LOW);
  delay(5000);

}
