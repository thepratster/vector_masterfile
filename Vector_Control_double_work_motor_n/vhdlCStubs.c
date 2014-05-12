#include <vhdlCStubs.h>
float fdiv32(float a,float b)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call fdiv32 ");
append_int(buffer,2); ADD_SPACE__(buffer);
append_float(buffer,a); ADD_SPACE__(buffer);
append_float(buffer,b); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,32); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
float ret_val__ = get_float(buffer,&ss);
return(ret_val__);
}
float fpadd32f(float x,float y)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call fpadd32f ");
append_int(buffer,2); ADD_SPACE__(buffer);
append_float(buffer,x); ADD_SPACE__(buffer);
append_float(buffer,y); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,32); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
float ret_val__ = get_float(buffer,&ss);
return(ret_val__);
}
uint32_t fpadd32fi(uint32_t x,uint32_t y)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call fpadd32fi ");
append_int(buffer,2); ADD_SPACE__(buffer);
append_uint32_t(buffer,x); ADD_SPACE__(buffer);
append_uint32_t(buffer,y); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,32); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint32_t ret_val__ = get_uint32_t(buffer,&ss);
return(ret_val__);
}
float fpmul32f(float x,float y)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call fpmul32f ");
append_int(buffer,2); ADD_SPACE__(buffer);
append_float(buffer,x); ADD_SPACE__(buffer);
append_float(buffer,y); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,32); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
float ret_val__ = get_float(buffer,&ss);
return(ret_val__);
}
float fpsub32f(float x,float y)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call fpsub32f ");
append_int(buffer,2); ADD_SPACE__(buffer);
append_float(buffer,x); ADD_SPACE__(buffer);
append_float(buffer,y); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,32); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
float ret_val__ = get_float(buffer,&ss);
return(ret_val__);
}
uint32_t fpsub32fi(uint32_t x,uint32_t y)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call fpsub32fi ");
append_int(buffer,2); ADD_SPACE__(buffer);
append_uint32_t(buffer,x); ADD_SPACE__(buffer);
append_uint32_t(buffer,y); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,32); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint32_t ret_val__ = get_uint32_t(buffer,&ss);
return(ret_val__);
}
float iq_err_calc(float torque_ref,float flux_rotor)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call iq_err_calc ");
append_int(buffer,2); ADD_SPACE__(buffer);
append_float(buffer,torque_ref); ADD_SPACE__(buffer);
append_float(buffer,flux_rotor); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,32); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
float ret_val__ = get_float(buffer,&ss);
return(ret_val__);
}
float omega_calc(float iq,float flux_rotor)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call omega_calc ");
append_int(buffer,2); ADD_SPACE__(buffer);
append_float(buffer,iq); ADD_SPACE__(buffer);
append_float(buffer,flux_rotor); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,32); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
float ret_val__ = get_float(buffer,&ss);
return(ret_val__);
}
float rotor_flux_calc(float id,float flux_rotor_prev)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call rotor_flux_calc ");
append_int(buffer,2); ADD_SPACE__(buffer);
append_float(buffer,id); ADD_SPACE__(buffer);
append_float(buffer,flux_rotor_prev); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,32); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
float ret_val__ = get_float(buffer,&ss);
return(ret_val__);
}
float theta_calc(float omega_r,float omega_m,float theta_prev,float del_t)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call theta_calc ");
append_int(buffer,4); ADD_SPACE__(buffer);
append_float(buffer,omega_r); ADD_SPACE__(buffer);
append_float(buffer,omega_m); ADD_SPACE__(buffer);
append_float(buffer,theta_prev); ADD_SPACE__(buffer);
append_float(buffer,del_t); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,32); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
float ret_val__ = get_float(buffer,&ss);
return(ret_val__);
}
uint32_t udiv32(uint32_t dividend,uint32_t divisor)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call udiv32 ");
append_int(buffer,2); ADD_SPACE__(buffer);
append_uint32_t(buffer,dividend); ADD_SPACE__(buffer);
append_uint32_t(buffer,divisor); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,32); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint32_t ret_val__ = get_uint32_t(buffer,&ss);
return(ret_val__);
}
void vector_control_daemon()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call vector_control_daemon ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
