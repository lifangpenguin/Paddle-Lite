/* Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#ifdef POOL_OP

#include "operators/kernel/pool_kernel.h"

namespace paddle_mobile {
namespace operators {

template <>
bool PoolKernelGpu<float>::Init(PoolParam *param) {
  std::string pooling_type = param->PoolingType();
  this->cl_helper_.AddKernel("pool_" + pooling_type, "pool_kernel.cl");
  return true;
}

template <>
void PoolKernelGpu<float>::Compute(const PoolParam &param) {
  auto kernel = this->cl_helper_.KernelAt(0);
  auto default_work_size =
      this->cl_helper_.DefaultWorkSize(*param.Output()->ClImage());

  auto input = param.Input()->ClImage()->GetCLImage();
  auto out = param.Output()->ClImage()->GetCLImage();

  framework::CLImageConverterFolder *input_folder_converter =
      reinterpret_cast<framework::CLImageConverterFolder *>(
          param.Input()->ClImage()->Converter());
  framework::CLImageConverterFolder *output_folder_converter =
      reinterpret_cast<framework::CLImageConverterFolder *>(
          param.Output()->ClImage()->Converter());

  const int in_height = input_folder_converter->HeightOfOneBlock();
  const int in_width = input_folder_converter->WidthOfOneBlock();
  const int out_height = output_folder_converter->HeightOfOneBlock();
  const int out_width = output_folder_converter->WidthOfOneBlock();

  std::string pooling_type = param.PoolingType();
  std::vector<int> ksize = param.Ksize();
  std::vector<int> strides = param.Strides();
  std::vector<int> paddings = param.Paddings();
  const int pad_top = paddings[0];
  const int pad_left = paddings[1];
  const int stride_h = strides[0];
  const int stride_w = strides[1];
  const int ksize_h = ksize[0];
  const int ksize_w = ksize[1];

  clSetKernelArg(kernel, 0, sizeof(cl_int), &in_height);
  clSetKernelArg(kernel, 1, sizeof(cl_int), &in_width);
  clSetKernelArg(kernel, 2, sizeof(cl_int), &out_height);
  clSetKernelArg(kernel, 3, sizeof(cl_int), &out_width);
  clSetKernelArg(kernel, 4, sizeof(cl_int), &pad_top);
  clSetKernelArg(kernel, 5, sizeof(cl_int), &pad_left);
  clSetKernelArg(kernel, 6, sizeof(cl_int), &stride_h);
  clSetKernelArg(kernel, 7, sizeof(cl_int), &stride_w);
  clSetKernelArg(kernel, 8, sizeof(cl_int), &ksize_h);
  clSetKernelArg(kernel, 9, sizeof(cl_int), &ksize_w);
  clSetKernelArg(kernel, 10, sizeof(cl_mem), &input);
  clSetKernelArg(kernel, 11, sizeof(cl_mem), &out);

  //  cl_event out_event = param.Output()->ClImage()->GetClEvent();
  //  cl_event wait_event = param.Input()->ClImage()->GetClEvent();
  clEnqueueNDRangeKernel(this->cl_helper_.CLCommandQueue(), kernel, 3, NULL,
                         default_work_size.data(), NULL, 0, NULL, NULL);
}

template class PoolKernelGpu<float>;

}  // namespace operators
}  // namespace paddle_mobile

#endif
