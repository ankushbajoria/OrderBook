#pragma once

#include "RawMsg.h"

#include <inttypes.h>

class TickHandler {
 public:
  void handle(const book::TradeMsg& msg);

  // accessors
  double lastTradedPx() const;
  size_t accumulatedQty() const;
 
 private:
  double m_lastTickPx;
  size_t m_accumulatedQty;
};

inline
void
TickHandler::handle(const book::TradeMsg& msg) {
  if (m_lastTickPx != msg.prc) {
    m_lastTickPx      = msg.prc;
    m_accumulatedQty  = 0;
  }

  m_accumulatedQty += msg.qty;
}

inline
double
TickHandler::lastTradedPx() const {
  return m_lastTickPx;
}

inline
size_t
TickHandler::accumulatedQty() const {
  return m_accumulatedQty;
}
