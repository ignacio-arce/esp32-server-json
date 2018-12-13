#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <ArduinoJson.h>

#define FORMAT_SPIFFS_IF_FAILED true

#ifdef __cplusplus
extern "C" {
#endif
  uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();


const char* ssid = "wifi name";
const char* password =  "wifi password";

int contador=0;

AsyncWebServer server(80);

static String getJsonString(String s) {
    s.remove(s.length()-1);
    return s + "]";
}

void setup(){
  Serial.begin(115200);

  delay(1000);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  // JSON

  static String jsonStr = "";
  static String salidaAux = "[";
  static String salida ="";

  server.on("/data.json", HTTP_GET, [salida](AsyncWebServerRequest *request){
    request->send(200, "application/json", salida);
  });

  server.begin();

  while(true) {
    StaticJsonBuffer<300> jsonBuffer;
    JsonArray& data = jsonBuffer.createArray();
    
    contador++;
    
    data.add(((temprature_sens_read()-32)/1.8));
    data.add(contador);
    data.printTo(jsonStr);
    
    salidaAux +=jsonStr + ",";
    jsonStr="";
    jsonBuffer.clear();
    salida = getJsonString(salidaAux);
    
    delay(3000);
  }
  
}

void loop(){}
