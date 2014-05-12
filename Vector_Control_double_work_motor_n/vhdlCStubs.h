#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <Pipes.h>
#include <SocketLib.h>
float fdiv32(float a,float b);
float fpadd32f(float x,float y);
uint32_t fpadd32fi(uint32_t x,uint32_t y);
float fpmul32f(float x,float y);
float fpsub32f(float x,float y);
uint32_t fpsub32fi(uint32_t x,uint32_t y);
float iq_err_calc(float torque_ref,float flux_rotor);
float omega_calc(float iq,float flux_rotor);
float rotor_flux_calc(float id,float flux_rotor_prev);
float theta_calc(float omega_r,float omega_m,float theta_prev,float del_t);
uint32_t udiv32(uint32_t dividend,uint32_t divisor);
void vector_control_daemon();
