//
// Created by crh on 22-12-25.
//
#include "upsample.hpp"
#include "layer/abstract/layer_factory.hpp"
namespace kuiper_infer {
UpSampleNearestLayer::UpSampleNearestLayer(uint32_t output_h, uint32_t output_w)
    : output_h_(output_h), output_w_(output_w) {

}
InferStatus UpSampleNearestLayer::Forward(const std::vector<std::shared_ptr<Tensor<float>>> &inputs,
                                   std::vector<std::shared_ptr<Tensor<float>>> &outputs) {
  if (inputs.empty()) {
    LOG(ERROR) << "The input feature map of upsample layer is empty";
    return InferStatus::kInferFailedInputEmpty;
  }

  if (inputs.size() != outputs.size()) {
    LOG(ERROR) << "The input and output size is not adapting";
    return InferStatus::kInferFailedInputOutSizeAdaptingError;
  }

  for (uint32_t i = 0; i < inputs.size(); ++i) {
    const sftensor &input_data = inputs.at(i);
    if (input_data == nullptr || input_data->empty()) {
      LOG(ERROR) << "The input feature map of upsample layer is empty";
      return InferStatus::kInferFailedInputEmpty;
    }
  }
    const uint32_t input_w = input_data.at(0)->cols();
    const uint32_t input_h = input_data.at(0)->rows();
    const float scale_w_ = (float)(output_w_ / input_w)
    const float scale_h_ = (float)(output_h_ / input_h)
  const uint32_t batch_size = inputs.size();
#pragma omp parallel for num_threads(batch_size)
  for (uint32_t i = 0; i < batch_size; ++i) {
    const arma::fcube &input_data = inputs.at(i)->data();
    std::shared_ptr<Tensor<float>> output = outputs.at(i);
    if (output == nullptr || output->empty()) {
      output = std::make_shared<Tensor<float>>(input_data.n_slices,
                                               uint32_t(output_h_),
                                               uint32_t(output_h_));
      outputs.at(i) = output;
    }
    auto &output_data = output->data();
    CHECK(output_data.n_rows == output_h_) << "The height of the feature map is not adapting!";
    CHECK(output_data.n_cols == output_h_) << "The width of the feature map is not adapting!";
    CHECK(input_data.n_slices == output_data.n_slices) << "The channel of the feature map is not adapting!";

    const uint32_t channels = input_data.n_slices;
    
    for (uint32_t c = 0; c < channels; ++c) {
      const arma::fmat &input_channel = input_data.slice(c);
      arma::fmat &output_channel = output_data.slice(c);

      for (uint32_t w = 0; w < output_w_; ++w) {
        const uint32_t src_w = uint32_t((float) w / scale_w_);
        CHECK(src_w < input_channel.n_cols);
        float *output_channel_ptr = output_channel.colptr(w);
        const float *input_channel_ptr = input_channel.colptr(src_w);

        for (uint32_t h = 0; h < output_h_; ++h) {
          const uint32_t src_h = uint32_t((float) h / scale_h_);
          CHECK(src_h < input_channel.n_rows);

          const float src_value = *(input_channel_ptr + src_h);
          *(output_channel_ptr + h) = src_value;
        }
      }
    }
  }
  return InferStatus::kInferSuccess;
}

ParseParameterAttrStatus UpSampleNearestLayer::GetInstance(const std::shared_ptr<RuntimeOperator> &op,
                                                    std::shared_ptr<Layer> &upsample_layer) {

  CHECK(op != nullptr) << "Upsample operator is null";
  const auto &params = op->params;
  CHECK(!params.empty()) << "Operator parameter is empty";

  if (params.find("output_height") == params.end()) {
    LOG(ERROR) << "Can not find the output_height parameter";
    return ParseParameterAttrStatus::kParameterMissingoutputHeight;
  }

    if (params.find("output_width") == params.end()) {
    LOG(ERROR) << "Can not find the output_width parameter";
    return ParseParameterAttrStatus::kParameterMissingoutputWidth;
  }

  const auto& output_height = dynamic_cast<RuntimeParameterIntArray*>(params.at("output_height"));

    if (output_height == nullptr) {
    LOG(ERROR) << "Can not find the output_height parameter";
    return ParseParameterAttrStatus::kParameterMissingoutputHeight;
  }
  const auto& output_width =
      dynamic_cast<RuntimeParameterIntArray*>(params.at("output_width"));

    if (output_width == nullptr) {
    LOG(ERROR) << "Can not find the output_width parameter";
    return ParseParameterAttrStatus::kParameterMissingoutputWidth;
  }

  upsampleNearest_layer = std::make_shared<UpSampleNearestLayer>(output_height->value, output_height->value);
  return ParseParameterAttrStatus::kParameterAttrParseSuccess;
}

LayerRegistererWrapper UpSamplerNearestGetInstance("F.upsample_nearest", UpSampleNearestLayer::GetInstance);
}