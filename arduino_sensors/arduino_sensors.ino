#include <Adafruit_BMP085_U.h>
#include <parser.h>
#define RED_LED 0
#define GREEN_LED 1
#define BLUE_LED 2
#define RED_PIN 2
#define GREEN_PIN 3
#define BLUE_PIN 4
#define SENSOR_DELAY 100000
long sensorDelay;
bool tempEnabled;
bool sendLedStates;
String ledCMD("LEDCMD");
String getLedStateCMD("GETLEDSTATE");
String EnableTemp("ENABLETEMP");
String Test("TEST");

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

// Process a packet from the Protocol
void ProcessPacket(Packet* packet)
{
  String command((char*)packet->Command);

  if (command == ledCMD) {
    ChangeLEDState(packet);
  }
  else if (command == Test) {
    SendTestResponse();
  }
  else if (command == getLedStateCMD){
    sendLedStates = true;
  }

  DeletePacket(packet); // done with the packet
  ResetParser(); // reset and get ready for next packet
}

void SendLedState(int led)
  {
    int state = -1;
    if (led == RED_LED)
    {
      state = digitalRead(RED_PIN);
    }
    else if (led == BLUE_LED)
    {
      state = digitalRead(BLUE_PIN);
    }
    else if (led == GREEN_LED)
    {
      state = digitalRead(GREEN_PIN);
    }

    if (state >= 0)
    {
      byte bytes[2];
      bytes[0] = led;
      bytes[1] = state;
      SerializeMessage("GETLEDSTATERESP", bytes, 2);
    }
  }

  void ChangeLEDState(Packet * packet) {
    if (packet->Data && packet->DataLength == 2)
    {
      if (packet->Data[0] == RED_LED) SetPinState(RED_PIN, packet->Data[1]);
      else if (packet->Data[0] == BLUE_LED) SetPinState(BLUE_PIN, packet->Data[1]);
      else if (packet->Data[0] == GREEN_LED) SetPinState(GREEN_PIN, packet->Data[1]);
    }
  }

  void SetPinState(int pin, int state)
  {
    if (state == 0)
    {
      digitalWrite(pin, LOW);
    }
    else if (state == 1)
    {
      digitalWrite(pin, HIGH);
    }
  }

  void SendTestResponse() {
    SerializeMessage("TESTRESP", "test response");
  }

  // Serialize a message according to Protocol format and send over the serial port
  void SerializeMessage(char* command, char* message)
  {
    Serial.println("[--");
    Serial.print("PKT.V1 "); Serial.println(command);
    if (message != NULL)
    {
      int messageLen = strlen(message);
      Serial.print("LEN: "); Serial.println(messageLen);
      Serial.print(message); // no line break after message
    }
    else
    {
      Serial.println("LEN: 0");
    }
  }

  void SerializeMessage(char* command, byte * data, int dataLen) {
    Serial.println("[--");
    Serial.print("PKT.V1 "); Serial.println(command);
    if (data != NULL && dataLen >= 0)
    {
      Serial.print("LEN: "); Serial.println(dataLen);
      for (int i = 0; i < dataLen; i++)
      {
        int val = (int)data[i];
        Serial.write(val);
      }
    }
    else
    {
      Serial.println("LEN: 0");
    }
  }

  void ReadTemperature() {
    char buff[64];
    float temp = 0.0;
    float pressure = 0.0;

    if (sensorDelay <= 0 && tempEnabled)
    {
      bmp.getTemperature(&temp);
      bmp.getPressure(&pressure);

      String tempStr(temp, 3);
      SerializeMessage("TEMP", tempStr.c_str());

      String pressStr(pressure, 3);
      SerializeMessage("PRESS", pressStr.c_str());
      sensorDelay = SENSOR_DELAY; // reset the delay

      if (sendLedStates)
      {
        SendLedState(RED_LED);
        SendLedState(GREEN_LED);
        SendLedState(BLUE_LED);
        sendLedStates = false;
      }

    }
    else {
      sensorDelay--; // keep waiting
    }
  }


  void setup() {
    Serial.begin(57600);
    ResetParser();
    tempEnabled = true;

    /* Initialize the sensor */
    if (!bmp.begin())
    {
      /* There was a problem detecting the BMP085 ... check your connections */
      Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
      while (1);
    }
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    sensorDelay = SENSOR_DELAY;
  }

  void loop() {
    // This checks the serial port to see if there is any
    // incoming message data.
    while (Serial.available()) {
      byte byteRead = Serial.read();
      ProcessStreamByte(byteRead, &ProcessPacket);
    }

    // Read the temperature and send an update to the serial partner
    ReadTemperature();

  }
