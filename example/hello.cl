
//#include <CL\cl.h>
//#include <stdio.h>

__kernel void helloworld(int a, float b)
{
	if(0 == get_global_id(0) && 0 == get_global_id(1))
		printf("hello world from device, a=%d, b=%f\n", a, b);
}
__kernel void add_vector(global char *in, global char *out)
{
	int iNum = get_global_id(0);
	out[iNum] = in[iNum] + 1;
}
