// Library Created by: Joshua Newport
// Used Cross the Road Electronics Documentation to make this Library

// ------------ VICTOR SPX CAN BUS LIBRARY ------------ //
#include "VictorSPX_CAN_2515.h"

// Include Libraries
#include <SPI.h>			// SPI
#include <mcp2515.h>	// MCP2515 CAN BUS LIBRARY
#include "Arduino.h"
//mcp2515_can CAN(2); // Set CS pin



VictorSPX_CAN_2515::VictorSPX_CAN_2515(const uint8_t _CS) {
	//Serial.begin(115200);
	/* _arbID = 0x00000000;
	_VictorSPXarbID = 0x01040000;
	_enableArbID = 0x000401BF;
	_PrcntOutputFrame = 0x040080;
	_MCP2515_CS_PIN = 10; */
	//mcp2515_can CAN(_CS); // Set CS pin
	CAN = new MCP2515(_CS);
}

int VictorSPX_CAN_2515::startCAN(void) {
	/*if (CAN_OK != CAN->begin(CAN_1000KBPS)) {
		Serial.println("CAN init fail, retry...");
		return 1;
	}
	else {
		Serial.println("CAN init ok!");
		return 0;
	}*/
}
	

void VictorSPX_CAN_2515::begin() {
	//mcp2515_can CAN(MCP2515_CS_PIN); // Set CS pin
	//SPI_CS_PIN = MCP2515_CS_PIN;
	int i = 0;
	/*while (i < 5) {
		if (startCAN() != 0) {
			Serial.println("CAN init fail, retry...");
			delay(100);
			i++;
		}
		else {
			Serial.println("CAN init ok!... Yay!");
			break;
		}
	}	*/
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
	//MotorFrame.data = frame;
	CAN->sendMessage(&MotorFrame);			// SEND CAN DATA TO THE MOTOR CONTROLLER
}

void VictorSPX_CAN_2515::enableDev() {
	//unsigned char frame[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	//CAN->sendMsgBuf(_enableArbID, 1, 8, enableFrame);
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

/* void VictorSPX_CAN_2515::disable() {
	CAN.sendMsgBuf(_enableArbID, 1, 8, 0x0000000000000000);
} */