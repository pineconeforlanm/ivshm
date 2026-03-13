// Copyright 2026 Lanm. All rights reserved.
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

#include <cstdint>
#include <expected>
#include <magic_enum/magic_enum.hpp>
#include <memory>
#include <source_location>
#include <string>
#include <string_view>

namespace ivshm {

/**
 * @class Error
 * @brief Represents a system error associated with a specific source code
 * location.
 * * This class leverages C++20's std::source_location to automatically capture
 * where an error was instantiated, providing rich context for debugging.
 */
class Error {
 public:
  /**
   * @enum Kind
   * @brief High-level categories of errors.
   */
  enum class Kind : std::uint8_t {
    Unknown,           ///< An undefined or unexpected error occurred.
    InvalidArgument,   ///< A provided argument does not meet the requirements.
    PermissionDenied,  ///< The operation failed due to insufficient privileges.
  };

  /**
   * @brief Constructs an Error object.
   * @param kind The category of the error.
   * @param location The source location where the error occurred (defaults to
   * the caller's location).
   */
  constexpr explicit Error(
      const Kind kind,
      const std::source_location location = std::source_location::current())
      : kind_(kind), location_(location) {}

  /**
   * @brief Gets the error category.
   * @return The Kind of error.
   */
  [[nodiscard]] auto GetErrorKind() const noexcept -> Kind { return kind_; }

  /**
   * @brief Accesses the raw source location.
   * @return The std::source_location object captured at construction.
   */
  [[nodiscard]] auto Location() const noexcept -> std::source_location {
    return location_;
  }

  /**
   * @brief Returns the full path of the source file where the error was raised.
   * @return A string_view containing the full file path.
   */
  [[nodiscard]] constexpr auto FullFileName() const noexcept
      -> std::string_view {
    return location_.file_name();
  }

  /**
   * @brief Extracts the file name from the full path (excluding directories).
   * @details This is computed at compile-time if the Error is created in a
   * constexpr context.
   * @return A string_view of the base file name.
   */
  [[nodiscard]] constexpr auto FileName() const noexcept -> std::string_view {
    const std::string_view full_path = location_.file_name();
    const std::size_t last_slash = full_path.find_last_of("/\\");
    return (last_slash == std::string_view::npos)
               ? full_path
               : full_path.substr(last_slash + 1);
  }

  /**
   * @brief Gets the line number in the source file.
   * @return 32-bit unsigned integer representing the line.
   */
  [[nodiscard]] constexpr auto Line() const noexcept -> std::uint32_t {
    return location_.line();
  }

  /**
   * @brief Gets the name of the function where the error occurred.
   * @return A string containing the function signature/name.
   */
  [[nodiscard]] constexpr auto FunctionName() const noexcept -> std::string {
    return location_.function_name();
  }

  /**
   * @brief Gets the column number in the source file.
   * @return 32-bit unsigned integer representing the column.
   */
  [[nodiscard]] constexpr auto Column() const noexcept -> std::uint32_t {
    return location_.column();
  }

  /**
   * @brief Generates a human-readable error message.
   * @param show_backtrace If true, appends stack trace information (if
   * supported).
   * @return A formatted string containing the error kind and location details.
   */
  [[nodiscard]] auto Message(bool show_backtrace = false) const -> std::string {
    if (!show_backtrace) {
      if (!magic_enum::enum_contains<Kind>(kind_)) {
        return std::format("Unknown error kind:{}",
                           static_cast<std::uint8_t>(kind_));
      }
      return std::format("Kind: [{}:{}] at {}:{}:{}",
                         static_cast<std::uint8_t>(kind_),
                         magic_enum::enum_name(static_cast<Kind>(kind_)),
                         FileName(), Line(), Column());
    }
    if (!magic_enum::enum_contains<Kind>(kind_)) {
      return std::format("Unknown error kind:{}\nbacktrace:\n{}",
                         static_cast<std::uint8_t>(kind_),
                         std::stacktrace::current());
    }
    return std::format("Kind: [{}:{}] at {}:{}:{}\nbacktrace:\n{}",
                       static_cast<std::uint8_t>(kind_),
                       magic_enum::enum_name(kind_), FileName(), Line(),
                       Column(), std::stacktrace::current());
  }

 private:
  Kind kind_{Kind::Unknown};       ///< Internal error category.
  std::source_location location_;  ///< Captured source metadata.
};

/** @brief Alias for the error category enumeration. */
using Kind = Error::Kind;

/** * @brief A type that represents either a value of type T or an Error of type
 * E.
 * @tparam T The type of the successful value. Defaults to void for
 * "success-only" signals.
 * @tparam E The type of the error object. Defaults to ivshm::Error.
 */
template <class T = void, class E = Error>
using expected = std::expected<T, E>;

/** @brief Helper to wrap an error value into a std::unexpected. */
template <class T = Error>
using unexpected = std::unexpected<T>;

/** @brief Type used to trigger the unexpected path in expected's constructor.
 */
using unexpect_t = std::unexpect_t;

/** @brief Global constant to indicate an error-state construction for
 * 'expected'. */
static constexpr unexpect_t unexpect{};  // NOLINT

/**
 * @brief Reads the entire content of a file into a string.
 * @param path The filesystem path to the target file.
 * @return An 'expected' containing the file content string on success, or an
 * Error on failure.
 */
auto ReadFileContent(const std::string &path) -> expected<std::string>;

/**
 * @brief Detects if the current execution environment is the Host system.
 * @return True if running on Host, false if running within a Guest VM.
 */
auto IsHost() -> bool;

}  // namespace ivshm
