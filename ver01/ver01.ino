#include <DHT.h>
#include <DHT_U.h>

#include <SoftwareSerial.h>
#include <MHZ19.h>

#define DHTPIN 13
#define DHTTYPE DHT11

unsigned long now=0;
unsigned long previousTime=0;
int autoMode;
int waterPump=0;
int light=1;
int heater=0;
int fan=0;
int lightState=1;

SoftwareSerial ss(12,14); //RX=D5,TX=D6에 꽂으면 됨.
MHZ19 mhz(&ss);

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  ss.begin(9600);

}

void loop() {
  now=millis();
  Serial.println(now);
//  서버에서 데이터 받는 부분이 제일 먼저 들어가야됨.

//  센서값 측정
  //#1.온습도센서 D7
  float humi, temp;
  temp = dht.readTemperature();
  humi = dht.readHumidity();

  if(isnan(humi) ||isnan(temp)){
    Serial.println("Failed to read from DHT sensor!!");
    return;
  }
  Serial.print("온도 : ");
  Serial.print(temp);
  Serial.println("C");
  Serial.print("습도 : ");
  Serial.print(humi);
  Serial.println("%");
  //#2.수위센서 A0
  int val =analogRead(A0); //아날로그 0번핀이 수위센서 val이 500정도보다 작으면 waterPump 작동하도록 할것.
  Serial.print("수위 : ");
  Serial.println(val);
  //#3.이산화탄소센서 Rx=D5, Tx=D6
  MHZ19_RESULT response = mhz.retrieveData();
  if (response == MHZ19_RESULT_OK)
  {
    Serial.print(F("CO2 : "));
    Serial.println(mhz.getCO2());
  }
  else
  {
    Serial.print(F("Error, code: "));
    Serial.println(response);
  }

//  센서값을 서버에 전송하는 부분이 여기에 해당함.

  
  autoMode=1; //이부분은 나중에 Http 통신으로 데이터 GET이 가능해지면 지워야됨.------------------
//  자동화 시스템(서버에서 auto값을 받아서 
//  if(auto==1)이면 센서값에 따라 액츄에이터 작동, 
//  else면 수동조작 데이터를 받아서 작동
  if(autoMode==1){
    if(val<400){ //적정 수위보다 낮으면 워터펌프가 작동함.
      
    }
    else{ //적정 수위보다 낮지 않다면 워터펌프가 작동안함.
      
    }
  
    if(mhz.getCO2()>1000){ //이산화탄소 농도가 1000ppm 이상일 경우 팬이 작동함.
      if(temp<25){ //온도가 25보다 작다면 히터가 작동함.(히터&팬 작동)
        
      }else{ //온도가 25보다 크다면 팬만(팬만 작동)
        
      }
    }
    else{ //이산화탄소 농도가 1000ppm 이하일 경우 팬이 작동하지 않음.
      if(temp<25){ //온도가 25보다 작다면 히터가 작동함.(히터&팬 작동)
        
      }else{ //온도가 25보다 크다면 아무것도 작동하지 않음.
        
      }
    }
    if((now-previousTime)>=43200000){ //12시간임 간격으로
      previousTime=now;
      if(lightState==1){
        lightState=0;
        digitalWrite(15,LOW);
      }
      else{
        lightState=1;
        digitalWrite(15,HIGH);
      }
    }
  }


//  수동조작모드의 경우 auto!=1일때
//  
  else{
    if(waterPump==1){
      
    }
    else{
      
    }

    if(light==1){
      
    }
    else{
      
    }

    if(fan==1){
      if(heater==1){ //히터와 팬 모두 작동.
        
      }
      else{ //팬만 작동.
        
      }
    }
    else{
      if(heater==1){ //히터와 팬 모두 작동.
        
      }
      else{ //히터와 팬 둘다 작동안함.
        
      }
    }
  }
 
  delay(5000);
}
