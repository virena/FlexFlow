#include "local-execution/local_slots_backing.h"

namespace FlexFlow {

LocalSlotsBacking::LocalSlotsBacking(
    TensorBackingMapping const &allocated_tensors,
    RuntimeArgConfig const &runtime_arg_config)
    : tensor_mapping(allocated_tensors),
      runtime_arg_config(runtime_arg_config){};

void LocalSlotsBacking::add_per_device_op_state(
    operator_guid_t const &op_guid,
    DeviceSpecific<DeviceStates> const &device_state) {
  this->per_device_op_states.insert({op_guid, device_state});
}

bool LocalSlotsBacking::is_tensor_allocated(
    tensor_guid_t const &tensor_id) const {
  return contains_key(this->tensor_mapping, tensor_id);
}

GenericTensorAccessorW const &LocalSlotsBacking::get_tensor_backing(
    tensor_guid_t const &tensor_id) const {
  return this->tensor_mapping.at(tensor_id);
}

TensorSlotsBacking LocalSlotsBacking::construct_tensor_slots_backing(
    OpTaskBinding const &binding, operator_guid_t const &op_guid) const {
  TensorSlotsBacking mapping;
  for (auto const &tensor_binding : binding.get_tensor_bindings()) {
    SlotGradId slot_grad_id = tensor_binding.first;
    OpTensorSpec tensor_spec = tensor_binding.second;
    std::vector<tensor_guid_t> tensor_guids;
    switch (tensor_spec.role) {
      case TensorRole::INPUT:
        tensor_guids = this->input_tensor_slots.at(op_guid);
        break;
      case TensorRole::WEIGHT:
        tensor_guids = this->weight_tensor_slots.at(op_guid);
        break;
      case TensorRole::OUTPUT:
        tensor_guids = this->output_tensor_slots.at(op_guid);
        break;
      default:
        throw mk_runtime_error(
            fmt::format("Invalid TensorRole")); // inserting role yields
                                                // "type_is_unformattable" error
    }
    GenericTensorAccessorW tensor_backing =
        this->get_tensor_backing(tensor_guids.at(tensor_spec.idx));
    mapping.insert({slot_grad_id, tensor_backing});
  }
  return mapping;
}

ArgSlotsBacking LocalSlotsBacking::construct_arg_slots_backing(
    OpTaskBinding const &binding, operator_guid_t const &op_guid) const {
  ArgSlotsBacking mapping;
  for (auto const &arg_binding : binding.get_arg_bindings()) {
    slot_id arg_slot = arg_binding.first;
    OpArgSpec op_arg_spec = arg_binding.second;
    if (std::holds_alternative<OpArgRefSpec>(op_arg_spec)) {
      mapping.insert({arg_slot,
                      resolve_op_arg_ref_spec(
                          std::get<OpArgRefSpec>(op_arg_spec), op_guid)});
    } else if (std::holds_alternative<RuntimeArgRefSpec>(op_arg_spec),
               op_guid) {
      mapping.insert({arg_slot,
                      resolve_runtime_arg_ref_spec(
                          std::get<RuntimeArgRefSpec>(op_arg_spec))});
    } else if (std::holds_alternative<ConcreteArgSpec>(op_arg_spec)) {
      mapping.insert({arg_slot, std::get<ConcreteArgSpec>(op_arg_spec)});
    } else {
      throw mk_runtime_error("Unhandled argument type");
    }
  }
  return mapping;
}

ConcreteArgSpec LocalSlotsBacking::resolve_op_arg_ref_spec(
    OpArgRefSpec const &op_arg_ref_spec, operator_guid_t const &op_guid) const {
  if (op_arg_ref_spec.holds<DeviceSpecific<DeviceStates>>()) {
    return ConcreteArgSpec::create(per_device_op_states.at(op_guid));
  } else if (op_arg_ref_spec.holds<ParallelTensorShape>()) {
    IndexOpArgRefType index_op_arg_ref =
        std::get<IndexOpArgRefType>(op_arg_ref_spec.get_ref_type());
    std::vector<tensor_guid_t> input_tensor_guids =
        this->input_tensor_slots.at(op_guid);
    GenericTensorAccessorW tensor_backing =
        this->get_tensor_backing(input_tensor_guids.at(index_op_arg_ref.idx));
    ParallelTensorShape shape = {
        get_tensor_shape(tensor_backing.shape, tensor_backing.data_type)};
    return ConcreteArgSpec::create(shape);
  } else {
    throw mk_runtime_error("Unhandled op arg ref type");
  }
}

ConcreteArgSpec LocalSlotsBacking::resolve_runtime_arg_ref_spec(
    RuntimeArgRefSpec const &runtime_arg_ref_spec) const {
  if (runtime_arg_ref_spec.holds<DeviceSpecific<PerDeviceFFHandle>>()) {
    return ConcreteArgSpec::create(this->runtime_arg_config.ff_handle);
  } else if (runtime_arg_ref_spec.holds<EnableProfiling>()) {
    return ConcreteArgSpec::create(this->runtime_arg_config.enable_profiling);
  } else if (runtime_arg_ref_spec.holds<ProfilingSettings>()) {
    return ConcreteArgSpec::create(this->runtime_arg_config.profiling_settings);
  } else {
    throw mk_runtime_error("Unhandled runtime arg ref type");
  }
}

} // namespace FlexFlow