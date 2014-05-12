#include <stdint.h>
#include <Pipes.h>

inline double fpmul64(double x, double y)
{
	return(x*y);
}

inline double fpadd64(double x, double y)
{
	return(x+y);
}

inline double fpsub64(double x, double y)
{
	return(x-y);
}

inline float fpmul32(float x, float y)
{
	return(x*y);
}

inline float fpadd32(float x, float y)
{
	return(x+y);
}

inline float fpsub32(float x, float y)
{
	return(x-y);
}

inline uint64_t fpadd64i(uint64_t x, uint64_t y)
{
	return(x+y);
}

inline uint64_t fpsub64i(uint64_t x, uint64_t y)
{
	return(x-y);
}

inline uint32_t fpadd32i(uint32_t x, uint32_t y)
{
	return(x+y);
}

inline uint32_t fpsub32i(uint32_t x, uint32_t y)
{
	return(x-y);
}

inline uint64_t udiv64(uint64_t dividend, uint64_t divisor) 
{
	uint64_t remainder = 0;
	uint64_t quotient = 0xffffffffffffffff;
	uint64_t curr_quotient = 1;
	uint64_t dividend_by_2 = 0;
	uint64_t shifted_divisor = 0;
	uint64_t count_temp = 0;
	remainder = 0;

	if(divisor == 0)
	{
		return(0xffffffffffffffff);
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
			curr_quotient = 1;
			dividend_by_2 = (dividend >> 1);
			shifted_divisor = divisor;
			count_temp = 0;
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
	
			quotient += curr_quotient;
			dividend -= shifted_divisor;
		}
		remainder = dividend;
	}
	return(quotient);
}


inline double fdiv(double a, double b)
{
	uint64_t mantissa_a = 0, mantissa_b = 0;
	uint64_t exponent_a = 0, exponent_b = 0;
	uint64_t sign_a = 0, sign_b = 0;
	uint64_t sign = 0;
	uint64_t exp = 0;
	uint64_t man = 0;
	uint64_t ival_a = 0, ival_b = 0;
	uint64_t temp = 0;
	double out_div = 0;

	if (a==0)
		return(0);
	else{
		
		ival_a = *((uint64_t *)&a);
		ival_b = *((uint64_t *)&b);


		exponent_a = ((ival_a & (0x7FF0000000000000))>>52);
		exponent_b = ((ival_b & (0x7FF0000000000000))>>52);

		mantissa_a = ((ival_a & (0x000FFFFFFFFFFFFF)) | (0x0010000000000000))<<10;
		mantissa_b = ((ival_b & (0x000FFFFFFFFFFFFF)) | (0x0010000000000000))>>6;
	
		sign_a = (ival_a & (0x8000000000000000))>>63;
		sign_b = (ival_b & (0x8000000000000000))>>63;

		sign = (sign_a ^ sign_b)<<63;
		exp = exponent_a - exponent_b;

		if (mantissa_b == 1)
			man = mantissa_a;
		else
		man = (udiv64((unsigned long int)mantissa_a,(unsigned long int)mantissa_b));

		temp = man;

		while( ( (temp & (0x0010000000000000)) != 0x0010000000000000) && (temp !=0) )
		{
		man = (man << 1);
		exp = exp - 1;
		temp = man;
		}

		man = ((man)& (0x000FFFFFFFFFFFFF));
		exp = (exp + 1036 + 29 -6 ) <<52; // 128 + 7 + 8 -3

		temp = (sign | exp | man);

		out_div = *((double *)&temp); 
	}
	return(out_div);
}

inline uint32_t udiv32(uint32_t dividend, uint32_t divisor) 
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

			quotient = fpadd32i(quotient,curr_quotient);
			dividend = fpsub32i(dividend,shifted_divisor);
	}

	remainder = dividend;
   }
   return(quotient);
}


inline float fdiv32(float a, float b)
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
		mantissa_b = ((ival_b & (0x007FFFFF)) | (0x00800000))>>3;

		sign_a = (ival_a & (0x80000000))>>31;
		sign_b = (ival_b & (0x80000000))>>31;

		sign = (sign_a ^ sign_b)<<31;
		exp = fpsub32i(exponent_a, exponent_b);
		man = (udiv32(mantissa_a,mantissa_b));

		temp = man;
		while( ( (temp & (0x00800000)) != 0x00800000) && (temp !=0) )
		{
		man = (man << 1);
		exp = fpsub32i(exp, 1);
		temp = man;
		}

		man = ((man)& (0x007FFFFF));
		exp = fpadd32i(exp,140 ) <<23; // 128 + 7 + 8 -3

		temp = (sign | exp | man);

		out_div = *((float *)&temp); 
	}
	return(out_div);
}
/*
inline float rotor_flux_calc(float del_t, float Lm, float id, float flux_rotor_prev, float tau_new, float tau_r){

	float temp_a = 0, temp_b = 0, temp_c = 0;
	float temp_flux_n = 0,temp_flux_d = 0;
	float flux_rotor = 0;
	//temp_a = fpmul32(del_t,Lm);
	temp_b = fpmul32(id,0.4145152); 
	temp_c = fpmul32(tau_r,flux_rotor_prev);

	temp_flux_n = fpadd32(temp_c,temp_b);
	//temp_flux_d = (tau_new);

	flux_rotor = fdiv32(temp_flux_n,tau_new);
	
	return(flux_rotor);
}*/

inline float rotor_flux_calc(float del_t, float Lm, float id, float flux_rotor_prev, float tau_new, float tau_r){

	double temp_a = 0, temp_b = 0, temp_c = 0;
	double temp_flux_n = 0,temp_flux_d = 0;
	float flux_rotor = 0;
	temp_a = (double)(50e-6) * Lm;
	temp_b = (double)id * temp_a; 
	temp_c = 0.103919753 * flux_rotor_prev;

	temp_flux_n = temp_c + temp_b;
	//temp_flux_d = (tau_new);

	flux_rotor = (float)fdiv(temp_flux_n,0.103969753);
	
	return(flux_rotor);
}
inline double omega_calc(double Lm, double iq, double tau_r, double flux_rotor){
	double temp_omega_n = 0,temp_omega_d = 0;
	double omega_r = 0;
	temp_omega_n = fpmul64(Lm,iq);
	temp_omega_d = fpmul64(tau_r,flux_rotor);
	omega_r = fdiv(temp_omega_n,temp_omega_d);
	return(omega_r);
}

double theta_calc(double omega_r, double omega_m, double del_t, double theta_prev){
	double temp_a = 0, temp_b = 0;
	double theta = 0;
	temp_a = fpmul64(omega_r,omega_m);
	temp_b = fpmul64(temp_a,del_t);
	theta = fpadd64(theta_prev,temp_a);
	return(theta);
}

inline float iq_err_calc(float Lr, float torque_ref, float constant_1, float flux_rotor){

	float temp_d = 0;
	float temp_iq_n = 0,temp_iq_d = 0;
	float iq_err = 0;

	if (flux_rotor<0.001)
		flux_rotor = 0.001;
	else flux_rotor = flux_rotor;
	
	temp_d = fpmul32(2.0,Lr);
	temp_iq_n = fpmul32(temp_d,torque_ref);
	temp_iq_d = fpmul32(constant_1,flux_rotor);

	iq_err = fdiv32((double)temp_iq_n,(double)temp_iq_d);
	return(iq_err);
}

void vector_control_daemon(){

	double id = 0; double iq = 0; double torque_ref = 0; double flux_ref = 0; double speed = 0; double speed_ref = 0;
	double torque_sat_high = 30, torque_sat_low = -30;
	double speed_err = 0, int_speed_err = 0, prop_speed_err = 0;
	double flux_err = 0, int_flux_err = 0, prop_flux_err = 0, flux_add = 0;
	double Kp = 40, Ki = 50;
	double Lm = 0.8096;
	double Lr = 0.84175;
	double tau_r = 0.103919753;
	double flux_rotor = 0;
	double flux_rotor_prev = 0;
	double del_t = 50e-6;
	double flux_ref_prev = 0;
	double tau_new = 0;
	double theta = 0;
	double theta_prev = 0;
	double omega_r = 0;
	double omega_m = 314.1592654;
	double id_err = 0;
	double iq_err = 0;
	double poles = 4;
	double nf = 0.3;
	double constant_temp = fpmul64(3.0,poles);
	double constant_1 = fpmul64(constant_temp,Lm);
	double int_speed_err_temp_1 = 0;
	double int_speed_err_temp_2 = 0;
	double int_flux_err_temp_1 = 0;
	double int_flux_err_temp_2 = 0;
	double flux_ref_calc_temp_1 = 0;
	double flux_ref_calc_temp_2 = 0;
	double del_t_mod = 0.0512; // del_t * 1024
	double tau_r_mod = 106.4138271;
	tau_new= fpadd64(del_t,tau_r);
	double tau_new_mod = 106.4650271;
	
	while(1){
	
		//Read Data from motor
		id  = read_float64("in_data1");
		iq  = read_float64("in_data2");
		speed  = read_float64("in_data3");
		speed_ref  = read_float64("in_data4");	
		
		//Generation of Reference Values
		speed_err = fpsub64(speed_ref,speed);
		//Torque Reference Value Calculations
		int_speed_err_temp_1 = fpmul64(del_t,speed_err);
		int_speed_err_temp_2 = fpadd64(int_speed_err_temp_1,int_speed_err);
		int_speed_err = fpmul64(Ki,int_speed_err_temp_2);  //////
		if (int_speed_err < -15.0)
			int_speed_err = -15.0;
		else if (int_speed_err > 15.0)
			int_speed_err = 15.0;
		else
			int_speed_err = int_speed_err;
	
		prop_speed_err = fpmul64(speed_err,Kp);
	
		torque_ref = fpadd64(int_speed_err,prop_speed_err);
		
		
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
			flux_ref_calc_temp_1 = fpmul64(-0.0002,speed_ref);
			flux_ref_calc_temp_2 = fpadd64( flux_ref_calc_temp_1, 1.4); 
			flux_ref = fpmul64(flux_ref_calc_temp_2 ,nf);
		}
		else if (speed_ref <=3000.0){
			flux_ref_calc_temp_1 = fpmul64(-0.00036,speed_ref);
			flux_ref_calc_temp_2 = fpadd64( flux_ref_calc_temp_1, 1.8); 
			flux_ref = fpmul64(flux_ref_calc_temp_2 ,nf);
		}
		else{ 
			flux_ref_calc_temp_1 = fpmul64(-0.00042,speed_ref);
			flux_ref_calc_temp_2 = fpadd64( flux_ref_calc_temp_1, 1.98); 
			flux_ref = fpmul64(flux_ref_calc_temp_2 ,nf);
		}
		
		//Vector Control Begins Here
		
		
		flux_rotor =  (double)rotor_flux_calc( (float)del_t_mod, (float) Lm, (float) id, (float) flux_rotor_prev,  (float)tau_new_mod, (float)tau_r_mod);
		//flux_rotor =  rotor_flux_calc( del_t,  Lm,  id,  flux_rotor_prev,  tau_new, tau_r);

		omega_r =  omega_calc( Lm,  iq,  tau_r,  flux_rotor);
		theta =  theta_calc( omega_r,  omega_m,  del_t,  theta_prev);

		iq_err = (double)iq_err_calc((float) Lr,  (float)torque_ref, (float) constant_1,  (float)flux_rotor);
		
		//iD Calculations
		flux_err = fpsub64(flux_ref,flux_rotor);
		int_flux_err_temp_1 = fpmul64(del_t,flux_err);
		int_flux_err_temp_2 = fpadd64(int_flux_err_temp_1,int_flux_err);
		int_flux_err = fpmul64(Ki,int_flux_err_temp_2); 		
		if (int_flux_err < -1.0)
			int_flux_err = -1.0;
		else if (int_flux_err > 1.0)
			int_flux_err = 1.0;
		else
			int_flux_err = int_flux_err;
		
		prop_flux_err = fpmul64(flux_err,Kp);
		
		flux_add = fpadd64(int_flux_err,prop_flux_err);
		
		if (flux_add < -2.0)
			flux_add = -2.0;
		else if (flux_add > 2.0)
			flux_add = 2.0;
		else
			flux_add = flux_add;
		
		id_err = fdiv(flux_add,Lm);

		
		flux_ref_prev = flux_ref;

		flux_rotor_prev = flux_rotor;
		theta_prev = theta;
		
		//Write Back Generated Data
		write_float64("out_data1",id_err);
		write_float64("out_data2",iq_err);
		write_float64("out_data3",torque_ref);
		write_float64("out_data4",flux_rotor);
	}
}
