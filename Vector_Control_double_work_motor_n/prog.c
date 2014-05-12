#include <stdint.h>
#include <Pipes.h>

float fpmul32f(float x, float y)
{
	return(x*y);//Optmize by using shift and add...
}

float fpadd32f(float x, float y)
{
	return(x+y);
}

float fpsub32f(float x, float y)
{
	return(x-y);
}


uint32_t fpadd32fi(uint32_t x, uint32_t y)
{
	return(x+y);
}

uint32_t fpsub32fi(uint32_t x, uint32_t y)
{
	return(x-y);
}


uint32_t udiv32(uint32_t dividend, uint32_t divisor) 
{
   uint32_t remainder = 0;
   uint32_t quotient = 0xffffffff;
   remainder = 0;
   if(divisor == 0)
   {
	return(0xffffffff);
   }
   else if(divisor == 1)
   {
	return(dividend);
   }
   else if(divisor > dividend)
   {
	quotient = 0;
	remainder = dividend;
	return(quotient);
   }
   else
   {
	quotient = 0;
	while(dividend >= divisor)
	{
		uint32_t curr_quotient = 1;
       		uint32_t dividend_by_2 = (dividend >> 1);
		uint32_t shifted_divisor = divisor;

		while(1)
		{
			if(shifted_divisor < dividend_by_2)
			{
				shifted_divisor = (shifted_divisor << 1);
				curr_quotient = (curr_quotient << 1);
			}
			else	
		  		break;
		}

			quotient = fpadd32fi(quotient,curr_quotient);
			dividend = fpsub32fi(dividend,shifted_divisor);
	}

	remainder = dividend;
   }
   return(quotient);
}


float fdiv32(float a, float b)
{
	uint32_t mantissa_a = 0, mantissa_b = 0;
	uint32_t exponent_a = 0, exponent_b = 0;
	uint32_t sign_a = 0, sign_b = 0;
	uint32_t sign = 0;
	uint32_t exp = 0;
	uint32_t man = 0;
	uint32_t ival_a = 0, ival_b = 0;
	uint32_t temp = 0;
	float out_div = 0;

	if (a==0)
		return(0);
	else{
		ival_a = *((uint32_t *)&a);
		ival_b = *((uint32_t *)&b);


		exponent_a = ((ival_a & (0x7F800000))>>23);
		exponent_b = ((ival_b & (0x7F800000))>>23);

		mantissa_a = ((ival_a & (0x007FFFFF)) | (0x00800000))<<7;
		mantissa_b = ((ival_b & (0x007FFFFF)) | (0x00800000))>>7;

		sign_a = (ival_a & (0x80000000))>>31;
		sign_b = (ival_b & (0x80000000))>>31;

		sign = (sign_a ^ sign_b)<<31;
		exp = fpsub32fi(exponent_a, exponent_b);
		man = (udiv32(mantissa_a,mantissa_b));

		temp = man;
		while( ( (temp & (0x00800000)) != 0x00800000) && (temp !=0) )
		{
		man = (man << 1);
		exp = fpsub32fi(exp, 1);
		temp = man;
		}

		man = ((man)& (0x007FFFFF));
		exp = fpadd32fi(exp,136) <<23; // 128 + 7 + 8 -3

		temp = (sign | exp | man);

		out_div = *((float *)&temp); 
	}
	return(out_div);
}

float rotor_flux_calc(float id, float flux_rotor_prev){
	
	float temp_a = 0, temp_b = 0;
	float flux_rotor = 0;

	temp_a = fpmul32f(flux_rotor_prev,0.99952);
	temp_b = fpmul32f(3.88608e-4,id);	
	flux_rotor = fpadd32f(temp_b,temp_a);
	return(flux_rotor);
}
float omega_calc(float iq, float flux_rotor){
	float temp_omega_n = 0;
	float omega_r = 0;
	temp_omega_n = fpmul32f(7.790626677,iq);
	omega_r = fdiv32(temp_omega_n,flux_rotor);
	return(omega_r);
}

float theta_calc(float omega_r, float omega_m, float theta_prev, float del_t){
	float temp_a = 0, temp_b = 0;
	float theta = 0;
	temp_a = fpadd32f(omega_r,omega_m);
	temp_b = fpmul32f(temp_a,del_t);
	theta = fpadd32f(theta_prev,temp_b);
	return(theta);
}

float iq_err_calc(float torque_ref, float flux_rotor){

	float temp_iq_n = 0;
	float iq_err = 0;

	temp_iq_n = fpmul32f(0.3465703228,torque_ref);
	iq_err = fdiv32(temp_iq_n,flux_rotor);
	return(iq_err);
}

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
		
		flux_rotor =  rotor_flux_calc( id,  flux_rotor_prev);
		
		omega_r =  omega_calc( iq, flux_rotor);
		theta =  theta_calc( omega_r,  omega_m, theta_prev,del_t);

		iq_err = iq_err_calc( torque_ref, flux_rotor);
		
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
/*
#include <stdint.h>
#include <Pipes.h>
//float flux_rotor_lpf_prev_value = 0;
//go lower 500 rpm

float fpmul32f(float x, float y)
{
	return(x*y);//Optmize by using shift and add...
}

float fpadd32f(float x, float y)
{
	return(x+y);
}

float fpsub32f(float x, float y)
{
	return(x-y);
}


uint32_t fpadd32fi(uint32_t x, uint32_t y)
{
	return(x+y);
}

uint32_t fpsub32fi(uint32_t x, uint32_t y)
{
	return(x-y);
}


uint32_t udiv32(uint32_t dividend, uint32_t divisor) 
{
   uint32_t remainder = 0;
   uint32_t quotient = 0xffffffff;
   remainder = 0;
   if(divisor == 0)
   {
	return(0xffffffff);
   }
   else if(divisor == 1)
   {
	return(dividend);
   }
   else if(divisor > dividend)
   {
	quotient = 0;
	remainder = dividend;
	return(quotient);
   }
   else
   {
	quotient = 0;
	while(dividend >= divisor)
	{
		uint32_t curr_quotient = 1;
       		uint32_t dividend_by_2 = (dividend >> 1);
		uint32_t shifted_divisor = divisor;

		while(1)
		{
			if(shifted_divisor < dividend_by_2)
			{
				shifted_divisor = (shifted_divisor << 1);
				curr_quotient = (curr_quotient << 1);
			}
			else	
		  		break;
		}

			quotient = fpadd32fi(quotient,curr_quotient);
			dividend = fpsub32fi(dividend,shifted_divisor);
	}

	remainder = dividend;
   }
   return(quotient);
}


float fdiv32(float a, float b)
{
	uint32_t mantissa_a = 0, mantissa_b = 0;
	uint32_t exponent_a = 0, exponent_b = 0;
	uint32_t sign_a = 0, sign_b = 0;
	uint32_t sign = 0;
	uint32_t exp = 0;
	uint32_t man = 0;
	uint32_t ival_a = 0, ival_b = 0;
	uint32_t temp = 0;
	float out_div = 0;

	if (a==0)
		return(0);
	else{
		ival_a = *((uint32_t *)&a);
		ival_b = *((uint32_t *)&b);


		exponent_a = ((ival_a & (0x7F800000))>>23);
		exponent_b = ((ival_b & (0x7F800000))>>23);

		mantissa_a = ((ival_a & (0x007FFFFF)) | (0x00800000))<<7;
		mantissa_b = ((ival_b & (0x007FFFFF)) | (0x00800000))>>7;

		sign_a = (ival_a & (0x80000000))>>31;
		sign_b = (ival_b & (0x80000000))>>31;

		sign = (sign_a ^ sign_b)<<31;
		exp = fpsub32fi(exponent_a, exponent_b);
		man = (udiv32(mantissa_a,mantissa_b));

		temp = man;
		while( ( (temp & (0x00800000)) != 0x00800000) && (temp !=0) )
		{
		man = (man << 1);
		exp = fpsub32fi(exp, 1);
		temp = man;
		}

		man = ((man)& (0x007FFFFF));
		exp = fpadd32fi(exp,136) <<23; // 128 + 7 + 8 -3

		temp = (sign | exp | man);

		out_div = *((float *)&temp); 
	}
	return(out_div);
}

float rotor_flux_calc(float del_t, float Lm, float id, float flux_rotor_prev, float tau_new, float tau_r){
	
	float temp_a = 0, temp_b = 0, temp_c = 0;
	float temp_flux_n = 0,temp_flux_d = 0;
	float flux_rotor = 0;

	temp_a = fpmul32f(id,Lm);
	temp_b = fpmul32f(50e-6,temp_a); 
	temp_c = fpmul32f(0.103919753,flux_rotor_prev);

	temp_flux_n = fpadd32f(temp_c,temp_b);

	flux_rotor = fdiv32(temp_flux_n,0.103969753);
	
	
	temp_a = fpmul32f(flux_rotor_prev,0.99952);
	temp_b = fpmul32f(0.8096,id);
	temp_c = fpmul32f(0.00048,temp_b);	
	flux_rotor = fpadd32f(temp_c,temp_a);
	return(flux_rotor);
}
float omega_calc(float Lm, float iq, float tau_r, float flux_rotor){
	float temp_omega_n = 0,temp_omega_d = 0;
	float omega_r = 0;
	temp_omega_n = fpmul32f(0.8096,iq);
	temp_omega_d = fpmul32f(tau_r,flux_rotor);
	omega_r = fdiv32(temp_omega_n,temp_omega_d);
	return(omega_r);
}

float theta_calc(float omega_r, float omega_m, float del_t, float theta_prev){
	float temp_a = 0, temp_b = 0;
	float theta = 0;
	temp_a = fpadd32f(omega_r,omega_m);
	//temp_a = fpmul32f(2,omega_m);
	temp_b = fpmul32f(temp_a,del_t);
	//temp_b = fpmul32f(omega_m,del_t);
	theta = fpadd32f(theta_prev,temp_b);
	return(theta);
}

float iq_err_calc(float Lr, float torque_ref, float constant_1, float flux_rotor){

	float temp_d = 0;
	float temp_iq_n = 0,temp_iq_d = 0;
	float iq_err = 0;
	float temp_flux_1 = 0;
	float temp_flux_2 = 0;
	float flux_rotor_lpf = 0;

		
	if (flux_rotor<0.001)
		flux_rotor = 0.001;
	else flux_rotor = flux_rotor;
	
	temp_flux_1 = fpmul32f(flux_rotor_lpf_prev_value,0.994986);
	temp_flux_2 = fpmul32f(0.005014,torque_ref);	
	flux_rotor_lpf = fpadd32f(temp_flux_2,temp_flux_1);
	flux_rotor_lpf_prev_value = flux_rotor_lpf;
	
	temp_d = fpmul32f(4.0,Lr);
	temp_iq_n = fpmul32f(temp_d,torque_ref);
	temp_iq_d = fpmul32f(constant_1,flux_rotor);

	iq_err = fdiv32(temp_iq_n,temp_iq_d);
	return(iq_err);
}

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
	float del_t_2 = 25e-6;
	float flux_err_prev = 0;
	float int_speed_err_prev = 0;
	
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
		if (speed_ref <=2000.0)
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
		}
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
		//int_flux_err_temp_2 = fpadd32f(int_flux_err_temp_1,int_flux_err_temp_2);
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
		write_float32("out_data",id_err);
		write_float32("out_data",iq_err);
		write_float32("out_data",theta);
		write_float32("out_data",flux_rotor);
	}
}
*/
