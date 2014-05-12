#include <stdio.h>
#include <stdint.h>
#include "prog.h"
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
