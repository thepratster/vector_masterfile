#include "prog.h"
void vector_control_daemon(){

	float id = 0; float iq = 0; float torque_ref = 0; float flux_ref = 0; float speed = 0; 
	float speed_ref = 0, speed_ref_temp = 0;
	float torque_sat_high = 20, torque_sat_low = -20;
	float speed_err = 0, int_speed_err = 0, prop_speed_err = 0;
	float flux_err = 0, int_flux_err = 0, prop_flux_err = 0, flux_add = 0;
	float Kp = 10, Ki = 5;
	float Kp_n = 40, Ki_n = 50;
	float Lm = 0.8096;
	float Lr = 0.84175;
	float tau_r = 0.103919753;
	float flux_rotor = 0;
	float flux_rotor_prev = 0;
	float del_t = 50e-6;
	float flux_ref_prev = 0;
	float tau_new = 0;
	float theta = 0;
	float theta_prev = 0;
	float omega_r = 0;
	float omega_m = 314.1592654;
	float id_err = 0;
	float iq_err = 0;
	float poles = 4;
	float nf = 0.300;
//	float constant_temp = fpmul32f(3000,poles);
//	float constant_1 = fpmul32f(constant_temp,Lm);
	float constant_1 = 9.7152;
	float speed_err_prev = 0;
	float int_speed_err_temp_0 = 0;
	float int_speed_err_temp_1 = 0;
	float int_speed_err_temp_2 = 0;
	float int_flux_err_temp_0 = 0;
	float int_flux_err_temp_1 = 0;
	float int_flux_err_temp_2 = 0;
	float flux_ref_calc_temp_1 = 0;
	float flux_ref_calc_temp_2 = 0;
	float id_prev = 0;
	float temp_flux_1 = 0;
	float temp_flux_2 = 0;
	float flux_rotor_lpf = 0;
	float flux_rotor_lpf_prev = 0;
	float temp_spd_1 = 0;
	float temp_spd_2 = 0;
	float spd_lpf = 0;
	float spd_lpf_prev = 0;
	//float del_t_2 = 25e-6;
	float flux_err_prev = 0;
	float int_speed_err_prev = 0;
	
	while(1){
	
		//Read Data from motor
		id  = read_float32("in_data1");
		iq  = read_float32("in_data2");
		speed  = read_float32("in_data3");
		speed_ref_temp  = read_float32("in_data4");	
		omega_m  = read_float32("in_data5");
				
		if(speed_ref < speed_ref_temp)
			speed_ref = speed_ref + 0.1;
		else if(speed_ref > speed_ref_temp)
			speed_ref = speed_ref - 0.1;
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
		//int_speed_err_temp_1 = fpmul32f(del_t,int_speed_err);
		//int_speed_err_temp_2 = fpmul32f(10,int_speed_err_temp_1); 
		//int_speed_err_temp_2 = fpadd32f(int_speed_err_temp_1,int_speed_err_temp_2);
		//int_speed_err = fpmul32f(10,int_speed_err_temp_2);
		int_speed_err = fpadd32f(int_speed_err_temp_1,int_speed_err_prev);
		int_speed_err_prev = int_speed_err;
		//int_speed_err = fpadd32f(int_speed_err,int_speed_err_temp_2); 
		if (int_speed_err < -10.0)
			int_speed_err = -10.0;
		else if (int_speed_err > 10.0)
			int_speed_err = 10.0;
		else
			int_speed_err = int_speed_err;
	
		prop_speed_err = fpmul32f(speed_err,5);
	
		torque_ref = fpadd32f(int_speed_err,prop_speed_err);
		
		
		if (torque_ref < torque_sat_low)
			torque_ref = torque_sat_low;
		else if (torque_ref > torque_sat_high)
			torque_ref = torque_sat_high;
		else
			torque_ref = torque_ref;
		
		//Flux Reference Value Calculations
		/*1if (speed_ref <=2000.0)
			flux_ref = nf;
		else if (speed_ref <=2500.0){
			flux_ref_calc_temp_1 = fpmul32f(-0.0002,speed_ref);
			flux_ref_calc_temp_2 = fpadd32f( flux_ref_calc_temp_1, 1.4); 
			flux_ref = fpmul32f(flux_ref_calc_temp_2 ,nf);
		}
		else if (speed_ref <=3000.0){
			flux_ref_calc_temp_1 = fpmul32f(-0.00036,speed_ref);
			flux_ref_calc_temp_2 = fpadd32f( flux_ref_calc_temp_1, 1.8); 
			flux_ref = fpmul32f(flux_ref_calc_temp_2 ,nf);
		}
		else{ 
			flux_ref_calc_temp_1 = fpmul32f(-0.00042,speed_ref);
			flux_ref_calc_temp_2 = fpadd32f( flux_ref_calc_temp_1, 1.98); 
			flux_ref = fpmul32f(flux_ref_calc_temp_2 ,nf);
		}*/
		flux_ref = nf;
		
		//Vector Control Begins Here
		tau_new= fpadd32f(del_t,tau_r);
		
		flux_rotor =  rotor_flux_calc( del_t,  Lm,  id,  flux_rotor_prev,  tau_new, tau_r);
		
		omega_r =  omega_calc( Lm,  iq,  tau_r,  flux_rotor);
		theta =  theta_calc( omega_r,  omega_m,  del_t,  theta_prev);

		iq_err = iq_err_calc( Lr,  torque_ref,  constant_1,  flux_rotor);
		
		//iD Calculations
		
		
		temp_flux_1 = fpmul32f(flux_rotor_lpf_prev,0.994986);
		temp_flux_2 = fpmul32f(0.005014,flux_rotor);	
		flux_rotor_lpf = fpadd32f(temp_flux_2,temp_flux_1);
		
		flux_rotor_lpf_prev = flux_rotor_lpf;
		
		flux_err = fpsub32f(flux_ref,flux_rotor_lpf);
		//int_flux_err_temp_0 = fpadd32f(flux_err_prev,flux_err);
		//int_flux_err_temp_1 = fpmul32f(del_t_2,int_flux_err_temp_0);
		//nt_flux_err_temp_2 = fpadd32f(int_flux_err_temp_1,int_flux_err_temp_2);
		//int_flux_err = fpmul32f(Ki_n,int_flux_err_temp_2); 		
		//int_flux_err = fpadd32f(int_flux_err,Ki_n); 		
		
		flux_err_prev = flux_err;
		int_flux_err_temp_1 = fpmul32f(del_t,flux_err);
		int_flux_err_temp_2 = fpmul32f(int_flux_err_temp_1,int_flux_err_temp_2);
		int_flux_err = fpmul32f(Ki_n,int_flux_err_temp_2); 		
		
		
		if (int_flux_err < -1)
			int_flux_err = -1;
		else if (int_flux_err > 1)
			int_flux_err = 1;
		else
			int_flux_err = int_flux_err;
		
		prop_flux_err = fpmul32f(flux_err,Kp_n);
		
		flux_add = fpadd32f(int_flux_err,prop_flux_err);
		
		if (flux_add < -2)
			flux_add = -2;
		else if (flux_add > 2)
			flux_add = 2 ;
		else
			flux_add = flux_add;
		
		id_err = fdiv32(flux_add,Lm);

		
		flux_ref_prev = flux_ref;
		id_prev = id;

		flux_rotor_prev = flux_rotor;
		theta_prev = theta;
		
		//Write Back Generated Data
		write_float32("out_data1",id_err);
		write_float32("out_data2",iq_err);
		write_float32("out_data3",theta);
		write_float32("out_data4",flux_rotor);
	}
}
