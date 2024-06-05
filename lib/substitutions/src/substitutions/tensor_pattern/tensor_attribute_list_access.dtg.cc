// THIS FILE WAS AUTO-GENERATED BY proj. DO NOT MODIFY IT!
// If you would like to modify this datatype, instead modify
// lib/substitutions/include/substitutions/tensor_pattern/tensor_attribute_list_access.struct.toml
/* proj-data
{
  "generated_from": "41f5449cd700b6d7ab017f3efa39dc1d"
}
*/

#include "substitutions/tensor_pattern/tensor_attribute_list_access.dtg.h"

#include "substitutions/tensor_pattern/tensor_attribute_key.dtg.h"
#include <sstream>

namespace FlexFlow {
TensorAttributeListIndexAccess::TensorAttributeListIndexAccess(
    ::FlexFlow::TensorAttributeKey const &attribute_key, int const &index)
    : attribute_key(attribute_key), index(index) {}
bool TensorAttributeListIndexAccess::operator==(
    TensorAttributeListIndexAccess const &other) const {
  return std::tie(this->attribute_key, this->index) ==
         std::tie(other.attribute_key, other.index);
}
bool TensorAttributeListIndexAccess::operator!=(
    TensorAttributeListIndexAccess const &other) const {
  return std::tie(this->attribute_key, this->index) !=
         std::tie(other.attribute_key, other.index);
}
bool TensorAttributeListIndexAccess::operator<(
    TensorAttributeListIndexAccess const &other) const {
  return std::tie(this->attribute_key, this->index) <
         std::tie(other.attribute_key, other.index);
}
bool TensorAttributeListIndexAccess::operator>(
    TensorAttributeListIndexAccess const &other) const {
  return std::tie(this->attribute_key, this->index) >
         std::tie(other.attribute_key, other.index);
}
bool TensorAttributeListIndexAccess::operator<=(
    TensorAttributeListIndexAccess const &other) const {
  return std::tie(this->attribute_key, this->index) <=
         std::tie(other.attribute_key, other.index);
}
bool TensorAttributeListIndexAccess::operator>=(
    TensorAttributeListIndexAccess const &other) const {
  return std::tie(this->attribute_key, this->index) >=
         std::tie(other.attribute_key, other.index);
}
} // namespace FlexFlow

namespace std {
size_t hash<FlexFlow::TensorAttributeListIndexAccess>::operator()(
    FlexFlow::TensorAttributeListIndexAccess const &x) const {
  size_t result = 0;
  result ^= std::hash<::FlexFlow::TensorAttributeKey>{}(x.attribute_key) +
            0x9e3779b9 + (result << 6) + (result >> 2);
  result ^=
      std::hash<int>{}(x.index) + 0x9e3779b9 + (result << 6) + (result >> 2);
  return result;
}
} // namespace std

namespace nlohmann {
FlexFlow::TensorAttributeListIndexAccess
    adl_serializer<FlexFlow::TensorAttributeListIndexAccess>::from_json(
        json const &j) {
  return {j.at("attribute_key").template get<::FlexFlow::TensorAttributeKey>(),
          j.at("index").template get<int>()};
}
void adl_serializer<FlexFlow::TensorAttributeListIndexAccess>::to_json(
    json &j, FlexFlow::TensorAttributeListIndexAccess const &v) {
  j["__type"] = "TensorAttributeListIndexAccess";
  j["attribute_key"] = v.attribute_key;
  j["index"] = v.index;
}
} // namespace nlohmann

namespace rc {
Gen<FlexFlow::TensorAttributeListIndexAccess>
    Arbitrary<FlexFlow::TensorAttributeListIndexAccess>::arbitrary() {
  return gen::construct<FlexFlow::TensorAttributeListIndexAccess>(
      gen::arbitrary<::FlexFlow::TensorAttributeKey>(), gen::arbitrary<int>());
}
} // namespace rc

namespace FlexFlow {
std::string format_as(TensorAttributeListIndexAccess const &x) {
  std::ostringstream oss;
  oss << "<TensorAttributeListIndexAccess";
  oss << " attribute_key=" << x.attribute_key;
  oss << " index=" << x.index;
  oss << ">";
  return oss.str();
}
std::ostream &operator<<(std::ostream &s,
                         TensorAttributeListIndexAccess const &x) {
  return s << fmt::to_string(x);
}
} // namespace FlexFlow