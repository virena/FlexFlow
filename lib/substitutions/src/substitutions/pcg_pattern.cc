#include "substitutions/pcg_pattern.h"
#include "substitutions/operator_pattern/satisfies_pattern.h"
#include "substitutions/sub_parallel_computation_graph.h"
#include "substitutions/tensor_pattern/satisfies_pattern.h"
#include "substitutions/unlabelled/pattern_value.h"

namespace FlexFlow {

static MatchAdditionalCriterion
    pcg_pattern_criteria(PCGPattern const &pattern,
                         SubParallelComputationGraph const &pcg) {
  return MatchAdditionalCriterion{
      [&](PatternNode const &patternNode, Node const &pcgNode) {
        return operator_satisfies_pattern(
            get_operator_attrs(pcg, pcgNode),
            get_operator_pattern(pattern, patternNode));
      },
      [&](PatternValue const &patternValue, OpenDataflowValue const &pcgValue) {
        return parallel_tensor_satisfies_pattern(
            get_parallel_tensor_attrs(pcg, pcgValue),
            get_tensor_pattern(pattern, patternValue));
      }};
}

std::vector<UnlabelledDataflowGraphPatternMatch>
    find_pattern_matches(PCGPattern const &pattern,
                         SubParallelComputationGraph const &pcg) {
  return find_pattern_matches(get_unlabelled_pattern(pattern),
                              pcg.raw_graph,
                              pcg_pattern_criteria(pattern, pcg));
}

UnlabelledGraphPattern get_unlabelled_pattern(PCGPattern const &p) {
  return UnlabelledGraphPattern{p.raw_graph};
}

TensorAttributePattern get_tensor_pattern(PCGPattern const &p,
                                          PatternValue const &v) {
  return p.raw_graph.at(raw_open_dataflow_value_from_pattern_value(v));
}

OperatorAttributePattern get_operator_pattern(PCGPattern const &p,
                                              PatternNode const &n) {
  return p.raw_graph.at(n.raw_node);
}

bool assignment_satisfies(
    SubParallelComputationGraph const &pcg,
    PCGPattern const &pattern,
    UnlabelledDataflowGraphPatternMatch const &patternMatch) {
  return unlabelled_pattern_does_match(get_unlabelled_pattern(pattern),
                                       pcg.raw_graph,
                                       patternMatch,
                                       pcg_pattern_criteria(pattern, pcg));
}

} // namespace FlexFlow