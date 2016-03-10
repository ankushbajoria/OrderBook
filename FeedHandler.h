#include "EventGrammar.h"
#include "Handler.h"
#include "LevelBook.h"

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>

class FeedHandler {
 public:
   FeedHandler();

   void processMessage(const std::string &line);
   void printCurrentOrderBook(std::ostream& os) const;

  private:
   book::EventGrammar<std::string::const_iterator> m_grammar{};
   book::Handler    m_handler{};

   book::LevelBook  m_book;
};

inline
FeedHandler::FeedHandler() {
  m_handler.setBook(&m_book);
}

inline
void
FeedHandler::processMessage(const std::string& line)
{
  using boost::spirit::ascii::space;
  using boost::spirit::qi::phrase_parse;
  book::Msg msg;

  auto iter = line.begin();
  auto end  = line.end();

  auto ret = phrase_parse(iter, end, m_grammar, space, msg);

  if (ret && iter == end)
  {
    boost::apply_visitor(m_handler, msg);
  }
}

inline
void
FeedHandler::printCurrentOrderBook(std::ostream& os) const {
  m_book.printBook(os, 5);
}
