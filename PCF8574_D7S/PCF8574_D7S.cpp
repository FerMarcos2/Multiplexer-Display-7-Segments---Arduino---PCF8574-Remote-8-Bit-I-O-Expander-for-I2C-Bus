/*
              PCF8574 Remote 8-Bit I/O Expander for I2C Bus                                                                 ||
MIT License                                                                                                                 ||
https://github.com/FerMarcos2/Multiplexer-Display-7-Segments---Arduino---PCF8574-Remote-8-Bit-I-O-Expander-for-I2C-Bus.git  ||
                                                                                                                            ||
Copyright (c) 2020 Fernando Marcos Marcos All right reserved.                                                               ||
                                                                                                                            ||
Permission is hereby granted, free of charge, to any person obtaining a copy                                                ||
of this software and associated documentation files (the "Software"), to deal                                               ||
in the Software without restriction, including without limitation the rights                                                ||
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell                                                   ||
copies of the Software, and to permit persons to whom the Software is                                                       ||
furnished to do so, subject to the following conditions:                                                                    ||
                                                                                                                            ||
The above copyright notice and this permission notice shall be included in all                                              ||
copies or substantial portions of the Software.                                                                             ||
                                                                                                                            ||
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR                                                  ||
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,                                                    ||
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE                                                 ||
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER                                                      ||
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,                                               ||
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE                                               ||
SOFTWARE.                                                                                                                   ||
______________________________________________________________________________________________________________________________
------------------------------------------------------------------------------------------------------------------------------
*/

#include "PCF8574_D7S.h"
#include "arduino.h"
#include "Wire.h"
PCF8574_D7S::PCF8574_D7S(byte addressSegment,boolean Common,int ndigits,char pin[]){
	Wire.begin();
	for(_k=0;_k<ndigits;_k++){
		pinMode(pin[_k],OUTPUT);
	}
	_ndigits=ndigits;
	_addressSegment=addressSegment;
	_Common=Common;
}
void PCF8574_D7S::Multiplexer(float number,char _pin[],int wait)
{
	float num=number; 
	long numentero=num; 
	String snumentero=String(numentero);
	String snumdecimal=String(num-numentero); 
	int inumdecimal=(num-numentero)*(pow(10,snumdecimal.length()-2))+.01;
	String numintdec;
	if((num-numentero)==0){
  		numintdec=String(numentero);
  	} else if((inumdecimal%10)==0){
  		numintdec=String(numentero)+String(inumdecimal/10);
	} else if(inumdecimal<10){
		numintdec=String(numentero)+'0'+String(inumdecimal);
	} else{
		numintdec=String(numentero)+String(inumdecimal);
	}
	for(int i=0;i<numintdec.length();i++){ 
		switch(numintdec[i]){ 
			case '0':Segments=B00000011;break;
			case '1':Segments=B10011111;break;     
			case '2':Segments=B00100101;break;     
			case '3':Segments=B00001101;break;
			case '4':Segments=B10011001;break;
			case '5':Segments=B01001001;break;
			case '6':Segments=B01000001;break;
			case '7':Segments=B00011111;break;
			case '8':Segments=B00000001;break;
			case '9':Segments=B00001001;break;
		}   
		if((i==snumentero.length()-1)&&((num-numentero)!=0)){
			Segments=Segments-B00000001;
		}
		if(_Common==false){ //Select Display Common Anode
			for(_k=0;_k<_ndigits;_k++){
				if(_k==(_ndigits-numintdec.length()+i)){
					digitalWrite(_pin[_k],HIGH);
				}else{
					digitalWrite(_pin[_k],LOW);
				}
			}
		}else{		 //Select Display Common Catode
			for(_k=0;_k<numintdec.length();_k--){
				if(i==_k){
					digitalWrite(_pin[_k],LOW);
				}else{
					digitalWrite(_pin[_k],HIGH);
				}
			}			
			Segments=~Segments;
		}
		Wire.beginTransmission(_addressSegment);
		Wire.write(Segments);
		Wire.endTransmission();
		delay(wait);
		digitalWrite(_pin[_ndigits-numintdec.length()+i],LOW);
	}	
}
