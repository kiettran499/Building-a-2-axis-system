//Transforming the motor's rotary motion into linear motion by using a threaded rod:
//Threaded rod's pitch = 2 mm. This means that one revolution will move the nut 2 mm.
//Default stepping = 400 step/revolution.
// 400 step = 1 revolution = 8 mm linear motion. (4 start 2 mm pitch screw)
// 1 cm = 10 mm =>> 10/8 * 400 = 4000/8 = 500 steps are needed to move the nut by 1 cm.
#include <SoftwareSerial.h>

#include <AccelStepper.h>
#define dirPin1 5
#define stepPin1 2
const float St_Speed1=400; //GO 180 NDEGREE
const float St_Accel1=900;

#define dirPin2 7
#define stepPin2 4
const float St_Speed2=400;
const float St_Accel2=900;

int directionMultiplier = 1; // = 1: positive direction, = -1: negative direction
bool newData, runallowed1, runallowed2 = false; // booleans for new data from serial, and runallowed flag

#define motorInterfaceType 1

AccelStepper Kietstepper1 = AccelStepper(motorInterfaceType, stepPin1, dirPin1);
AccelStepper Kietstepper2 = AccelStepper(motorInterfaceType, stepPin2, dirPin2);

// Define boundaries for X and Y axes
const long X_MIN = 0;
const long X_MAX = 8500; // Example maximum value for X axis
const long Y_MIN = 0;
const long Y_MAX = 8500; // Example maximum value for Y axis
long X_dir;
long Y_dir;

// // Function to check if a string represents a valid integer
// bool isInteger(String str) {
//   if (str.length() == 0) return false;
//   for (int i = 0; i < str.length(); i++) {
//     if (!isDigit(str.charAt(i)) && !(i == 0 && str.charAt(i) == '-')) {
//       return false;
//     }
//   }
//   return true;
// }


void setup() {
	Serial.begin(9600);
	pinMode(LED_BUILTIN, OUTPUT);

	Serial.println("Demonstration of AccelStepper Library"); //print a messages
	Serial.println("Send 'C' for printing the commands.");//   while (!Serial) {
//     ; // Wait for the serial port to connect. Needed for native USB port only
//   }
//   Serial.println("Arduino is ready");

	Kietstepper1.setMaxSpeed(St_Speed1);
	Kietstepper1.setAcceleration(St_Accel1);
	Kietstepper1.disableOutputs(); //disable outputs

	Kietstepper2.setMaxSpeed(St_Speed2);
	Kietstepper2.setAcceleration(St_Accel2);
	Kietstepper2.disableOutputs(); //disable outputs

}


void loop() {
    //Constantly looping through these 2 functions.

  // if (Serial.available() > 0) {
  //   String data = Serial.readStringUntil('\n'); // read until the end of line
  //   Serial.print("Received: ");
  //   Serial.println(data);
  //   // Parse the received data
  //   int commaIndex = data.indexOf(' '); //read the comma ","
  //   String data1 = data.substring(0, commaIndex); //data1 is counted from 0 to ","
  //   String data2 = data.substring(commaIndex + 1); // data2 is from after "," 

	checkSerial(); //check serial port for new commands

	RunTheMotor1(); //function to handle the motor
	RunTheMotor2(); //function to handle the motor

}

void RunTheMotor1() //function for the motor
{
    if (runallowed1 == true)
    {
        Kietstepper1.enableOutputs(); //enable pins
        Kietstepper1.run(); //step the motor (this will step the motor by 1 step at each loop)  
    }
    else //program enters this part if the runallowed is FALSE, we do not do anything
    {
        Kietstepper1.disableOutputs(); //disable outputs
        return;
    }
}

void RunTheMotor2() //function for the motor
{
    if (runallowed2 == true)
    {
        Kietstepper2.enableOutputs(); //enable pins
        Kietstepper2.run(); //step the motor (this will step the motor by 1 step at each loop)  
    }
    else //program enters this part if the runallowed is FALSE, we do not do anything
    {
        Kietstepper2.disableOutputs(); //disable outputs
        return;
    }
}

void checkSerial() {

  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n'); // read until the end of line
    newData = true; //indicate that there is a new data by setting this bool to true

    // Serial.print("Received: ");
    // Serial.println(data);
    // Parse the received data
    int first_spaceIndex = data.indexOf(' '); //read the space " "
    String data1 = data.substring(0, first_spaceIndex); //data1 is counted from 0 to " "
    String data2 = data.substring(first_spaceIndex + 1); // data2 is from after " " 

    int second_spaceIndex = data.indexOf(' ',first_spaceIndex+1);
    String data3 = data.substring(second_spaceIndex + 1); // data2 is from after "," 

    long x = data1.toInt(); // convert data1 to int
    long y = data2.toInt(); // convert data2 to int
    Serial.println(x);
    Serial.println(y);


    // long x = Serial.parseInt();
    // long y = Serial.parseInt();

    // long x = data1; // convert data1 to int
	  // long y = data2; // convert data2 to int



    // Validate and convert the strings to integers
    // if (isInteger(data1) && isInteger(data2)) {
    //   long x = data1.toInt();
    //   long y = data2.toInt();
    // }
	// Constrain the coordinates to be within the boundaries
	// Be constrained to the nearest boundary value, preventing the motors from moving out of the defined range. 
	// For example, if X_MAX is set to 2000 and you send the coordinate 2500,1500, the X motor will move to 2000 instead of 2500
	X_dir = constrain(x, X_MIN, X_MAX);
	Y_dir = constrain(y, Y_MIN, Y_MAX);
	
	
	if (newData == true) //we only enter this long switch-case statement if there is a new command from the computer
	{

		RotateAbsolute_X();
		RotateAbsolute_Y();	
		
		if (x > 8500 || x < 0 || y > 8500 || y < 0) {
			for (int i = 0; i < 3; i++) {
				digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
				delay(500);                      // wait for 1/2 second
				digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
				delay(500);                      // wait for 1/2 second
			}
		}
		
		if (data3 == "home") {
			runallowed1 = true;  
			runallowed2 = true;     
   
			GoHome();
		}
		
		if (data3 == "stop") {
			Kietstepper1.stop(); //stop motor
			Kietstepper2.stop(); //stop motor
			
			Kietstepper1.disableOutputs(); //disable power
			Kietstepper2.disableOutputs(); //disable power
			runallowed1 = false;     
			runallowed2 = false;     
		}
	}


  }
}


void GoHome()
{  
    if (Kietstepper1.currentPosition() == 0 && Kietstepper2.currentPosition() == 0)
    {
        Serial.println("We are at the home position.");
        Kietstepper1.disableOutputs(); //disable power
        Kietstepper2.disableOutputs(); //disable power

    }
    else
    {
        // stepper.setMaxSpeed(400); //set speed manually to 400. In this project 400 is 400 step/sec = 1 rev/sec.
        Kietstepper1.moveTo(0); //set abolute distance to move
        Kietstepper2.moveTo(0); //set abolute distance to move
    }
}


void RotateAbsolute_X()
{
    //We move to an absolute position.
    //The AccelStepper library keeps track of the position.
    //The direction is determined by the multiplier (+1 or -1)
    //Why do we need negative numbers? - If you drive a threaded rod and the zero position is in the middle of the rod...
    // long X = X_dir;
    runallowed1 = true; //allow running - this allows entering the RunTheMotor() function.
    // stepper.setMaxSpeed(receivedSpeed); //set speed  
    Kietstepper1.moveTo(directionMultiplier * X_dir); //set relative distance   
}

void RotateAbsolute_Y()
{
    //We move to an absolute position.
    //The AccelStepper library keeps track of the position.
    //The direction is determined by the multiplier (+1 or -1)
    //Why do we need negative numbers? - If you drive a threaded rod and the zero position is in the middle of the rod...
    // long Y = Y_dir;
    runallowed2 = true; //allow running - this allows entering the RunTheMotor() function.
    // stepper.setMaxSpeed(receivedSpeed); //set speed
    Kietstepper2.moveTo(directionMultiplier * Y_dir); //set relative distance   
}



