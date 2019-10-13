#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "THE ELITE FOUR'S KEY WENT HERE";


void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
  Blynk.begin(auth); 
    while (Blynk.connect() == false) {
    // Wait until connected
  }
pinMode(3,OUTPUT);
pinMode(4,OUTPUT);
pinMode(5,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}
