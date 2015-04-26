/**
 * Sends some data over serial which will be read in the browser.
 *
 * Code adapted from:
 * https://github.com/WorldFamousElectronics/PulseSensor_Amped_Arduino
 * http://www.pulsesensor.com
 */

// Variables that you might want to change
// Pulse Sensor purple wire connected to analog pin 0
//#define PULSE_PIN 0;
// This should be a constant, but for easier maintenance, we use what the
// "lib" expects
int pulsePin = 0;


// Volatile Variables, used in the interrupt service routine.  You should not need to change these.
// int that holds raw Analog in 0. updated every 2mS
volatile int BPM;
// holds the incoming raw data
volatile int Signal;
// int that holds the time interval between beats! Must be seeded!
volatile int IBI = 600;
// "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean Pulse = false;  
// becomes true when Arduoino finds a beat.
volatile boolean QS = false;       


// Main setup tassk
void setup() {
  // Run serial very fast
  Serial.begin(115200);
  
  // sets up to read Pulse Sensor signal every 2mS
  interruptSetup();
  
  // UN-COMMENT THE NEXT LINE IF YOU ARE POWERING The Pulse Sensor AT LOW VOLTAGE,
  // AND APPLY THAT VOLTAGE TO THE A-REF PIN
  // analogReference(EXTERNAL);
}


//  Main loop
void loop() {

  // Send data to serial
  sendDataToSerial('S', Signal);
  
  //  A Heartbeat Was Found.  BPM and IBI have been Determined.
  if (QS == true) {
    // send heart rate with a 'B' prefix
    sendDataToSerial('B',BPM);
    // send time between beats with a 'Q' prefix   
    sendDataToSerial('Q',IBI);
    // Clear beat flag
    QS = false;
  }

  // Short break
  delay(20);
}
