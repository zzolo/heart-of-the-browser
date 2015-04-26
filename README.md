# Heart of the Browser

Visualizing a heart beat in the browser.

## Setup

### Hardware

You will need the following:

* An [Arduino](http://store.arduino.cc/category/11) (code only works with Arduino and tested with an Uno)
* A [Pulse Sensor](http://pulsesensor.com/)
* A USB cable to connect the Arduino to your computer (or some other way to get the serial output to your computer).

Connect the Pulse Sensor to the Arduino with the following configuration.

1. Connect the Ground (black) wire to the GND pin on the Arduino.
1. Connect the Power (red) wire to the 5V pin on the Arduino.
1. Connect the Data (purple) wire to the A0 analog pin on the Arduino.

![cablehookup](https://github.com/WorldFamousElectronics/PulseSensor_Amped_Arduino/blob/master/pics/cablehookup.png)

### Software

Get this code:

1. Use the **Download ZIP** button or use [Git](http://git-scm.com/book/en/v2/Getting-Started-Installing-Git) to checkout the code.

### Arduino code

1. Plug-in the Arduino to the computer with the USB cable.
1. Download and open the [Arduino IDE](http://www.arduino.cc/en/main/Software).
1. Go to *File -> Open* and open the `heart-of-the-browser.ino` file from this repository.
1. *Upload* the sketch to your Arduino.  (You can open up the serial monitor in the Ardunio application to double check that data is being output)

## Visualizing

The main visualization is for a web browser.  The following commands are for the Command Line (Terminal) and are expected to be run from the root of this project (except for the first one).

1. Change to the projects directory: `cd heart-of-the-browser`
1. Install [NodeJS](https://nodejs.org/).
1. Install [SerialBot](https://github.com/zzolo/serialbot): `npm install serialbot -g`
1. Install [Bower](http://bower.io/): `npm install bower -g`
1. Install Bower dependencies: `bower install`
1. Run SerialBot with a similar command; you will need to change the serial port path: `serialbot start /dev/cu.usbmodemXXX -b 115200`
    * Use the following command to get a list of serial ports to find the one that the Arduino is connect to: `serialbot list`
1. Open `index.html` in a web browser.
    * It is suggest to use a simple web server.  For instance the following will install a simple web server and make one at port `8080`: `npm install http-server -g && http-server`
