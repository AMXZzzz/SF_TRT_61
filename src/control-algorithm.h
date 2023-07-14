#pragma once

#include <windows.h>

struct Algorithm {

	VOID PidControl(float*, float, float, float);
	VOID FOVControl(float*, const float&,const int&, const int&, const int&);
	VOID MaxMovePixel(float* ,const int&);

private:
	float deviation = 0;		
	float last_deviation = 0;		
	float target_amount = 0;	
	float P = 0;
	float I = 0;
	float D = 0;
	float pi = 3.14159265358979323846;

}; 

