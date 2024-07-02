#include <WiFi.h>
#include <HTTPClient.h>
#include <Deneyap_DokunmatikTusTakimi.h> 
#include <ArduinoJson.h>
#include <Deneyap_OLED.h>               

OLED OLED;         //oled icin class tanimlamasi
Keypad TusTakimi;  //Tus takimi icin class tanimlamasi
 
const char* ssid = "Wifi Adi";
const char* password = "Wifi sifresi";

// API bilgileri
const char* apiEndpoint = "http://api.geonames.org/searchJSON";
const char* username = "kullanici adiniz";  //kullanici adina "http://api.geonames.org" adresine uye olurken sectiginiz kullanici adini yazmalisiniz

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

  if(!OLED.begin(0x7A)) {  // OLED ekran başlatma
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
      Serial.println(cityCode);           // seri monitore yazdir
  }

    // API çağrısı
    if (WiFi.status() == WL_CONNECTED) {
     HTTPClient http; 

      String url = String(apiEndpoint) + "?country=TR&q=" + cityCode + "&username=" + username;
      http.begin(url);
      int httpCode = http.GET(); // istekte bulun

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
