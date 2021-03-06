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

#include "op_param.h"
namespace paddle_mobile {
namespace operators {

#ifdef CONV_OP
Print &operator<<(Print &printer, const ConvParam &conv_param) {
  printer << "parameter of conv: "
          << "\n";
  printer << "  stride: "
          << " (" << conv_param.Strides()[0] << conv_param.Strides()[1] << ") "
          << "\n";
  printer << "  paddings: "
          << " (" << conv_param.Paddings()[0] << conv_param.Paddings()[1]
          << ") "
          << "\n";
  printer << "  dilations: "
          << " (" << conv_param.Dilations()[0] << conv_param.Dilations()[1]
          << ") "
          << "\n";
  printer << "  groups: " << conv_param.Groups() << "\n";
  printer << "  input  dims: " << conv_param.Input()->dims() << "\n";
  printer << "  filter dims: " << conv_param.Filter()->dims() << "\n";
  printer << "  output dims: " << conv_param.Output()->dims();
  return printer;
}
#endif

}  // namespace operators
}  // namespace paddle_mobile
