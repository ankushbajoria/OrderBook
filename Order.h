#pragma once

#include <inttypes.h>

namespace book {

struct Order {
  uint32_t  orderid;
  char      side;
  size_t    qty;
  double    prc;

  Order(uint32_t _orderid, char _side, size_t _qty, double _prc)
    : orderid(_orderid),
      side(_side),
      qty(_qty),
      prc(_prc)
  { }
};

} // namespace book
