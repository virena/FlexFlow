#ifndef _FLEXFLOW_RESHAPE_H
#define _FLEXFLOW_RESHAPE_H

#include "local-execution/op_task_invocation.h"
#include "local-execution/sim_environment.h"
#include "op-attrs/ops/reshape.h"

namespace FlexFlow {

std::vector<task_id_t> get_task_ids(ReshapeAttrs const &);

TaskImplFunction get_reshape_init_task_impl();
TaskImplFunction get_reshape_fwd_task_impl();
TaskImplFunction get_reshape_bwd_task_impl();

OpTaskSignature get_reshape_init_signature();
OpTaskSignature get_reshape_fwd_signature();
OpTaskSignature get_reshape_bwd_signature();

OpTaskInvocation init(ReshapeAttrs const &);
OpTaskInvocation forward(ReshapeAttrs const &);
OpTaskInvocation backward(ReshapeAttrs const &);

CostMetrics measure_operator_cost(SimEnvFactory const &sim_factory,
                                  ReshapeAttrs const &attrs,
                                  InputParallelTensorDesc const &input,
                                  ProfilingSettings const &settings,
                                  MachineView const &machine_view);

} // namespace FlexFlow

#endif
