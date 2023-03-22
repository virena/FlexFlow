#ifndef _FLEXFLOW_OPS_KERNELS_CONV_2D_KERNELS_H
#define _FLEXFLOW_OPS_KERNELS_CONV_2D_KERNELS_H

#include "kernels/device.h"
#include "kernels/op_meta.h"

namespace FlexFlow {

class Conv2DMeta : public OpMeta {
public:
  Conv2DMeta(FFHandler handler);
  ffTensorDescriptor_t inputTensor, biasTensor, outputTensor;
  ffFilterDescriptor_t filterDesc;
  ffActivationDescriptor_t actiDesc;
  ffConvolutionDescriptor_t convDesc;
  ffConvolutionFwdAlgo_t fwdAlgo;
  ffConvolutionBwdFilterAlgo_t bwdFilterAlgo;
  ffConvolutionBwdDataAlgo_t bwdDataAlgo;
  bool relu, use_bias;
  char op_name[MAX_OPNAME];
};

namespace Kernels {
namespace Conv2D {

void init_kernel(Conv2DMeta *m,
                 int input_w,
                 int input_h,
                 int input_c,
                 int input_n,
                 int output_w,
                 int output_h,
                 int output_c,
                 int output_n,
                 int kernel_h,
                 int kernel_w,
                 int groups,
                 int stride_h,
                 int stride_w,
                 int pad_h,
                 int pad_w,
                 float const *input_ptr,
                 float *output_ptr,
                 float const *kernel_ptr,
                 float *kernel_grad_ptr,
                 float *forward_time = nullptr,
                 float *backward_time = nullptr);

void forward_kernel_wrapper(Conv2DMeta const *m,
                            float const *input_ptr,
                            float *output_ptr,
                            float const *filter_ptr,
                            float const *bias_ptr);
void backward_kernel_wrapper(Conv2DMeta const *m,
                             float const *input_ptr,
                             float *input_grad_ptr,
                             float const *output_ptr,
                             float *output_grad_ptr,
                             float const *kernel_ptr,
                             float *kernel_grad_ptr,
                             float *bias_grad_ptr);

namespace Internal {

void forward_kernel(Conv2DMeta const *m,
                    float const *input_ptr,
                    float *output_ptr,
                    float const *filter_ptr,
                    float const *bias_ptr,
                    ffStream_t stream);

void backward_kernel(Conv2DMeta const *m,
                     float const *input_ptr,
                     float *input_grad_ptr,
                     float const *output_ptr,
                     float *output_grad_ptr,
                     float const *kernel_ptr,
                     float *kernel_grad_ptr,
                     float *bias_grad_ptr,
                     ffStream_t stream);

#if defined(FF_USE_CUDA) || defined(FF_USE_HIP_CUDA)
#else
#endif

} // namespace Internal
} // namespace Conv2D
} // namespace Kernels
} // namespace FlexFlow

#endif // _FLEXFLOW_OPS_KERNELS_CONV_2D_KERNELS_H
