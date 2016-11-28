#include <ESP8266WiFi.h>

const char* ssid = "wifissid";
const char* password = "passwd";

char thingSpeakAddress[] = "api.thingspeak.com";
String thingtweetAPIKey = "PYQ870XZMGJLM4AB";

WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Booting...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while(WiFi.waitForConnectResult() != WL_CONNECTED){
    Serial.println("Connection Failed. Rebooting...");
    delay(5000);
    ESP.restart();  
  }

  Serial.println("Connected to wifi");
}

void loop() {
  // put your main code here, to run repeatedly:
  String twStr = "@maxonK おふろたまったよ" + String(millis());
  updateTwitterStatus(twStr);

  //deepSleep
  //1:μ秒での復帰までのタイマー時間設定  2:復帰するきっかけの設定（モード設定）
  ESP.deepSleep(60 * 1000 * 1000 , WAKE_RF_DEFAULT);
  //deepsleepモード移行までのダミー命令
  delay(1000);
}

void updateTwitterStatus(String tsData)
{
  if (client.connect(thingSpeakAddress, 80))
  { 
    Serial.println("Connected to ThingSpeak.");
    Serial.println("Posting: " + tsData);

    // Create HTTP POST Data
    tsData = "api_key="+thingtweetAPIKey+"&status="+tsData;

    client.print("POST /apps/thingtweet/1/statuses/update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");

    client.print(tsData);
    client.stop();
  }
  else
  {
     Serial.println("Connection failed.");
  }
}
