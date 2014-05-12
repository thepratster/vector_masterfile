#include "prog.h"
float rotor_flux_calc(float del_t, float Lm, float id, float flux_rotor_prev, float tau_new, float tau_r){

	float temp_a = 0, temp_b = 0, temp_c = 0;
	float temp_flux_n = 0,temp_flux_d = 0;
	float flux_rotor = 0;

	temp_a = fpmul32f(flux_rotor_prev,0.9995);
	temp_b = fpmul32f(0.8096,id);
	temp_c = fpmul32f(0.0005,temp_b);	
	flux_rotor = fpadd32f(temp_c,temp_a);
	return(flux_rotor);
}
