// Game controller sketch
// Mike Dice
// November 2019
//
// The purpose of this sketch is to capture the analog device
// state for the input devices that make up the game controller.
// The input devices currently implemented are
// 1. 2 Axis Joystick
// 2. Rotary Encoder
// 3. TODO: button(s)

// defs used by rotary encoder
#define MAX_INT 65000
#define MOVE_COUNT 0
#define VELOCITY_DELAY 100


// global variables used by rotary encoder
volatile unsigned int temp, 
    leftTicks= 0,
    lastLeft = 0,
    rightTicks = 0,
    lastRight = 0,
    isStopped = 1;
    
// global variables used by joystick
volatile unsigned int lastX = 0,
  lastY = 0;
    

void setup() { 
 //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  delay(500);

  // hardware configuration used by rotary encoder  
  pinMode(2, INPUT_PULLUP); // internal pullup input pin 2 
  pinMode(3, INPUT_PULLUP); // internal pullup input pin 3

  //A rising pulse from encoder activates interrupt ai0. Interrupt 0 is attached to digital pin 2 on moust Arduinos.
  attachInterrupt(0, handleInterrupt0, RISING);
   
  //B rising pulse from encoder activates interrup ai1. Interrupt 1 is attached to digital pin 3 on most Arduinos.
  attachInterrupt(1, handleInterrupt1, RISING);
} 

void loop() { 
  
  // read the joystick state
  int x = analogRead(A0);
  int y = analogRead(A1);
  
  // build up a comma separated line to send to Raspberry Pi
  // Comma separated is easy to interpret in Python on the Pi
  if (lastX != x || lastY != y)
  {
    Serial.print("#stic");
    Serial.print(",");
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.print("\n");
  }
  lastX = x;
  lastY = y;
  
  
  // check the number of tickes moved every 100 milliseconds to get 
  // velocity (#tics/100ms). Print direction moved and velocity
  if (leftTicks != lastLeft || rightTicks != lastRight)
  {
    isStopped = 0;
    if (leftTicks > lastLeft + MOVE_COUNT)
    {
      /*
      Serial.print(leftTicks);
      Serial.print(" ");
      Serial.print(rightTicks);
      Serial.print("  ");
      */
   
      Serial.print("#rot");
      Serial.print(",left,");
      Serial.print(leftTicks-lastLeft);
      Serial.print("\n");
      lastLeft = leftTicks;
    }
    else if (rightTicks > lastRight + MOVE_COUNT)
    {
      /*
      Serial.print(leftTicks);
      Serial.print(" ");
      Serial.print(rightTicks);
      Serial.print("  ");
      */
  
      Serial.print("#rot");
      Serial.print(",right,");
      Serial.print(rightTicks-lastRight);
      Serial.print("\n");
      lastRight = rightTicks;
    }
  }
  else if (isStopped==0)
  {
      /*
      Serial.print(leftTicks);
      Serial.print(" ");
      Serial.print(rightTicks);
      Serial.print("  ");
      */

      Serial.print("#rot");
      Serial.print(",stop,0");
      Serial.print("\n");
      isStopped = 1;
  }

  
  delay(VELOCITY_DELAY);
}

void handleInterrupt0() 
{
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(3)==LOW) 
  {
    if (leftTicks + 1 >= MAX_INT)
    {
      leftTicks = 0;
      lastLeft = 0;
    }
    else
    {
      leftTicks++;
    }
  }
}
 
void handleInterrupt1() 
{
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(2)==LOW) 
  {
    if (rightTicks +1 >= MAX_INT)
    {
      rightTicks = 0;
      lastRight = 0;
    }
    else
    {
      rightTicks++;
    }
  }
}

