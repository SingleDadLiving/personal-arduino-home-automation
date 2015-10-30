#include <nRF905.h>
#include <SPI.h>
#include <dht.h>
#include <util/crc16.h>
#include <mb.h>

//## Set Global Variables ##
dht DHT;
int verboseOut = 0;
int errorOut = 1;
uint8_t serno[] = { 0x02, 0x1c, 0xb8, 0x01, 0, 0x55, 0, 0xa2 };

//## Define other Vars ##
#define DHT21_PIN 5

//## Primary Setup Block ##
void setup() {
	if (verboseOut == 1)
		Serial.print(checkcrc());
  init_nRF905(ADDR_GardenManager);
  Serial.begin(BASE_BaudRate);
  init_DHT();
}
//## Primary Program Loop ##
void loop() {
	//Build Packet and Prep for Transmission
	DataPacket TxPacket = Build_DataPacket(ADDR_Server,ADDR_GardenManager,ADDR_GardenManager,ADDR_Server,2,Get_DHT_Values());
  char b[sizeof(TxPacket)];
  memcpy(b, &TxPacket, sizeof(TxPacket));

	SendPacketViaNRF905(b);

  delay(1000);
}
//## Initialize DHT (Temp and humidity sensor) ##
void init_DHT() {
  if (verboseOut == 1) {
    Serial.println("DHT TEST PROGRAM ");
    Serial.print("LIBRARY VERSION: ");
    Serial.println(DHT_LIB_VERSION);
    Serial.println();
    Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
  }
}
//## Function to return formatted DHT readings ##
char *Get_DHT_Values(){
  dht oDHT21 = check_DHT("myDHT21");

  if (verboseOut == 1){
    // DISPLAY DATA
    Serial.print("H");
    Serial.print(oDHT21.humidity, 1);
    Serial.print("|T");
    Serial.println(oDHT21.temperature, 1);
  }
  char data[NRF905_MAX_PAYLOAD - 8] = {0};// Make data
  char sH[5];
  char sT[5];
  dtostrf(oDHT21.humidity, 3, 1, sH);
  dtostrf(oDHT21.temperature, 3, 1, sT);
	if (verboseOut == 1){
		Serial.println(sH);
		Serial.println(sT);
	}
  sprintf(data,"H%s", sH);
  sprintf(data + strlen(data),"|T%s", sT);
	return data;
}
//## Function to check DHT sensor values ##
dht check_DHT(String sensorName) {
  dht temp_DHT;
  if (verboseOut == 1)
    Serial.print(sensorName + ", \t");// READ DATA
  int chk = temp_DHT.read21(DHT21_PIN);
  switch (chk) {
    case DHTLIB_OK:
      if (verboseOut == 1)
        Serial.print("OK,\t");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      if (errorOut == 1)
        Serial.print("Checksum error,\t");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      if (errorOut == 1)
        Serial.print("Time out error,\t");
      break;
    default:
      if (errorOut == 1)
        Serial.print("Unknown error,\t");
      break;
  }
  return temp_DHT;
}

