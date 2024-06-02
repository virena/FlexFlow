#ifndef _FLEXFLOW_LOCAL_EXECUTION_LOCAL_BACKING_H
#define _FLEXFLOW_LOCAL_EXECUTION_LOCAL_BACKING_H

#include "local-execution/task_registry.h"

namespace FlexFlow {

struct LocalTrainingBacking {
  LocalTrainingBacking(Allocator const &,
                       ComputationGraph const &,
                       TensorBackingMapping const &,
                       RuntimeArgConfig const &);
  ~LocalTrainingBacking() = default;

  void execute_init();
  void execute_forward();
  void execute_backward();
  void execute_update();

  DeviceSpecific<DeviceStates>
      call_init_task_impl(task_id_t, TaskArgumentAccessor const &);
  void call_task_impl(task_id_t, TaskArgumentAccessor);

  TaskArgumentAccessor get_task_arg_accessor(OpTaskInvocation const &,
                                             operator_guid_t const &) const;

private:
  Allocator allocator;
  ComputationGraph computation_graph;

  TaskRegistry task_registry;
};

// -- err (maybe): will this resolve correctly?
OpTaskInvocation init(CompGraphOperatorAttrs const &);
OpTaskInvocation forward(CompGraphOperatorAttrs const &);
OpTaskInvocation backward(CompGraphOperatorAttrs const &);

} // namespace FlexFlow

#endif
