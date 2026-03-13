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

#include <ivshm/utils/logger.h>
#include <ivshm_version.h>

#include <memory>

namespace {

constexpr std::string_view k_log_tag{"exp.ivshm.writer"};
constexpr std::string_view k_config_dir{DEFAULT_CONFIG_PATH};
constexpr std::string_view k_config_filename{"config/ivshm.yaml"};

auto InitLog() -> void {
  ivshm::SetMinSeverity(ivshm::Severity::TRACE);
  ivshm::SetAsync(false);
  ivshm::SetConsole(true);
}

auto RealMain([[maybe_unused]] const int argc,
              [[maybe_unused]] const char **argv) -> int {
  InitLog();

  IVSHM_LOG_TAG(INFO, k_log_tag, "Summary:{}", ivshm::Version::Summary());

  return EXIT_SUCCESS;
}

auto ImplMain(const int argc, const char **argv) -> int {
  try {
    return RealMain(argc, argv);
  } catch (const std::exception &exp) {
    IVSHM_LOG_TAG(ERROR, k_log_tag, "Exception: {}", exp.what());
    return EXIT_FAILURE;
  }
}

}  // namespace

auto main(const int argc, const char **argv) -> int {
  return ImplMain(argc, argv);
}
