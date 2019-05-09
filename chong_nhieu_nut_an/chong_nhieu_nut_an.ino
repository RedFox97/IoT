const int buttonPin = 4;    // the number of the pushbutton pin
const int ledPin = 17;      // the number of the LED pin

// Variables will change:
int ledState;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 300;    // the debounce time; increase if the output flickers

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (digitalRead(buttonPin) != lastButtonState) {          // Nút được nhấn hoặc nhả
    lastDebounceTime = millis();                            // Gán thời gian ấn nút
  }
  Serial.println(millis() - lastDebounceTime);
  if ((millis() - lastDebounceTime) > debounceDelay) {      // Bấm nút được đủ thời gian debounceDelay thì thực hiện lệnh
    if (digitalRead(buttonPin) != buttonState) {            // Kiểm tra trạng thái hiện tại với trạng thái nút bấm lần bấm trước
      buttonState = digitalRead(buttonPin);                 // Lưu trạng thái bấm nút mới
      if (buttonState == HIGH) {                            // Nếu nút đã được nhả thì thay đổi trạng thái led
        ledState = !ledState;
      }
    }
  }
  digitalWrite(ledPin, ledState);
  lastButtonState = digitalRead(buttonPin);                 // Liên tục cập nhật trạng thái nút nhấn
}
