#include "doctest/doctest.h"
#include "kernels/batch_norm_kernels.h"
#include "kernels/local_allocator.h"
#include "test_utils.h"
#include <algorithm>
#include <vector>

template <typename T>
void allocate_ptrs(std::vector<T **> &gpu_data_ptrs,
                   const std::vector<size_t> &num_elements,
                   Allocator &allocator) {
  for (size_t i = 0; i < gpu_data_ptrs.size(); ++i) {
    *gpu_data_ptrs[i] =
        static_cast<T *>(allocator.allocate(num_elements[i] * sizeof(float)));
  }
}

using namespace ::FlexFlow;

TEST_SUITE(FF_TEST_SUITE) {
  TEST_CASE("Test BatchNorm Forward and Backward Kernel") {
    size_t output_n = 1, output_c = 10, output_h = 10, output_w = 10;
    size_t num_elements = output_n * output_c * output_h * output_w;

    PerDeviceFFHandle handle;
    setPerDeviceFFHandle(&handle);
    cudaStream_t stream;
    checkCUDA(cudaStreamCreate(&stream));

    Allocator allocator = get_local_memory_allocator();

    BatchNormPerDeviceState state =
        Kernels::BatchNorm::init_kernel(handle, allocator, nullptr, output_n,
                                        output_c, output_h, output_w, true);

    float *input_data, *output_data, *scale, *bias;
    std::vector<float **> ptrs = {&input_data, &output_data, &scale, &bias};
    std::vector<size_t> sizes = {num_elements, num_elements, output_c,
                                 output_c};

    allocate_ptrs(ptrs, sizes, allocator);
    randomFillDeviceData(&input_data, num_elements);
    fillDeviceDataOnes(&scale, output_c);
    fillDeviceDataZeros(&bias, output_c);

    Kernels::BatchNorm::forward_kernel(stream, state, input_data, output_data,
                                       scale, bias);

    std::vector<float> host_output_data(num_elements);
    checkCUDA(cudaMemcpy(host_output_data.data(), output_data,
                         num_elements * sizeof(float), cudaMemcpyDeviceToHost));

    float *grad_input, *grad_output_data;
    std::vector<float **> ptrs_grad = {&grad_input, &grad_output_data};
    allocate_ptrs(ptrs_grad, {num_elements, num_elements}, allocator);

    Kernels::BatchNorm::backward_kernel(
        stream, state, input_data, grad_output_data, output_data, grad_input,
        scale, scale, bias, num_elements);

    std::vector<float> host_grad_input(num_elements);
    checkCUDA(cudaMemcpy(host_grad_input.data(), grad_input,
                         num_elements * sizeof(float), cudaMemcpyDeviceToHost));

    Kernels::BatchNorm::cleanup_kernel(allocator, state.inputTensor,
                                       state.biasTensor, state.outputTensor,
                                       state.actiDesc, true, nullptr);

    checkCUDA(cudaStreamDestroy(stream));
    checkCUDA(cudaFree(handle.workSpace));
  }
}