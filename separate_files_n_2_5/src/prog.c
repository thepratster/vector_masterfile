#include "prog.h"
void vector_control_daemon(){

	float id = 0; float iq = 0; float torque_ref = 0; float flux_ref = 0; float speed = 0; 
	float speed_ref = 0, speed_ref_temp = 0;
	float speed_err = 0, int_speed_err = 0, prop_speed_err = 0;
	float flux_err = 0, int_flux_err = 0, prop_flux_err = 0, flux_add = 0;
	float Lm = 0.8096;
	float Lr = 0.84175;
	float tau_r = 0.103919753;
	float flux_rotor = 0;
	float flux_rotor_prev = 0;
	float del_t = 50e-6;
	float theta = 0;
	float theta_prev = 0;
	float omega_r = 0;
	float omega_m = 0;
	float id_err = 0;
	float iq_err = 0;
	float speed_err_prev = 0;
	float int_speed_err_temp_0 = 0;
	float int_speed_err_temp_1 = 0;
	float int_speed_err_temp_2 = 0;
	float int_flux_err_temp_0 = 0;
	float int_flux_err_temp_1 = 0;
	float int_flux_err_temp_2 = 0;
	float flux_ref_calc_temp_1 = 0;
	float flux_ref_calc_temp_2 = 0;
	float temp_flux_1 = 0;
	float temp_flux_2 = 0;
	float flux_rotor_lpf = 0;
	float flux_rotor_lpf_prev = 0;
	float temp_spd_1 = 0;
	float temp_spd_2 = 0;
	float spd_lpf = 0;
	float spd_lpf_prev = 0;
	float flux_err_prev = 0;
	float int_speed_err_prev = 0;
	float temp_a = 0, temp_b = 0;
	float temp_omega_n = 0;
	float temp_iq_n = 0;
	
	while(1){
	
		//Read Data from motor
		id  = read_float32("in_data");
		iq  = read_float32("in_data");
		speed  = read_float32("in_data");
		speed_ref_temp  = read_float32("in_data");	
		omega_m  = read_float32("in_data");
				
		if(speed_ref < speed_ref_temp)
			speed_ref = speed_ref + 0.05;
		else if(speed_ref > speed_ref_temp)
			speed_ref = speed_ref - 0.05;
		else speed_ref = speed_ref;
		
		
		//Generation of Reference Values
		
		temp_spd_1 = fpmul32f(spd_lpf_prev,0.3);
		temp_spd_2 = fpmul32f(0.7,speed);	
		spd_lpf = fpadd32f(temp_spd_2,temp_spd_1);
		spd_lpf_prev = spd_lpf;
		
		speed_err = fpsub32f(speed_ref,spd_lpf);
		//Torque Reference Value Calculations
		int_speed_err_temp_0 = fpadd32f(speed_err,speed_err_prev);
		int_speed_err_temp_1 = fpmul32f(250e-6,int_speed_err_temp_0);
		speed_err_prev = speed_err;
		int_speed_err = fpadd32f(int_speed_err_temp_1,int_speed_err_prev);
		int_speed_err_prev = int_speed_err;
		if (int_speed_err < -10.0)
			int_speed_err = -10.0;
		else if (int_speed_err > 10.0)
			int_speed_err = 10.0;
		else
			int_speed_err = int_speed_err;
	
		prop_speed_err = fpmul32f(speed_err,5);
	
		torque_ref = fpadd32f(int_speed_err,prop_speed_err);
		
		
		if (torque_ref < -20)
			torque_ref = -20;
		else if (torque_ref > 20)
			torque_ref = 20;
		else
			torque_ref = torque_ref;
		
		//Flux Reference Value Calculations

		flux_ref = 0.3;
		
		//Vector Control Begins Here
		
		//flux_rotor =  rotor_flux_calc( id,  flux_rotor_prev);
		temp_a = fpmul32f(flux_rotor_prev,0.99952);
		temp_b = fpmul32f(3.88608e-4,id);	
		flux_rotor = fpadd32f(temp_b,temp_a);
		
		//omega_r =  omega_calc( iq, flux_rotor);
		temp_omega_n = fpmul32f(7.790626677,iq);
		omega_r = fdiv32(temp_omega_n,flux_rotor);
		
		//theta =  theta_calc( omega_r,  omega_m, theta_prev,del_t);
		temp_a = fpadd32f(omega_r,omega_m);
		temp_b = fpmul32f(temp_a,del_t);
		theta = fpadd32f(theta_prev,temp_b);

		//iq_err = iq_err_calc( torque_ref, flux_rotor);
		temp_iq_n = fpmul32f(0.3465703228,torque_ref);
		iq_err = fdiv32(temp_iq_n,flux_rotor);
		
		//iD Calculations
		
		
		temp_flux_1 = fpmul32f(flux_rotor_lpf_prev,0.994986);
		temp_flux_2 = fpmul32f(0.005014,flux_rotor);	
		flux_rotor_lpf = fpadd32f(temp_flux_2,temp_flux_1);
		
		flux_rotor_lpf_prev = flux_rotor_lpf;
		
		flux_err = fpsub32f(flux_ref,flux_rotor_lpf);
		
		flux_err_prev = flux_err;
		int_flux_err_temp_1 = fpmul32f(del_t,flux_err);
		int_flux_err_temp_2 = fpadd32f(int_flux_err_temp_1,int_flux_err_temp_2);
		int_flux_err = fpmul32f(50,int_flux_err_temp_2); 		
				
		if (int_flux_err < -1)
			int_flux_err = -1;
		else if (int_flux_err > 1)
			int_flux_err = 1;
		else
			int_flux_err = int_flux_err;
		
		prop_flux_err = fpmul32f(flux_err,40);
		
		flux_add = fpadd32f(int_flux_err,prop_flux_err);
		
		if (flux_add < -2)
			flux_add = -2;
		else if (flux_add > 2)
			flux_add = 2 ;
		else
			flux_add = flux_add;
		
		id_err = fdiv32(flux_add,Lm);

		flux_rotor_prev = flux_rotor;
		theta_prev = theta;
		
		//Write Back Generated Data
		write_float32("out_data",id_err);
		write_float32("out_data",iq_err);
		write_float32("out_data",theta);
		write_float32("out_data",flux_rotor);
	}
}
