//
// Created by jiajuku on 17-12-28.
//

#ifndef NCNN_OCL_INIT_H
#define NCNN_OCL_INIT_H

#include <stdio.h>
#include <iostream>
#include <assert.h>

#include <CL/cl.hpp>

#define OCL_CHECK(err){ \
    if(err != CL_SUCCESS)\
    {\
        printf("error code:%d, %s, %d line\n", err,__FILE__, __LINE__);\
        assert(0);\
    }\
}

#define OCL_CHECK_KERNEL(err, idx){ \
    if(err != CL_SUCCESS)\
    {\
        printf("the %dth parameter error\n", idx);\
        assert(0);\
    }\
}


class socl
{
public:
	socl() {};
	~socl() {};

	cl::Platform    get_ocl_platform() { return platform; }
	cl::Device      get_ocl_device() { return device; }
	cl::Context     get_ocl_context() { return context; }
	cl::CommandQueue get_ocl_cmdq() { return cmdq; }

	int cl_init(std::string kernel_file_path);
	cl::Kernel& get_ocl_kernel(std::string kernel_name);

	template <typename... Types>
	void kernel_run(cl::Kernel kernel, cl::NDRange glb_item, cl::NDRange loc_item, Types... args)
	{
		int idx = 0;
		kernel_op(kernel, idx, args...);
		OCL_CHECK(get_ocl_cmdq().enqueueNDRangeKernel(kernel, cl::NullRange, glb_item, loc_item, NULL, NULL));
	}

private:
	cl_int CreateKernels(cl::Program &program);
	std::string ReadSourceFromFile(std::string filePath);
	cl_int SaveBinFile(const char * const fileName, const char * const binary);

	template <typename T, typename ...Types>
	void kernel_op(cl::Kernel kernel, int idx, T arg0, Types... args)
	{
		OCL_CHECK_KERNEL(kernel.setArg(idx, arg0), idx);
		kernel_op(kernel, ++idx, args...);
	}

	cl::Platform      platform;
	cl::Device        device;
	cl::Program       program;
	cl::Context       context;
	cl::CommandQueue  cmdq;
	std::map< const std::string, cl::Kernel>	kernel_map;
};

    //template <typename T>
    //void kernel_op(cl::Kernel kernel, int idx, T arg0){
    //    kernel.setArg(idx++, arg0);
    //    return;
    //}


	inline std::string preprocess(std::string name)
	{
		std::string pname = name;
		while (pname.find("/") != std::string::npos) {
			pname.replace(pname.find("/"), 1, "_");
		}

		return pname;
	}

	inline void check_mem(std::string filepath)
	{
		std::string file1 = filepath + "_cpu.txt";
		std::string file2 = filepath + "_ocl.txt";
	
		FILE *fid1 = fopen(file1.c_str(), "rb");
		FILE *fid2 = fopen(file2.c_str(), "rb");
	
		float data1, data2;
		int len1 = 0, len2 = 0;
		fscanf(fid1, "%d", &len1);
		fscanf(fid2, "%d", &len2);
		int i = 0;
		for (i = 0; i < len1; i++)
		{
			fscanf(fid1, "%f", &data1);
			fscanf(fid2, "%f", &data2);
	
			if (abs(data2 - data1) > 0.0001f)
			{
				if(data1 != -431602080.0f)
				printf("error %d: org = %f, ocl = %f\n", i, data1, data2);
				break;
			}
		}
	
		printf("%s\tfinish\n", filepath.c_str());
	}

#endif //NCNN_OCL_INIT_H
