#include "utils/graph/series_parallel/intermediate_sp_decomposition_tree.h"
#include "utils/containers/extend.h"
#include "utils/graph/series_parallel/binary_sp_decomposition_tree/generic_binary_sp_decomposition_tree/get_left_child.h"
#include "utils/graph/series_parallel/binary_sp_decomposition_tree/generic_binary_sp_decomposition_tree/get_right_child.h"
#include "utils/graph/series_parallel/binary_sp_decomposition_tree/generic_binary_sp_decomposition_tree/visit.h"
#include "utils/overload.h"

namespace FlexFlow {

struct FlattenAST {
  void add_flattened_child_to_parent(
      IntermediateSpDecompositionTree &parent,
      std::variant<IntermediateSpDecompositionTree, Node> const &child) {
    if (std::holds_alternative<Node>(child)) {
      parent.children.push_back(child);
      return;
    }

    IntermediateSpDecompositionTree child_node =
        std::get<IntermediateSpDecompositionTree>(child);

    if (parent.type == child_node.type) {
      extend(parent.children, child_node.children);
    } else {
      parent.children.push_back(child);
    }
  }

  std::variant<IntermediateSpDecompositionTree, Node>
      operator()(IntermediateSpDecompositionTree const &ast_node) {
    IntermediateSpDecompositionTree result(ast_node.type, {});
    for (std::variant<IntermediateSpDecompositionTree, Node> const &child :
         ast_node.children) {
      std::variant<IntermediateSpDecompositionTree, Node> flattened_child =
          flatten_ast(child);
      add_flattened_child_to_parent(result, flattened_child);
    }
    return result;
  }

  std::variant<IntermediateSpDecompositionTree, Node>
      operator()(Node const &ast_node) {
    return ast_node;
  }
};

std::variant<IntermediateSpDecompositionTree, Node> flatten_ast(
    std::variant<IntermediateSpDecompositionTree, Node> const &ast) {
  return std::visit(FlattenAST{}, ast);
}

std::variant<IntermediateSpDecompositionTree, Node>
    from_binary_sp_tree(GenericBinarySPDecompositionTree<Node> const &binary) {
  return visit<std::variant<IntermediateSpDecompositionTree, Node>>(
      binary,
      overload{
          [](Node const &n) { return n; },
          [](GenericBinarySeriesSplit<Node> const &s) {
            return IntermediateSpDecompositionTree{
                SplitType::SERIES,
                {
                    from_binary_sp_tree(get_left_child(s)),
                    from_binary_sp_tree(get_right_child(s)),
                },
            };
          },
          [](GenericBinaryParallelSplit<Node> const &p) {
            return IntermediateSpDecompositionTree{
                SplitType::PARALLEL,
                {
                    from_binary_sp_tree(get_left_child(p)),
                    from_binary_sp_tree(get_right_child(p)),
                },
            };
          },
      });
}

std::variant<IntermediateSpDecompositionTree, Node>
    from_binary_sp_tree(BinarySPDecompositionTree const &binary) {
  return from_binary_sp_tree(binary.raw_tree);
}

} // namespace FlexFlow