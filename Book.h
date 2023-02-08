
#ifndef SDDS_BOOK_H
#define SDDS_BOOK_H
#include "Publication.h"

namespace sdds {
   class Book : public Publication {
      char* m_author{};
   public:
      Book();
      //Book(const char* name);
      //rule of three
      Book(const Book& toCopy);
      Book& operator=(const Book& toAssign);
      virtual ~Book();
      void set(int member_id);
      operator bool() const;
      virtual char type()const;
      std::ostream& write(std::ostream& ostr = std::cout) const;
      std::istream& read(std::istream& istr = std::cin);
   };
}
#endif // ! SDDS_BOOK_H 
