/*
 * RemoteControls.cpp
 *
 * Created: 30/06/2012 4:53:29
 *  Author: Riwels
 */ 
//-------------------------------------------------------------------------
#include <Arduino.h>
#include "RemoteControls.h"

//------------------------------------------------------------------------
RemoteControl::RemoteControl(uint8_t IRSensorPin,bool EnableDebug){
	DebugMode = EnableDebug;
	Pin = IRSensorPin;
	pinMode(Pin,INPUT);
}
//------------------------------------------------------------------------
unsigned long RemoteControl::PulseIn(unsigned long WaitingTime){
	return pulseIn(Pin,HIGH,WaitingTime);
}
//------------------------------------------------------------------------
uint8_t RemoteControl::PressedButton(){
	buffer = 0;
	if(PulseIn(MaxWaitingTime)>FirstPulseLength){
		for(a=0;a<Pulses;a++) Data[a] = PulseIn(FirstPulseLength);
		for(a=0;a<Pulses;a++) {
			if((Data[a] > HighPulseLength*2)|(Data[a]<LowPulseLength))return 0;
			buffer <<= 1;
			if(Data[a] > HighPulseLength) buffer |= 1;
		}
		if (DebugMode){
						Serial.println((uint8_t)buffer);
		}
	}
	return (uint8_t)buffer;
}
//------------------------------------------------------------------------

//------------------------------------------------------------------------

//------------------------------------------------------------------------

//------------------------------------------------------------------------

//------------------------------------------------------------------------

//------------------------------------------------------------------------
