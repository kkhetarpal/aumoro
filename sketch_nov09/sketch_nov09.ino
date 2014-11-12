/****************************************************************************************************************************************************************************/
//sensor input variables and pins
int sonarsensor1 = A0, sonarsensor2 = A1, sonarsensor3 = A2;
int irsensorleft = A3, irsensorright = A4;

//states declaration for each sensor
int sonar_left, sonar_front, sonar_right, ir_left, ir_right;
//int max_speed = 250, average_speed = 200, low_speed = 150;
int max_speed = 0, average_speed = 65, low_speed = 75;
int state = 0;
int speed = 0;
char c;

int sonarsensor1_val = 0, sonarsensor2_val = 0, sonarsensor3_val = 0;
int Lsonar[4] = {0}, Fsonar[4] = {0}, Rsonar[4] = {0}; //array for moving average of sensor readings
int irsensorleft_val = 0, irsensorright_val = 0;

const int M1_dir1 = 50;
const int M1_dir2 = 52;
const int pwm1= 2;
const int M2_dir1 = 40;
const int M2_dir2 = 42;
const int pwm2= 4;

int reading_number = 0;
/****************************************************************************************************************************************************************************/



/***************************************************************************/
/**Setup**/
/***************************************************************************/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  
  // make the pins outputs:
  pinMode(sonarsensor1, INPUT);    //sonar left        //A0
  pinMode(sonarsensor2, INPUT);    //sonar front       //A1
  pinMode(sonarsensor3, INPUT);    //sonar right       //A2
  
  pinMode(irsensorleft, INPUT);    //ir left           //A3
  pinMode(irsensorright, INPUT);   //ir right          //A4
    
  
  pinMode(M1_dir1, OUTPUT); 
  pinMode(M1_dir2, OUTPUT); 
  pinMode(pwm1, OUTPUT); 
  pinMode(M2_dir1, OUTPUT); 
  pinMode(M2_dir2, OUTPUT); 
  pinMode(pwm2, OUTPUT); 
  pinMode(48, OUTPUT);
}
//end of setup//
/***************************************************************************/

void loop() 
{
  // put your main code here, to run repeatedly:
  
  
  //data acquisition module 
  readSensor(reading_number);   // 0,1,2,3
  
  reading_number += 1;        
  
  //generate states for sonar sensor
  sonar_left  = generatestates(sonarsensor1_val);        
  sonar_front = generatestates(sonarsensor2_val);        
  sonar_right = generatestates(sonarsensor3_val);           
    

  //generate states for ir sensor
//  ir_left = genstate(irsensorleft_val);
//  ir_right = genstate(irsensorright_val);
  
  
  //Print Sensor Values
//  Serial.print("sonar_left:"); 
//  Serial.print(sonar_left); 
//  Serial.print(" ");
//  Serial.print("sonar_front:"); 
//  Serial.print(sonar_front);
//  Serial.print(" ");
//  Serial.print("sonar_right:"); 
//  Serial.print(sonar_right);
//  Serial.print(" ");
//  Serial.print("ir_left:"); 
//  Serial.print(ir_left); 
//  Serial.print(" ");
//  Serial.print("ir_right:"); 
//  Serial.println(ir_right);
  
  //decision making module
  decide_motion(); 
  
  
  
   
  while (Serial.available() > 0)     //while user gives input
      {
      
      c =  Serial.read();               //read user input
      
      if (c == 'f')                   
        {
          
        //forward(50);  // high speed                              //actual speed = 255-50 = 205
        forward(75);  // high speed                              //actual speed = 255-75 = 180
        //forward(100);  // one motor not moving                   //actual speed = 255-100 = 155
        //forward(150);  // high speed                             //actual speed = 255-150 = 105
        //forward(200);  // high speed 
        
        }
      else if(c == 'b')              
        {
          
        backward(50);
              
        }
        
      else if(c == 's')
        {
          
        runMotor1(0);
        runMotor2(0);

        }
      else if(c == 'l')              
        {
          
        turn('l',50);  
              
        }
      else if(c == 'r')
        {

          turn('r',50);
          
        }         
 
        
        
      } //end of while
//Basic test to run a single motor connected to M1dir1 and M1dir2
//          digitalWrite(M1_dir1, HIGH);
//          digitalWrite(M1_dir2, LOW);
//          digitalWrite(48, HIGH);
//          analogWrite(pwm1, 150); 
} //end of loop



//function to run motor 1
void runMotor1(int speed_M1)
{
  if (speed_M1 > 0)        //move forward
    {
      digitalWrite(M1_dir1, HIGH);
      digitalWrite(M1_dir2, LOW);
      digitalWrite(48, HIGH);
      analogWrite(pwm1, 255-speed_M1); 
    }  
  else if(speed_M1 < 0)    //move backward
    {
      digitalWrite(M1_dir1, LOW);
      digitalWrite(M1_dir2, HIGH);
      digitalWrite(48, HIGH);
      analogWrite(pwm1, 255+speed_M1);
    }
  else if(speed_M1 == 0)
    {
      digitalWrite(M1_dir1, LOW);
      digitalWrite(M1_dir2, LOW);
      digitalWrite(48, HIGH);
      analogWrite(pwm1, 0);
    }
}
//end of function to run motor 1
//*******************************************************//



//function to run motor 2
void runMotor2(int speed_M2)
{
  if (speed_M2 > 0)        //move forward
    {
      digitalWrite(M2_dir1, HIGH);
      digitalWrite(M2_dir2, LOW);
      digitalWrite(44, HIGH);
      analogWrite(pwm2, 255 - speed_M2); 
    }  
  else if(speed_M2 < 0)    //move backward
    { 
      digitalWrite(M2_dir1, LOW);
      digitalWrite(M2_dir2, HIGH);
      digitalWrite(44, HIGH);
      analogWrite(pwm2, 255 + speed_M2);
    }
  else if(speed_M2 == 0)
    {
      digitalWrite(M2_dir1, LOW);
      digitalWrite(M2_dir2, LOW);
      digitalWrite(44, HIGH);
      analogWrite(pwm2, speed_M2);
    }
}

//end of function to run motor 2
//*******************************************************//



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



/***************************************************************************/
/**Data acquisition module - sense and preprocess**/
/***************************************************************************/
void readSensor(int index)
{
  
  Lsonar[index % 4] = analogRead(sonarsensor1);
  Fsonar[index % 4] = analogRead(sonarsensor2);
  Rsonar[index % 4] = analogRead(sonarsensor3);
  
  
//  sonarsensor1_val = analogRead(sonarsensor1);    //Sensor Inputs    
//  sonarsensor2_val = analogRead(sonarsensor2);
//  sonarsensor3_val = analogRead(sonarsensor3);

   sonarsensor1_val = 0;
   sonarsensor2_val = 0;
   sonarsensor3_val = 0;

  for (int k=0 ; k<4; k++){
    
    sonarsensor1_val += Lsonar[k];
    sonarsensor2_val += Fsonar[k];
    sonarsensor3_val += Rsonar[k];
  }
 
   sonarsensor1_val /= 4;
   sonarsensor2_val /= 4;
   sonarsensor3_val /= 4;
 

  
  //irsensorleft_val = analogRead(irsensorleft);
  //irsensorright_val = analogRead(irsensorright);
   
  //complimentary filter
  //irsensorleft_val  = (0.8 * irsensorleft_val) + (0.2 * irsensorleft_val);
  //irsensorright_val = (0.8 * irsensorright_val) + (0.2 * irsensorright_val);
  
  //printing sensor values
  Serial.print("Sonar Left:"); 
  Serial.print(sonarsensor1_val);              //15 - 45 
  Serial.print(" "); 
  Serial.print("Sonar Front:"); 
  Serial.print(sonarsensor2_val);              //15 - 58
  Serial.print(" "); 
  Serial.print("Sonar Right:"); 
  Serial.print(sonarsensor3_val);              //16 - 61
  Serial.print(" "); 
  Serial.print("IR L:"); 
  Serial.print(irsensorleft_val);              //linear characteristic plot
  Serial.print(" "); 
  Serial.print("IR R:");
  Serial.println(irsensorright_val);             //linear characteristic plot
  
}
//end of sense and preprocess module
/***************************************************************************/





/***************************************************************************/
/**Generate States for Sonar Sensor Readings**/
/***************************************************************************/
int generatestates(float sensorvalue)
{
 if (sensorvalue <= 60)                                                            
   state = 1;                           //Obstacle Detected                         
 else if (sensorvalue > 60)                                      
   state = 0;                           //Not Detected
   
 return state;
}
//end of generate states function
/***************************************************************************/




/***************************************************************************/
/**Gen States for IR Sensor Readings**/
/***************************************************************************/
//int genstate(float senseval)
//{
// if (senseval <= 400 )                                    //State VERY CLOSE denoted by value 0
//   state = 3;
// else  if ((senseval > 400) && (senseval <= 700))         //State CLOSE denoted by value 1
//   state = 2;
// else if ((senseval > 700) && (senseval <= 800))          //State SAFE denoted by value 2
//   state = 1;
//  else if ((senseval > 800) && (senseval <= 900))         //State FAR denoted by value 3
//   state = 0;
//   
// return state;
//}
//end of gen states function
/***************************************************************************/


/***************************************************************************/
/**Decision Making Module**/
/***************************************************************************/
void decide_motion()
{
  
  if (sonar_front == 0) //nothing in the front //go forward always
    {    
    forward(average_speed);                              //change it to max_speed later
    Serial.println("Moving Forward"); 
    delay(800);
    }
   else if (sonar_front == 1)                            //If obstacle detected in front 
    {
      if(sonar_left == 0)                                 //Checking left //if no obstacle detected in left :: turn left
         {
           turn('l', average_speed);
           Serial.println("Turning Left"); 
           delay(800);
         }
      else if(sonar_right == 0)                           //Else checking //if no obstacle detected in right :: turn right
         {
           turn('r', average_speed);
           Serial.println("Turning Right"); 
           delay(800);
         }
      else
         {
           backward(average_speed);
           Serial.println("Moving Backward"); 
           delay(800);
        }
    }
}
//end of decision making module
/***************************************************************************/




