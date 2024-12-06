/*
	MQ4.h
	Created by Fergus Baker
	22 August 2020
	Released into the public domain.
	
	Based by the work done by Fergus Baker.
	Modified by Veronica Oviedo to work with a MQ4 CH4 Gas Sensor
	31 July 2023 // thx Fergus
*/

#ifndef MQ4_H
#define MQ4_H

#include "Arduino.h"

/*
	CONFIGURATION OPTIONS 
*/
#define CALIBRATION_SECONDS 15
//#define _R0 0.025	// uncomment to define hardcoded R0 value
					// may be desireable if no calibration delay wanted

/*
IMPORTANT:

Calibration takes CALIBRATION_SECONDS + 1 seconds to let the device heat up! Else faulty readings.

Datasheet:
https://www.parallax.com/sites/default/files/downloads/605-00007-MQ-7-Datasheet.pdf

Datasheet has a graph for sensitivity characteristic, following the equation

	ppm(Rs/R0) = A0 * Rs/R0 ^ (A1)

with fitted parameters
	A0
	A1
and variable
	Rs	surface resistance at various gas concentrations
	R0	sensor resistance at 100ppm CO in clean air

NB: ppm == 100 defines Rs/R0 == 1.

The device reads analog pin, which can be translated into Rs/RL where RL is a resistance load.
In the datasheet, RL is said to be ~10 kOhm, thus we use 
	Rs = (Rs/RL) * RL

Calculated fitted parameters using inverse of ppm(Rs/R0) and plot in datasheet:
*/

#define _COEF_A0 1012.7
#define _COEF_A1 -2.786

/* 
datasheet provides typical load resistance RL to be ~10 kOhm
*/
#define _LOAD_RES 10.0

/*
need to calibrate device by knowing R0 -- in clean air, approx 10ppm, ratio is
	Rs / R0 = 5.0
*/
#define _CALIBRATION_CONSTANT 5.0

class MQ4 {
public:
	MQ4(int pin) : a_pin(pin), v_in(5.0) { 
		;	// 5.0 Volts assumed / recommended
	}
	MQ4(int pin, float v_in) : a_pin(pin), v_in(v_in) {
		;
	}

	float readPpm();
	float readRs();
	float readRsRL();	// Rs / RL
	void calibrate();

	float getR0();

private:
	int a_pin;
	float v_in;

	#ifndef _R0 // If no hardcoded R0 value

	float R0;

	#else 		// else make constant

	const R0 = _R0;

	#endif		// end

	float convertVoltage(int voltage);
};

#endif
