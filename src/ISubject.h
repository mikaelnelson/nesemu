#pragma once

#include <memory>
#include <vector>

#include "IObserver.h"

template <typename T>
class ISubject {
 public:
  virtual ~ISubject() = default;

  void add_observer(std::shared_ptr<IObserver<T>> observer) {
    observers.push_back(observer);
  }

  void remove_observer(std::shared_ptr<IObserver<T>> observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer),
                    observers.end());
  }

  void notify_observers(const T data) {
    for (auto& observer : observers) {
      observer->update(data);
    }
  }

  size_t observers_count() const { return observers.size(); }

 private:
  std::vector<std::shared_ptr<IObserver<T>>> observers;
};
