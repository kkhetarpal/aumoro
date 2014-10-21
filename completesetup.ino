/*
  Motor Control.
*/


//User Input
char c, l, r;
int speed = 0;
int max_speed = 250, reduced_speed = 180;

//sensor input variables and pins
int sensor1 = A0, sensor2 = A1, sensor3 = A2;
float sensor1Value = 0, sensor2Value = 0, sensor3Value = 0;
float new_sensor1Value = 0, new_sensor2Value = 0, new_sensor3Value = 0;



//states
int state = 0;
int left,right,front;


// pins for the Motor 1:
const int M1_dir1 = 50;
const int M1_dir2 = 52;
const int pwm1= 2;
const int M2_dir1 = 40;
const int M2_dir2 = 42;
const int pwm2= 4;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // make the pins outputs:
  pinMode(M1_dir1, OUTPUT); 
  pinMode(M1_dir2, OUTPUT); 
  pinMode(pwm1, OUTPUT); 
  pinMode(M2_dir1, OUTPUT); 
  pinMode(M2_dir2, OUTPUT); 
  pinMode(pwm2, OUTPUT); 
  
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  
}




void loop()
{
   
    
      //read sensor values
      readSensor();
      
      //generate states for each sensor
      right = generatestates(sensor1Value);         //A0 == Sensor1     //Left is A0 
      left = generatestates(sensor2Value);        //A1 == Sensor1     //Front is A1 
      front = generatestates(sensor3Value);        //A2 == Sensor1     //Right is A2
      
      //Print Sensor Values
      //Serial.print("left:"); 
      //Serial.print(left); 
      //Serial.print(" ");
      //Serial.print("front:"); 
      //Serial.print(front);
      //Serial.print(" ");
      //Serial.print("right:"); 
      //Serial.println(right);
            
      //decision making module
      decide_motion();
      
      
      
      
      //Keyboard Control of Motors       
      while (Serial.available() > 0)     //while user gives input
      {

      c =  Serial.read();               //read user input
      
      if (c == 'A')                    //turn left  //Keyboard input A
        {
            turn('l', 200);
        
        }
      else if(c == 'W')                //forward  //Keyboard input W
        {
        
            forward(250);
      
        }
      else if(c == 'D')                //turn right  //Keyboard input D
        {
        
            turn('r', 200);
       
        } 
       else if(c == 'X')              //backward    //Keyboard input X
        {
        
            backward(250);
       
        } 
       else if(c == 'S')              //backward    //Keyboard input X
        {
        
            runMotor1(0);
            runMotor2(0);
       
        } 
      } //end of while
      
      
} //end of main loop
//*******************************************************//  





//function to run motor 1
void runMotor1(int speed_M1)
{
  if (speed_M1 > 0)        //move forward
    {
    
      digitalWrite(M1_dir1, HIGH);
      digitalWrite(M1_dir2, LOW);
      analogWrite(pwm1, 255-speed_M1);
    }  
  else if(speed_M1 < 0)    //move backward
    {
      digitalWrite(M1_dir1, LOW);
      digitalWrite(M1_dir2, HIGH);
      analogWrite(pwm1, 255+speed_M1);
    }
  else if(speed_M1 == 0)
    {
      digitalWrite(M1_dir1, LOW);
      digitalWrite(M1_dir2, LOW);
      analogWrite(pwm1, speed_M1);
    }
}
//end of function to run motor 1
//*******************************************************//



//function to run motor 2
void runMotor2(int speed_M2)
{
  if (speed_M2 > 0)        //move forward
    {
    
      digitalWrite(M2_dir1, LOW);
      digitalWrite(M2_dir2, HIGH);
      analogWrite(pwm2, 255 - speed_M2);
    }  
  else if(speed_M2 < 0)    //move backward
    {
      digitalWrite(M2_dir1, HIGH);
      digitalWrite(M2_dir2, LOW);
      analogWrite(pwm2, 255 + speed_M2);
    }
  else if(speed_M2 == 0)
    {
      digitalWrite(M2_dir1, LOW);
      digitalWrite(M2_dir2, LOW);
      analogWrite(pwm2, speed_M2);
    }
}

//end of function to run motor 2
//*******************************************************//



//function to turn left or right
void turn(char direction, int sharpness)
{
   if( direction == 'l')              //turn left
    {
      runMotor1(speed + sharpness);
      runMotor2(speed - sharpness);
            
    } 
   else if( direction == 'r')              //turn right
    {
      runMotor1(speed - sharpness);
      runMotor2(speed + sharpness);
            
    } 
}
//end of function to turn
//*******************************************************//




////function to read Sensor Values 
//void readSensor()
//{
//  new_sensor1Value = analogRead(sensor1);    //Sensor Inputs    
//  new_sensor2Value = analogRead(sensor2);
//  new_sensor3Value = analogRead(sensor3);
//   
//  //complimentary filter
//  sensor1Value = (0.8*sensor1Value) + (0.2* new_sensor1Value);
//  sensor2Value = (0.8*sensor2Value) + (0.2* new_sensor2Value);
//  sensor3Value = (0.8*sensor3Value) + (0.2* new_sensor3Value);
//  
//  //printing sensor values
//  //Serial.println(sensor1Value);
//  //Serial.println(sensor2Value);
//  //Serial.println(sensor3Value);
//  //
//}
////end of read sensor function
////*******************************************************//  



//imdl
//function to read Sensor Values 
void readSensor()
{
  sensor1Value = analogRead(sensor1);    //Sensor Inputs    
  //sensor2Value = analogRead(sensor2);
  sensor3Value = analogRead(sensor3);
   
//  //complimentary filter
//  sensor1Value = (0.8*sensor1Value) + (0.2* new_sensor1Value);
//  sensor2Value = (0.8*sensor2Value) + (0.2* new_sensor2Value);
//  sensor3Value = (0.8*sensor3Value) + (0.2* new_sensor3Value);
//  
  //printing sensor values
  Serial.print(sensor1Value);
  Serial.print(" ");
  //Serial.print(sensor2Value);
  Serial.println(sensor3Value);
  //
}
//end of read sensor function
//*******************************************************//  




//
////function to generate states from sensor readings
//int generatestates(float sensorvalue)
//{
// if (sensorvalue >= 450)                                    //State STOP denoted by value 0
//   state = 0;
// else  if ((450 < sensorvalue) && (sensorvalue >= 350))     //State CLOSE denoted by value 1
//   state = 1;
// else if ((350 < sensorvalue) && (sensorvalue >= 320))      //State APPROACHING CLOSE denoted by value 2
//   state = 2;
//  else if ((sensorvalue < 320))                             //State FAR denoted by value 3
//   state = 3;
//   
// return state;
//}
////end of generate states function
////*******************************************************//  



//function to move forward
void forward(int fwd_speed)
{
    runMotor1(fwd_speed);
    runMotor2(fwd_speed);
  
}
//end of forward function
//*******************************************************//  


void backward(int bck_speed)
{
    runMotor1(-bck_speed);
    runMotor2(-bck_speed);
  
}
//end of backward function
//*******************************************************//  




void stop(int stp_speed)
{
    runMotor1(0);
    runMotor2(0);
}
//end of stop function
//*******************************************************//  




//function to decide motion
//void decide_motion()
//{
//   if ((left == 0) && (front == 0) && (right == 0)) 
//   {
//     backward(200);
//     //stop(0);
//   }
//   else if ((left == 3) && (front == 3) && (right == 3)) 
//   {
//     forward(max_speed);
//   }
//   else if ((left == 3) && ((right == 1) || (right == 2) || (right == 0))) 
//   {
//     turn('l',220);
//   }
//   else if ((right == 3) && ((left == 1) || (left == 2) || (left == 0))) 
//   {
//     turn('r',220);
//   }
//   else if ((front == 1) || (front == 2))
//   {
//     forward(reduced_speed);
//   }
//}
////end of function to decide motion
////*******************************************************//  
  
  
  
  
//void decide_motion()
//{
//   if ((left == 0) && (right == 0)) 
//   {
//     backward(255);
//     turn('l',255);
//     delay(800);
//   }
//   else if ((left == 3) && (right == 3)) 
//   {
//     forward(max_speed);
//   }
//  else if ((right == 3) && ((left == 1) || (left == 0) || (left == 2)))
//   {
//     turn('r',255);
//     delay(800);
//   }
//  else if ((left == 3) && ((right == 1) || (right == 0)) || (right == 2))
//   {
//     turn('l',255);
//     delay(800);
//   }
//  else if (((left == 2) && (right == 2))) 
//   {
//     forward(175);
//   }
//   else if (((left == 1) && (right == 1))) 
//   {
//     backward(255);
//     turn('l',255);
//     delay(800);
//   }
//}
//  
 
 
 
//imdl //obstacle avoidance  
void decide_motion()
{
   if ((left == 0) && (right == 0))         //00      //backward
   {
     backward(255);
     turn('l',255);
     turn('l',255);
     delay(800);
   }
   else if ((left == 0) && (right == 1))     //01    //right
   {
     turn('r', 255);
     delay(100);
     turn('r', 255);
     delay(100);
   }
   else if ((left == 1) && (right == 0))     //10    //left
   {
     turn('l',255);
     delay(100);
     turn('l',255);
     delay(100);
   }
   else if ((left == 1) && (right == 1))     //11    //forward
   {
     forward(255);
   }
   
}
  
  
  
//imdl
//function to generate states from sensor readings
int generatestates(float sensorvalue)
{
 if (sensorvalue >= 350)                                    //0 == object detected
   state = 0;
 else if ((sensorvalue < 350))                             //1 == object not detected
   state = 1;
   
 return state;
}
//end of generate states function
//*******************************************************//  

