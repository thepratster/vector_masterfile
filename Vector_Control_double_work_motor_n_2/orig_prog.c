#include <stdint.h>
#include <Pipes.h>

inline uint64_t udiv32(uint64_t dividend, uint64_t divisor) 
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
		man = (udiv32((unsigned long int)mantissa_a,(unsigned long int)mantissa_b));

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

inline double rotor_flux_calc(double del_t, double Lm, double id, double flux_rotor_prev, double tau_new, double tau_r){

	double temp_a = 0, temp_b = 0, temp_c = 0;
	double temp_flux_n = 0,temp_flux_d = 0;
	double flux_rotor = 0;
	temp_a = del_t * Lm;
	temp_b = id * temp_a; 
	temp_c = tau_r * flux_rotor_prev;

	temp_flux_n = temp_c + temp_b;
	//temp_flux_d = (tau_new);

	flux_rotor = fdiv(temp_flux_n,tau_new);
	
	return(flux_rotor);
}

inline double omega_calc(double Lm, double iq, double tau_r, double flux_rotor){
	double temp_omega_n = 0,temp_omega_d = 0;
	double omega_r = 0;
	temp_omega_n = (Lm * iq);
	temp_omega_d = (tau_r * flux_rotor);
	omega_r = fdiv(temp_omega_n,temp_omega_d);
	return(omega_r);
}

double theta_calc(double omega_r, double omega_m, double del_t, double theta_prev){
	double temp_a = 0, temp_b = 0;
	double theta = 0;
	temp_a = (omega_r + omega_m);
	temp_b = temp_a * del_t;
	theta = theta_prev + temp_a;
	return(theta);
}

inline double iq_err_calc(double Lr, double torque_ref, double constant_1, double flux_rotor){

	double temp_d = 0;
	double temp_iq_n = 0,temp_iq_d = 0;
	double iq_err = 0;

	if (flux_rotor<0.001)
		flux_rotor = 0.001;
	else flux_rotor = flux_rotor;
	
	temp_d = (2 * Lr);
	temp_iq_n = temp_d * torque_ref;
	temp_iq_d = (constant_1 * flux_rotor);

	iq_err = fdiv((double)temp_iq_n,(double)temp_iq_d);
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
	double constant_1 = 3 *poles * Lm;

	
	while(1){
	
		//Read Data from motor
		id  = read_float64("in_data1");
		iq  = read_float64("in_data2");
		speed  = read_float64("in_data3");
		speed_ref  = read_float64("in_data4");	
		
		//Generation of Reference Values
		speed_err = speed_ref - speed;
		//Torque Reference Value Calculations
		int_speed_err = (int_speed_err + del_t*speed_err) * Ki;
		if (int_speed_err < -15)
			int_speed_err = -15;
		else if (int_speed_err > 15)
			int_speed_err = 15;
		else
			int_speed_err = int_speed_err;
	
		prop_speed_err = speed_err * Kp;
	
		torque_ref = int_speed_err + prop_speed_err;
		
		
		if (torque_ref < torque_sat_low)
			torque_ref = torque_sat_low;
		else if (torque_ref > torque_sat_high)
			torque_ref = torque_sat_high;
		else
			torque_ref = torque_ref;
		
		//Flux Reference Value Calculations
		if (speed_ref <=2000)
			flux_ref = nf;
		else if (speed_ref <=2500)
			flux_ref = (-0.0002*speed_ref + 1.4)*nf;
		else if (speed_ref <=3000)
			flux_ref = (-0.00036*speed_ref + 1.8)*nf;
		else 
			flux_ref = (-0.00042*speed_ref + 1.98)*nf;
	
		//Vector Control Begins Here
		tau_new= (del_t + tau_r);
		
		flux_rotor =  rotor_flux_calc( del_t,  Lm,  id,  flux_rotor_prev,  tau_new, tau_r);
		
		omega_r =  omega_calc( Lm,  iq,  tau_r,  flux_rotor);
		theta =  theta_calc( omega_r,  omega_m,  del_t,  theta_prev);

		iq_err = iq_err_calc( Lr,  torque_ref,  constant_1,  flux_rotor);
		
		//iD Calculations
		flux_err = flux_ref - flux_rotor;
		int_flux_err = (int_flux_err + del_t*flux_err) * Ki;
		
		if (int_flux_err < -1)
			int_flux_err = -1;
		else if (int_flux_err > 1)
			int_flux_err = 1;
		else
			int_flux_err = int_flux_err;
		
		prop_flux_err = flux_err * Kp;
		
		flux_add = int_flux_err + prop_flux_err;
		
		if (flux_add < -2)
			flux_add = -2;
		else if (flux_add > 2)
			flux_add = 2;
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
