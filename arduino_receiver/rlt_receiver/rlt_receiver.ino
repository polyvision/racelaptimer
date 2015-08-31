/*
 * 
 */
#include <IRremote.h>
#include <IRremoteInt.h>

#define IR_REMOTE_PIN 11
IRrecv irrecv(IR_REMOTE_PIN);
decode_results results;

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) 
  {
      Serial.println(results.value, DEC); // Print the Serial 'results.value'
      irrecv.resume();   // Receive the next value
  }
}
