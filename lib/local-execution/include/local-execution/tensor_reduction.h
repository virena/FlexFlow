#ifndef _FLEXFLOW_LOCAL_EXECUTION_TENSOR_REDUCTION_H
#define _FLEXFLOW_LOCAL_EXECUTION_TENSOR_REDUCTION_H

#include "local-execution/reduced_tensor_t.dtg.h"
#include "pcg/tensor_guid_t.dtg.h"

namespace FlexFlow {

reduced_tensor_t lower(tensor_guid_t const &);

std::vector<reduced_tensor_t> lower(std::vector<tensor_guid_t> const &);

} // namespace FlexFlow

#endif