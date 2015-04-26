// This code has been borrowed from:
// https://github.com/WorldFamousElectronics/PulseSensor_Amped_Arduino
//
// It's not really a library just a set of functions

// Some workarounds
boolean serialVisual = false;
//int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 13;                // pin to blink led at each beat
int fadePin = 5;                  // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin


//////////
/////////  All Serial Handling Code,
/////////  It's Changeable with the 'serialVisual' variable
/////////  Set it to 'true' or 'false' when it's declared at start of code.
/////////

void serialOutput(){   // Decide How To Output Serial.
 if (serialVisual == true){
     arduinoSerialMonitorVisual('-', Signal);   // goes to function that makes Serial Monitor Visualizer
 } else{
      sendDataToSerial('S', Signal);     // goes to sendDataToSerial function
 }
}


//  Decides How To OutPut BPM and IBI Data
void serialOutputWhenBeatHappens(){
 if (serialVisual == true){            //  Code to Make the Serial Monitor Visualizer Work
    Serial.print("*** Heart-Beat Happened *** ");  //ASCII Art Madness
    Serial.print("BPM: ");
    Serial.print(BPM);
    Serial.print("  ");
 } else{
        sendDataToSerial('B',BPM);   // send heart rate with a 'B' prefix
        sendDataToSerial('Q',IBI);   // send time between beats with a 'Q' prefix
 }
}



//  Sends Data to Pulse Sensor Processing App, Native Mac App, or Third-party Serial Readers.
void sendDataToSerial(char symbol, int data ){
    Serial.print(symbol);

    Serial.println(data);
  }





void ledFadeToBeat(){
    fadeRate -= 15;                         //  set LED fade value
    fadeRate = constrain(fadeRate,0,255);   //  keep LED fade value from going into negative numbers!
    analogWrite(fadePin,fadeRate);          //  fade LED
  }


//  Code to Make the Serial Monitor Visualizer Work
void arduinoSerialMonitorVisual(char symbol, int data ){
  const int sensorMin = 0;      // sensor minimum, discovered through experiment
const int sensorMax = 1024;    // sensor maximum, discovered through experiment

  int sensorReading = data;
  // map the sensor range to a range of 12 options:
  int range = map(sensorReading, sensorMin, sensorMax, 0, 11);

  // do something different depending on the
  // range value:
  switch (range) {
  case 0:
    Serial.println("");     /////ASCII Art Madness
    break;
  case 1:
    Serial.println("---");
    break;
  case 2:
    Serial.println("------");
    break;
  case 3:
    Serial.println("---------");
    break;
  case 4:
    Serial.println("------------");
    break;
  case 5:
    Serial.println("--------------|-");
    break;
  case 6:
    Serial.println("--------------|---");
    break;
  case 7:
    Serial.println("--------------|-------");
    break;
  case 8:
    Serial.println("--------------|----------");
    break;
  case 9:
    Serial.println("--------------|----------------");
    break;
  case 10:
    Serial.println("--------------|-------------------");
    break;
  case 11:
    Serial.println("--------------|-----------------------");
    break;

  }
}


/*
  These notes put together by Joel Murphy for Pulse Sensor Amped, Winter 2015
  The code that this section is attached to uses a timer interrupt
  to sample the Pulse Sensor with consistent and regular timing.
  The code is setup to read Pulse Sensor signal at 500Hz (every 2mS).
  The reasoning for this can be found here:
  http://pulsesensor.com/pages/pulse-sensor-amped-arduino-v1dot1

  There are issues with using different timers to control the Pulse Sensor sample rate.
  Sometimes, user will need to switch timers for access to other code libraries.
  Also, some other hardware may have different timer setup requirements. This page
  will cover those different needs and reveal the necessary settings. There are two
  part of the code that will be discussed. The interruptSetup() routine, and
  the interrupt function call. Depending on your needs, or the Arduino variant that you use,
  check below for the correct settings.


  ******************************************************************************************
  ARDUINO UNO, Pro 328-5V/16MHZ, Pro-Mini 328-5V/16MHz (or any board with ATmega328P running at 16MHz)

 >> Timer2

    Pulse Sensor Arduino UNO uses Timer2 by default.
    Use of Timer2 interferes with PWM on pins 3 and 11.
    There is also a conflict with the Tone library, so if you want tones, use Timer1 below.

      void interruptSetup(){
        // Initializes Timer2 to throw an interrupt every 2mS.
        TCCR2A = 0x02;     // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
        TCCR2B = 0x06;     // DON'T FORCE COMPARE, 256 PRESCALER
        OCR2A = 0X7C;      // SET THE TOP OF THE COUNT TO 124 FOR 500Hz SAMPLE RATE
        TIMSK2 = 0x02;     // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
        sei();             // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED
      }

    use the following interrupt vector with Timer2

      ISR(TIMER2_COMPA_vect)

 >> Timer1

    Use of Timer1 interferes with PWM on pins 9 and 10.
    The Servo library also uses Timer1, so if you want servos, use Timer2 above.

      void interruptSetup(){
        // Initializes Timer1 to throw an interrupt every 2mS.
        TCCR1A = 0x00; // DISABLE OUTPUTS AND PWM ON DIGITAL PINS 9 & 10
        TCCR1B = 0x11; // GO INTO 'PHASE AND FREQUENCY CORRECT' MODE, NO PRESCALER
        TCCR1C = 0x00; // DON'T FORCE COMPARE
        TIMSK1 = 0x01; // ENABLE OVERFLOW INTERRUPT (TOIE1)
        ICR1 = 16000;  // TRIGGER TIMER INTERRUPT EVERY 2mS
        sei();         // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED
      }

    Use the following ISR vector for the Timer1 setup above

      ISR(TIMER1_OVF_vect)

 >> Timer0

    DON'T USE TIMER0! Timer0 is used for counting delay(), millis(), and micros().
                      Messing with Timer0 is highly unadvised!

  ******************************************************************************************
  ARDUINO Fio, Lilypad, ProMini328-3V/8MHz (or any board with ATmega328P running at 8MHz)

  >> Timer2

    Pulse Sensor Arduino UNO uses Timer2 by default.
    Use of Timer2 interferes with PWM on pins 3 and 11.
    There is also a conflict with the Tone library, so if you want tones, use Timer1 below.

      void interruptSetup(){
        // Initializes Timer2 to throw an interrupt every 2mS.
        TCCR2A = 0x02;     // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
        TCCR2B = 0x05;     // DON'T FORCE COMPARE, 128 PRESCALER
        OCR2A = 0X7C;      // SET THE TOP OF THE COUNT TO 124 FOR 500Hz SAMPLE RATE
        TIMSK2 = 0x02;     // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
        sei();             // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED
      }

    use the following interrupt vector with Timer2

      ISR(TIMER2_COMPA_vect)

 >> Timer1

    Use of Timer1 interferes with PWM on pins 9 and 10.
    The Servo library also uses Timer1, so if you want servos, use Timer2 above.

      void interruptSetup(){
        // Initializes Timer1 to throw an interrupt every 2mS.
        TCCR1A = 0x00; // DISABLE OUTPUTS AND PWM ON DIGITAL PINS 9 & 10
        TCCR1B = 0x11; // GO INTO 'PHASE AND FREQUENCY CORRECT' MODE, NO PRESCALER
        TCCR1C = 0x00; // DON'T FORCE COMPARE
        TIMSK1 = 0x01; // ENABLE OVERFLOW INTERRUPT (TOIE1)
        ICR1 = 8000;  // TRIGGER TIMER INTERRUPT EVERY 2mS
        sei();         // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED
      }

    Use the following ISR vector for the Timer1 setup above

      ISR(TIMER1_OVF_vect)

 >> Timer0

    DON'T USE TIMER0! Timer0 is used for counting delay(), millis(), and micros().
                      Messing with Timer0 is highly unadvised!


  ******************************************************************************************
  ARDUINO Leonardo (or any board with ATmega32u4 running at 16MHz)

  >> Timer1

    Use of Timer1 interferes with PWM on pins 9 and 10.

      void interruptSetup(){
          TCCR1A = 0x00;
          TCCR1B = 0x0C; // prescaler = 256
          OCR1A = 0x7C;  // count to 124
          TIMSK1 = 0x02;
          sei();
      }

  The only other thing you will need is the correct ISR vector in the next step.

      ISR(TIMER1_COMPA_vect)


  ******************************************************************************************
  ARDUINO Flora, Fio v3 (or any other board with ATmega32u4 running at 8MHz)

  >> Timer1

    Use of Timer1 interferes with PWM on pins 9 and 10.

      void interruptSetup(){
          TCCR1A = 0x00;
          TCCR1B = 0x0C; // prescaler = 256
          OCR1A = 0x3E;  // count to 62
          TIMSK1 = 0x02;
          sei();
      }

  The only other thing you will need is the correct ISR vector in the next step.

      ISR(TIMER1_COMPA_vect)

*/


volatile int rate[10];                    // array to hold last ten IBI values
volatile unsigned long sampleCounter = 0;          // used to determine pulse timing
volatile unsigned long lastBeatTime = 0;           // used to find IBI
volatile int P =512;                      // used to find peak in pulse wave, seeded
volatile int T = 512;                     // used to find trough in pulse wave, seeded
volatile int thresh = 525;                // used to find instant moment of heart beat, seeded
volatile int amp = 100;                   // used to hold amplitude of pulse waveform, seeded
volatile boolean firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
volatile boolean secondBeat = false;      // used to seed rate array so we startup with reasonable BPM


void interruptSetup(){
  // Initializes Timer2 to throw an interrupt every 2mS.
  TCCR2A = 0x02;     // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
  TCCR2B = 0x06;     // DON'T FORCE COMPARE, 256 PRESCALER
  OCR2A = 0X7C;      // SET THE TOP OF THE COUNT TO 124 FOR 500Hz SAMPLE RATE
  TIMSK2 = 0x02;     // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
  sei();             // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED
}


// THIS IS THE TIMER 2 INTERRUPT SERVICE ROUTINE.
// Timer 2 makes sure that we take a reading every 2 miliseconds
ISR(TIMER2_COMPA_vect){                         // triggered when Timer2 counts to 124
  cli();                                      // disable interrupts while we do this
  Signal = analogRead(pulsePin);              // read the Pulse Sensor
  sampleCounter += 2;                         // keep track of the time in mS with this variable
  int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise

    //  find the peak and trough of the pulse wave
  if(Signal < thresh && N > (IBI/5)*3){       // avoid dichrotic noise by waiting 3/5 of last IBI
    if (Signal < T){                        // T is the trough
      T = Signal;                         // keep track of lowest point in pulse wave
    }
  }

  if(Signal > thresh && Signal > P){          // thresh condition helps avoid noise
    P = Signal;                             // P is the peak
  }                                        // keep track of highest point in pulse wave

  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  if (N > 250){                                   // avoid high frequency noise
    if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) ){
      Pulse = true;                               // set the Pulse flag when we think there is a pulse
//      digitalWrite(blinkPin,HIGH);                // turn on pin 13 LED
      IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
      lastBeatTime = sampleCounter;               // keep track of time for next pulse

      if(secondBeat){                        // if this is the second beat, if secondBeat == TRUE
        secondBeat = false;                  // clear secondBeat flag
        for(int i=0; i<=9; i++){             // seed the running total to get a realisitic BPM at startup
          rate[i] = IBI;
        }
      }

      if(firstBeat){                         // if it's the first time we found a beat, if firstBeat == TRUE
        firstBeat = false;                   // clear firstBeat flag
        secondBeat = true;                   // set the second beat flag
        sei();                               // enable interrupts again
        return;                              // IBI value is unreliable so discard it
      }


      // keep a running total of the last 10 IBI values
      word runningTotal = 0;                  // clear the runningTotal variable

      for(int i=0; i<=8; i++){                // shift data in the rate array
        rate[i] = rate[i+1];                  // and drop the oldest IBI value
        runningTotal += rate[i];              // add up the 9 oldest IBI values
      }

      rate[9] = IBI;                          // add the latest IBI to the rate array
      runningTotal += rate[9];                // add the latest IBI to runningTotal
      runningTotal /= 10;                     // average the last 10 IBI values
      BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
      QS = true;                              // set Quantified Self flag
      // QS FLAG IS NOT CLEARED INSIDE THIS ISR
    }
  }

  if (Signal < thresh && Pulse == true){   // when the values are going down, the beat is over
//    digitalWrite(blinkPin,LOW);            // turn off pin 13 LED
    Pulse = false;                         // reset the Pulse flag so we can do it again
    amp = P - T;                           // get amplitude of the pulse wave
    thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
    P = thresh;                            // reset these for next time
    T = thresh;
  }

  if (N > 2500){                           // if 2.5 seconds go by without a beat
    thresh = 512;                          // set thresh default
    P = 512;                               // set P default
    T = 512;                               // set T default
    lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date
    firstBeat = true;                      // set these to avoid noise
    secondBeat = false;                    // when we get the heartbeat back
  }

  sei();                                   // enable interrupts when youre done!
}// end isr
