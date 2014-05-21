#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthreadUtils.h>
#include <Pipes.h>
#include <pipeHandler.h>
#include "math.h"
#ifndef SW
#include "vhdlCStubs.h"
#endif

void scalar_control();

void im_zep(double *iq, double *iq_prev, double *id, double *id_prev, double *flq, double *flq_prev, double *fld, double *fld_prev, double *spd_prev, double vd,double vq,double *torque,double load_torque,double *time, double *spd, double *theta, double *theta_prev, double *theta_sin, double *theta_cos, double *f_omega)   
{

	double	alpha = 0,
		beta = 0,
		sigma = 0,
		mu = 0,
		gamma = 0,
		omega = 314.15,
		inertia = 0.013,
		rs = 4.9,
		rr = 8.1,
		lls = 0.03215,
		llr = 0.03215,
		ls = 0,
		lr = 0,
		lm = 0.8096,
		poles = 4,
		cont = 0,
		time_period = 25e-9;
                                                   
	double k1 = 0,k2 = 0,k3 = 0,k4 = 0;
	double l1 = 0,l2 = 0,l3 = 0,l4 = 0;
	double m1 = 0,m2 = 0,m3 = 0,m4 = 0;
	double n1 = 0,n2 = 0,n3 = 0,n4 = 0;
	double o1 = 0,o2 = 0,o3 = 0,o4 = 0;
	double delta = 0;
	double f_rotor = 0, omega_r = 0;
	
	ls = lm + lls;
	lr = lm + llr;
	alpha = rr/lr;
	sigma = ls - lm*lm/lr;
	beta = lm/(sigma*lr);
	mu = (3/2)*(poles/2)*(lm/(inertia*lr));
	gamma = lm*lm*rr/(sigma*lr*lr)+rs/sigma;
	cont = (3*poles*lm/(4*lr));
	
        if ( 0.0 == *time ) {           
                  
		*iq_prev = 0.0;
		*id_prev = 0.0;
		*flq_prev = 0.0;
		*fld_prev = 0.0;
		*spd_prev = 0.0;
		*iq = 0.0;
		*id = 0.0;
		*flq = 0.0;
		*fld = 0.0;
		*spd = 0.0;
		*torque = 0.0;
		*iq_prev = *iq;
		*id_prev = *id;
		*flq_prev = *flq;
		*fld_prev = *fld;
		*spd_prev = *spd;
		delta = time_period;
		*time = *time + delta;
		*theta = 0;
		*theta_prev = 0;
		*theta_sin = 0;
		*theta_cos = 0;
		omega = *f_omega;
  
        }
        else {       
		*iq_prev = *iq;
		*id_prev = *id;
		delta = time_period;
		omega = *f_omega;

		k1 = -gamma*(*iq_prev) - omega*(*id_prev) + alpha*beta*(*flq_prev) - beta*(*spd_prev)*(*fld_prev) + (vq)/sigma;
		l1 = omega*(*iq_prev) - gamma*(*id_prev) + beta*(*spd_prev)*(*flq_prev) + alpha*beta*(*fld_prev) + (vd)/sigma;
		m1 = alpha*lm*(*iq_prev) - alpha*(*flq_prev) - (omega-(*spd_prev))*(*fld_prev);
		n1 = alpha*lm*(*id_prev) + (omega-(*spd_prev))*(*flq_prev) - alpha*(*fld_prev);
		o1 = ((cont*((*fld_prev)*(*iq_prev) - (*flq_prev)*(*id_prev)))-load_torque)/inertia;

		k2 = -gamma*((*iq_prev) + delta/2*k1) - omega*((*id_prev)+delta/2*l1) + alpha*beta*((*flq_prev)+delta/2*m1) - beta*((*spd_prev)+delta/2*o1)*((*fld_prev)+delta/2*n1) + (vq)/sigma;
		l2 = omega*((*iq_prev) + delta/2*k1) - gamma*((*id_prev) + delta/2*l1) + beta*((*spd_prev) + delta/2*o1)*((*flq_prev) + delta/2*m1) + alpha*beta*((*fld_prev) + delta/2*n1) + (vd)/sigma;
		m2 = alpha*lm*((*iq_prev) + delta/2*k1) - alpha*((*flq_prev) + delta/2*m1) - (omega-((*spd_prev) + delta/2*o1))*((*fld_prev) + delta/2*n1);
		n2 = alpha*lm*((*id_prev) + delta/2*l1) + (omega-((*spd_prev) + delta/2*o1))*((*flq_prev) + delta/2*m1) - alpha*((*fld_prev) + delta/2*n1);
		o2 = ((cont*(((*fld_prev) + delta/2*n1)*((*iq_prev) + delta/2*k1) - ((*flq_prev) + delta/2*m1)*((*id_prev) + delta/2*l1)))-load_torque)/inertia;

		k3 = -gamma*((*iq_prev) + delta/2*k2) - omega*((*id_prev)+delta/2*l2) + alpha*beta*((*flq_prev)+delta/2*m2) - beta*((*spd_prev)+delta/2*o2)*((*fld_prev)+delta/2*n2) + (vq)/sigma;
		l3 = omega*((*iq_prev) + delta/2*k2) - gamma*((*id_prev) + delta/2*l2) + beta*((*spd_prev) + delta/2*o2)*((*flq_prev) + delta/2*m2) + alpha*beta*((*fld_prev) + delta/2*n2) + (vd)/sigma;
		m3 = alpha*lm*((*iq_prev) + delta/2*k2) - alpha*((*flq_prev) + delta/2*m2) - (omega-((*spd_prev) + delta/2*o2))*((*fld_prev) + delta/2*n2);
		n3 = alpha*lm*((*id_prev) + delta/2*l2) + (omega-((*spd_prev) + delta/2*o2))*((*flq_prev) + delta/2*m2) - alpha*((*fld_prev) + delta/2*n2);
		o3 = ((cont*(((*fld_prev) + delta/2*n2)*((*iq_prev) + delta/2*k2) - ((*flq_prev) + delta/2*m2)*((*id_prev) + delta/2*l2)))-load_torque)/inertia;

		k4 = -gamma*((*iq_prev) + delta*k3) - omega*((*id_prev)+delta*l3) + alpha*beta*((*flq_prev)+delta*m3) - beta*((*spd_prev)+delta*o3)*((*fld_prev)+delta*n3) + (vq)/sigma;
		l4 = omega*((*iq_prev) + delta*k3) - gamma*((*id_prev) + delta*l3) + beta*((*spd_prev) + delta*o3)*((*flq_prev) + delta*m3) + alpha*beta*((*fld_prev) + delta*n3) + (vd)/sigma;
		m4 = alpha*lm*((*iq_prev) + delta*k3) - alpha*((*flq_prev) + delta*m3) - (omega-((*spd_prev) + delta*o3))*((*fld_prev) + delta*n3);
		n4 = alpha*lm*((*id_prev) + delta*l3) + (omega-((*spd_prev) + delta*o3))*((*flq_prev) + delta*m3) - alpha*((*fld_prev) + delta*n3);
		o4 = ((cont*(((*fld_prev) + delta*n3)*((*iq_prev) + delta*k3) - ((*flq_prev) + delta*m3)*((*id_prev) + delta*l3)))-load_torque)/inertia;

		*iq = (*iq_prev) + delta*(k1 + 2*k2 + 2*k3 + k4)/6;
		*id = (*id_prev) + delta*(l1 + 2*l2 + 2*l3 + l4)/6;
		*flq = (*flq_prev) + delta*(m1 + 2*m2 + 2*m3 + m4)/6;
		*fld = (*fld_prev) + delta*(n1 + 2*n2 + 2*n3 + n4)/6;
		*spd = (*spd_prev) + delta*(o1 + 2*o2 + 2*o3 + o4)/6;
		*torque = cont*((*iq)*(*fld) - (*id)*(*flq));

			
		
		*theta = *theta_prev + (+omega+*spd) * delta;
		
		if(*flq == 0){
			*theta_sin = 0;
			*theta_cos = 0;
		//	*theta = 0;
		}
		else{
			*theta_cos = acos((*fld)/(f_rotor));
			*theta_sin = asin((*flq)/(f_rotor));
		//	*theta = atan((*flq)/((*fld)));
		}
		
		//if(*fld == 0)
		//	*theta_cos = 0;
		//else
			
		
		
		
		*time = *time + delta;
		//*iq_prev = *iq;
		//*id_prev = *id;
		*flq_prev = *flq;
		*fld_prev = *fld;
		*spd_prev = *spd;
		//*theta_prev_prev = *theta_prev;
		*theta_cos = *theta;*theta_sin = *theta;
		*theta_prev = *theta;
	}
}


#ifdef SW
DEFINE_THREAD(scalar_control)
#endif

int main(int argc, char* argv[]){

	#ifdef SW
	init_pipe_handler();
	PTHREAD_DECL(scalar_control);
	PTHREAD_CREATE(scalar_control);
	#endif
	
	char buffer[BUFSIZ];
	char filename_id[] 	= "/home/pratik/Files/MTP/ahir-master/MTP_Project/Vector_Control_double_work/id1.m";
	char filename_id_err[] 	= "/home/pratik/Files/MTP/ahir-master/MTP_Project/Vector_Control_double_work/id_err1.m";
	char filename_iq[] 	= "/home/pratik/Files/MTP/ahir-master/MTP_Project/Vector_Control_double_work/iq1.m";
	char filename_iq_err[] 	= "/home/pratik/Files/MTP/ahir-master/MTP_Project/Vector_Control_double_work/iq_err1.m";
	char filename_f_ref[] 	= "/home/pratik/Files/MTP/ahir-master/MTP_Project/Vector_Control_double_work/f_ref1.m";
	char filename_t_ref[] 	= "/home/pratik/Files/MTP/ahir-master/MTP_Project/Vector_Control_double_work/t_ref1.m";
	char filename_time[] 	= "/home/pratik/Files/MTP/ahir-master/MTP_Project/Vector_Control_double_work/time1.m";
	char filename_spd[] 	= "/home/pratik/Files/MTP/ahir-master/MTP_Project/Vector_Control_double_work/spd1.m";	
	char filename_spd_ref[] = "/home/pratik/Files/MTP/ahir-master/MTP_Project/Vector_Control_double_work/spd_ref1.m";
	char filename_vd[] 	= "/home/pratik/Files/MTP/ahir-master/MTP_Project/Vector_Control_double_work/vd1.m";
	char filename_vq[] 	= "/home/pratik/Files/MTP/ahir-master/MTP_Project/Vector_Control_double_work/vq1.m";
	char filename_theta[] 	= "/home/pratik/Files/MTP/ahir-master/MTP_Project/Vector_Control_double_work/theta1.m";
	char filename_torque[] 	= "/home/pratik/Files/MTP/ahir-master/MTP_Project/Vector_Control_double_work/torque1.m";
	char filename_fld[] 	= "/home/pratik/Files/MTP/ahir-master/MTP_Project/Vector_Control_double_work/fld1.m";
	char filename_flq[] 	= "/home/pratik/Files/MTP/ahir-master/MTP_Project/Vector_Control_double_work/flq1.m";
	char filename_test[]	= "/home/pratik/Files/MTP/ahir-master/MTP_Project/Vector_Control_double_work/test.txt";	

	FILE *fp_id = NULL;
	FILE *fp_id_err = NULL;
	FILE *fp_iq = NULL;
	FILE *fp_iq_err = NULL;
	FILE *fp_f_ref = NULL;
	FILE *fp_t_ref = NULL;
	FILE *fp_time = NULL;
	FILE *fp_spd = NULL;
	FILE *fp_spd_ref = NULL;
	FILE *fp_vd = NULL;
	FILE *fp_vq = NULL;
	FILE *fp_theta = NULL;
	FILE *fp_torque = NULL;
	FILE *fp_fld = NULL;
	FILE *fp_flq = NULL;
	
	FILE *fp_test = NULL;
	
	fp_test = fopen(filename_test, "w"); 
	if(fp_test == NULL) {
		printf("Failed to open file for writing\n");
	}
	fputs("test_ref Values Generated\n", fp_test); 
	fclose(fp_test);
	
	fp_id = fopen(filename_id, "w"); 
	if(fp_id == NULL) {
		printf("Failed to open file for writing\n");
	}
	fputs("id = [ ", fp_id); 
	fclose(fp_id);
	
	fp_id_err = fopen(filename_id_err, "w"); 
	if(fp_id_err == NULL) {
		printf("Failed to open file for writing\n");
	}
	fputs("id_err = [ ", fp_id_err); 
	fclose(fp_id_err);
	
	fp_iq = fopen(filename_iq, "w"); 
	if(fp_iq == NULL) {
		printf("Failed to open file for writing\n");
	}
	fputs("iq = [ ", fp_iq); 
	fclose(fp_iq);
	
	fp_iq_err = fopen(filename_iq_err, "w"); 
	if(fp_iq_err == NULL) {
		printf("Failed to open file for writing\n");
	}
	fputs("iq_err = [ ", fp_iq_err); 
	fclose(fp_iq_err);
	
	fp_f_ref = fopen(filename_f_ref, "w"); 
	if(fp_f_ref == NULL) {
		printf("Failed to open file for writing\n");
	}
	fputs("f_ref = [ ", fp_f_ref); 
	fclose(fp_f_ref);
	
	fp_t_ref = fopen(filename_t_ref, "w"); 
	if(fp_t_ref == NULL) {
		printf("Failed to open file for writing\n");
	}
	fputs("t_ref = [ ", fp_t_ref); 
	fclose(fp_t_ref);
	
	fp_time = fopen(filename_time, "w"); 
	if(fp_time == NULL) {
		printf("Failed to open file for writing\n");
	}
	fputs("time = [ ", fp_time); 
	fclose(fp_time);
	
	fp_spd = fopen(filename_spd, "w"); 
	if(fp_spd == NULL) {
		printf("Failed to open file for writing\n");
	}
	fputs("spd = [ ", fp_spd); 
	fclose(fp_spd);
	
	fp_spd_ref = fopen(filename_spd_ref, "w"); 
	if(fp_spd_ref == NULL) {
		printf("Failed to open file for writing\n");
	}
	fputs("spd_ref = [ ", fp_spd_ref); 
	fclose(fp_spd_ref);
	
	fp_vd = fopen(filename_vd, "w"); 
	if(fp_vd == NULL) {
		printf("Failed to open file for writing\n");
	}
	fputs("vd = [ ", fp_vd); 
	fclose(fp_vd);
	
	fp_vq = fopen(filename_vq, "w"); 
	if(fp_vq == NULL) {
		printf("Failed to open file for writing\n");
	}
	fputs("vq = [ ", fp_vq); 
	fclose(fp_vq);
	
	fp_theta = fopen(filename_theta, "w"); 
	if(fp_theta == NULL) {
		printf("Failed to open file for writing\n");
	}
	fputs("theta = [ ", fp_theta); 
	fclose(fp_theta);
	
	fp_torque = fopen(filename_torque, "w"); 
	if(fp_torque == NULL) {
		printf("Failed to open file for writing\n");
	}
	fputs("torque = [ ", fp_torque); 
	fclose(fp_torque);
	
	fp_fld = fopen(filename_fld, "w"); 
	if(fp_fld == NULL) {
		printf("Failed to open file for writing\n");
	}
	fputs("fld = [ ", fp_fld); 
	fclose(fp_fld);
	
	fp_flq = fopen(filename_flq, "w"); 
	if(fp_flq == NULL) {
		printf("Failed to open file for writing\n");
	}
	fputs("flq = [ ", fp_flq); 
	fclose(fp_flq);

	double id_err = 0,iq_err = 0, theta = 0, speed = 0;
	double voltage_iteration = 150;
	double tol_error = 0.1;
	double t_ref = 0,f_ref = 0;
	double	iq = 0, 
	iq_prev = 0,
	id =0,
	id_prev = 0,
	flq = 0,
	flq_prev = 0,
	fld = 0,
	fld_prev = 0,
	spd=0,
	spd_prev = 0,
	vd = 0,
	vq = 0,
	torque = 0,
	load_torque = 0,
	time = 0;
	double va=0, vb=0, vc=0;
	double valpha =0, vbeta=0;
	float iq_float,id_float,spd_float,spd_ref_float,omega_m_float, temp;
	float id_err_float,iq_err_float,t_ref_float,f_ref_float;
	double spd_ref = 157;
	int i=0,count = 0;
	double theta_m = 0, theta_prev =0,theta_sin =0,theta_cos =0,theta_motor =0;
	double v_alpha =0, v_beta =0;
	double i_alpha =0, i_beta =0 , va_prev = 0, vc_prev = 0, vb_prev = 0;
	double i_alpha_err =0, i_beta_err =0;
	double ia = 0, ib=0, ic =0;
	double ia_err = 0, ib_err=0, ic_err =0;
	double link_voltage = 415;
	int no_of_cycles = 5000; // 50u/500n (Ideal time necessary for conputation for FPGA/Motor_iteration_step)
	uint32_t temp_var = 0,temp_var1 = 0;

	float id_in = 0, iq_in = 0, ialpha =0, ibeta =0;
	double f_omega = 0;
	
	uint16_t spd_int =0, spd_ref_int =0;
	uint8_t gate_pulses = 0;
	
	int8_t temp_1 = 0;
	uint16_t v_out = 0, f_out = 0, temp_4 = 0;
	uint8_t count_tri = 0;
	uint16_t count_sin = 0, r = 0, y = 240, b = 120;
	int16_t a = 0;
	uint8_t t_var = 0, t_b = 0, t_r = 0, t_y = 0;
	int8_t sin_r = 0, sin_y = 0, sin_b = 0;
	int8_t sin_table[360] = {  0 ,  2 ,  3 ,  5 ,  7 ,  9 , 10 , 12 , 14 , 16 , 17 , 19 , 21 , 22 , 24 , 26 , 28 , 29 , 31 , 33 , 34 , 36 , 37 , 39 , 41 , 42 , 44 , 45 , 47 , 48 , 50 , 52 , 53 , 54 , 56 , 57 , 59 , 60 , 62 , 63 , 64 , 66 , 67 , 68 , 69 , 71 , 72 , 73 , 74 , 75 , 77 , 78 , 79 , 80 , 81 , 82 , 83 , 84 , 85 , 86 , 87 , 87 , 88 , 89 , 90 , 91 , 91 , 92 , 93 , 93 , 94 , 95 , 95 , 96 , 96 , 97 , 97 , 97 , 98 , 98 , 98 , 99 , 99 , 99 , 99 ,100 ,100 ,100 ,100 ,100 ,100 ,100 ,100 ,100 ,100 ,100 , 99 , 99 , 99 , 99 , 98 , 98 , 98 , 97 , 97 , 97 , 96 , 96 , 95 , 95 , 94 , 93 , 93 , 92 , 91 , 91 , 90 , 89 , 88 , 87 , 87 , 86 , 85 , 84 , 83 , 82 , 81 , 80 , 79 , 78 , 77 , 75 , 74 , 73 , 72 , 71 , 69 , 68 , 67 , 66 , 64 , 63 , 62 , 60 , 59 , 57 , 56 , 54 , 53 , 52 , 50 , 48 , 47 , 45 , 44 , 42 , 41 , 39 , 37 , 36 , 34 , 33 , 31 , 29 , 28 , 26 , 24 , 22 , 21 , 19 , 17 , 16 , 14 , 12 , 10 ,  9 ,  7 ,  5 ,  3 ,  2 , -0 , -2 , -3 , -5 , -7 , -9 ,-10 ,-12 ,-14 ,-16 ,-17 ,-19 ,-21 ,-22 ,-24 ,-26 ,-28 ,-29 ,-31 ,-33 ,-34 ,-36 ,-37 ,-39 ,-41 ,-42 ,-44 ,-45 ,-47 ,-48 ,-50 ,-52 ,-53 ,-54 ,-56 ,-57 ,-59 ,-60 ,-62 ,-63 ,-64 ,-66 ,-67 ,-68 ,-69 ,-71 ,-72 ,-73 ,-74 ,-75 ,-77 ,-78 ,-79 ,-80 ,-81 ,-82 ,-83 ,-84 ,-85 ,-86 ,-87 ,-87 ,-88 ,-89 ,-90 ,-91 ,-91 ,-92 ,-93 ,-93 ,-94 ,-95 ,-95 ,-96 ,-96 ,-97 ,-97 ,-97 ,-98 ,-98 ,-98 ,-99 ,-99 ,-99 ,-99 ,-100 ,-100 ,-100 ,-100 ,-100 ,-100 ,-100 ,-100 ,-100 ,-100 ,-100 ,-99 ,-99 ,-99 ,-99 ,-98 ,-98 ,-98 ,-97 ,-97 ,-97 ,-96 ,-96 ,-95 ,-95 ,-94 ,-93 ,-93 ,-92 ,-91 ,-91 ,-90 ,-89 ,-88 ,-87 ,-87 ,-86 ,-85 ,-84 ,-83 ,-82 ,-81 ,-80 ,-79 ,-78 ,-77 ,-75 ,-74 ,-73 ,-72 ,-71 ,-69 ,-68 ,-67 ,-66 ,-64 ,-63 ,-62 ,-60 ,-59 ,-57 ,-56 ,-54 ,-53 ,-52 ,-50 ,-48 ,-47 ,-45 ,-44 ,-42 ,-41 ,-39 ,-37 ,-36 ,-34 ,-33 ,-31 ,-29 ,-28 ,-26 ,-24 ,-22 ,-21 ,-19 ,-17 ,-16 ,-14 ,-12 ,-10 , -9 , -7 , -5 , -3 , -2 };
	
	while(time < 5){
		
		spd_int = spd;
		spd_ref_int = spd_ref;
		
		//write_uint16("in_data1",spd_int);
		write_uint16("in_data",spd_ref_int);
		
		v_out = read_uint16("out_data3");
		f_out = read_uint16("out_data4");
		
		//v_out = 5;
		//f_out = 4444;
		
		f_omega = v_out*31.415;
				
		for(i = 0; i< no_of_cycles; i++){
			im_zep(&iq,&iq_prev,&id,&id_prev,&flq,&flq_prev,&fld,&fld_prev,&spd_prev,vd,vq,&torque,load_torque,&time,&spd,&theta_m,&theta_prev,&theta_sin,&theta_cos,&f_omega);	
			 
		if (count_tri >= 2)
			count_tri = 0;
		else
			count_tri = count_tri + 1;
			

		if (count_tri >= 2){
			if (a >= 1000)
		 		t_var = 0;
		 	else if (a <= -1000)
		 		t_var = 1;
		 		
		 	if (t_var == 0)
		 		a = a-1;
		 	else 
		 		a = a+1;
		 }
		 
		if (count_sin >= f_out){
			count_sin = 0;
			if (r==359)
				r = 0;
			else
				r = (r+1);
				
			if (y==359)
				y = 0;
			else
				y = (y+1);
				
			if (b==359)
				b = 0;
			else
				b = (b+1);
		
		}
		else
			count_sin = count_sin + 1;
			
		sin_r = sin_table[r];
		sin_y = sin_table[y];
		sin_b = sin_table[b];
		

		if (v_out*sin_r > a)
			t_r = '4';
		else
			t_r = '0';
		
		if (v_out*sin_y > a)
			t_y = '2';
		else
			t_y = '0';
		
		if (v_out*sin_b > a)
			t_b = '1';
		else
			t_b = '0';
			
		gate_pulses = t_r + t_y + t_b;
		
		
		t_r = gate_pulses & (0x04);
		t_y = gate_pulses & (0x02);
		t_b = gate_pulses & (0x01);
		
		if (t_r)
			va = link_voltage;
		else  
			va = 0;
		
		if (t_y)
			vb = link_voltage;
		else  
			vb = 0;
		
		if (t_b)
			vc = link_voltage;
		else  
			vc = 0;
		
		/*
		va = 0.5*338.8460811 * sin(157 * time);
		vb = 0.5*338.8460811 * sin((157 * time)- 2.094333);
		vc = 0.5*338.8460811 * sin((157 * time)+ 2.094333);*/
		
		valpha = 0.666*(va - (vb + vc)*0.5);
          	vbeta = 0.666*(0.866*(vb - vc));
          
          	theta_motor = theta_motor + f_omega* 25e-9;
          	
          	vd = valpha * cos(theta_motor) + vbeta * sin(theta_motor);  
          	vq = -valpha * sin(theta_motor) + vbeta * cos(theta_motor);
          	
          	}
		
		if (load_torque == 0)
			load_torque=0.005;
		else
			load_torque=load_torque;
			
		if (time>1){
			load_torque = 0;
		}
		if (time>2){
			spd_ref = 500;
		}
		if (time>3){
			spd_ref = 100;			
		}
		if (time>4){
			spd_ref = 200;			
		}		
		if (time>5){
			load_torque = 0;			
		}
		if (time>6){
			spd_ref = 900;
		}
		if (time>7){
			load_torque = 5;			
		}
		if (time>8){
			spd_ref = 1610;			
		}
		if (time>9){
			spd_ref = 1400;
		}		
		if (time>10){
			load_torque = 0;			
		}
		
	

		fp_id = fopen(filename_id, "a"); // open file for appending !!!
		fprintf(fp_id," %20.18f ",id);
		fclose(fp_id);
		
		fp_id_err = fopen(filename_id_err, "a"); // open file for appending !!! 
		fprintf(fp_id_err," %d ",sin_r);
		fclose(fp_id_err);
		
		fp_iq_err = fopen(filename_iq_err, "a"); // open file for appending !!! 
		fprintf(fp_iq_err," %d ",a);
		fclose(fp_iq_err);
		
		fp_f_ref = fopen(filename_f_ref, "a"); // open file for appending !!! 
		fprintf(fp_f_ref," %20.18f ",vd);
		fclose(fp_f_ref);
		
		fp_t_ref = fopen(filename_t_ref, "a"); // open file for appending !!! 
		fprintf(fp_t_ref," %20.18f ",vq);
		fclose(fp_t_ref);
		
		fp_spd_ref = fopen(filename_spd_ref, "a"); // open file for appending !!! 
		fprintf(fp_spd_ref," %d ",v_out);
		fclose(fp_spd_ref);
		
		fp_time = fopen(filename_time, "a"); // open file for appending !!! 
		fprintf(fp_time," %20.18f ",time);
		fclose(fp_time);
		
		fp_theta = fopen(filename_theta, "a"); // open file for appending !!! 
		fprintf(fp_theta," %20.18f ",theta_sin);
		fclose(fp_theta);
		
		fp_torque = fopen(filename_torque, "a"); // open file for appending !!! 
		fprintf(fp_torque," %20.18f ",torque);
		fclose(fp_torque);
		
		fp_vd = fopen(filename_vd, "a"); // open file for appending !!! 
		fprintf(fp_vd," %20.18f ",vd);
		fclose(fp_vd);
		
		fp_vq = fopen(filename_vq, "a"); // open file for appending !!! 
		fprintf(fp_vq," %20.18f ",vq);
		fclose(fp_vq);
		
		fp_fld = fopen(filename_fld, "a"); // open file for appending !!! 
		fprintf(fp_fld," %d ",f_out);
		fclose(fp_fld);
		
		fp_flq = fopen(filename_flq, "a"); // open file for appending !!! 
		fprintf(fp_flq," %20.18f ",va);
		fclose(fp_flq);
		
		fp_iq = fopen(filename_iq, "a"); // open file for appending !!! 
		fprintf(fp_iq," %20.18f ",iq);
		fclose(fp_iq);
		
		fp_spd = fopen(filename_spd, "a"); // open file for appending !!! 
		fprintf(fp_spd," %20.18f ",spd);
		fclose(fp_spd);
		
		fp_test = fopen(filename_test, "a"); // open file for appending !!! 
		if(fp_test == NULL)
		{
			printf("Failed to open file for appending\n");
		}
		fprintf(fp_test,"-------------------\n");
		fprintf(fp_test,"id = %20.18f id_err=%20.18f iq = %20.18f iq_err=%20.18f speed=%20.18f torque=%20.18f id_prev=%20.18f time = %20.33f vd = %20.18f vq = %20.18f \n t_ref = %20.18f \n f_ref = %20.18f \n", id,id_err,iq,iq_err,((spd*60)/6.28),torque,id_prev, time,vd,vq, t_ref, f_ref); // write a formatted string to file 
		fprintf(fp_test,"-------------------\n");
		fclose(fp_test);
		
		//if (id_err>-2)
			fprintf(stdout," ------------------------- \n 1 = %20.45f \n 2 = %20.18f \n ", spd,time);
		
		//fprintf(stdout," ------------------------- \n 1 = %20.45f \n 2 = %20.18f \n ", id_err,iq_err); 

	}
	
	fp_id = fopen(filename_id, "a"); // open file for appending !!! 
	fprintf(fp_id," ]; ");
	fclose(fp_id);

	fp_id_err = fopen(filename_id_err, "a"); // open file for appending !!! 
	fprintf(fp_id_err," ]; ");
	fclose(fp_id_err);

	fp_iq_err = fopen(filename_iq_err, "a"); // open file for appending !!! 
	fprintf(fp_iq_err," ]; ");
	fclose(fp_iq_err);

	fp_f_ref = fopen(filename_f_ref, "a"); // open file for appending !!! 
	fprintf(fp_f_ref," ]; ");
	fclose(fp_f_ref);

	fp_t_ref = fopen(filename_t_ref, "a"); // open file for appending !!! 
	fprintf(fp_t_ref," ]; ");
	fclose(fp_t_ref);

	fp_spd_ref = fopen(filename_spd_ref, "a"); // open file for appending !!! 
	fprintf(fp_spd_ref," ]; ");
	fclose(fp_spd_ref);

	fp_time = fopen(filename_time, "a"); // open file for appending !!! 
	fprintf(fp_time," ]; ");
	fclose(fp_time);

	fp_theta = fopen(filename_theta, "a"); // open file for appending !!! 
	fprintf(fp_theta," ]; ");
	fclose(fp_theta);

	fp_torque = fopen(filename_torque, "a"); // open file for appending !!! 
	fprintf(fp_torque," ]; ");
	fclose(fp_torque);

	fp_vd = fopen(filename_vd, "a"); // open file for appending !!! 
	fprintf(fp_vd," ]; ");
	fclose(fp_vd);

	fp_vq = fopen(filename_vq, "a"); // open file for appending !!! 
	fprintf(fp_vq," ]; ");
	fclose(fp_vq);

	fp_fld = fopen(filename_fld, "a"); // open file for appending !!! 
	fprintf(fp_fld," ]; ");
	fclose(fp_fld);

	fp_flq = fopen(filename_flq, "a"); // open file for appending !!! 
	fprintf(fp_flq," ]; ");
	fclose(fp_flq);

	fp_iq = fopen(filename_iq, "a"); // open file for appending !!! 
	fprintf(fp_iq," ]; ");
	fclose(fp_iq);

	fp_spd = fopen(filename_spd, "a"); // open file for appending !!! 
	fprintf(fp_spd," ]; ");
	fclose(fp_spd);
	

	#ifdef SW
	close_pipe_handler();
	PTHREAD_CANCEL(scalar_control);
#endif
    return 0;
}

