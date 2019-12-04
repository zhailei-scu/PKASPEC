
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

__global__ void Kernel_Statistic();
__global__ void Kernel_Statistic() {
	int tid = threadIdx.y*blockDim.x + threadIdx.x;
	int bid = blockIdx.y*gridDim.x + blockIdx.x;
	int cid = bid * 1 + tid;




}

void Dev_Statistic() {
	Kernel_Statistic <<<1, 1 >>> ();
}