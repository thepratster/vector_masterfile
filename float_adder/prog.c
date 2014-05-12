uint64_t udiv32(uint64_t dividend, uint64_t divisor) 
{
   uint64_t remainder = 0;
   uint64_t quotient = 0xffffffffffffffff;
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
		uint64_t curr_quotient = 1;
       		uint64_t dividend_by_2 = (dividend >> 1);
		uint64_t shifted_divisor = divisor;

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


double fdiv(double a, double b)
{
	uint64_t mantissa_a, mantissa_b;
	uint64_t exponent_a, exponent_b;
	uint64_t sign_a, sign_b;
	uint64_t sign;
	uint64_t exp;
	uint64_t man;
	uint64_t ival_a, ival_b;
	uint64_t temp;
	double out_div;

	ival_a = *((uint64_t *)&a);
	ival_b = *((uint64_t *)&b);


	exponent_a = ((ival_a & (0x7FF0000000000000))>>52);
	exponent_b = ((ival_b & (0x7FF0000000000000))>>52);

	mantissa_a = ((ival_a & (0x000FFFFFFFFFFFFF)) | (0x0010000000000000))<<7;
	mantissa_b = ((ival_b & (0x000FFFFFFFFFFFFF)) | (0x0010000000000000))>>3;
	
	sign_a = (ival_a & (0x1000000000000000))>>63;
	sign_b = (ival_b & (0x1000000000000000))>>63;

	sign = (sign_a ^ sign_b)<<63;
	exp = exponent_a - exponent_b;
	man = (udiv32(mantissa_a,mantissa_b));

	temp = man;
	while( ( (temp & (0x0010000000000000)) != 0x0010000000000000) && (temp !=0) )
	{
	man = (man << 1);
	exp = exp - 1;
	temp = man;
	}

	man = ((man)& (0x000FFFFFFFFFFFFF));
	exp = (exp + 1036 ) <<52; // 128 + 7 + 8 -3

	temp = (sign | exp | man);

	out_div = *((double *)&temp); 

	return(out_div);
}
