#include <stdint.h>

uint64_t udiv64(uint64_t dividend, uint32_t divisor)
{
        uint64_t quotient = 0;
        if (divisor != 0)
        {
                while (dividend >= divisor)
                {
                        uint64_t curr_quotient = 1;
                        uint64_t shifted_divisor = divisor;
                        uint64_t reduced_dividend_by_2 = (dividend >> 1);
                        while (1)
                        {
                                if (shifted_divisor < reduced_dividend_by_2)
                                {
                                        shifted_divisor = (shifted_divisor << 1);
                                        curr_quotient = (curr_quotient << 1);
                                }
                                else
                                {
                                        break;
                                }
                        }
                        quotient += curr_quotient;
                        dividend -= shifted_divisor;
                }
        }
        return (quotient);
}

int64_t divideSigned(int64_t dividend, int32_t divisor)
{
        int8_t sign = 1;
        uint64_t udividend;
        uint32_t udivisor;
        if (dividend < 0)
        {
                sign = -sign;
                udividend = (uint64_t)(-dividend);
        }
        else
                udividend = (uint64_t) dividend;

        if (divisor < 0)
        {
                sign = -sign;
                udivisor = (uint32_t)(-divisor);
        }
        else
                udivisor = (uint32_t) divisor;

        int64_t quotient = 0;
        if (udivisor != 0)
        {
                quotient = divideUnsigned(udividend, udivisor);
                if (sign < 0)
                        quotient = -quotient;
        }
        return (quotient);
}
