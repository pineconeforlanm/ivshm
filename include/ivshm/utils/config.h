// Copyright (c) 2026 Lanm. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <ivshm/interface/channel.h>
#include <ivshm/utils/common.h>

#include <vector>

namespace ivshm {

// services:
//   size: 32  # Size in MB, supports values between 1 and 512.
//   channels: # Supports a minimum of 1 channel and a maximum of 16 channels.
//     - 0

struct Services {
  std::size_t size;
  std::vector<Channel> channels;
};
YLT_REFL(Services, size, channels)

struct Config {
  Services services;
};
YLT_REFL(Config, services)
}  // namespace ivshm
