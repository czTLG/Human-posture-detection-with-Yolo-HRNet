//
// Created by fss on 23-3-4.
//

#include "runtime/runtime_attr.hpp"
namespace kuiper_infer {
void RuntimeAttribute::clear() {
  if (!this->weight_data.empty()) {
    std::vector<char> tmp = std::vector<char>();
    this->weight_data.swap(tmp);
  }
}
}  // namespace kuiper_infer


wget -r -np -nH --cut-dirs=2 https://github.com/zjhellofss/tmp/tree/c059e9433d2fddc4cf40c64cc67c07bc0c517b0a/yolo/demo/
