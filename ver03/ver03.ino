#include <ArduinoJson.h>

#include <DHT.h>
#include <DHT_U.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <SoftwareSerial.h>
#include <MHZ19.h>

#define DHTPIN 13
#define DHTTYPE DHT11

const char* ssid="KT_GiGA_2G_MYHOUSE";
const char* password="g01099844615";

unsigned long now=0;
unsigned long previousTime=0;
unsigned long previousForPost=0;
unsigned long previousForGet=0;

int lightState=1;
int autoMode=1;
int relay1=1; //기본값
int relay2=0;
int relay3=0;
int relay4=0;
String payload;

SoftwareSerial ss(12,14); //RX=D5,TX=D6에 꽂으면 됨.
MHZ19 mhz(&ss);

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  ss.begin(9600);
  WiFi.begin(ssid,password);
  Serial.println("Connecting");
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds");
  pinMode(4,OUTPUT); //D4
  pinMode(5,OUTPUT); //D3
  pinMode(16,OUTPUT); //D2
  pinMode(1,OUTPUT); //D1 D0와 D1은 핀 꽂아놓고 업로드하면 업로드 안되는것으로 앎. ------------

  digitalWrite(1,HIGH);
}


void loop() {
  now=millis();
//  서버에서 데이터 받는 부분이 제일 먼저 들어가야됨.
  if(now<previousForGet){ //49.7일 연속으로 켜져있을경우 생기는 오버플로우 문제 방지
    previousForGet=0;
    previousTime=0;
    previousForPost=0;
  }
  
  if((now-previousForGet)>=1000){
    previousForGet=now;
    if ((WiFi.status()== WL_CONNECTED)) {
      WiFiClient client;
      HTTPClient http;
  
      Serial.print("[HTTP] begin...\n");
      if (http.begin(client, "http://ziot.i4624.tk/sql/meka/table/final")) {  // HTTP

        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();
  
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            payload = http.getString();
            Serial.println(payload);
            StaticJsonBuffer<400> jsonBuffer;
            JsonArray& array1=jsonBuffer.parseArray(payload);
            JsonObject& obj1=array1[0];
            relay1=obj1["relay1"];  //->relay1=>조명조절 D1
            relay2=obj1["relay2"];  //->relay2=>수위조절 D2
            relay3=obj1["relay3"];  //->relay3=>환기조절 D3
            relay4=obj1["relay4"];  //->relay4=>히터조절 D4
            autoMode=obj1["autoMode"];
          }
        } else {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
  
        http.end();
      } else {
        Serial.printf("[HTTP} Unable to connect\n");
      }
    }
  }
  
//  센서값 측정
  //#1.온습도센서 D7
  float humi, temp;
  temp = dht.readTemperature();
  humi = dht.readHumidity();

  if(isnan(humi) ||isnan(temp)){
    Serial.println("Failed to read from DHT sensor!!");
  }
  Serial.print("온도 : ");
  Serial.print(temp);
  Serial.println("C");
  Serial.print("습도 : ");
  Serial.print(humi);
  Serial.println("%");
  //#2.수위센서 A0
  int val =analogRead(A0);
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
  if((now-previousForPost)>=5000){
    previousForPost=now;
    String httpRequestData = "{\"temperature\":\"" + String(temp) + "\",\"relay1\":\"" + String(relay1)+"\",\"relay2\":\"" + String(relay2)+"\",\"relay3\":\"" + String(relay3)+"\",\"relay4\":\"" + String(relay4)+"\",\"autoMode\":\"" + String(autoMode)+ "\",\"humi\":\"" + String(humi) + "\",\"waterLevel\":\"" + String(val) + "\",\"co2\":\"" + String(mhz.getCO2())+"\"}";
    if ((WiFi.status() == WL_CONNECTED)) {

      WiFiClient client;
      HTTPClient http;
  
      Serial.print("[HTTP] begin...\n");
      // configure traged server and url
      http.begin(client, "http://ziot.i4624.tk/sql/meka/insert"); //HTTP
      http.addHeader("Content-Type", "application/json");
  
      Serial.print("[HTTP] POST...\n");
      // start connection and send HTTP header and body
      int httpCode = http.POST(httpRequestData);
  
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] POST... code: %d\n", httpCode);
  
        // file found at server
        if (httpCode == HTTP_CODE_OK) {
          const String& payload = http.getString();
          Serial.println("received payload:\n<<");
          Serial.println(payload);
          Serial.println(">>");
        }
      } else {
        Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
  
      http.end();
    }
  }
  //물공급 탱크에 센서를 다는건 아날로그 입력핀의 개수가 부족하여 새로운 보드에 데이터를 따로 보내게
  //만들어야함.
  //autoMode 기본값은 1임.
//  자동화 시스템(서버에서 auto값을 받아서 
//  if(auto==1)이면 센서값에 따라 액츄에이터 작동, 
//  else면 수동조작 데이터를 받아서 작동

  if(autoMode==1){
    if(val<400){ //적정 수위보다 낮으면 워터펌프가 작동함.
      digitalWrite(16,HIGH);
    }
    else{ //적정 수위보다 낮지 않다면 워터펌프가 작동안함.
      digitalWrite(16,LOW);
    }
  
    if(mhz.getCO2()>400){ //이산화탄소 농도가 400ppm 이상일 경우 팬이 작동함.
      if(temp<25){ //온도가 25보다 작다면 히터가 작동함.(히터&팬 작동)
        digitalWrite(5,HIGH); //D3는 팬
        digitalWrite(4,HIGH); //D4는 히터
      }else{ //온도가 25보다 크다면 팬만(팬만 작동)
        digitalWrite(5,HIGH);
        digitalWrite(4,LOW);
      }
    }
    else{ //이산화탄소 농도가 400ppm 이하일 경우 팬이 작동하지 않음.
      if(temp<25){ //온도가 25보다 작다면 히터가 작동함.(히터&팬 작동)
        digitalWrite(5,HIGH);
        digitalWrite(4,HIGH);
      }else{ //온도가 25보다 크다면 아무것도 작동하지 않음.
        digitalWrite(5,LOW);
        digitalWrite(4,LOW);
      }
    }
    if((now-previousTime)>=43200000){ //12시간임 간격으로
      previousTime=now;
      if(lightState==1){
        lightState=0;
        digitalWrite(1,LOW); //D1이 조명핀임.
      }
      else{
        lightState=1;
        digitalWrite(1,HIGH);
      }
    }
  }

//  수동조작모드의 경우 auto!=1일때
//  
  else{
    if(relay2==1){
      digitalWrite(16,HIGH);
    }
    else{
      digitalWrite(16,LOW);
    }

    if(relay1==1){
      digitalWrite(1,HIGH);
    }
    else{
      digitalWrite(1,LOW);
    }

    if(relay3==1){
      if(relay4==1){ //히터와 팬 모두 작동.
        digitalWrite(5,HIGH);
        digitalWrite(4,HIGH);
      }
      else{ //팬만 작동.
        digitalWrite(5,HIGH);
        digitalWrite(4,LOW);
      }
    }
    else{
      if(relay4==1){ //히터와 팬 모두 작동.
        digitalWrite(5,HIGH);
        digitalWrite(4,HIGH);
      }
      else{ //히터와 팬 둘다 작동안함.
        digitalWrite(5,LOW);
        digitalWrite(4,LOW);
      }
    }
  }
}
