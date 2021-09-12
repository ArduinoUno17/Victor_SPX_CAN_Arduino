// Library Created by: Joshua Newport
// Used Cross the Road Electronics Documentation to make this Library

// ------------ VICTOR SPX CAN BUS LIBRARY ------------ //

#include <SPI.h>			// SPI
#include <mcp2515.h>	// MCP2515 CAN BUS LIBRARY

#ifndef VictorSPX_CAN_2515_h
#define VictorSPX_CAN_2515_h


enum NeutralMode
{
  EEPROMSetting     = 0x00,
  COAST      		= 0x11,
  BRAKE      		= 0x12
};


class VictorSPX_CAN_2515
{
public:
	VictorSPX_CAN_2515(const uint8_t _CS);
	void begin();
	void setNeutralMode(int DevID, int NeutralModeSet);
	void setOutput(int DevID, float Speed);
	void enableDev();
	//void disable();
	

private:
	MCP2515 *CAN;
	int startCAN(void);
	//unsigned char enableFrame[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	struct can_frame MotorFrame;
	struct can_frame enableFrame;
	//unsigned char disableFrame[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	// bool disabled = true;
	// long _arbID;
	// long _VictorSPXarbID;
	// long _enableArbID;
	// long _PrcntOutputFrame;
	// int _MCP2515_CS_PIN;
	long _arbID = 0x00000000;
	long _VictorSPXarbID = 0x01040000;
	long _enableArbID = 0x000401BF;
	long _PrcntOutputFrame = 0x040080;
	int _MCP2515_CS_PIN = 10;
	unsigned char Braking[63];			// Keep Track of Which Motors are Using Braking
	float MtrSpeeds[63];		// Keep Track of Each Motor Speed
	//unsigned char DeviceIDs[64]; 
};

#endif