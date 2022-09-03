#include "flexflow/parallel_ops/parallel_op.h"

namespace FlexFlow {

ParallelOp::ParallelOp(FFModel &model,
                       OperatorType op_type,
                       char const *name,
                       const ParallelTensor input)
    : Op(model,
         op_type,
         name,
         1 /*num_inputs*/,
         0 /*num_weights*/,
         1 /*num_ouputs*/,
         input) {}

bool ParallelOp::is_parallel_op() const {
  return true;
}

ParallelOpJoinResult try_join_parallel_ops(ParallelOpInfo const &_first,
                                           ParallelOpInfo const &_second) {
  ParallelOpJoinResult result;

  ParallelOpInfo first(_first);
  ParallelOpInfo second(_second);

  if (first.parallel_dim != second.parallel_dim) {
    return result;
  }

  if (first.op_type == second.op_type) {
    ParallelOpInfo joined(first);
    joined.parallel_degree *= second.parallel_degree;
    result.op = joined;
    result.join_did_succeed = true;
  } else if ((first.op_type == OP_REPARTITION &&
              second.op_type == OP_COMBINE) ||
             (first.op_type == OP_COMBINE &&
              second.op_type == OP_REPARTITION)) {
    if (first.parallel_degree < second.parallel_degree) {
      swap(first, second);
    }
    assert(first.parallel_degree % second.parallel_degree == 0);
    ParallelOpInfo joined(first);
    joined.parallel_degree /= second.parallel_degree;
    if (joined.parallel_degree != 1) {
      result.op = joined;
    }
    result.join_did_succeed = true;
  }

  return result;
}

using PCG::Node;
Node FFModel::get_or_create_parallel_op_node(
    const ParallelTensor input, ParallelOpInfo const &parallel_op_info) {
  int op_type = parallel_op_info.op_type;
  int parallel_dim = parallel_op_info.parallel_dim;
  int parallel_degree = parallel_op_info.parallel_degree;

  switch (op_type) {
    case OP_COMBINE:
      CombineParams combine_params;
      combine_params.combine_legion_dim = parallel_dim;
      combine_params.combine_degree = parallel_degree;
      return this->get_or_create_node<Combine>(input, combine_params);
    case OP_REPARTITION:
      RepartitionParams partition_params;
      partition_params.repartition_legion_dim = parallel_dim;
      partition_params.repartition_degree = parallel_degree;
      return this->get_or_create_node<Repartition>(input, partition_params);
    case OP_REPLICATE:
      ReplicateParams replicate_params;
      replicate_params.replicate_legion_dim = parallel_dim;
      replicate_params.replicate_degree = parallel_degree;
      return this->get_or_create_node<Replicate>(input, replicate_params);
    case OP_REDUCTION:
      ReductionParams reduction_params;
      reduction_params.reduction_legion_dim = parallel_dim;
      reduction_params.reduction_degree = parallel_degree;
      return this->get_or_create_node<Reduction>(input, reduction_params);
    default:
      assert(false && "Unsupported parallel op");
  }
}

void swap(ParallelOpInfo &l, ParallelOpInfo &r) {
  using std::swap;

  swap(l.op_type, r.op_type);
  swap(l.parallel_dim, r.parallel_dim);
  swap(l.parallel_degree, r.parallel_degree);
}

}; // namespace FlexFlow
