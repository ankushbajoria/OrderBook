#pragma once

#include "Order.h"
#include "LevelBook.h"
#include "RawMsg.h"

#include <unordered_map>

namespace book {

class QuoteHandler {
 public:
  bool processAdd(const AddOrderMsg& msg);
  bool processModify(const ModifyOrderMsg& msg);
  bool processCancel(const CancelOrderMsg& msg);

  void setBook(LevelBook* book) {
    m_book = book; 
  }

 private:
   using OrderMap = std::unordered_map<uint32_t, Order*>;
   OrderMap   m_orderMap;

   LevelBook* m_book;
};

inline
bool
QuoteHandler::processAdd(const AddOrderMsg& msg) {
  if (m_orderMap.count(msg.orderid) != 0) {
    // LOG duplicate refnum
    return false;
  }

  auto order = new Order(msg.orderid, msg.side, msg.qty, msg.prc);

  m_orderMap.insert(std::make_pair(msg.orderid, order));
  m_book->addOrder(order);

  return true;
}

inline
bool
QuoteHandler::processModify(const ModifyOrderMsg& msg) {
  if (m_orderMap.count(msg.orderid) == 0) {
    // LOG missing orderid
    return false;
  }

  auto order  = m_orderMap[msg.orderid];
  auto oldPx  = order->prc;
  auto oldQty = order->qty;

  order->prc  = msg.prc;
  order->qty  = msg.qty;
  order->side = msg.side;

  m_book->modifyOrder(order, oldQty, oldPx); 

  return true;
}

inline
bool
QuoteHandler::processCancel(const CancelOrderMsg& msg) {
  if (m_orderMap.count(msg.orderid) == 0) {
    // LOG missing orderid
    return false;
  }

  auto order = m_orderMap[msg.orderid];
  m_orderMap.erase(msg.orderid);
  m_book->removeOrder(order);

  delete order;

  return true;
}

} // namespace book
