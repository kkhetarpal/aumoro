const int M1_dir1 = 50;
const int M1_dir2 = 52;
const int pwm1= 2;
const int M2_dir1 = 40;
const int M2_dir2 = 42;
const int pwm2= 4;

char c;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // make the pins outputs:
  pinMode(M1_dir1, OUTPUT); 
  pinMode(M1_dir2, OUTPUT); 
  pinMode(pwm1, OUTPUT); 
  pinMode(M2_dir1, OUTPUT); 
  pinMode(M2_dir2, OUTPUT); 
  pinMode(pwm2, OUTPUT); 
  pinMode(48, OUTPUT);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  
  while (Serial.available() > 0)     //while user gives input
      {

      c =  Serial.read();               //read user input
      
      if (c == 'f')                   s
        {
          
        digitalWrite(M1_dir1, HIGH);
        digitalWrite(M1_dir2, LOW);
        digitalWrite(48, HIGH);
        analogWrite(pwm1, 70); 
        
        }
      else if(c == 'b')              
        {
          
        digitalWrite(M1_dir1, LOW);
        digitalWrite(M1_dir2, HIGH);
        digitalWrite(48, HIGH);
        analogWrite(pwm1, 100);
              
        }
        
       else if(c == 's')
        {
        digitalWrite(M1_dir1, LOW);
        digitalWrite(M1_dir2, LOW);
        digitalWrite(48, HIGH);
        analogWrite(pwm1, 0);
        }
        
        
        else if (c == 'q')                   
        {
          
        digitalWrite(M2_dir1, HIGH);
        digitalWrite(M2_dir2, LOW);
        digitalWrite(44, HIGH);
        analogWrite(pwm2, 70); 
        
        }
      else if(c == 'w')              
        {
          
        digitalWrite(M2_dir1, LOW);
        digitalWrite(M2_dir2, HIGH);
        digitalWrite(44, HIGH);
        analogWrite(pwm2, 100);
              
        }
        
       else if(c == 'e')
        {
        digitalWrite(M2_dir1, LOW);
        digitalWrite(M2_dir2, LOW);
        digitalWrite(44, HIGH);
        analogWrite(pwm2, 0);
        }
        
        
        
        
        
        
        
        
      }

//          digitalWrite(M1_dir1, HIGH);
//          digitalWrite(M1_dir2, LOW);
//          digitalWrite(48, HIGH);
//          analogWrite(pwm1, 150); 


}
