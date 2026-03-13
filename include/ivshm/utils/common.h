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

#include <ylt/struct_json/json_writer.h>
#include <ylt/struct_json/json_reader.h>
#include <ylt/struct_xml/xml_writer.h>
#include <ylt/struct_xml/xml_reader.h>
#include <ylt/struct_yaml/yaml_writer.h>
#include <ylt/struct_yaml/yaml_reader.h>

#include <string>
#include <vector>
#include <ylt/struct_pack.hpp>
#include <ylt/struct_pb.hpp>

namespace ivshm {

/**
 * @enum FormatMode
 * @brief Specifies the output format for the custom std::formatter.
 */
enum class FormatMode : std::uint8_t {
  JSON = 0,  ///< JavaScript Object Notation (Default).
  DEFAULT = JSON,
  YAML = 1,  ///< YAML Ain't Markup Language.
  XML = 2,   ///< Extensible Markup Language.
  PACK = 3,  ///< High-performance binary serialization (StructPack).
  PB = 4,    ///< Google Protocol Buffers.
  PROTOBUF = PB,
};

/**
 * @concept Serializable
 * @brief Requirements for a type to be compatible with ivshm serialization.
 * @details A type must be compatible with yalantinglibs (struct_json,
 * struct_yaml, struct_xml, struct_pack, and struct_pb) to satisfy this concept.
 */
template <typename T>
concept Serializable = requires(T type, std::string string) {
  struct_json::to_json(type, string);
  struct_yaml::to_yaml(type, string);
  struct_xml::to_xml(type, string);
  struct_pack::serialize_to(type, string);
  struct_pb::to_pb(type, string);
};
}  // namespace ivshm

/**
 * @brief Template specialization of std::formatter for any ivshm::Serializable
 * type.
 * @details Allows printing structured objects using std::format with
 * specifiers:
 * - `{:j}` : JSON (default)
 * - `{:y}` : YAML
 * - `{:x}` : XML
 * - `{:b}` : Binary (StructPack, hex output)
 * - `{:p}` : Protobuf (hex output)
 * * Example: `std::format("{:j}", my_struct_obj);`
 */
template <ivshm::Serializable T>
struct std::formatter<T> {
  ivshm::FormatMode mode = ivshm::FormatMode::DEFAULT;
  /**
   * @brief Parses the format specifier in the format string.
   * @param ctx The parse context.
   * @return Iterator to the end of the format specifier.
   * @throws std::format_error if the specifier is invalid.
   */
  constexpr auto parse(format_parse_context &ctx) {  // NOLINT
    const auto *iter = ctx.begin();
    if (iter == ctx.end() || *iter == '}') return iter;

    switch (*iter) {
      case 'j':
        mode = ivshm::FormatMode::JSON;
        break;
      case 'y':
        mode = ivshm::FormatMode::YAML;
        break;
      case 'x':
        mode = ivshm::FormatMode::XML;
        break;
      case 'b':
        mode = ivshm::FormatMode::PACK;
        break;
      case 'p':
        mode = ivshm::FormatMode::PB;
        break;
      default:
        throw format_error(
            "Invalid format specifier: 'j'son, 'y'aml, 'x'ml, 'b'inary, 'p'b "
            "supported.");
    }

    iter++;
    if (iter != ctx.end() && *iter != '}') {
      throw format_error("Invalid format specifier syntax.");
    }
    return iter;
  }

  /**
   * @brief Formats the object based on the parsed mode.
   * @param obj The serializable object.
   * @param ctx The format context.
   * @return The output iterator.
   */
  auto format(const T &obj, format_context &ctx) const {  // NOLINT
    std::string out_str;
    switch (mode) {
      case ivshm::FormatMode::YAML:
        struct_yaml::to_yaml(obj, out_str);
        break;
      case ivshm::FormatMode::XML:
        struct_xml::to_xml(obj, out_str);
        break;
      case ivshm::FormatMode::PACK:
        out_str = ToHexString(struct_pack::serialize<std::string>(obj));
        break;
      case ivshm::FormatMode::PB:
        struct_pb::to_pb(obj, out_str);
        out_str = ToHexString(out_str);
        break;
      case ivshm::FormatMode::JSON:
      default:
        struct_json::to_json(obj, out_str);
        break;
    }
    return format_to(ctx.out(), "{}", out_str);
  }

 private:
  /**
   * @brief Utility to convert binary data to a hex string for readable output.
   * @param binary The raw binary string.
   * @return A hex-encoded string.
   */
  static auto ToHexString(const std::string &binary) -> std::string {
    std::string res;
    res.reserve(binary.size() * 2);
    for (const auto &item : binary) {
      constexpr std::array k_hex_chars = {'0', '1', '2', '3', '4', '5',
                                          '6', '7', '8', '9', 'A', 'B',
                                          'C', 'D', 'E', 'F'};
      res.push_back(k_hex_chars[item >> 4]);
      res.push_back(k_hex_chars[item & 0x0F]);  // NOLINT
    }
    return res;
  }
};  // namespace std
