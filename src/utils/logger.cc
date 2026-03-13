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

#include <chrono>
#include <functional>
#include <string>
#include <utility>

namespace ivshm {
auto InitLog(Severity min_severity, const std::string &filename, bool async,
             bool enable_console, std::size_t max_file_size,
             std::size_t max_files, bool flush_every_time) -> void {
  Logger::instance().init(min_severity, async, enable_console, filename,
                          max_file_size, max_files, flush_every_time);
}

auto SetMinSeverity(Severity severity) -> void {
  Logger::instance().set_min_severity(severity);
}

auto GetMinSeverity() -> Severity {
  return Logger::instance().get_min_severity();
}

auto SetSampleDuration(std::chrono::milliseconds milliseconds) -> void {
  Logger::instance().set_sample_duration(milliseconds);
}

auto AddAppenders(std::function<void(Record &record)> app) -> void {
  Logger::instance().add_appender(std::move(app));
}

void SetSampleInterval(std::chrono::milliseconds milliseconds) {
  Logger::instance().set_sample_interval(milliseconds);
}

auto SetConsole(bool enable) -> void { Logger::instance().set_console(enable); }

auto GetConsole() -> bool { return Logger::instance().get_console(); }

auto SetAsync(bool enable) -> void { Logger::instance().set_async(enable); }

auto GetAsync() -> bool { return Logger::instance().get_async(); }

auto Flush() -> void { Logger::instance().flush(); }

auto StopAsyncLog() -> void { Logger::instance().stop_async_log(); }

auto AddAppender(std::function<void(Record &)> function) -> void {
  Logger::instance().add_appender(std::move(function));
}
}  // namespace ivshm
