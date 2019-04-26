//***************************************************************//
//  Name         : HTTP Clients                                  //
//  Author       : RedFox                                        //
//  Website      : lamchucongnghe.com                            //
//  Fanpage      : fb.com//lamchucongnghevn                      //
//  Shop sendo   : sendo.vn/shop/lamchucongnghe_com              //
//***************************************************************//
/* Find Thumbprint
 * Open website -> F12 -> Security teb -> Details -> Thumbprint
 */
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

HTTPClient http;
const char* host = "https://***********************************";
const char* fingerprint = "*************************";

void setup() {
  Serial.begin(9600);
  WiFi.begin("Ten WiFi","Mat khau");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  http.begin(host,fingerprint);
  Serial.println(http.getString());
  if(http.GET() == 200){
    String data = http.getString();
    Serial.println(data);
  }
  http.end();
}

void loop() 
  {
    
  }
