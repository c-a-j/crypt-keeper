#pragma once

namespace ck::config {
  template <typename T>
  struct member_value;

  template <typename Owner, typename Value>
  struct member_value<Value Owner::*> {
    using type = Value;
  };

  template <typename T>
  using member_value_t = typename member_value<T>::type;
}
