#include <ESP8266WiFi.h>
#include <time.h>

String  t;
int timeZone = 7;

void setup() {
  Serial.begin(9600);
  configTime(timeZone * 3600, 0, "pool.ntp.org", "time.nist.gov");
  WiFi.begin("hieu","12344321");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void get_time() {
  time_t now = time(nullptr);
  t = ctime(&now);
  Serial.println(t);
}

void loop() {
  get_time();
  delay(1000);
}
