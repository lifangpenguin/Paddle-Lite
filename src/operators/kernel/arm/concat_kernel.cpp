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

#ifdef CONCAT_OP

#include "operators/kernel/concat_kernel.h"
#include "operators/kernel/central-arm-func/concat_arm_func.h"

namespace paddle_mobile {
namespace operators {

template <>
bool ConcatKernelCpu<float>::Init(ConcatParam *param) {
  return true;
}

template <>
void ConcatKernelCpu<float>::Compute(const ConcatParam &param) {
  if (param.Inputs()[0]->LodTensor()->type() == type_id<int8_t>().hash_code()) {
    ConcatCompute<int8_t>(param);
  } else {
    ConcatCompute<float>(param);
  }
  param.Out()->LodTensor()->set_lod(param.Inputs()[0]->LodTensor()->lod());
}

}  // namespace operators
}  // namespace paddle_mobile

#endif
