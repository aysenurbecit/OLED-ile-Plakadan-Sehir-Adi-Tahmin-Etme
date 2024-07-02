#include <WiFi.h>
#include <HTTPClient.h>
#include <Deneyap_DokunmatikTusTakimi.h> 
#include <ArduinoJson.h>
#include <Deneyap_OLED.h>               

OLED OLED;       
Keypad TusTakimi; 
 
const char* ssid = "SUPERONLINE";
const char* password = "Mm127734**";

// API bilgileri
const char* apiEndpoint = "http://api.geonames.org/searchJSON";
const char* username = "aysenurbecit";

String cityCode = "";

void setup() {
  Serial.begin(115200);
 // OLED.begin(0x7A);
  // Wi-Fi bağlantısı
 if (!TusTakimi.begin(0x0E)) {                     // begin(slaveAdress) fonksiyonu ile cihazların haberleşmesi başlatılması
    Serial.println("I2C bağlantısı başarısız ");    // I2C bağlantısı başarısız olursa seri terminale yazdırılması
    while (1);
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Bağlanılıyor...");
  }
  Serial.println("Wi-Fi bağlı.");

  // OLED ekran başlatma
  if(!OLED.begin(0x7A)) {
    Serial.println("OLED ekran bulunamadı");
    for(;;);
  }
  OLED.clearDisplay();
}

void loop() {
  int Code = TusTakimi.KeypadRead();  
        Serial.println(Code);          // Tuş takımına basılan değeri okuması
  if (Code != 0xFF) {                        // Tuş takımına basıldığında
    if (cityCode.length() <= 2) {
      cityCode += String(Code);           // cityCode'a tuş takımı değerini ekle
    }
      Serial.println(cityCode);
  }

   // cityCode.trim();

    // API çağrısı
    if (WiFi.status() == WL_CONNECTED) {
     HTTPClient http; 
     //http://api.geonames.org/searchJSON?country=TR&code=34&username=aysenurbecit
    // http://api.geonames.org/searchJSON?country=TR&code=34&username=aysenurbecit ry=TR&featureCode=PPLC&code=

      String url = String(apiEndpoint) + "?country=TR&q=" + cityCode + "&username=" + username;
      http.begin(url);
      int httpCode = http.GET();

      if (httpCode > 0) { // Basarili istek durumunda devam et
          String payload = http.getString();
          char json[512]; // String'i char dizisine cevir
          payload.toCharArray(json, 512);

          // JSON verisini işle
          StaticJsonDocument<512> doc;
          deserializeJson(doc, json);
          String cityName = doc["geonames"][0]["toponymName"];
          String cityInfo = doc["geonames"][0]["population"];

        // Ekranda gösterme
        OLED.clearDisplay();

        OLED.setTextXY(0, 0);
        OLED.putString("Sehir Kodu: " + cityCode);
        OLED.setTextXY(2, 0);
        OLED.putString("Sehir: " + cityName); // Turkce karakterler bosluk olarak gorunecektir
        OLED.setTextXY(4, 0);
        OLED.putString("Bilgi: " + cityInfo);

      } 
      else {
        Serial.println("HTTP hatası: " + String(httpCode));
      }
      http.end();
    } else {
      Serial.println("Wi-Fi bağlantısı yok");
    }
  }
