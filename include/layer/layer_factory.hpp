//
// Created by fss on 22-11-17.
//

#ifndef KUIPER_INFER_SOURCE_LAYER_LAYER_FACTORY_HPP_
#define KUIPER_INFER_SOURCE_LAYER_LAYER_FACTORY_HPP_
#include <map>
#include <string>
#include <memory>
#include "layer.hpp"
#include "runtime/runtime_op.hpp"

namespace kuiper_infer {
class LayerRegisterer {
 public:
 /**
  * 这是一个函数指针类型的定义，名字为 Creator。该函数指针类型可以指向一个接受两个参数并返回 ParseParameterAttrStatus 类型的函数。
    函数的第一个参数是一个名为 op 的 std::shared_ptr<RuntimeOperator> 类型的引用，表示运行时算子的智能指针。std::shared_ptr 是 C++11 中引入的智能指针，它可以自动管理资源的释放，避免了手动释放内存的问题。
    函数的第二个参数是一个名为 layer 的 std::shared_ptr<Layer> 类型的引用，表示网络层的智能指针。
    函数的返回值是一个枚举类型 ParseParameterAttrStatus，表示参数解析的状态。
 */
  typedef ParseParameterAttrStatus (*Creator)(const std::shared_ptr<RuntimeOperator> &op,
                                              std::shared_ptr<Layer> &layer);

  typedef std::map<std::string, Creator> CreateRegistry;

  static void RegisterCreator(const std::string &layer_type, const Creator &creator);

  static std::shared_ptr<Layer> CreateLayer(const std::shared_ptr<RuntimeOperator> &op);

  static CreateRegistry &Registry();
};

class LayerRegistererWrapper {
 public:
  LayerRegistererWrapper(const std::string &layer_type, const LayerRegisterer::Creator &creator) {
    LayerRegisterer::RegisterCreator(layer_type, creator);
  }
};

}

#endif //KUIPER_INFER_SOURCE_LAYER_LAYER_FACTORY_HPP_
