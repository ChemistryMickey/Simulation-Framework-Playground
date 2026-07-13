#pragma once
#include <optional>

template <typename T> class LateRef {
  std::optional<std::reference_wrapper<T>> ref;

public:
  LateRef() = default;
  LateRef(T &t) : ref(t) {}

  bool has_value() const noexcept { return ref.has_value(); }

  explicit operator bool() const noexcept { return has_value(); }

  T &operator*() { return ref->get(); }

  const T &operator*() const { return ref->get(); }

  T *operator->() { return &ref->get(); }

  const T *operator->() const { return &ref->get(); }
};
