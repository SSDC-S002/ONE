/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Builders.h"

#include "kernels/ResizeNearestNeighbor.h"

namespace luci_interpreter
{

std::unique_ptr<Kernel>
build_kernel_CircleResizeNearestNeighbor(std::vector<const Tensor *> &&inputs,
                                         std::vector<Tensor *> &&outputs, const uint32_t op_index,
                                         KernelBuilder &builder)
{
  assert(inputs.size() == 2);

  const Tensor *input = inputs.at(0);
  const Tensor *size = inputs.at(1);
  Tensor *output = outputs.at(0);

  circle::OperatorT oper_t;
  builder.get_circle_reader()->operators()[op_index]->UnPackTo(&oper_t);
  const auto *options = oper_t.builtin_options.AsResizeNearestNeighborOptions();

  ResizeNearestNeighborParams params{};
  params.align_corners = options->align_corners;
  // TODO update half_pixel_centers after CircleResizeNearestNeighbor updated
  // Current CircleResizeNearestNeighbor don't have half_pixel_centers.
  // default value on current is false.
  // it need to be updated when CircleResizeNearestNeighbor updated.
  params.half_pixel_centers = false;

  return std::make_unique<kernels::ResizeNearestNeighbor>(input, size, output, params);
}

} // namespace luci_interpreter
