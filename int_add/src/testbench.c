#include <stdio.h>
#include <stdint.h>
#include <Pipes.h>
uint32_t fdiv(uint32_t, uint32_t);

int main(int argc, char* argv[])
{
  if(argc < 3)
  {
     fprintf(stderr, "%s <float> <uint32_t>\n", argv[0]);
     return(1);
  }
  	uint64_t h=1,i=1,j=1,k=1,l=1,d=1,e=1,f=1,g=1;
  uint32_t c = fdiv(atof(argv[1]), atof(argv[2]));
  /*	uint64_t	d = read_uint64("in_data1");
	uint64_t	e = read_uint64("in_data2");
	uint64_t	f = read_uint64("in_data3");
	uint64_t	g = read_uint64("in_data4");*/
  

  fprintf(stdout,"Result = %lu", c);
  return(0);
}

