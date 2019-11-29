#include <parser.h>

int commandCount = 0;

// Process a packet from the Protocol
void ProcessPacket(Packet* packet){
   char buffer[128];
   sprintf(buffer, "Arduino processed command %s, command count %d", packet->Command, commandCount++);
   SerializeMessage("ACK", buffer);

   DeletePacket(packet); // done with the packet
   ResetParser(); // reset and get ready for next packet
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

void setup() {
    Serial.begin(57600);
    ResetParser();
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available()){
    byte byteRead = Serial.read();
    ProcessStreamByte(byteRead, &ProcessPacket);
  }
}
