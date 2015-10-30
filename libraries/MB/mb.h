#ifndef mb_h
#define mb_h

#include <Arduino.h> 

//## Define other Vars ##
#define ADDR_Server {0x58, 0x6F, 0x2E, 0x10} // 58.111.46.10 Address of this device (4 bytes)
#define ADDR_GardenManager {0x01, 0x01, 0x01, 0x01} // Address of GardenManager (4 bytes)
#define ADDR_BasementMonitor {0x02, 0x02, 0x02, 0x02} // Address of BasementMonitor (4 bytes)
#define ADDR_CatManager {0x03, 0x03, 0x03, 0x03} // Address of CatManager (4 bytes)
#define ADDR_BarManager {0x04, 0x04, 0x04, 0x04} // Address of BarManager (4 bytes)
#define ADDR_UNSUED {0xFE, 0x4C, 0xA6, 0xE5} // Address of UNUSED (4 bytes)

#define BASE_BaudRate 9600
#define BASE_TIMEOUT 1000 // 1 second ping timeout


//## Structs ##
struct DataPacket {
  uint8_t PacketID;
  uint8_t To;
  uint8_t From;
  uint8_t SourceAddress;
  uint8_t DestinationAddress;
  uint8_t Type;
  char Payload[24];
  uint16_t CRC;
};

#endif
