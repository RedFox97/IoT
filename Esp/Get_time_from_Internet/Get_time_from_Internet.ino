/*
 * Author: RedFox97
 */
#include <ESP8266WiFi.h>
#include <time.h>
#include <Ticker.h>

const char* ssid = "........";
const char* password = "........";

Ticker flip;

String dayOfWeek, thang, ngay, gio, phut, giay, nam;
int _gio, _phut, _giay;
void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  capNhatThoiGian();
  flip.attach(1,ngat);
}

void loop() {
  if(nam.toInt() < 2000) {
    capNhatThoiGian();
  }
}

void ngat() {
  _giay++;
  if(_giay == 60) {
    _phut++;
    _giay = 0;
  }
  if(_phut == 60) {
    _gio++;
    _phut = 0;
  }
  if(_gio == 24)
    capNhatThoiGian();
  Serial.println(thang+"-"+ngay+"-"+nam);
}

void capNhatThoiGian() {
  time_t now = time(nullptr);
  String data = ctime(&now);
  Serial.print(data);
  // Tách dữ liệu
  dayOfWeek     = data.substring(0,3);
  String thang  = data.substring(4,7);
  ngay           = data.substring(8,10);
  gio          = data.substring(11,13);
  phut        = data.substring(14,16);
  giay        = data.substring(17,19);
  nam          = data.substring(20,24);

  //Biến đổi
  dayOfWeek.toUpperCase();
  if(ngay.toInt() < 10) {
    ngay = "0" + ngay;
  }
  if(thang=="Jan")  thang="01";
  if(thang=="Feb")  thang="02";
  if(thang=="Mar")  thang="03";
  if(thang=="Apr")  thang="04";
  if(thang=="May")  thang="05";
  if(thang=="Jun")  thang="06";
  if(thang=="Jul")  thang="07";
  if(thang=="Aug")  thang="08";
  if(thang=="Sep")  thang="09";
  if(thang=="Oct")  thang="10";
  if(thang=="Nov")  thang="11";
  if(thang=="Dec")  thang="12";
  _gio       = gio.toInt();
  _phut      = phut.toInt();
  _giay      = giay.toInt();
}
