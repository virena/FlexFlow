#ifndef _PCG_PARALLEL_TENSOR_ATTRS_H
#define _PCG_PARALLEL_TENSOR_ATTRS_H

#include "pcg/tensor_attrs.dtg.h"
#include "pcg/parallel_tensor_attrs.dtg.h"
#include "op-attrs/parallel_tensor_shape.h"

namespace FlexFlow {

std::vector<TensorAttrs>
    get_piece_attrs(std::vector<ParallelTensorAttrs> const &);
TensorAttrs get_piece_attrs(ParallelTensorAttrs const &);

} // namespace FlexFlow

#endif