volatile unsigned int temp, 
    leftTicks= 0,
    lastLeft = 0,
    rightTicks = 0,
    lastRight = 0;
#define MAX_INT 65000
#define MOVE_COUNT 0
    
void setup() {
  Serial.begin (9600);

  pinMode(2, INPUT_PULLUP); // internal pullup input pin 2 
  
  pinMode(3, INPUT_PULLUP); // internalเป็น pullup input pin 3
//Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(0, ai0, RISING);
   
  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(1, ai1, RISING);
  }
   
  void loop() {
    // check the number of tickes moved every 100 milliseconds to get 
    // velocity (#tics/100ms). Print direction moved and velocity
    if (leftTicks > lastLeft + MOVE_COUNT){
      /*
      Serial.print(leftTicks);
      Serial.print(" ");
      Serial.print(rightTicks);
      Serial.print("  ");
      */

      Serial.print("Moving left at velocity ");
      Serial.println(leftTicks-lastLeft);
      lastLeft = leftTicks;
    }
    else if (rightTicks > lastRight + MOVE_COUNT){
      /*
      Serial.print(leftTicks);
      Serial.print(" ");
      Serial.print(rightTicks);
      Serial.print("  ");
      */

      Serial.print("Moving right at velocity ");
      Serial.println(rightTicks-lastRight);
      lastRight = rightTicks;
    }
    else{
      /*
      Serial.print(leftTicks);
      Serial.print(" ");
      Serial.print(rightTicks);
      Serial.print("  ");
      */
      Serial.println("Stopped");
    }
    
    delay(100);
  }
   
  void ai0() {
    // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
    // Check pin 3 to determine the direction
    if(digitalRead(3)==LOW) {
      if (leftTicks + 1 >= MAX_INT){
        leftTicks = 0;
        lastLeft = 0;
      }
      else{
        leftTicks++;
      }
    }
  }
   
  void ai1() {
    // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
    // Check with pin 2 to determine the direction
    if(digitalRead(2)==LOW) {
      if (rightTicks +1 >= MAX_INT)
      {
        rightTicks = 0;
        lastRight = 0;
      }
      else{
        rightTicks++;
      }
    }
  }
