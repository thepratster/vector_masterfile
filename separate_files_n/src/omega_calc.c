#include "prog.h"
float omega_calc(float Lm, float iq, float tau_r, float flux_rotor){
	float temp_omega_n = 0,temp_omega_d = 0;
	float omega_r = 0;
	temp_omega_n = fpmul32f(Lm,iq);
	temp_omega_d = fpmul32f(tau_r,flux_rotor);
	omega_r = fdiv32(temp_omega_n,temp_omega_d);
	return(omega_r);
}
