#pragma once

#include "Order.h"

namespace book {

struct IntPriceUtil {
  static const uint32_t INT_PRICE_MULTIPLIER = 10000ul;

  static uint64_t convertToInt(double prc) { return prc * INT_PRICE_MULTIPLIER; }
  static double   convertToDouble(uint64_t prc) { return static_cast<double>(prc) / INT_PRICE_MULTIPLIER; }
};

class LevelBook {
 public:
  enum Side {
    SELL  = 0,
    BUY   = 1,
    NUM_SIDES = 2,
  };

 private:
  struct Level {
    Side      side;
    uint64_t  levelPx;
    size_t    levelSz;
    size_t    numOrders;

    Level(Side _side, uint64_t _prc, size_t qty)
      : side(_side),
        levelPx(_prc),
        levelSz(qty),
        numOrders(1)
    { }

    friend bool operator< (const Level& l1, const Level& l2) {
      return  (l1.side == BUY  && l1.levelPx > l2.levelPx) ||
              (l1.side == SELL && l1.levelPx < l2.levelPx);
    }
  };

  using LevelBookHalf = std::vector<Level*>;
  LevelBookHalf m_levels[NUM_SIDES];

 public:
  void addOrder(Order* order);
  void modifyOrder(Order* order, size_t oldQty, double oldPx);
  void removeOrder(Order* order);

  void printBook(std::ostream& os, size_t nLevels) const;

 private:
  void updateLevel(Level* level, Order* order);
  void insertLevel(uint64_t prc, size_t qty, Side side);
};

inline
void
LevelBook::updateLevel(Level* level, Order* order) {
  level->levelSz += order->qty;
  ++level->numOrders;

  return;
}

inline
void
LevelBook::insertLevel(uint64_t prc, size_t qty, Side side) {
  auto level = new Level(side, prc, qty);

  auto iter = m_levels[side].begin();
  while (iter != m_levels[side].end() && *(*iter) < *level) {
    ++iter;
  }

  m_levels[side].insert(iter, level);

  return;
}

inline
void
LevelBook::addOrder(Order* order) {
  Side side     = static_cast<Side>(order->side == 'B');
  auto intPrice = IntPriceUtil::convertToInt(order->prc);

  auto& bookHalf = m_levels[side];

  for (auto level : bookHalf) {
    if (level->levelPx == intPrice) {
      updateLevel(level, order);

      return;
    }
  }

  insertLevel(intPrice, order->qty, side);
}

inline
void
LevelBook::modifyOrder(Order* order, size_t oldQty, double oldPx) {
  size_t newqty   = order->qty;
  double newprc   = order->prc;

  order->qty      = oldQty;
  order->prc      = oldPx;

  // probably this is a bit inefficient, will have to do for now
  removeOrder(order);

  order->qty = newqty;
  order->prc = newprc;

  addOrder(order);

  return;
}

inline
void
LevelBook::removeOrder(Order* order) {
  Side side       = static_cast<Side>(order->side == 'B');
  auto intPrice   = IntPriceUtil::convertToInt(order->prc);

  auto iter = m_levels[side].begin();
  while (iter != m_levels[side].end()) {
    auto level = *iter;

    if (level->levelPx == intPrice) {
      level->levelSz -= order->qty;
      --level->numOrders;

      if (level->levelSz == 0) {
        m_levels[side].erase(iter);
      }

      return;
    }

    iter++;
  }

  return;
}

inline
void
LevelBook::printBook(std::ostream& os, size_t nLevels) const {
  size_t doubleSidedTillDepth = std::min(m_levels[0].size(), m_levels[1].size());

  std::string emptyLevelBuy   = "[\t0 ]\t( 0 )\t0.00\t";
  std::string emptyLevelSell  = "0.00\t( 0 )\t[\t0 ]";

  size_t i = 0;
  for (; i < doubleSidedTillDepth && i < nLevels; i++) {
    auto buyLevel   = m_levels[BUY][i];
    auto sellLevel  = m_levels[SELL][i];

    os << "[\t"   << buyLevel->numOrders << " ]"
       << "\t( "   << buyLevel->levelSz   << " )"
       << "\t"    << IntPriceUtil::convertToDouble(buyLevel->levelPx)   << "\t|"
       << "\t"    << IntPriceUtil::convertToDouble(sellLevel->levelPx)  << "\t"
       << "( "     << sellLevel->levelSz   << " )\t"
       << "[\t"   << sellLevel->numOrders << " ]"
       << std::endl;
  }

  if (i == nLevels) {
    os << "----------------------\n" << std::endl;
    return;
  }
  
  auto side = m_levels[BUY].size() == i ? SELL : BUY;
  auto oneSidedLevels = std::min(nLevels, m_levels[side].size());

  for (size_t j = i; j < oneSidedLevels; j++) {
    auto level = m_levels[side][j];

    if (side == SELL) {
      os << emptyLevelBuy << "|\t" << IntPriceUtil::convertToDouble(level->levelPx)
         << "\t( " << level->levelSz   << " )\t"
         << "[\t" << level->numOrders << " ]" << std::endl;
    } else {
      os << "[\t" << level->numOrders << " ]" 
         << "\t( " << level->levelSz   << " )\t"
         << IntPriceUtil::convertToDouble(level->levelPx) << "\t|"
         << "\t" << emptyLevelSell << std::endl;
    }
  }

  os << "----------------------\n" << std::endl;
}

} // namespace book
