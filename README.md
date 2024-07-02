# OLED-ile-Plakadan-Sehir-Adi-Tahmin-Etme
Tus takimindan girilen plaka kodu Geonames API ile taratilir, ait oldugu sehir ve bilgileri OLED ekrana basilir.

## Gereksinimler
- Tus Takımı
- OLED ekran
- Deneyap Kart 1A

## Kurulum - Kullanım
I2C bağlantı kabloları ile bağlantılar yapılır. 

Oyun nasıl oynanır?

Önce bir plaka söylenir ve söylenen plaka kodunun ait olduğu şehir ismi tahmin edilmeye çalışılır. Tahminden sonra, tuş takımından plaka değeri girilir. Girilen plaka değeri "Geonames" API ile taranır ve karşılık geldiği şehir adıyla mevcut nüfusu ekrana yazdırılır. Başka bir plaka numarası girildiğinde bu işlem tekrar eder. 

Oyunumuz Türkiye için plaka kodlarını taratan bir formattadır. Dilerseniz başka ülkelere de koddaki url de değişiklikler yaparak uyarlayabilirsiniz. Şehir isimleri ile beraber o şehre özgü diğer bilgileri de görüntülemek için kodu dilediğiniz şekilde (JSON'daki mevcut verilere bağımlı) düzenleyebilirsiniz. 
