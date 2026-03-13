///////////////////////////////////////////////////////////////////////////////////
//  Copyright (C), ${ORGANIZATION_NAME}, 2026
//  FileName:       channel.h
//  Author:         lanm
//  Version:        0.1.0.0
//  Date:           2026-03-13
//  Description:
//  Function List:
//  Input:
//  Output:
//  Notes:
//  History:
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <ivshm/utils/common.h>

namespace ivshm {

class Channel {
 public:
  constexpr Channel() noexcept = default;
  constexpr explicit Channel(const std::size_t id) noexcept : channel_(id) {}

  ~Channel() noexcept = default;

  constexpr auto GetTopic() const noexcept -> auto { return channel_; }

  constexpr auto ToString() const noexcept -> std::string_view {
    static const auto k_str = std::format("ivshm-{}.channel", channel_);
    return k_str;
  }

  static constexpr auto get_alias_field_names(Channel*) noexcept -> auto {
    return std::array{ylt::reflection::field_alias_t{"channel", 0}};
  }
  static constexpr auto get_alias_struct_name(Channel*) noexcept
      -> std::string_view {
    return "Channel";
  }
  YLT_REFL(Channel, channel_)

 private:
  std::size_t channel_;
};

}  // namespace ivshm
