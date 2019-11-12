/*
 * Breitbandlambda uses one serial port, so better to use an arduino mega 2560 if you want to print out debug messages on Serial
 * 
 */


#define LAMBDASERIAL Serial2
#include <BreitBandLambda.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  delay(500);
  int la = LAMBDA1.Lambda();
  if(la<0) {
    Serial.println(pstr_lambdaErrors(la));
    delay(5);
   }
  else {
   int a=la/100;
   Serial.print("lambda=");
   Serial.print(a==1?"1.":"0.");
   int hundreds=la - 100 * a;
   int tenth = hundreds / 10;
   Serial.print(tenth);
   int m = hundreds - tenth * 10;
   Serial.print(m);
  }
}
