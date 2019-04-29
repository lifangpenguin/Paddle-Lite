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

#ifdef BILINEAR_INTERP_OP

#include "operators/bilinear_interp_op.h"

namespace paddle_mobile {
namespace operators {
template <typename T>
void BilinearInterpOp<T>::InferShape() const {
  PADDLE_MOBILE_ENFORCE(this->param_.InputX()->LodTensor() != nullptr,
                        "Input(X) of BilinearInterOp should not be null.");
  PADDLE_MOBILE_ENFORCE(this->param_.Out()->LodTensor() != nullptr,
                        "Output(Out) of BilinearInterOp should not be null.");

  auto dim_x = this->param_.InputX()->LodTensor()->dims();  // NCHW format
  int out_h = this->param_.OutH();
  int out_w = this->param_.OutW();
  PADDLE_MOBILE_ENFORCE(dim_x.size() == 4, "X's dimension must be 4");

  if (this->param_.InputOutPutSize() != nullptr &&
      this->param_.InputOutPutSize()->LodTensor() != nullptr) {
    auto out_size_dim =
        this->param_.InputOutPutSize()->LodTensor()->dims();

    PADDLE_MOBILE_ENFORCE(out_size_dim.size() == 1,
                          "OutSize's dimension size must be 1");
    PADDLE_MOBILE_ENFORCE(out_size_dim[0] == 2, "OutSize's dim[0] must be 2");
  }
  std::vector<int64_t> dim_out({dim_x[0], dim_x[1], out_h, out_w});
  this->param_.Out()->LodTensor()->Resize(framework::make_ddim(dim_out));
}

}  // namespace operators
}  // namespace paddle_mobile

namespace ops = paddle_mobile::operators;
REGISTER_OPERATOR(bilinear_interp, ops::BilinearInterpOp);

#endif
