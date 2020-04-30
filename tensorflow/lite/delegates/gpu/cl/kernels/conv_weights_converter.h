/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef TENSORFLOW_LITE_DELEGATES_GPU_CL_KERNELS_CONV_WEIGHTS_CONVERTER_H_
#define TENSORFLOW_LITE_DELEGATES_GPU_CL_KERNELS_CONV_WEIGHTS_CONVERTER_H_

#include "tensorflow/lite/delegates/gpu/cl/cl_command_queue.h"
#include "tensorflow/lite/delegates/gpu/cl/cl_kernel.h"
#include "tensorflow/lite/delegates/gpu/cl/kernels/conv_common.h"
#include "tensorflow/lite/delegates/gpu/cl/kernels/gpu_operation.h"
#include "tensorflow/lite/delegates/gpu/common/status.h"
#include "tensorflow/lite/delegates/gpu/common/types.h"

namespace tflite {
namespace gpu {
namespace cl {

class ConverterToConvWeights : public GPUOperation {
 public:
  explicit ConverterToConvWeights(const OperationDef& definition)
      : GPUOperation(definition), work_group_size_(8, 4, 1) {}
  absl::Status AddToQueue(CLCommandQueue* queue) override;
  absl::Status Tune(const TuningParameters& params) override;

  absl::Status Compile(const CreationContext& creation_context) override;

  // Move only
  ConverterToConvWeights(ConverterToConvWeights&& operation);
  ConverterToConvWeights& operator=(ConverterToConvWeights&& operation);
  ConverterToConvWeights(const ConverterToConvWeights&) = delete;
  ConverterToConvWeights& operator=(const ConverterToConvWeights&) = delete;

 private:
  absl::Status BindArguments();
  int3 GetGridSize() const;

  CLKernel kernel_;
  int3 work_group_size_;
};

// We expect src BHWC tensor and we assume that B is O, H = 1, W = 1, C is I
// C must be divisible by 4 and B must be divisible by 16;
// as dst we expect Tensor with storage type BUFFER and
// dst.b * dst.h * dst.w * dst.c = src.b * src.c
ConverterToConvWeights CreateConverterToConvWeights(
    const OperationDef& definition);

}  // namespace cl
}  // namespace gpu
}  // namespace tflite

#endif  // TENSORFLOW_LITE_DELEGATES_GPU_CL_KERNELS_CONV_WEIGHTS_CONVERTER_H_