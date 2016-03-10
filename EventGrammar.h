#pragma once

#include "RawMsg.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>

namespace book
{
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;

  template <typename Iterator>
  struct EventGrammar : qi::grammar<Iterator, Msg(), ascii::space_type>
  {
      EventGrammar() : EventGrammar::base_type(message)
      {
          
          using qi::uint_;
          using qi::lit;
          using qi::double_;
          using qi::lexeme;
          using ascii::char_;

          add_message %=
              lit("A")
              >> ","
              >>  uint_ >> ','
              >>  char_ >> ","
              >>  uint_ >> ','
              >>  double_
              ;

          modify_message %=
              lit("M")
              >> ","
              >> uint_ >> ","
              >> char_ >> ","
              >> uint_ >> ","
              >> double_
              ;

          cancel_message %=
              lit("X")
              >> ","
              >> uint_ >> ","
              >> char_ >> ","
              >> uint_ >> ","
              >> double_
              ;

          trade_message %=
              lit("T")
              >> ","
              >> uint_    >> ","
              >> double_
              ;

          message = (modify_message | add_message | cancel_message | trade_message);
      }

      qi::rule<Iterator, AddOrderMsg(),     ascii::space_type> add_message;
      qi::rule<Iterator, ModifyOrderMsg(),  ascii::space_type> modify_message;
      qi::rule<Iterator, CancelOrderMsg(),  ascii::space_type> cancel_message;
      qi::rule<Iterator, TradeMsg(),        ascii::space_type> trade_message;
      qi::rule<Iterator, Msg(),             ascii::space_type> message;
  };
} // book
