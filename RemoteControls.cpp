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
RemoteControl::RemoteControl(uint8_t IRSensorPin){
	Pin = IRSensorPin;
	pinMode(Pin,INPUT);
#ifdef RemoteControlDebug
	Level = 1;
#endif
}
//------------------------------------------------------------------------
#ifndef RemoteControlDebug
unsigned long RemoteControl::PressedButton(){
	buffer = 0;
	if(pulseIn(Pin,GetValuesFrom,MaxWaitingTime)>FirstPulseLength){
		for(a=0;a<Pulses;a++) Data[a] = pulseIn(Pin,GetValuesFrom,FirstPulseLength);
		for(a=0;a<Pulses;a++) {
			if((Data[a] > FirstPulseLength+5000)|(Data[a]<LowPulseLength))return 0;
			buffer <<= 1;
			if(Data[a] > HighPulseLength) buffer |= 1;
		}
	}
	return buffer;
}
#else
void RemoteControl::Line(){
	Serial.println("-----------------------------");
}
unsigned long RemoteControl::Read(){
	return Serial.readString().toInt();
}
void RemoteControl::show1(uint8_t times){
	for(a=0;a<times;a++){
		Serial.print(a+1);
		Serial.print("	| ");
		Serial.print(High[a]);
		Serial.println("	|");
	}
}	
unsigned long RemoteControl::PressedButton(){
	if(Level==1){
		for(a=0;a<64;a++){
			High[a] = pulseIn(Pin,HIGH,50000);
			Low [a] = pulseIn(Pin,LOW ,50000);
		}	
		Line();
		for(a=0;a<64;a++){
			Serial.print(a+1);
			Serial.print("	| H | ");
			Serial.print(High[a]);
			Serial.print("		| L | ");
			Serial.print(Low[a]);
			Serial.println("		|");
		}
		Line();
		Serial.println("Step 1: Send 1 to get values from HIGH or 0 from LOW");
		if(Serial.available()){
			a = Read();
			if(a==0){
				GetFrom = false;
				Level   = 2;
			}else if(a==1){
				GetFrom = true;
				Level   = 2;
			}
			Serial.flush();
		}
	}else if(Level==2){
		for(a=0;a<64;a++){
			High[a] = pulseIn(Pin,GetFrom,100000);
		}
		Line();
		show1(64);
		Line();
		Serial.println("Step 2: Send first pulse length");
		delay(1000);
		if(Serial.available()){
			Low[10] = Read();
			if(Low[10]>1000){
				Low[0]  = Low[10];
				Level   = 3;
			}else Serial.println("Very low value");
			Serial.flush();
		}
	}else if(Level==3){
		if(pulseIn(Pin,GetFrom,100000)>Low[0]){
			for(a=0;a<64;a++){
				High[a] = pulseIn(Pin,GetFrom,Low[0]);
			}
			
			Line();
			show1(64);
			Line();
			Serial.println("Step 3: Send amount of bits");
			if(Serial.available()){
				Low[10] = Read();
				if(Low[10]>7){
					Bits    = Low[10];
					Level   = 4;
				}else Serial.println("Very low value");
				Serial.flush();
			}
		}		
	}else if(Level==4){
		if(pulseIn(Pin,GetFrom,100000)>Low[0]){
			for(a=0;a<Bits;a++){
				High[a] = pulseIn(Pin,GetFrom,100000);
			}
			show1(Bits);
			Serial.println("Step 4a: Send high pulse value");	
			if(Serial.available()){
				Low[10] = Read();
				if(Low[10]>300&&a<Low[0]){
					Low[1]  = Low[10];
					Level   = 5;
				}else Serial.println("Wrong value");
				Serial.flush();
			}
		}
	}else if(Level==5){
		Serial.println("Step 4b: Send low pulse value");
		delay(5000);
		if(Serial.available()){
			Low[10] = Read();
			if(Low[10]>50&&Low[10]<Low[1]){
				Low[2]  = Low[10];
				Level   = 6;
			}else Serial.println("Wrong value");
			Serial.flush();
		}
	}else if(Level==6){
		Line();
		Serial.println("Copy and paste this code at the start of your project:");
		Serial.println(" ");
		Serial.print("#define Pulses ");Serial.println(Bits);
		Serial.println("#define MaxWaitingTime 100000");
		Serial.print("#define FirstPulseLength ");Serial.println(Low[0]);
		Serial.print("#define HighPulseLength "); Serial.println(Low[1]);
		Serial.print("#define LowPulseLength ");  Serial.println(Low[2]);
		Serial.print("#define GetValuesFrom ");
		if(GetFrom) Serial.println("HIGH"); else Serial.println("LOW");
		Level = 7;	
	}else if(Level==7){
		buffer=0;
		if(pulseIn(Pin,GetFrom,100000)>Low[0]){
			for(a=0;a<Bits;a++)
				High[a] = pulseIn(Pin,GetFrom,Low[0]);
			for(a=0;a<Bits;a++) {
				if((High[a] > Low[0]+5000)|(High[a]<Low[2]))return 0;
				buffer <<= 1;
				if(High[a] > Low[1]) buffer |= 1;
			}
			Line();
			Serial.print("8b  ");
			Serial.println((uint8_t)buffer);
			Serial.print("16b ");
			Serial.println((uint16_t)buffer);
			Serial.print("32b ");
			Serial.println((uint32_t)buffer);
		}		
	}
	return buffer;
}
#endif
//------------------------------------------------------------------------

//------------------------------------------------------------------------

//------------------------------------------------------------------------

//------------------------------------------------------------------------

//------------------------------------------------------------------------

//------------------------------------------------------------------------
