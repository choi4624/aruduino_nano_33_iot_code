int water_pin = A5;

/*
 * 수위센서
 * 아날로그 핀을 이용
 * 물에 닿지 않을 경우 0값이 나오고, 물이 올라오면 수치가 올라감.
 * 수치는 cm나 mm같은 단위가 아니므로 이를 표준화하기 어려움. 때문에 이에 대한
 * 데이터를 다듬어서 보내주긴 어려움.
 * 
 * 
 * 수위센서를 자동으로 이용할 경우 특정 높이까지 차오를 경우 워터펌프의 작동을 
 * 중지시키는 방향으로 설계하는 것이 좋아보임.
 */


void setup() {
  Serial.begin(9600);
  
}

void loop() {
  int val =analogRead(A5);
  Serial.println(val);
  delay(100);

}
