#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <Pipes.h>
#include <pipeHandler.h>

void vector_control_daemon(){

float Lm = 0.8096;
float Lr = 0.84175;
float tau_r = 0.103919753;
float flux_rotor;
float flux_rotor_prev = 0;
float del_t = 0.000000025;
float theta;
float theta_prev = 0;
float omega_r;
float omega_m = 314.1592654;
float id;
float iq;
float id_err;
float iq_err;
float torque_ref;
float flux_ref;
float flux_ref_prev = 0;
uint8_t poles = 4;

	while(1){
	
		id  = read_float("in_data");
		iq  = read_float("in_data");
		torque_ref  = read_float("in_data");
		flux_ref  = read_float("in_data");	
		
		flux_rotor = ((del_t * Lm * id) + (tau_r * flux_rotor_prev))/(1 + tau_r);
		
		omega_r = (Lm * iq)/(tau_r * flux_rotor);
		theta = theta_prev + del_t * (omega_r + omega_m);
		
		iq_err =  (2 * Lr * torque_ref)/(3 * poles * Lm * flux_rotor);
		
		id_err = (flux_ref + tau_r * (flux_ref - flux_ref_prev)) / Lm ;
		
		flux_rev_prev = flux_ref;
		flux_rotor_prev = flux_rotor;
		theta_prev = theta;
		
		write_float("out_data",id_err);
		write_float("out_data",iq_err);
	}
}
