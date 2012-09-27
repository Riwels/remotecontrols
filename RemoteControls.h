/*
 * RemoteControls.h
 *
 * Created: 30/06/2012 4:54:27
 *  Author: Riwels
 */ 

#ifndef REMOTECONTROLS_H_
#define REMOTECONTROLS_H_


//#define RemoteControlDebug	// ArduinoIDE users, uncomment to set debug mode

#ifndef RemoteControlDebug
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
#endif	//RemoteControlDebug

#include <Arduino.h>

class RemoteControl{
	public:
		RemoteControl(uint8_t IRSensorPin);
		unsigned long PressedButton();
		uint8_t Pin;

	private:
		uint8_t a;
		unsigned long buffer;
#ifdef RemoteControlDebug
		uint16_t High[64];
		uint16_t Low [64];
		uint8_t Level;
		bool GetFrom;
		uint8_t Bits;
		void Line();
		unsigned long Read();
		void show1(uint8_t times);		

#else
		uint16_t Data[Pulses];
#endif
};

#endif /* REMOTECONTROLS_H_ */