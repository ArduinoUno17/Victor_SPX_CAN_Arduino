# Victor_SPX_CAN_Arduino
CTRE Victor SPX CAN Library for Arduino
Created by: ArduinoUno17

Code created from Cross the Road Electronics Documentation.


Command List
________________________
VictorSPX_CAN_2515(const uint8_t _CS);                    Chip Select Pin of CAN Transceiver SPI Module
void begin();                                             Begin, actual does nothing. I'm not a good programmer.
void setNeutralMode(int DevID, int NeutralModeSet);       DevID -> CAN Device ID  | NeutralModeSet -> NEUTRAL or COAST
void setOutput(int DevID, float Speed);                   DevID -> CAN Device ID  | Speed between -1 and 1
void enableDev();                                         Enable Motor Controller
