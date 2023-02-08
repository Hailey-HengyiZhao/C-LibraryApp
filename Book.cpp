#include <iostream>
#include <iomanip>
#include "Book.h"
#include "Utils.h"

using namespace std;

namespace sdds{
   Book::Book(){
      m_author = nullptr;
   }
   Book::Book(const Book& toCopy) :Publication{toCopy} {
      m_author = ut.alcpy(toCopy.m_author);
   }
   Book& Book::operator=(const Book& toAssign){
      if (this != &toAssign) {
         Publication::operator=(toAssign);
         delete[] m_author;
         m_author = nullptr;
         m_author = ut.alcpy(toAssign.m_author);
      }
      return *this;
   }

   Book::~Book(){
      delete[] m_author;
      m_author = nullptr;
   }

   void Book::set(int member_id){
      Publication::set(member_id);
      Publication::resetDate();
   }

   Book::operator bool() const{
      return m_author != nullptr && m_author[0] != 0 && Publication::operator bool();
   }

   char Book::type() const{
      return 'B';
   }

   std::ostream& Book::write(std::ostream& ostr) const{
      int count{};
      Publication::write(ostr);
      if (operator bool()) {
         if (conIO(ostr)) {
            ostr << " ";
            if (ut.strlen(m_author) >= SDDS_AUTHOR_WIDTH) {
               while (count < SDDS_AUTHOR_WIDTH) {
                  ostr << m_author[count];
                  count++;
               }
            }
            else ostr <<setw(SDDS_AUTHOR_WIDTH) << setfill(' ') << m_author;
            ostr << " |";
         }
         else ostr << "\t" << m_author;
      }
      return ostr;
   }

   std::istream& Book::read(std::istream& istr){
      Publication::read(istr);
      if (!istr.fail()) {
         char* temp_author{};
         delete[] m_author;
         m_author = nullptr;
         if (conIO(istr)) {
            istr.ignore(1, '\n');
            cout << "Author: ";
            temp_author = ut.getDynCstr(istr);
         }
         else {
            istr.ignore(1,'\t');
            temp_author = new char[256];
            istr.get(temp_author, 256);
         }
         if (istr.fail()) {
            istr.setstate(ios::failbit);
         }
         else {
            m_author = ut.alcpy(temp_author, 256);
            delete[] temp_author;
         }
      }
      else {
         cout << "Author: ";
         istr.clear();
         istr.setstate(ios::failbit);
      }
      return istr;
   }


}