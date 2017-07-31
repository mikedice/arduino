// Mike Dice (mikedice417@hotmail.com)
// July, 2017
// Simple driver for 7 segment digit display

// A map that matches my disorganized wiring
// A one in the map represents pin turned on
// A zero in the map represents pin turned off
static int numbers[10][8] = {
    {0,1,1,0,1,1,1,1}, // 0
    {0,0,0,0,1,1,0,0}, // 1
    {1,0,1,0,1,0,1,1}, // 2
    {1,0,1,0,1,1,1,0}, // 3
    {1,1,0,0,1,1,0,0}, // 4
    {1,1,1,0,0,1,7,0}, // 5
    {1,1,0,0,0,1,1,1}, // 6
    {0,0,1,0,1,1,0,0}, // 7
    {1,1,1,0,1,1,1,1}, // 8
    {1,1,1,0,1,1,0,0}, // 9
};

void setup() {
  // initialize 8 pins. Pins 8-1 inclusive
  for (int i = 8; i>0; i--) pinMode(i, OUTPUT);
}

void writeNumber(int number_to_write)
{
   for (int pin = 0; pin<8; pin++)
   {
      int pinValue = numbers[number_to_write][pin];
      digitalWrite(pin+1, pinValue);
   }
}

void loop() 
{
  for (int i = 0; i<10; i++){
    writeNumber(i);
    delay(500);
  }
}
