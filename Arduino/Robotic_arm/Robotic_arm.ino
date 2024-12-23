
#include <Servo.h> //Servo header file


//Declare object for 5 Servo Motors  

Servo Servo_0;

Servo Servo_1;

Servo Servo_2;

Servo Servo_3;

Servo Gripper;


//Global Variable Declaration 

int S0_pos, S1_pos, S2_pos, S3_pos, G_pos; 

int P_S0_pos, P_S1_pos, P_S2_pos, P_S3_pos, P_G_pos;

int C_S0_pos, C_S1_pos, C_S2_pos, C_S3_pos, C_G_pos;

int POT_0,POT_1,POT_2,POT_3,POT_4;

int buttonPin = 2; // the digital pin to read the state of the push button
int buttonState = 0; // variable to store the state of the push button



int saved_data[700]; //Array for saving recorded data


int array_index=0;

bool isRecording;
bool isPlaying;


int action_pos;

int action_servo;


void setup() {

Serial.begin(9600); //Serial Monitor for Debugging


//Declare the pins to which the Servo Motors are connected to 

Servo_0.attach(3);

Servo_1.attach(5);

Servo_2.attach(6);

Servo_3.attach(9);

Gripper.attach(10);


//Write the servo motors to initial position 

Servo_0.write(0);

Servo_1.write(70);

Servo_2.write(0);

Servo_3.write(0);

Gripper.write(1);
 

}


void Read_POT() //Function to read the Analog value form POT and map it to Servo value

{

   POT_0 = analogRead(A0); POT_1 = analogRead(A1); POT_2 = analogRead(A3); POT_3 = analogRead(A4); POT_4 = analogRead(A5); //Read the Analog values form all five POT

   S0_pos = map(POT_0,0,1024,0,180); //Map it for 1st Servo (Base motor)

   S1_pos = map(POT_1,0,1024,30,90); //Map it for 2nd Servo (Hip motor)

   S2_pos = map(POT_2,0,1024,0,70); //Map it for 3rd Servo (Shoulder motor)

   S3_pos = map(POT_3,0,1024,0,80); //Map it for 4th Servo (Neck motor)

   G_pos  = map(POT_4,0,1024,0,180);  //Map it for 5th Servo (Gripper motor)

}


void Record() //Function to Record the movements of the Robotic Arm

{

Read_POT(); //Read the POT values  for 1st time


//Save it in a variable to compare it later

   P_S0_pos = S0_pos;

   P_S1_pos = S1_pos;

   P_S2_pos = S2_pos;

   P_S3_pos = S3_pos;

   P_G_pos  = G_pos;

   

Read_POT(); //Read the POT value for 2nd time

  

   if (P_S0_pos == S0_pos) //If 1st and 2nd value are same

   {

    Servo_0.write(S0_pos); //Control the servo

    

    if (C_S0_pos != S0_pos) //If the POT has been turned 

    {

      saved_data[array_index] = S0_pos + 0; //Save the new position to the array. Zero is added for zeroth motor (for understading purpose)

      array_index++; //Increase the array index 

    }

    

    C_S0_pos = S0_pos; //Saved the previous value to check if the POT has been turned 

   }


//Similarly repeat for all 5 servo Motors

   if (P_S1_pos == S1_pos)

   {

    Servo_1.write(S1_pos);

    

    if (C_S1_pos != S1_pos)

    {

      saved_data[array_index] = S1_pos + 1000; //1000 is added for 1st servo motor as differentiator 

      array_index++;

    }

    

    C_S1_pos = S1_pos;

   }


   if (P_S2_pos == S2_pos)

   {

    Servo_2.write(S2_pos);

    

    if (C_S2_pos != S2_pos)

    {

      saved_data[array_index] = S2_pos + 2000; //2000 is added for 2nd servo motor as differentiator 

      array_index++;

    }

    

    C_S2_pos = S2_pos;

   }


   if (P_S3_pos == S3_pos)

   {

    Servo_3.write(S3_pos); 

    

    if (C_S3_pos != S3_pos)

    {

      saved_data[array_index] = S3_pos + 3000; //3000 is added for 3rd servo motor as differentiater 

      array_index++;

    }

    

    C_S3_pos = S3_pos;   

   }


   if (P_G_pos == G_pos)

   {

    Gripper.write(G_pos);

    

    if (C_G_pos != G_pos)

    {

      saved_data[array_index] = G_pos + 4000; //4000 is added for 4th servo motor as differentiator 

      array_index++;

    }

    

    C_G_pos = G_pos;

   }

   
  delay(100); 

}


 


void Play() //Functon to play the recorded movements on the Robotic ARM

{

  for (int Play_action=0; Play_action<array_index; Play_action++) //Navigate through every saved element in the array 

  {

    action_servo = saved_data[Play_action] / 1000; //The fist character of the array element is split for knowing the servo number

    action_pos = saved_data[Play_action] % 1000; //The last three characters of the array element is split to know the servo postion 


    switch(action_servo){ //Check which servo motor should be controlled 

      case 0: //If zeroth motor

        Servo_0.write(action_pos);

      break;


      case 1://If 1st motor

        Servo_1.write(action_pos);

      break;


      case 2://If 2nd motor

        Servo_2.write(action_pos);

      break;


      case 3://If 3rd motor

        Servo_3.write(action_pos);

      break;


      case 4://If 4th motor

        Gripper.write(action_pos);

      break;

    }


    delay(100);

    

  }

}


void startRecording() {
  // turn on the LED to indicate that recording has started
  digitalWrite(13, HIGH);

  // start recording
  isRecording = true;
}
 
void startPlayback() {
  // send a message through the serial port to indicate that playback has started
  Serial.println("Playback started");

  // start playback
  isPlaying = true;
}

void loop() {

 // read the state of the push button
  buttonState = digitalRead(buttonPin);

  // if the push button is pressed, start recording
  if (buttonState == HIGH) {
    Record();
  }

  // if the push button is released, stop recording and start playback
  if (buttonState == LOW && isRecording) {
    Play();
  }

}
