#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <Pipes.h>

/*
float fpmul(float x, float y)
{
	return(x*y);
}

float fpadd(float x, float y)
{
	return(x+y);
}

float fpsub(float x, float y)
{
	return(x-y);
}*/

uint32_t udiv32(uint32_t dividend, uint32_t divisor) 
{
   uint32_t remainder = 0;
   uint32_t quotient = 0xffffffff;
   remainder = 0;
   if(divisor == 0)
   {
	return(quotient);
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

		quotient += curr_quotient;
		dividend -= shifted_divisor;
	}

	remainder = dividend;
   }
   return(quotient);
}


float fdiv(float a, float b)
{
	uint32_t mantissa_a, mantissa_b;
	uint32_t exponent_a, exponent_b;
	uint32_t sign_a, sign_b;
	uint32_t sign;
	uint32_t exp;
	uint32_t man;
	uint32_t ival_a, ival_b;
	uint32_t temp;
	float out_div;

	ival_a = *((uint32_t *)&a);
	ival_b = *((uint32_t *)&b);


	exponent_a = ((ival_a & (0x7F800000))>>23);
	exponent_b = ((ival_b & (0x7F800000))>>23);

	mantissa_a = ((ival_a & (0x007FFFFF)) | (0x00800000))<<7;
	mantissa_b = ((ival_b & (0x007FFFFF)) | (0x00800000))>>3;
	
	sign_a = (ival_a & (0x80000000))>>31;
	sign_b = (ival_b & (0x80000000))>>31;

	sign = (sign_a ^ sign_b)<<31;
	exp = exponent_a - exponent_b;
	man = (udiv32(mantissa_a,mantissa_b));

	temp = man;
	while( ( (temp & (0x00800000)) != 0x00800000) && (temp !=0) )
	{
	man = (man << 1);
	exp = exp - 1;
	temp = man;
	}

	man = ((man)& (0x007FFFFF));
	exp = (exp + 140 ) <<23; // 128 + 7 + 8 -3

	temp = (sign | exp | man);

	out_div = *((float *)&temp); 

	return(out_div);
}


void vector_control_daemon(){

	float Lm = 0.8096;
	float Lr = 0.84175;
	float tau_r = 0.103919753;
	float flux_rotor;
	float flux_rotor_prev = 0;
	float del_t = 25e-9;
	float temp_flux_n,temp_flux_d;
	float flux_ref;
	float flux_ref_prev = 0;
	float tau_new;
	float id;
	float iq;
	float torque_ref;
	float theta;
	float theta_prev = 0;
	float omega_r;
	float omega_m = 314.1592654;
	float id_err;
	float iq_err;
	float poles = 4;
	float temp_iq_n,temp_iq_d;
	float temp_omega_n,temp_omega_d;
	float temp_id_n,temp_id_d;
	float temp_a,temp_b;

	while(1){
	
		id  = read_float32("in_data1");
		iq  = read_float32("in_data2");
		torque_ref  = read_float32("in_data3");
		flux_ref  = read_float32("in_data4");	
		
		/*
		tau_new= fpadd(1.0,tau_r);
		
		temp_flux_n = fpmul(fpmul(del_t,Lm),id) + fpmul(tau_r,flux_rotor_prev);
		temp_flux_d = (tau_new);
		flux_rotor = fdiv(temp_flux_n,temp_flux_d);
		
		temp_omega_n = fpmul(Lm,iq);
		temp_omega_d = fpmul(tau_r,flux_rotor);
		
		omega_r = fdiv(temp_omega_n,temp_omega_d);
		
		theta = fpadd(theta_prev , fpmul(del_t , fpadd(omega_r , omega_m)));
		
		temp_iq_n = fpmul(0.66666666666666 , fpmul( Lr , torque_ref));
		temp_iq_d = fpmul(poles ,fpmul( Lm , flux_rotor));
		iq_err = fdiv(temp_iq_n,temp_iq_d);
		
		temp_id_n = fpadd(flux_ref , fpmul(tau_r , fpsub(flux_ref , flux_ref_prev)));
		temp_id_d = Lm ;
		id_err = fdiv(temp_id_n,temp_id_d);
		*/
		
		tau_new= (1.0 + tau_r);
		
		temp_a = del_t * Lm;
		temp_a = id * temp_a;
		
		temp_b = tau_r * flux_rotor_prev;
		
		temp_flux_n = temp_a + temp_b;
		temp_flux_d = (tau_new);
		flux_rotor = fdiv(temp_flux_n,temp_flux_d);
		
		temp_omega_n = (Lm * iq);
		temp_omega_d = (tau_r * flux_rotor);
		omega_r = fdiv(temp_omega_n,temp_omega_d);
		
		temp_a = (omega_r + omega_m);
		temp_a = (omega_r + omega_m) * del_t;
		theta = theta_prev + temp_a;
		
		temp_a = (0.666666666666 * Lr);
		temp_iq_n = temp_a * torque_ref;
		temp_b = poles * Lm;
		temp_iq_d = (temp_b * flux_rotor);
		iq_err = fdiv(temp_iq_n,temp_iq_d);
		
		temp_a = (flux_ref - flux_ref_prev);
		temp_a = tau_r * temp_a;
		temp_id_n = (flux_ref + temp_a);
		temp_id_d = Lm ;
		id_err = fdiv(temp_id_n,temp_id_d);
		
		/*
		temp_flux_n = ((del_t * Lm * id) + (tau_r * flux_rotor_prev));
		temp_flux_d = (tau_new);
		flux_rotor = fdiv(temp_flux_n,temp_flux_d);
		//flux_rotor = ((del_t * Lm * id) + (tau_r * flux_rotor_prev))/(tau_new);
		
		temp_omega_n = (Lm * iq);
		temp_omega_d = (tau_r * flux_rotor);
		omega_r = fdiv(temp_omega_n,temp_omega_d);
		//omega_r = (Lm * iq)/(tau_r * flux_rotor);
		
		theta = theta_prev + del_t * (omega_r + omega_m);
		
		temp_iq_n = (2 * Lr * torque_ref);
		temp_iq_d = (3 * poles * Lm * flux_rotor);
		iq_err = fdiv(temp_iq_n,temp_iq_d);
		//iq_err = temp1 / temp2;
		
		
		temp_id_n = (flux_ref + tau_r * (flux_ref - flux_ref_prev));
		temp_id_d = Lm ;
		id_err = fdiv(temp_id_n,temp_id_d);
		//id_err = (flux_ref + tau_r * (flux_ref - flux_ref_prev)) / Lm ;
		*/
				
		flux_ref_prev = flux_ref;
		flux_rotor_prev = flux_rotor;
		theta_prev = theta;
			
		write_float32("out_data1",id_err);
		write_float32("out_data2",iq_err);
	}
}
