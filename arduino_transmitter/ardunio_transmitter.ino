#include <IRremote.h>
#include <IRremoteInt.h>

IRsend irsend;
const int LED = 13;

void setup()
{
  pinMode(LED, OUTPUT);
}

void loop() {
  
  digitalWrite(LED, HIGH); // LED anschalten
  irsend.sendSony(10, 12);
  digitalWrite(LED, LOW); // LED ausschalten
  delay(300); 
}
