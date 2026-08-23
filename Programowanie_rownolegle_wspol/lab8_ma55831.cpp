#include <CL/cl.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <chrono>
#include <ctime>
#include <functional>
int main() {
    using namespace std::chrono;
     srand(time(nullptr));
    for (unsigned int N=64;N<=4096;N*=2){
    std::cout<<"macierz o rozmiarze "<<N<<"x"<<N<<std::endl;

    unsigned int K= N;
    unsigned int M=N;
    std::vector<int> A(M * K, 1);
    std::vector<int> B(K * N, 1);
    std::vector<int> C(M * N, 0);

    cl_int err;
    cl_platform_id platform;
    clGetPlatformIDs(1, &platform, nullptr);

    cl_device_id device;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);

    cl_context context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &err);
    cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, nullptr, &err);

 
    cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,sizeof(int) * M * K, A.data(), &err);
    cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * K * N, B.data(), &err);
    cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY,sizeof(int) * M * N, nullptr, &err);

   
    const auto start{steady_clock::now()};
    const char* kernelSource = R"CLC(
    __kernel void matrix_multiply_2d(
        __global const int* A,
        __global const int* B,
        __global int* C,
        const unsigned int M,
        const unsigned int N,
        const unsigned int K)
    {
        unsigned int col = get_global_id(0);
        unsigned int row = get_global_id(1);

        if (row >= M || col >= N) return;

        int sum = 0;
        for (unsigned int k = 0; k < K; ++k) {
            sum += A[row * K + k] * B[k * N + col];
        }
        C[row * N + col] = sum;
    }
    )CLC";
    cl_program program = clCreateProgramWithSource(context, 1, &kernelSource, nullptr, &err);
    clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);

    cl_kernel kernel = clCreateKernel(program, "matrix_multiply_2d", &err);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);
    clSetKernelArg(kernel, 3, sizeof(unsigned int), &M);
    clSetKernelArg(kernel, 4, sizeof(unsigned int), &N);
    clSetKernelArg(kernel, 5, sizeof(unsigned int), &K);

   // Ustawienie rozmiaru grupy roboczej i globalnego rozmiaru
    size_t globalSize[] = { M, N };
    size_t localSize[] = { 16, 16 }; // Wymiary grupy roboczej
    // Uruchomienie kernela
    err = clEnqueueNDRangeKernel(queue, kernel, 2, nullptr, globalSize, localSize, 0, nullptr, nullptr);
    clFinish(queue);
    // Pobranie wyników
    err = clEnqueueReadBuffer(queue, bufferC, CL_TRUE, 0, sizeof(int) * M * N, C.data(), 0, nullptr, nullptr);
    // Wyświetlenie wyników
    std::cout << "C[0] = " << C[0] << std::endl;
    // Czyszczenie zasobów
    const auto finish{steady_clock::now()};
    const duration<double> elapsed_seconds{finish - start};
    std::cout << elapsed_seconds.count() << '\n';
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    }
    
    return 0;
}
