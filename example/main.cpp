#include <stdlib.h>
#include <stdio.h>

//class A {
//public:
//	A() { printf("create A\n"); };
//	~A() { printf("destory A\n"); };
//};
//
//class B {
//public:
//	B() { printf("create B\n"); };
//	~B() { printf("destory B\n"); }
//};
//
//class C {
//public:
//	C() { printf("create C\n"); };
//	~C() { printf("destory C \n"); }
//};
//
//class D {
//public:
//	D() { printf("create D\n"); };
//	~D() { printf("destory D\n"); }
//};
//
//class Fm {
//public:
//	Fm() {
//		//a = new A;
//		//b = B();
//		//c = new C;
//		//d = new D;
//	};
//	~Fm()
//	{
//		//delete a;
//		//delete b;
//		//delete c;
//		//delete d;
//	}
//	void foo()
//	{
//		b = B();
//	}
//private:
//	A a;
//	B b;
//	C c;
//	D d;
//};

#include "../socl/socl.h"
int main()
{
	socl so;
	so.cl_init("../../example/hello.cl");

	cl::Kernel k1 = so.get_ocl_kernel("helloworld");

	int a = 10;
	float b = 3.33f;

	so.kernel_run(k1, cl::NDRange(1), cl::NDRange(1), a, b);

	printf("finish\n");
	return 0;
}