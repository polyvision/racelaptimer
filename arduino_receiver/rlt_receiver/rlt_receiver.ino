/*
 * 
 */
#include <IRremote.h>
#include <IRremoteInt.h>

#define LED_PIN 8
#define IR_REMOTE_PIN 10
IRrecv irrecv(IR_REMOTE_PIN);
decode_results results;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

  pinMode(13, OUTPUT);
}

void loop() {
  
  if (irrecv.decode(&results)) 
  {
      Serial.println(results.value, DEC); // Print the Serial 'results.value'
      digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(10);              // wait for a second
      digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
      irrecv.resume();   // Receive the next value
  }
  
  delay(10);              // wait for a second
}
