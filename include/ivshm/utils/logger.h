// Copyright [2026] <pinecone>
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

#include <functional>
#include <string>
#include <ylt/easylog.hpp>
namespace ivshm {

/** @brief Unique identifier for the ivshm internal logger instance. */
constexpr std::size_t k_log_id = 0xff;

/** @brief The underlying logger instance type. */
using Logger = easylog::logger<k_log_id>;

/** @brief Log severity levels (TRACE, DEBUG, INFO, WARN, ERROR, CRITICAL,
 * FATAL). */
using Severity = easylog::Severity;

/** @brief Data structure representing a single log record. */
using Record = easylog::record_t;

/**
 * @brief Global initialization of the logging system.
 * @param min_severity The minimum severity level to be recorded.
 * @param filename Target log file path. If empty, file output is disabled.
 * @param async Whether to enable asynchronous logging thread.
 * @param enable_console Whether to output logs to the console/terminal.
 * @param max_file_size Maximum size per log file (in bytes) before rotation.
 * @param max_files Maximum number of rotated log files to retain.
 * @param flush_every_time If true, forces a flush after every write operation.
 */
auto InitLog(Severity min_severity, const std::string &filename = "",
             bool async = true, bool enable_console = true,
             std::size_t max_file_size = 0, std::size_t max_files = 0,
             bool flush_every_time = false) -> void;

/** @brief Dynamically updates the runtime minimum severity filter. */
auto SetMinSeverity(Severity severity) -> void;

/** @brief Retrieves the current minimum severity filter. */
auto GetMinSeverity() -> Severity;

/** @brief Configures the sampling duration to prevent log flooding. */
auto SetSampleDuration(std::chrono::milliseconds milliseconds) -> void;

/** @brief Registers a custom callback (appender) for post-processing log
 * records. */
auto AddAppenders(std::function<void(Record &record)> app) -> void;

/** @brief Sets the interval for sampled logging. */
void SetSampleInterval(std::chrono::milliseconds milliseconds);

/** @brief Enables or disables console (stdout) logging output at runtime. */
auto SetConsole(bool enable) -> void;

/** @brief Checks if console logging is currently enabled. */
auto GetConsole() -> bool;

/** @brief Enables or disables asynchronous logging mode at runtime. */
auto SetAsync(bool enable) -> void;

/** @brief Checks if asynchronous logging is currently enabled. */
auto GetAsync() -> bool;

/** @brief Manually flushes the log buffers to persistent storage. */
auto Flush() -> void;

/** @brief Safely shuts down the asynchronous logging thread. */
auto StopAsyncLog() -> void;

/** @brief Helper to add a custom log appender. */
auto AddAppender(std::function<void(Record &)> function) -> void;

}  // namespace ivshm

#define IVSHM_LOG_IMPL(severity, ...)                        \
  if (!ivshm::Logger::instance().check_severity(severity)) { \
    ;                                                        \
  } else if (auto ctm = std::chrono::system_clock::now();    \
             ivshm::Logger::instance().check_tm(ctm))        \
  ivshm::Logger::instance() +=                               \
      ivshm::Record(ctm, severity, GET_STRING(__FILE__, __LINE__)).ref()

#ifndef IVSHM_LOG
#define IVSHM_LOG(severity, ...) \
  IVSHM_LOG_IMPL(ivshm::Severity::severity, __VA_ARGS__, 0)
#endif

#define IVSHM_LOGV_IMPL(severity, fmt, ...)                        \
  if (!ivshm::Logger::instance().check_severity(severity)) {       \
    ;                                                              \
  } else if (auto ctm = std::chrono::system_clock::now();          \
             ivshm::Logger::instance().check_tm(ctm))              \
  ivshm::Logger::instance() +=                                     \
      ivshm::Record(ctm, severity, GET_STRING(__FILE__, __LINE__)) \
          .sprintf(fmt, __VA_ARGS__)

#if __has_include(<fmt/format.h>) || __has_include(<format>)

#define IVSHM_LOGFMT_IMPL0(severity, ...)                          \
  if (!ivshm::Logger::instance().check_severity(severity)) {       \
    ;                                                              \
  } else if (auto ctm = std::chrono::system_clock::now();          \
             ivshm::Logger::instance().check_tm(ctm))              \
  ivshm::Logger::instance() +=                                     \
      ivshm::Record(ctm, severity, GET_STRING(__FILE__, __LINE__)) \
          .format(std::format(__VA_ARGS__))

/**
 * @def IVSHM_LOGFMT
 * @brief Modern log macro using C++20/23 std::format style.
 * @param severity One of TRACE, DEBUG, INFO, WARN, ERROR, CRITICAL, FATAL.
 * @param ... Format string and its corresponding arguments.
 */
#ifndef IVSHM_LOGFMT
#define IVSHM_LOGFMT(severity, ...) \
  IVSHM_LOGFMT_IMPL0(ivshm::Severity::severity, __VA_ARGS__)
#endif

#ifndef IVSHM_MLOGFMT
#define IVSHM_MLOGFMT(severity, ...) \
  IVSHM_LOGFMT_IMPL0(ivshm::Severity::severity, __VA_ARGS__)
#endif

/**
 * @def IVSHM_LOG_TAG
 * @brief Logs a formatted message prefixed with a specific tag.
 * @param severity Log severity level.
 * @param tag A string identifier for grouping logs (e.g., "VM_IO").
 * @param format Format string.
 */
#ifndef IVSHM_LOG_TAG
#define IVSHM_LOG_TAG(severity, tag, format, ...)                    \
  IVSHM_LOGFMT_IMPL0(ivshm::Severity::severity, "[{}] " format, tag, \
                     __VA_ARGS__)
#endif

#endif

#ifndef IVSHM_LOG_TRACE
#define IVSHM_LOG_TRACE IVSHM_LOG(TRACE)
#endif
#ifndef IVSHM_LOG_DEBUG
#define IVSHM_LOG_DEBUG IVSHM_LOG(DEBUG)
#endif
#ifndef IVSHM_LOG_INFO
#define IVSHM_LOG_INFO IVSHM_LOG(INFO)
#endif
#ifndef IVSHM_LOG_WARN
#define IVSHM_LOG_WARN IVSHM_LOG(WARN)
#endif
#ifndef IVSHM_LOG_ERROR
#define IVSHM_LOG_ERROR IVSHM_LOG(ERROR)
#endif
#ifndef IVSHM_LOG_CRITICAL
#define IVSHM_LOG_CRITICAL IVSHM_LOG(CRITICAL)
#endif
#ifndef IVSHM_LOG_FATAL
#define IVSHM_LOG_FATAL IVSHM_LOG(FATAL)
#endif

#ifndef IVSHM_LOGT
#define IVSHM_LOGT IVSHM_LOG_TRACE
#endif
#ifndef IVSHM_LOGD
#define IVSHM_LOGD IVSHM_LOG_DEBUG
#endif
#ifndef IVSHM_LOGI
#define IVSHM_LOGI IVSHM_LOG_INFO
#endif
#ifndef IVSHM_LOGW
#define IVSHM_LOGW IVSHM_LOG_WARN
#endif
#ifndef IVSHM_LOGE
#define IVSHM_LOGE IVSHM_LOG_ERROR
#endif
#ifndef IVSHM_LOGC
#define IVSHM_LOGC IVSHM_LOG_CRITICAL
#endif
#ifndef IVSHM_LOGF
#define IVSHM_LOGF IVSHM_LOG_FATAL
#endif
