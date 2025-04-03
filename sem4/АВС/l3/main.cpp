#include <fstream>
#include <memory>
#define CL_HPP_ENABLE_EXCEPTIONS
#include <CL/cl.h>
#include <CL/opencl.hpp>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

const float A = 1.00001;
const float B = 40000.0;
const float STEP = 1.0;
const int SIZE = (B-A)/STEP;

const float EPS = 0.00001;


struct kernel_context {
	cl::Buffer x_d;
	cl::Buffer Y_d;
	cl::Buffer S_d;
	cl::Buffer iterations_d;
	cl::Buffer clocks_d;
};


cl::Context get_gpu_context(){
	cl_int res;
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	if(platforms.empty()) {
		std::cerr << "No OpenCL platforms" << '\n';
		exit(EXIT_FAILURE);
	}

	for(auto platform : platforms ) {
		std::vector<cl::Device> devices;
		platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
		for(auto device : devices){
			auto type = device.getInfo<CL_DEVICE_TYPE>();
			if(type == CL_DEVICE_TYPE_GPU) {
				std::cout << "Using platform: " 
					  << platform.getInfo<CL_PLATFORM_NAME>() << "\n";
				std::cout << "Using device: "
					  << device.getInfo<CL_DEVICE_NAME>() << "\n";
				return cl::Context{device};
			}
		}
	}

	std::cout << "No GPU was found in your configuration";
	std::exit(EXIT_FAILURE);
}


kernel_context data_transfer(const cl::Context& ctx,cl::CommandQueue& cq) {

	std::vector<double> x_vec(SIZE);
	std::vector<double> y_vec(SIZE);

	for(auto [i,x] = std::tuple{0,A};
		x < B && i < SIZE;
		x += STEP, i++) {
		x_vec[i] = x;
		y_vec[i] = std::log(x)/2.0;
	}

	cl::Buffer x_d(ctx, CL_MEM_READ_ONLY, sizeof(double)*SIZE);
	cl::Buffer y_d(ctx, CL_MEM_READ_ONLY, sizeof(double)*SIZE);
	cq.enqueueWriteBuffer(x_d, CL_TRUE, 0, sizeof(double)*SIZE, x_vec.data());
	cq.enqueueWriteBuffer(y_d, CL_TRUE, 0, sizeof(double)*SIZE, y_vec.data());


	cl::Buffer iterations_d(ctx, CL_MEM_READ_WRITE, sizeof(int)*SIZE);
	cl::Buffer S_d(ctx, CL_MEM_READ_WRITE, sizeof(double)*SIZE);
	cl::Buffer clocks_d(ctx, CL_MEM_READ_WRITE, sizeof(ulong)*SIZE);
	return kernel_context{
		.x_d = std::move(x_d),
		.Y_d = std::move(y_d),
		.S_d = std::move(S_d),
		.iterations_d = std::move(iterations_d),
		.clocks_d = std::move(clocks_d)
	};
}


/*
 * NOTE : Kernel uses AMD-specific way (gfx11) to get clock.
 *        It may not work if you have another AMD gpu.
 *        It won't work if you have non-AMD GPU.
 *
 * Sources : https://llvm.org/docs/AMDGPU/AMDGPUAsmGFX11.html
  *          https://www.amd.com/content/dam/amd/en/documents/radeon-tech-docs/instruction-set-architectures/rdna3-shader-instruction-set-architecture-feb-2023_0.pdf
 */
cl::Kernel make_kernel(const cl::Context& ctx, const kernel_context& k_ctx, const cl::Device& dev) {

	std::string SRC =
		R"opencl(


		ulong get_time()
		{
			ulong t = 0;
			__asm__ volatile (
				"s_sendmsg_rtn_b64 %0, sendmsg(MSG_RTN_GET_REALTIME); \n"
				"s_waitcnt lgkmcnt(0); \n"
				: "=r"(t)
			);
			return t;

			// uint t = 0;
			// __asm__ volatile("s_getreg_b32 %0, hwreg(29)" : "=r"(t));
			// return t;
		}

		void kernel calcS (global const double * x_b, global const double * y_b,
		                   global int * iters_b, global double * S_b,
				   global ulong * clocks_b) {

			int id = get_global_id(0);
			double x = x_b[id];
			double y = y_b[id];

			double EPS = ###EPS###;
			double x_mul = (x-1.0)/(x+1.0);
			double accum = (x-1.0)/(x+1.0);

			ulong start = get_time();
			int i = 1;
			while(fabs(accum - y) > EPS){
				x_mul *= (x-1.0)*(x-1.0)/(x+1.0)/(x+1.0);
				accum += x_mul /(2*i+1);
				i++;
			}
			ulong end = get_time();

			clocks_b[id] = end-start;
			iters_b[id] = i;
			S_b[id] = accum;
		}

		)opencl";
	SRC.replace(SRC.find("###EPS###"), sizeof("###EPS###")-1, std::to_string(EPS));

	cl::Program prg(ctx, SRC);
	try {
		prg.build(dev, "-save-temps");
	} catch(cl::Error& e) {
		std::string log = prg.getBuildInfo<CL_PROGRAM_BUILD_LOG>(dev);
		std::cout << "Build failed. Printing log...\n" << log;
		std::exit(EXIT_FAILURE);
	}


	cl::Kernel kernel_calcS{prg, "calcS"};

	kernel_calcS.setArg(0, k_ctx.x_d);
	kernel_calcS.setArg(1, k_ctx.Y_d);
	kernel_calcS.setArg(2, k_ctx.iterations_d);
	kernel_calcS.setArg(3, k_ctx.S_d);
	kernel_calcS.setArg(4, k_ctx.clocks_d);

	return kernel_calcS;
}





int main(){
	std::cout << "SIZE : " << SIZE << '\n';
	cl::Context ctx = get_gpu_context();
	auto cq = cl::CommandQueue{ctx, CL_QUEUE_PROFILING_ENABLE};

	kernel_context k_ctx = data_transfer(ctx, cq);
	cl::Kernel kernel = make_kernel(ctx, k_ctx, ctx.getInfo<CL_CONTEXT_DEVICES>()[0]);

	cq.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(SIZE));

	std::vector<ulong> clocks_buf(SIZE);
	std::vector<int> iters_buf(SIZE);
	std::vector<int> res_buf(SIZE);
	cq.enqueueReadBuffer(k_ctx.clocks_d, false , 0, SIZE * sizeof(double), clocks_buf.data());
	cq.enqueueReadBuffer(k_ctx.iterations_d, false , 0, SIZE * sizeof(int), iters_buf.data());
	cq.enqueueReadBuffer(k_ctx.S_d, false , 0, SIZE * sizeof(int), res_buf.data());
	cq.finish();


	std::ofstream of {"results.txt"};
	for(int i = 0; i < SIZE; ++i) {
		of << iters_buf[i] << " " << (double)clocks_buf[i] << " " << res_buf[i] << '\n';
	}
	of.close();

	std::exit(EXIT_SUCCESS);

}
