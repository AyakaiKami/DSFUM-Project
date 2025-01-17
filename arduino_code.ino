#include <IRremote.h>

int i=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  IrReceiver.begin(3, ENABLE_LED_FEEDBACK);
  pinMode(13, OUTPUT);
  Serial.println("IR-receiver active");
}

void interpret(decode_results *results)
{
  ///
}

void loop() {
  ///if there is a command
  if (IrReceiver.decode()) 
  {
    Serial.println(IrReceiver.decodedIRData.decodedRawData,HEX);
    //IrReceiver.printIRResultShort(&Serial); for debugging 
    IrReceiver.resume();
  }
}
