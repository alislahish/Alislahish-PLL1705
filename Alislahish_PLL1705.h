/**
*
* Alislahish_PLL1705.h
* by anwar hahj jefferson-george
*
* This file can be used to represent the functions and settings of the
*
*  PLL1705 3.3V Dual PLL Multiclock Generator by Burr Brown Products from Texas Instruments.
*
*  Product page:
*  http://www.ti.com/product/pll1705
*  Datasheet (Rev. A):
*  http://www.ti.com/lit/gpn/pll1705
**********************************************************************
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "Arduino.h"
#include "ICUsingMCP23017.h"

//only include this library description once
#ifndef ALISLAHISH_PLL1705_H
#define ALISLAHISH_PLL1705_H

//valid sampling frequencies this generator can be used with
enum class PLLSamplingFrequencies{
	KHZ_32000 = 0x00,
	KHZ_44100 = 0x01,
	KHZ_48000 = 0x02,
	KHZ_64000 = 0x03,
	KHZ_88200 = 0x04,
	KHZ_96000 = 0x05
};

//generator sampling rates
enum class PLLSamplingRates{
	SINGLE = 0x01,
	DOUBLE = 0x02
};

//sampling rates for the SCKO1 output 
enum class SCKO1SamplingRates{
	MHZ_33 = 0x00, //Low 33.8688 MHz
	MHZ_16 = 0x01 //High 16.9344 MHz
};

//library interface description
class Alislahish_PLL1705 : public ICUsingMCP23017
{
	public:
		Alislahish_PLL1705(uint8_t CSELpin, uint8_t FS1pin, uint8_t FS2pin, uint8_t SRpin);
		void selectSamplingFrequency(PLLSamplingFrequencies freq);
		void begin();
		void begin(PLLSamplingFrequencies freq );

	private:
		//operating params
		PLLSamplingFrequencies _fs;
		PLLSamplingRates _sr;
		SCKO1SamplingRates _scko1;
		//hardware connects
		uint8_t _CSELpin;	//pin 12
		uint8_t _FS1pin; 	//pin 5
		uint8_t _FS2pin; 	//pin 6
		uint8_t _SRpin;  	//pin 7

		void setPinModes();
		void setPins();
};

#endif