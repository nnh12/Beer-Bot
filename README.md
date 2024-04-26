# Project Overview: 

The general aim of our project is to create an autonomous beverage delivery car that begins moving once a drink is placed onto a platform detecting the weight of a drink on it, allowing any patron to grab the drink off the car as it drives along. Once in motion, the car keeps driving until ultrasonic sensors on the front and back of the car detect the drop off from a table or bar top to the floor. Once the edge is detected, the vehicle stops and then reverses direction to drive back towards the bartender or barista at the other side it came from.
We have three different MSP430 microcontrollers. We have one for PWM for the motors, the second for ultrasonic sensors, and the third for the weight sensor. 

### Motor Controller:
In order to drive the motors, we integrated a separate large current 50A H-Bridge motor driver. In essence, our MSP430 motor.c will input two PWM channels into the driver to control the speed and direction of the wheel.
The two different PWM channels (Channel 1 + Channel 2) correspond to the forward or reverse direction. If the car needs to go forward, then the MSP will turn on Channel 1 and will turn off Channel 2. If the car needs to go in reverse, then the MSP will turn on Channel 2 and will turn off Channel 1. When we “turn it off,” we adjust the duty cycle of the PWM to zero and when we “turn it ON”  we set the PWM to approximately 71%.


### Ultrasonic Distance Sensors:
To detect the edges of tables or bar tops, we implemented two ultrasonic distance sensors, with each one being on either the front or back of the car. The ultrasonic sensor, HC-SR04, provides non-contact measurements within 2cm-400cm with 3mm of accuracy. The sensor includes an ultrasonic transmitter, receiver, and control circuit. The sensor uses the interrupt trigger trigger for at least 10us high level signal, with the module also sending eight 40kHz sonic bursts. It detects whether a pulse signal is received back. If the signal returned back meets the criteria of being through high level, the time of high output IO duration is the time required for the signal to return from when it was sent. 

If a “far” distance is read (as in the edge was detected), the ultrasonic sensor will communicate to the motors over GPIO to tell it to go backwards or stop via an interrupt.

### Weight Sensor:
For the weight sensor, we integrated a Hx711 Amplifier Board. This board has an internal built-in Analog to Digital Converter (ADC). The Hx711 will transmit the weight information over a serial wire and a clock pin. The Clock pin will indicate when the transmitted bits should be read. The microcontroller will communicate with the Hx711 Amplifier over Serial to obtain a digital value. If the value is greater than a threshold, the microcontroller will communicate over GPIO to the Motor controller to power the motors up via an interrupt.  


