#pragma once

template <typename T>
class IObserver {
 public:
  virtual ~IObserver() = default;

  virtual void update(const T& data) = 0;
};
