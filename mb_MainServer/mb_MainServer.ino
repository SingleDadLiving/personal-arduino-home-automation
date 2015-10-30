#include <nRF905.h>
#include <SPI.h>
#include <mb.h>

//## Set Global Variables ##
int verboseOut = 0;
int errorOut = 1;

//## Define other Vars ##

//## Primary Setup Block ##
void setup() {
  init_nRF905(ADDR_Server);
  Serial.begin(BASE_BaudRate);

}
//## Primary Program Loop ##
void loop() {
  if (verboseOut == 1)
    Serial.println(F("Waiting for ping..."));
  byte buffer[NRF905_MAX_PAYLOAD];// Make buffer for data
  while (!nRF905_getData(buffer, sizeof(buffer))); // Wait for data
  if (verboseOut == 1)
    Serial.println(F("Got ping"));
  byte addr[] = ADDR_GardenManager; nRF905_setTXAddress(addr); // Set address of device to send to
  nRF905_setData(buffer, sizeof(buffer));// Set payload data (reply with data received)
  if (verboseOut == 1)
    Serial.println(F("Sending reply..."));
  while (!nRF905_send()); // Send payload (send fails if other transmissions are going on, keep trying until success)
  nRF905_receive();// Put back into receive mode
  if (verboseOut == 1)
    Serial.println(F("Reply sent"));


  if (verboseOut == 1)
    Serial.print(F("Data: "));// Printout ping contents
  DataPacket* tmp = (DataPacket*)buffer;
  Serial.print("PacketID: " + (String)tmp->PacketID + "\t");
  Serial.print("To: " + (String)tmp->To + "\t");
  Serial.print("From: " + (String)tmp->From + "\t");
  Serial.print("Source: " + (String)tmp->SourceAddress + "\t");
  Serial.print("Dest: " + (String)tmp->DestinationAddress + "\t");
  Serial.print("Type: " + (String)tmp->Type + "\t");
  Serial.print("Payload: " + (String)tmp->Payload + "\t");
  Serial.print("CRC: " + (String)tmp->CRC + "\t");
  //Serial.write(buffer, sizeof(buffer));
  Serial.println();
}
