#ifndef _FLEXFLOW_OPS_KERNELS_REVERSE_KERNELS_H
#define _FLEXFLOW_OPS_KERNELS_REVERSE_KERNELS_H

#include "device.h"

namespace FlexFlow {

using legion_coord_t = long long;

namespace Kernels {
namespace Reverse {

void forward_kernel(ffStream_t stream,
                    float const *in_ptr,
                    float *out_ptr,
                    legion_coord_t num_out_blks,
                    legion_coord_t reverse_dim_size,
                    legion_coord_t in_blk_size,
                    legion_coord_t output_size);

void backward_kernel(ffStream_t stream,
                     float const *out_grad_ptr,
                     float *in_grad_ptr,
                     legion_coord_t num_out_blks,
                     legion_coord_t reverse_dim_size,
                     legion_coord_t in_blk_size,
                     legion_coord_t input_size);

} // namespace Reverse
} // namespace Kernels
} // namespace FlexFlow

#endif // _FLEXFLOW_OPS_KERNELS_REVERSE_KERNELS_H
