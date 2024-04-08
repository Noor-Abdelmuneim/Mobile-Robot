#include <SharpIR.h>
#include <SoftwareSerial.h>
#define IR0 A0
#define IR1 A1 
#define IR2 A2 
#define model0 1080 // for GP2Y0A21YK0F  used 1080
#define model1 1080  
#define model2 1080  
SharpIR SharpIR0(IR0, model0);
SharpIR SharpIR1(IR1, model1);
SharpIR SharpIR2(IR2, model2);
SoftwareSerial BT(2, 4);
int motor2Pin1 = 7; // pin 6* on L293D IC
int motor2Pin2 = 8; // pin 5* on L293D IC
int motor2EnablePin = A4; // pin 4** on L293D IC  
int motor1Pin1 = 12; // pin 8* on L293D IC
int motor1Pin2 = 13; // pin 7* on L293D IC
int motor1EnablePin = A5; // pin 1** on L293D IC        
int state;
int Speed = 190; 
int timer = 0; 
int Control = 0;
int chk = 0;
int set = 35;
void setup() {
    pinMode (A0, INPUT);
    pinMode (A1, INPUT);
    pinMode (A2, INPUT);
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor1EnablePin, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(motor2EnablePin, OUTPUT);
    Serial.begin(9600);
    BT.begin(9600); // Setting the baud rate of Software Serial Library  
    delay(500);   
}
void loop() {
    delay(800);   
    //if some date is sent, reads it and saves in state
    if(BT.available() > 0){     
      state = BT.read(); 
      Serial.println(state);
      if(state > 10){Speed = state;}  
      if(state == 6){Control = 0;} 
      if(state == 7){Control = 1;}  }             
    analogWrite(motor1EnablePin, Speed);  // not work because no PWM pins were used
    analogWrite(motor2EnablePin, Speed);
    int distanceLft=SharpIR0.distance();  // this returns the distance for sensor 0
     delay(150); 
    int distanceFnt=SharpIR1.distance();  // this returns the distance for sensor 1
     delay(150); 
    int distanceRht=SharpIR2.distance();  // this returns the distance for sensor 2
     delay(150); 
   if(((distanceFnt<set)||(distanceLft<set)||(distanceRht<set)) && (chk==1)){chk = 2; Stop();}
   if((distanceFnt>=set)&&(distanceLft>=set)&&(distanceRht>=set)){chk = 0;}
    if(Control == 0){         // Manual control via bluetooth
        // if the state is '1' the DC motor will go forward
        if ((state == 1) && (chk==0)){chk = 1; forword();Serial.println("Go Forward!");}
        // if the state is '2' the motor will Reverse
        else if (state == 2){backword();Serial.println("Reverse!");}
        // if the state is '3' the motor will turn left
        else if (state == 3){turnLeft();Serial.println("Turn LEFT");}
        // if the state is '4' the motor will turn right
        else if (state == 4){turnRight();Serial.println("Turn RIGHT");}
        // if the state is '5' the motor will Stop
        else if (state == 5) {Stop();Serial.println("STOP!");}}  
     if(Control == 1){         // Automatic control (Autonomous)
        if(distanceFnt<set){Stop(); delay(300);backword(); delay(600);turnRight();delay(600);forword();}
        if(distanceLft<set){turnRight();delay(600);forword();}
        if(distanceRht<set){turnLeft();delay(600);forword();}
        if((distanceFnt<set)&&(distanceLft<set)&&(distanceRht<set)){Stop(); delay(300); backword(); delay(600);turnLeft();delay(600);forword();}
}
   timer = timer + 1;
   if(timer==200){
     if(distanceFnt>200){distanceFnt=200;} 
     BT.print("A");
     BT.print(";"); 
     BT.print(distanceFnt); //send distance to MIT App
     BT.println(";");
     timer = 0; }
}
void forword(){
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
}

void backword(){
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH); 
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
}
void turnRight(){
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
}
void turnLeft(){
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH); 
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
}
void Stop(){
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
}
