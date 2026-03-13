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
#include <ivshm/utils/utils.h>

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace ivshm {
namespace fs = std::filesystem;

namespace {
constexpr std::string_view k_log_tag{"ivshm.utils"};
}

auto ReadFileContent(const std::string& path) -> expected<std::string> {
  if (!fs::exists(path) || !fs::is_regular_file(path)) {
    return unexpected{Error{Kind::InvalidArgument}};
  }

  std::ifstream file(path);
  if (!file.is_open()) {
    IVSHM_LOG_TAG(ERROR, k_log_tag, R"(Open file "{}" failed)", path);
    return unexpected{Error{Kind::PermissionDenied}};
  }
  std::string content((std::istreambuf_iterator(file)),
                      std::istreambuf_iterator<char>());
  return content;
}

auto IsHost() -> bool {
  static auto is_check{false};
  static auto is_host{false};

  auto check = [&]() -> void {
    using std::literals::string_view_literals::operator""sv;

    constexpr auto k_host_identifier{"none"sv};
    constexpr auto k_cmd{"systemd-detect-virt"};
    constexpr auto k_buffer_size{128};
    std::array<char, k_buffer_size> buffer{};

    const auto pipe = std::shared_ptr<FILE>(::popen(k_cmd, "r"), ::pclose);
    if (!pipe) {
      std::stringstream error_ss;
      error_ss << "popen [" << k_cmd << "] failed";
      IVSHM_LOG_TAG(ERROR, k_log_tag, "{}", error_ss.str());
      throw std::runtime_error(error_ss.str());
    }

    while (::fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
      if (const auto len = ::strlen(buffer.data());
          len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
      }
      is_check = true;
      is_host = (k_host_identifier == buffer.data());
      return;
    }
    IVSHM_LOG_TAG(ERROR, k_log_tag, "{}", "Check system information error");
    throw std::runtime_error("Check system information error");
  };

  if (!is_check) {
    static std::once_flag flag;
    std::call_once(flag, check);
  }
  return is_host;
}
}  // namespace ivshm
