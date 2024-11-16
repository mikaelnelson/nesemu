#pragma once

template <typename AddressWidth, typename DataWidth>
class IBus {
 public:
  virtual ~IBus() = default;

  virtual DataWidth read(const AddressWidth address) const = 0;
  virtual void write(const AddressWidth address, const DataWidth data) = 0;
};
