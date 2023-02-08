#define _CRT_SECURE_NO_WARNINGS
#include <iomanip>
#include <iostream>
#include "Menu.h"
#include "Utils.h"
using namespace std;

namespace sdds {
   MenuItem::MenuItem() {
      m_content = nullptr;
   }
   MenuItem::MenuItem(const char* menuitemConent) {
      if (menuitemConent == nullptr) {
         m_content = nullptr;
      }
      else {
         delete[] m_content;
         m_content = nullptr;
         m_content = new char[ut.strlen(menuitemConent) + 1];
         ut.strcpy(m_content, menuitemConent);
      }
   }
   MenuItem::~MenuItem(){
      delete[] m_content;
      m_content = nullptr;
   }

   MenuItem::operator bool() const{
      return m_content != nullptr;
   }
   MenuItem::operator const char* () const{
      return m_content;
   }
   std::ostream& MenuItem::display(std::ostream& ostr) const{
      if (m_content != nullptr) {
         ostr << m_content;
      }
      return ostr;
   }
   
   //-------------------------------------------------------------------------//
   //------------------------------MENU---------------------------------------//
   void Menu::setEmpty() {
      m_title.m_content = nullptr;
      m_size = 0;
      m_item[0] = nullptr;
   }

   Menu::Menu(){
      setEmpty();
   }

   Menu::Menu(const char* title){
      operator()(title);
   }

   Menu::~Menu(){
      for (int i = 0; i < m_size; i++) {
         delete m_item[i];
         m_item[i] = nullptr;
      }
   }

   std::ostream& Menu::displayMenu(std::ostream& ostr) const{
      if (m_title) {
         displayTitle();
         ostr << endl;
      }
      for (int i = 0; i < m_size; i++) {
         ostr << " " << i + 1 << "- " << m_item[i]->m_content << endl;
      }
      ostr << " 0- Exit" << endl << "> ";
      return ostr;
   }

   std::ostream& Menu::displayTitle(std::ostream& ostr) const{
      return ostr << m_title.m_content;
   }

   unsigned int Menu::run(){
      unsigned int i = 21;
      displayMenu();
      while (i > MAX_MENU_ITEMS) {
         cin >> i;
         if (cin.fail()) {
            cout << "Invalid Selection, try again: ";
            cin.clear();
            i = 21;
         }
         if ((i < 0 || i >(unsigned int)m_size) && i != 21) {
            cout << "Invalid Selection, try again: ";
            cin.clear();
            i = 21;
         }
         cin.ignore(1000, '\n');
      }
      return i;
   }

   unsigned int Menu::operator~(){
      unsigned int i = 21;
      displayMenu();
      while (i > MAX_MENU_ITEMS) {
         cin >> i;
         if (cin.fail()) {
            cout << "Invalid Selection, try again: ";
            cin.clear();
            i = 21;
         }
         if ((i < 0 || i > (unsigned int)m_size) && i != 21) {
            cout << "Invalid Selection, try again: ";
            cin.clear();
            i = 21;
         }
         cin.ignore(1000, '\n');
      }
      return i;
   }

   Menu& Menu::operator()(const char* title)
   {
      if (title != nullptr) {
         m_title.m_content = new char[ut.strlen(title) + 1];
         ut.strcpy(m_title.m_content, title);
         m_size = 0;
         m_item[0] = nullptr;
      }
      else {
         setEmpty();
      }
      return *this;
   }

   Menu& Menu::operator<<(const char* menuitemConent){
      if (menuitemConent != nullptr) {
         m_item[m_size] = new MenuItem(menuitemConent);
         m_size++;
      }
      else {
         setEmpty();
      }

      return *this;
   }

   Menu::operator int() const{
      return m_size;
   }

   Menu::operator unsigned int() const{
      return m_size;
   }

   Menu::operator bool() const{
      return m_size > 0;
   }

   const char* Menu::readtitle() const{
      return m_title.m_content;
   }

   const char* Menu::operator[](int index) const{
      return *m_item[index% m_size];
   }


   std::ostream& operator<<(std::ostream& ostr, const Menu& menu){
      if (menu.m_size > 0) {
         menu.displayTitle();
      }
      return ostr;
   }


}