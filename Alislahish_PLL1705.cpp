/**********************************************************************
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
**********************************************************************/

#include "Alislahish_PLL1705.h"

/**
 * constructor
 */
Alislahish_PLL1705::Alislahish_PLL1705(uint8_t CSELpin, uint8_t FS1pin, uint8_t FS2pin, uint8_t SRpin)
	:	_CSELpin(CSELpin),
		_FS1pin(FS1pin),
		_FS2pin(FS2pin),
		_SRpin(SRpin)

{}


/**
 * choose a sampling frequency and automatically update the sampling rate
 */
void Alislahish_PLL1705::selectSamplingFrequency(PLLSamplingFrequencies freq){
	uint8_t sr = static_cast<uint8_t>(freq);
	_sr = (sr<3) ? PLLSamplingRates::SINGLE : PLLSamplingRates::DOUBLE; //determine sampling rate
	_fs = freq;
	setPins();
}

/**
* choose a SCKO1 output frequency
*/
void Alislahish_PLL1705::selectSCKOFrequency(SCKO1SamplingRates rate){
	_scko1 = rate;
	setPins();
}
/**
* start using this IC at a default frequency
*/
void Alislahish_PLL1705::begin(){
	begin(PLLSamplingFrequencies::HZ_44100);
}

/**
* start using this IC at a given frequency
*/
void Alislahish_PLL1705::begin(PLLSamplingFrequencies freq){
	setPinModes();
	selectSamplingFrequency(freq);
	selectSCKOFrequency(SCKO1SamplingRates::MHZ_16);
}

/**
* set pins to the appropriate (current) operating params
*/
void Alislahish_PLL1705::setPinModes(){
	ICUsingMCP23017::pinMode(_CSELpin, OUTPUT);
	ICUsingMCP23017::pinMode(_FS1pin, OUTPUT);
	ICUsingMCP23017::pinMode(_FS2pin, OUTPUT);
	ICUsingMCP23017::pinMode(_SRpin, OUTPUT);
}

/**
* set pins to the appropriate (current) operating params
*/
void Alislahish_PLL1705::setPins(){
	//SCKO1 frequency
	switch(_scko1){
		case SCKO1SamplingRates::MHZ_33:
			ICUsingMCP23017::digitalWrite(_CSELpin, LOW);
			break;
		case SCKO1SamplingRates::MHZ_16:
			ICUsingMCP23017::digitalWrite(_CSELpin, HIGH);
			break;
	}

	//sampling frequency
	switch(_fs){
		case PLLSamplingFrequencies::HZ_32000:
			ICUsingMCP23017::digitalWrite(_FS2pin, HIGH);
			ICUsingMCP23017::digitalWrite(_FS1pin, LOW);
			ICUsingMCP23017::digitalWrite(_SRpin, LOW);
			break;
		case PLLSamplingFrequencies::HZ_44100:
			ICUsingMCP23017::digitalWrite(_FS2pin, LOW);
			ICUsingMCP23017::digitalWrite(_FS1pin, HIGH);
			ICUsingMCP23017::digitalWrite(_SRpin, LOW);
			break;
		case PLLSamplingFrequencies::HZ_48000:
			ICUsingMCP23017::digitalWrite(_FS2pin, LOW);
			ICUsingMCP23017::digitalWrite(_FS1pin, LOW);
			ICUsingMCP23017::digitalWrite(_SRpin, LOW);
			break;
		case PLLSamplingFrequencies::HZ_64000:
			ICUsingMCP23017::digitalWrite(_FS2pin, HIGH);
			ICUsingMCP23017::digitalWrite(_FS1pin, LOW);
			ICUsingMCP23017::digitalWrite(_SRpin, HIGH);
			break;
		case PLLSamplingFrequencies::HZ_88200:
			ICUsingMCP23017::digitalWrite(_FS2pin, LOW);
			ICUsingMCP23017::digitalWrite(_FS1pin, HIGH);
			ICUsingMCP23017::digitalWrite(_SRpin, HIGH);
			break;
		case PLLSamplingFrequencies::HZ_96000:
			ICUsingMCP23017::digitalWrite(_FS2pin, LOW);
			ICUsingMCP23017::digitalWrite(_FS1pin, LOW);
			ICUsingMCP23017::digitalWrite(_SRpin, HIGH);
			break;						
	}
}