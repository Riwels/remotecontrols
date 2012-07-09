/*
 * RemoteControls.h
 *
 * Created: 30/06/2012 4:54:27
 *  Author: Riwels
 */ 

#ifndef REMOTECONTROLS_H_
#define REMOTECONTROLS_H_

#ifndef Pulses
#warning: Pulses not defined. Default is 32
#define Pulses 32
#endif

#ifndef MaxWaitingTime
#warning: MaxWaitingTime not defined. Default is 100000
#define MaxWaitingTime 100000
#endif

#ifndef FirstPulseLength
#warning: FirstPulseLength not defined. Default is 4000
#define FirstPulseLength 4000
#endif

#ifndef HighPulseLength
#warning: HighPulseLength not defined. Default is 1400
#define HighPulseLength 1400
#endif

#ifndef LowPulseLength
#warning: LowPulseLength not defined. Default is 300
#define LowPulseLength 300
#endif

#ifndef GetValuesFrom
#warning: GetValuesFrom not defined. Default is HIGH
#define GetValuesFrom HIGH
#endif

#include <Arduino.h>

class RemoteControl{
	public:
		RemoteControl(uint8_t IRSensorPin,bool EnableDebug);
		uint8_t PressedButton();
		bool DebugMode;

	private:
		uint8_t Pin;
		uint16_t Data[Pulses];
		uint8_t a;
		unsigned long PulseIn(unsigned long WaitingTime);
		unsigned long buffer;
};

#endif /* REMOTECONTROLS_H_ */