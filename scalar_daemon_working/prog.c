#include<stdint.h>
uint16_t fpadd32fi(uint16_t x, uint16_t y)
{
	return(x+y);
}

uint16_t fpsub32fi(uint16_t x, uint16_t y)
{
	return(x-y);
}

uint16_t udiv16(uint16_t dividend, uint16_t divisor) 
{
   uint16_t remainder = 0;
   uint16_t quotient = 0xffff;
   remainder = 0;
   if(divisor == 0)
   {
	return(0xffff);
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
		uint16_t curr_quotient = 1;
       		uint16_t dividend_by_2 = (dividend >> 1);
		uint16_t shifted_divisor = divisor;

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

void scalar_control(){

	uint16_t v_out = 0, f_out = 0;
	uint16_t control_speed = 0;
	int8_t slip_min = -10,slip_max=10;
	uint16_t f_max = 314;
	uint16_t v_max = 415;
	uint16_t f_min = 63;
	uint16_t v_min = 80;
	uint16_t poles = 4;
	int8_t error = 0;
	uint16_t speed = 0, speed_ref = 0;

	while(1)
	{
		speed_ref  = read_uint16("in_data");
		control_speed = speed_ref;
		if (control_speed > f_max){
			v_out = 10;
		 	f_out = 2222;
		}
		else if (control_speed > f_min){
		 	v_out = (65*control_speed)>>11;
		 	f_out = udiv16(22220,v_out);
		}
		else{
		 	v_out = 2; 
		 	f_out = 11111;
		}
		 
		write_uint16("out_data3",v_out);
		write_uint16("out_data4",f_out);

		 	 
	}
}
