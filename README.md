# Building-a-2-axis-system

<p align="center">
  <img width="500" src="https://github.com/kiettran499/Building-a-2-axis-system/blob/main/8ab2c9de-ff6f-4c74-bce4-126497427e46.jpg">
  
## Description

- This is self-learning project which builds a 2-axis system and controlled by two Stepper motors (move to specified X,Y positions in space). Then a software is built for the user to control the system. The detail of the software is: the Controller (human) can dictate the System via REST API (using Postman to send message). Then the message from REST API is sent to the Socket.io server. Next, an Arduino (used to control motors) receives command from Socket.io server and executes the tasks. And the server software must run on Docker.
- Boundary Test: mechanisms to prevent human error:
+ The robot or software must not collapse due to incorrect user input such as entering a character, word or sentence.
+ Upon detecting a positional error like when the controller enters a value that exceeds the coordinate limit or smaller than 0, it will trigger an LED alert and go to the end position of the system or origin.

<p align="center">
  <img width="500" src="https://github.com/kiettran499/Building-a-2-axis-system/blob/main/440531668_463032149453070_369006714125849922_n.png">

## Hardware

- Frame made from Aluminum Extrusion
- Arduino Nano Board
- Stepper Motor Driver Microstep
- NEMA 23 Stepper Motor
- Adapter

## Software/Tool

- Arduino IDE
- VS Code
- Fast API
- Postman
- Socket.io
- Docker

