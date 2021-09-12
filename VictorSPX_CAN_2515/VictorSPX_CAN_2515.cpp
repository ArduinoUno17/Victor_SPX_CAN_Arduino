// Library Created by: Joshua Newport
// Used Cross the Road Electronics Documentation to make this Library

// ------------ VICTOR SPX CAN BUS LIBRARY ------------ //
#include "VictorSPX_CAN_2515.h"

// Include Libraries
#include <SPI.h>			// SPI
#include <mcp2515.h>	// MCP2515 CAN BUS LIBRARY
#include "Arduino.h"



VictorSPX_CAN_2515::VictorSPX_CAN_2515(const uint8_t _CS) {
	CAN = new MCP2515(_CS);
}

int VictorSPX_CAN_2515::startCAN(void) {
}
	

void VictorSPX_CAN_2515::begin() {
	CAN->reset();
	CAN->setBitrate(CAN_1000KBPS);
	CAN->setNormalMode();
}

// SETS THE NEUTRAL MODE OF THE MOTOR CONTROLLER
// VALID OPTIONS ARE [EEPROMSetting, COAST, BRAKE]
void VictorSPX_CAN_2515::setNeutralMode(int DevID, int NeutralModeSet) {
	Braking[DevID] = NeutralModeSet;		// Save the Neutral Mode.
	setOutput(DevID, MtrSpeeds[DevID]);		// Update the Motor Output -> This will set the Neutral Mode.
}

// SETS THE SPEED OF THE MOTOR CONTROLLER.
// VALID SPEED RANGE -1 : 1
void VictorSPX_CAN_2515::setOutput(int DevID, float Speed) {
	//unsigned char frame[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};	// Empty Frame
	float NewSpeed = Speed * 1023;
	long MtrSpeed = long(NewSpeed);
	MtrSpeeds[DevID] = Speed;		// SAVE THE MOTOR SPEED.
	
	// Convert Speed to Three Bytes
	byte firstByte = byte(MtrSpeed >> 0x10);
	byte secondByte = byte(MtrSpeed >> 0x08);
	byte thirdByte = byte(MtrSpeed);
	
	MotorFrame.data[0] = firstByte;		// SET THE FIRST BIT OF THE FRAME
	MotorFrame.data[1] = secondByte;		// SET THE SECOND BIT OF THE FRAME
	MotorFrame.data[2] = thirdByte;		// SET THE THIRD BIT OF THE FRAME
	MotorFrame.data[3] = 0x00;
	MotorFrame.data[4] = 0x00;
	MotorFrame.data[5] = 0x00;
	MotorFrame.data[6] = Braking[DevID];	// SET THE SEVENTH BIT OF THE FRAME. THIS IS THE BRAKING MODE.
	MotorFrame.data[7] = 0x20;			// SET THE EIGHTH BIT OF THE FRAME. THIS IS USED WITH BRAKING MODE... I GUESS???
	MotorFrame.can_id  = (long(DevID) | _PrcntOutputFrame | _VictorSPXarbID) | CAN_EFF_FLAG;
	MotorFrame.can_dlc = 8;
	CAN->sendMessage(&MotorFrame);			// SEND CAN DATA TO THE MOTOR CONTROLLER
}

void VictorSPX_CAN_2515::enableDev() {
	enableFrame.can_id  = 0x000401BF | CAN_EFF_FLAG;
	enableFrame.can_dlc = 8;
	enableFrame.data[0] = 0x01;
	enableFrame.data[1] = 0x00;
	enableFrame.data[2] = 0x00;
	enableFrame.data[3] = 0x00;
	enableFrame.data[4] = 0x00;
	enableFrame.data[5] = 0x00;
	enableFrame.data[6] = 0x00;
	enableFrame.data[7] = 0x00;
	CAN->sendMessage(&enableFrame);
}
