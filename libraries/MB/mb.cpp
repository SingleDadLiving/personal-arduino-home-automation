#include "mb.h" 


void on(){
	digitalWrite(LED_PIN,HIGH); //set the pin HIGH and thus turn LED on
}

//## Initialize nRF905 radio ##
void init_nRF905(byte SourceAddress[]) {
  nRF905_init();// Start up
  byte addr[] = SourceAddress; nRF905_setRXAddress(addr); // Set address of this device
  nRF905_setTransmitPower(NRF905_PWR_n10);// Lowest transmit level -10db
  nRF905_setLowRxPower(NRF905_LOW_RX_ENABLE);// Reduce receive sensitivity to save a few mA
  nRF905_receive();// Put into receive mode
  if (verboseOut == 1)
    Serial.println(F("Client started"));
}
//## Function to Send Data via nRF905 ##
void SendPacketViaNRF905(char *dataToTransmit){
  nRF905_powerUp();// Turn on module
  unsigned long startTime = millis();
  byte addr[] = ADDR_Server;nRF905_setTXAddress(addr);// Set address of device to send to
  nRF905_setData(dataToTransmit, sizeof(dataToTransmit));// Set payload data
  while (!nRF905_send());// Send payload (send fails if other transmissions are going on, keep trying until success)
  nRF905_receive();// Put into receive mode
  byte buffer[NRF905_MAX_PAYLOAD];// Make buffer for reply
  bool success;

  unsigned long sendStartTime = millis();// Wait for reply with timeout
  while (1){
    success = nRF905_getData(buffer, sizeof(buffer));
    if (success) // Got data
      break;

    // Timeout
    if (millis() - sendStartTime > BASE_TIMEOUT)
      break;
  }
    nRF905_powerDown();// Turn off module

  if (success){
    unsigned int totalTime = millis() - startTime;
    if (verboseOut == 1){
      Serial.print(F("Ping time: "));
      Serial.print(totalTime);
      Serial.println(F("ms"));
    }

    if (verboseOut == 1){
      Serial.print(F("Data from server: "));// Printout ping contents
			Serial.write(buffer, sizeof(buffer));
      Serial.println();
		}
  }else{
    if (verboseOut == 1)
      Serial.println(F("Ping timed out"));
  }
}
//## Build DataPacket ##
DataPacket Build_DataPacket(uint8_t To, uint8_t From, uint8_t SourceAddress, uint8_t DestinationAddress, uint8_t Type,byte PacketPayload[]){
  DataPacket tPack;
  tPack.PacketID = random(250);
  tPack.To = To;
  tPack.From = From;
  tPack.SourceAddress = SourceAddress;
  tPack.DestinationAddress = DestinationAddress;
  tPack.Type = Type;
  strcpy(tPack.Payload, PacketPayload);                        
  tPack.CRC = MakeCRC(&tPack);
	return tPack;
}
//## Function to calculate CRC ##
uint16_t MakeCRC(struct DataPacket *InPacket){
 uint16_t TempCRC = 0;
 TempCRC = _crc16_update(TempCRC, InPacket->PacketID);
 TempCRC = _crc16_update(TempCRC, InPacket->To);
 TempCRC = _crc16_update(TempCRC, InPacket->From);
 TempCRC = _crc16_update(TempCRC, InPacket->SourceAddress);
 TempCRC = _crc16_update(TempCRC, InPacket->DestinationAddress);
 TempCRC = _crc16_update(TempCRC, InPacket->Type);
 //TempCRC = _crc16_update(TempCRC, InPacket->Payload);
 return TempCRC;
}
//## Function to test CRC ##
uint16_t checkcrc(void){
 uint16_t crc = 0;
 uint8_t i;
 for (i = 0; i < sizeof serno / sizeof serno[0]; i++) {
   crc = _crc16_update(crc, serno[i]);
 }
 //Serial.println(crc);
 return crc; 
}
