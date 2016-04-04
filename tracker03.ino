/*
 * Servo control limits
 * Tilt:  650   up
 *        1800  down
 *        
 * Pan:   600   right
 *        2000  left
 *        
 * Pins:
 * A0   RSSI Left
 * A1   RSSI Right
 * A2   RSSI Bottom
 * D9   Tilt servo yellow lead
 * D10  Pan servo yellow lead
 */

#include <Servo.h>
Servo servoT;
Servo servoP;

int pan = 1225;
int tilt = 1300;

int antL = A0;
int antR = A1;
int antB = A2;

bool boolSignal = 0;

float antLRSSI = 1; // A0 value
float antRRSSI = 1; // A1 value
float antBRSSI = 1; // A2 value
float antURSSI; // (antL + antR)/2
///////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  
  // Define the limits of the servos rotation.
  servoP.attach(10,600,2000); 
  servoT.attach(9,650,1800);

  // Set the gimbal to a level starting position.
  servoP.write(1200); // about centred
  servoT.write(1350); //about level
}
///////////////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
  readRSSI();
  //pointAnt();
}
///////////////////////////////////////////////////////////////////////////////////////////////
void readRSSI()
{
  // Read the signal strength (RSSI) from each of the positioning antenna.
  
  // Comment these out and the real ones back in once I can read the RSSI.
  //antLRSSI = 1;
  //antRRSSI = 1;
  //antBRSSI = 1;
  
  antLRSSI = analogRead(antL);
  antRRSSI = analogRead(antR);
  antBRSSI = analogRead(antB);
  antURSSI = (antLRSSI + antRRSSI)/2;

  Serial.print("L RSSI:\t");
  Serial.print(antLRSSI);
  Serial.print("\tR RSSI:\t");
  Serial.print(antRRSSI);
  Serial.print("\tU RSSI:\t");
  Serial.print(antURSSI);
  Serial.print("\tB RSSI:\t");
  Serial.println(antBRSSI);
}
///////////////////////////////////////////////////////////////////////////////////////////////
void pointAnt()
{
  // Now we know where the signal strength is strongest, we can use the servos on the gimbal to 
  // point the receiving antenna to the signal source.
  if (antLRSSI > 500)
  {
    if (antRRSSI > 500)
    {
      if (antBRSSI > 500)
      {
        if (antURSSI > 500)
        {
          boolSignal = 0;
          //We've not got a signal on any of the positioning antennas so return to central position
          servoP.write(1225);
          servoP.write(1300);
        }
        else
        {
          boolSignal = 1;
        }
      }
    }
  }

  if (boolSignal = 1) // We have a signal, so follow it
  {
    // pan
    // right
    if (antLRSSI > antRRSSI) // The RSSI output goes low for high signal strength. Not what you'd expect.
    {
      servoP.write(pan); 
      pan--;
    }
    // left
    if (antLRSSI < antRRSSI)
    {
      servoP.write(pan); 
      pan++; 
    }

    // tilt
    // up
    if (antBRSSI > antURSSI)
    {
      servoP.write(tilt); 
      tilt--; 
    }
    // down
    if (antBRSSI < antURSSI)
    {
      servoP.write(tilt); 
      tilt++; 
    }
  }
}
