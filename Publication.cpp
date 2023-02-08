
#include <iostream>
#include <iomanip>
#include <cstring>
#include "Publication.h"
#include "Utils.h"


using namespace std;

namespace sdds {
   void Publication::setEmpty(){
      m_title = nullptr;
      m_shelfId[0] = 0;
      m_membership = 0;
      m_libRef = -1;
   }
   Publication::Publication() {
      setEmpty();
   }

   Publication::Publication(const Publication& pulication){
      operator=(pulication);
   }

   Publication::~Publication() {
         delete[] m_title;
         m_title = nullptr;
   }

   void Publication::set(int member_id) {
      // Sets the membership attribute to either zero or a five-digit integer.
      m_membership = member_id;
   }

   void Publication::setRef(int value) {
      // Sets the **libRef** attribute value
      m_libRef = value;
   }

   void Publication::resetDate() {
      // Sets the date to the current date of the system.
      m_date.setCurrentDay();
   }

   char Publication::type() const {
      //Returns the character 'P' to identify this object as a "Publication object"
      return 'P';
   }

   bool Publication::onLoan() const {
      //Returns true is the publication is checkout (membership is non-zero)
      return m_membership != 0;
   }

   Date Publication::checkoutDate() const {
      //Returns the date attribute
      return m_date;
   }

   bool Publication::operator==(const char* title) const{
      //Returns true if the argument title appears anywhere in the title of the 
      //publication. Otherwise, it returns false; (use strstr() function in <cstring>)
      return m_title != nullptr && ut.strstr(m_title, title) != NULL;
   }

   Publication& Publication::operator=(const Publication& PU){
      if (this != &PU && PU.m_title != nullptr) {
         delete[] m_title;
         m_title = nullptr;
         m_title = ut.alcpy(PU.m_title);
         ut.strcpy(m_shelfId, PU.m_shelfId);
         m_date = PU.m_date;
         m_libRef = PU.m_libRef;
         m_membership = PU.m_membership;
      }
      return *this;
   }

   Publication::operator const char* () const {
      //Returns the title attribute
      return m_title;
   }

   int Publication::getRef() const {
      //Returns the libRef attirbute. 
      return m_libRef;
   }

   ostream& Publication::write(ostream& os) const {
      /*               1         2         3         4         5         6         7
              1234567890123456789012345678901234567890123456789012345678901234567890
              | P001 | The Toronto Star.............. | 34037 | 2021 / 11 / 17 |
              | P007 | Macleans Magazine............. | N / A | 2021 / 11 / 11 |*/
      int count{};
      if (operator bool())
      {
         if (conIO(os)){
            os << "| " << m_shelfId << " | ";
            if(ut.strlen(m_title) >= SDDS_TITLE_WIDTH){
               while (count < SDDS_TITLE_WIDTH) {
                  os << m_title[count];
                  count++;
               }
            }
            else {
               os << setw(30) << std::left << setfill('.') << m_title;
            }
            os << " | ";
            if(m_membership ==0){
               os << " N/A ";
               
            }else{
               os << m_membership ;
            }
            os << " | " << m_date << " |" ;
         }
         else{
            os << type() << "\t" << m_libRef << "\t" << m_shelfId << "\t"
               << m_title << "\t" << m_membership << "\t"
               << m_date;
         }
      }
      return os;
   }

   istream& Publication::read(istream& is) {

      delete[] m_title;
      m_title = nullptr;

      char temp_shelfId[SDDS_SHELF_ID_LEN + 1]{};
      char temp_title[1280]{};
      Date tempDate{};
      int temp_libRef{};
      int temp_membership{};

      if (conIO(is)) {
         cout << "Shelf No: ";
         is.getline(temp_shelfId, SDDS_SHELF_ID_LEN + 1,'\n');
         if (ut.strlen(temp_shelfId) != 4) {
            memset(temp_shelfId, '\0', SDDS_SHELF_ID_LEN + 1);
            cout << "Title: Date: ";
            is.setstate(ios::failbit);
         }
         else {
            cout << "Title: ";
            is.getline(temp_title, 1280, '\n');
            cout << "Date: ";
            is >> tempDate;
            if (tempDate.errCode() != 0) {
               is.clear();
               is.setstate(ios::failbit);
            }
         }
      }
      else {
         is >> temp_libRef;
         is.ignore(1, '\t');
         is >> temp_shelfId;
         is.ignore(1, '\t');
         is.getline(temp_title, 1280, '\t');
         is >> temp_membership;
         is.ignore(1, '\t');
         is >> tempDate;
      }

      if (temp_shelfId == nullptr || temp_title == nullptr || temp_title[0] == '\0' || temp_shelfId[0] == '\0' || tempDate.errCode() != 0) {
         setEmpty();
         is.setstate(ios::failbit);
      }
      else {
         m_title = ut.alcpy(temp_title);
         ut.strcpy(m_shelfId, temp_shelfId);
         m_date = tempDate;
         m_libRef = temp_libRef;
         m_membership = temp_membership;
      }
      return is;
   }

   bool Publication::conIO(ios& io) const {
      return &io == &std::cin || &io == &std::cout;
   }


   Publication::operator bool() const{
      return m_shelfId != nullptr && m_title != nullptr && m_title[0] != ' ' && m_shelfId[0] != ' ';
   }

   //helper function
   std::ostream& operator<<(std::ostream& os, const Publication& P) {
      return P.write(os);
   }

   std::istream& operator>>(std::istream& is, Publication& P) {
      return P.read(is);
   }

};