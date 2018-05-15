//
// Created by jiajuku on 17-12-28.
//
#include <iostream>
#include <CL/cl.h>
#include <map>

#include "socl.h"

int socl::cl_init(std::string kernel_file_path)
{
	cl_int err = CL_SUCCESS;

	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	if (platforms.size() == 0) {
		printf("Platform size 0\n");
		return -1;
	}
	platform = platforms[0];
	std::string platformName = platform.getInfo<CL_PLATFORM_VENDOR>();
	std::cout << "GPU vendor:" << platformName << std::endl;

	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
	if (devices.size() == 0) {
		printf("devices size 0\n");
		return -1;
	}
	device = devices[0];

	context = cl::Context(device);
	cmdq = cl::CommandQueue(context, device, 0, &err);  OCL_CHECK(err);

	// create and build program
	std::string source_code = ReadSourceFromFile(kernel_file_path);
	if (!source_code.empty())
	{
		program = cl::Program(context, source_code, CL_FALSE, &err); OCL_CHECK(err);
		err = program.build();
		if (err != CL_SUCCESS)
		{
			std::string builfInfo;
			builfInfo = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device, &err);
			std::cout << builfInfo << std::endl;
			assert(0);
			return err;
		}

		CreateKernels(program);
	}

	return 0;
}

cl::Kernel& socl::get_ocl_kernel(std::string kernel_name)
{
	if (kernel_map.empty())
	{
		std::cout << "there are not kernels, please check that if kernel file exits\n";
		return static_cast<cl::Kernel>(NULL);
	}

	if (kernel_map.find(kernel_name) != kernel_map.end())
		return kernel_map[kernel_name];

	std::cout << "no such kernel:" << kernel_name << std::endl;
	return static_cast<cl::Kernel>(NULL);
}

cl_int socl::SaveBinFile(const char * const fileName, const char * const binary)
{
	FILE *fid = NULL;
	fid = fopen(fileName, "wb");
	fwrite(binary, sizeof(char), strlen(binary), fid);
	fclose(fid);
	printf("save kernel bin file success\n");

	return 0;
}

std::string socl::ReadSourceFromFile(std::string filePath)
{
	if (filePath.empty())
	{
		std::cout << "on kernel file" << std::endl;
		return nullptr;
	}
	FILE * input = NULL;
	size_t size = 0, val;
	char* binary = NULL;
	std::string sourceString;

	input = fopen(filePath.c_str(), "rb");
	if (input == NULL)
	{
		printf("kernel file open fail!\n");
		assert(0);
	}
	fseek(input, 0L, SEEK_END);
	size = ftell(input);
	rewind(input);
	binary = (char*)malloc(size);
	if (binary == NULL)
	{
		assert(0);
	}
	val = fread(binary, sizeof(char), size, input);
	fclose(input);
	sourceString.assign(binary, size);
	free(binary);

	return sourceString;
}

cl_int socl::CreateKernels(cl::Program &program)
{
	cl_int state = CL_SUCCESS;

	std::vector<cl::Kernel> tmpKernel;
	program.createKernels(&tmpKernel);

	for (auto& kernel : tmpKernel) {
		auto kernelName = kernel.getInfo<CL_KERNEL_FUNCTION_NAME>();
		std::string myKernelName(kernelName.c_str());
		if (kernel_map.find(myKernelName) != kernel_map.end())
		{
			std::cout << "ERROR: the kernel :" + myKernelName + \
				"has existed, Please check files of kernels." \
				<< std::endl;
			return -1;
		}
		kernel_map.insert(std::make_pair(myKernelName, kernel));
	}
	return state;
}


