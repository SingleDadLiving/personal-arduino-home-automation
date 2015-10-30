#include <nRF905.h>
#include <SPI.h>
#include <util/crc16.h>
#include <mb.h>

//## Set Global Variables ##
int verboseOut = 0;
int errorOut = 1;
uint8_t serno[] = { 0x02, 0x1c, 0xb8, 0x01, 0, 0x55, 0, 0xa2 };

//## Define other Vars ##

//## Primary Setup Block ##
void setup(){
	if (verboseOut == 1)
		Serial.print(checkcrc());
  init_nRF905(ADDR_BasementMonitor);
  Serial.begin(BASE_BaudRate);
}
//## Primary Program Loop ##
void loop(){
	//Build Packet and Prep for Transmission
	DataPacket TxPacket = Build_DataPacket(ADDR_Server,ADDR_BasementMonitor,ADDR_BasementMonitor,ADDR_Server,2,Get_WATERSENSOR_Values());
  char b[sizeof(TxPacket)];
  memcpy(b, &TxPacket, sizeof(TxPacket));
	
	SendPacketViaNRF905(b);

  delay(1000);
}
//## Function to return formatted Water readings ##
char *Get_WATERSENSOR_Values(){

  int waterSensorValue = analogRead(A5);

  char data[NRF905_MAX_PAYLOAD] = {0};// Make data
  sprintf(data, "Water Sensor Val: %hhu", waterSensorValue);

	return data;
}
