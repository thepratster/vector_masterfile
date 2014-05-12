#include <stdint.h> // for uint32_t
#include <Pipes.h>
uint32_t fdiv(uint32_t x, uint32_t y)
{
	return(x-y);//Optmize by using shift and add...
}
