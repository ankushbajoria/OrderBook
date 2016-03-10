#pragma once

#include "LevelBook.h"
#include "QuoteHandler.h"
#include "TickHandler.h"

#include <boost/variant.hpp>
#include <iostream>

namespace book {
class Handler : public boost::static_visitor<void> {
 public:
  template <typename T>
  void operator() (T)
  { }

  void setBook(LevelBook* book);

 private:
  QuoteHandler  m_quoteHandler;
  TickHandler   m_tickHandler;
};

inline
void
Handler::setBook(LevelBook* book) {
  m_quoteHandler.setBook(book);
}

template <>
inline void
Handler::operator() <AddOrderMsg> (AddOrderMsg msg) {
  using boost::fusion::operators::operator<<;
  // std::cout << "Add: " << msg << std::endl;

  if (not m_quoteHandler.processAdd(msg)) {
  }
}

template <>
inline void
Handler::operator() <ModifyOrderMsg> (ModifyOrderMsg msg) {
  using boost::fusion::operators::operator<<;
  // std::cout << "Modify: " << msg << std::endl;

  if (not m_quoteHandler.processModify(msg)) {
  }
}

template <>
inline void
Handler::operator() <CancelOrderMsg> (CancelOrderMsg msg) {
  using boost::fusion::operators::operator<<;
  // std::cout << "Cancel: " << msg << std::endl;

  if (not m_quoteHandler.processCancel(msg)) {
  }
}

template <>
inline void
Handler::operator() <TradeMsg> (TradeMsg msg) {
  m_tickHandler.handle(msg);

#if 0
  std::cout
    << m_tickHandler.accumulatedQty()
    << " @ " 
    << m_tickHandler.lastTradedPx() 
    << std::endl;
#endif

}

}
