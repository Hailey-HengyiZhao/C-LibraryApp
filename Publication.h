
#ifndef SDDS_PUBLICATION_H
#define SDDS_PUBLICATION_H
#include "Date.h"
#include "Lib.h"
#include "Streamable.h"

namespace sdds {
   class Publication: public Streamable{
      char* m_title{};
      char m_shelfId[SDDS_SHELF_ID_LEN + 1];
      int m_membership {};
      int m_libRef {};
      Date m_date {};
      void setEmpty();
   public:
      Publication();
      Publication(const Publication& pulication); //rule of three
      virtual ~Publication();//rule of three

      virtual void set(int member_id);
      void setRef(int value);
      void resetDate();

      virtual char type()const;
      bool onLoan()const;
      Date checkoutDate()const;
      bool operator==(const char* title)const;
      Publication& operator=(const Publication& PU);//rule of three
      operator const char* ()const;
      int getRef()const;

      bool conIO(std::ios& io)const;
      virtual std::ostream& write(std::ostream& os = std::cout) const;
      virtual std::istream& read(std::istream& istr= std::cin);
      operator bool() const;
      
   };
   //helper
   std::ostream& operator<<(std::ostream& os, const Publication& P);
   std::istream& operator>>(std::istream& is, Publication& P);
}
#endif // !SDDS_PUBLICATION_H
