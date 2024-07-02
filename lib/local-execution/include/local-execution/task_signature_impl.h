#ifndef _FLEXFLOW_LOCAL_EXECUTION_TASK_SIGNATURE_IMPL_H
#define _FLEXFLOW_LOCAL_EXECUTION_TASK_SIGNATURE_IMPL_H

#include "local-execution/device_specific.h"
#include "local-execution/device_states.h"
#include "local-execution/tasks.h"
#include "task_argument_accessor.h"
#include "utils/variant.h"

namespace FlexFlow {

using TaskImplFunction = std::variant<
    std::function<DeviceSpecific<DeviceStates>(TaskArgumentAccessor const &)>,
    std::function<std::optional<float>(TaskArgumentAccessor const &)>>;

struct TaskSignatureAndImpl {
  TaskImplFunction impl_function;
  OpTaskSignature task_signature;

  bool operator==(TaskSignatureAndImpl const & other) const {
    return std::addressof(this->impl_function) == std::addressof(other.impl_function) && 
            this->task_signature == other.task_signature;
  }

  bool operator!=(TaskSignatureAndImpl const & other) const {
   return std::addressof(this->impl_function) != std::addressof(other.impl_function) || 
            this->task_signature != other.task_signature;
  }
};

TaskSignatureAndImpl get_task_sig_impl(task_id_t const &);

} // namespace FlexFlow

#endif
