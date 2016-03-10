#pragma once

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/variant.hpp>

namespace book
{
    struct AddOrderMsg {
      uint32_t orderid;
      char     side;
      size_t   qty;
      double   prc;
    };

    struct ModifyOrderMsg {
      uint32_t orderid;
      char     side;
      size_t   qty;
      double   prc;
    };

    struct CancelOrderMsg {
      uint32_t orderid;
      char     side;
      size_t   qty;
      double   prc;
    };

    struct TradeMsg {
      size_t qty;
      double prc;
    };

    typedef boost::variant<
      AddOrderMsg,
      ModifyOrderMsg,
      CancelOrderMsg,
      TradeMsg> Msg;

} // book

BOOST_FUSION_ADAPT_STRUCT(
    book::AddOrderMsg,
    (uint32_t, orderid)
    (char,     side)
    (size_t,   qty)
    (double,   prc)
)

BOOST_FUSION_ADAPT_STRUCT(
    book::ModifyOrderMsg,
    (uint32_t, orderid)
    (char,     side)
    (size_t,   qty)
    (double,   prc)
)

BOOST_FUSION_ADAPT_STRUCT(
    book::CancelOrderMsg,
    (uint32_t, orderid)
    (char,     side)
    (size_t,   qty)
    (double,   prc)
)

BOOST_FUSION_ADAPT_STRUCT(
    book::TradeMsg,
    (size_t, qty)
    (double, prc)
)
//]


