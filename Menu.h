
#ifndef SDDS_MENU_H
#define SDDS_MENU_H
#include <iostream>
namespace sdds {

   const unsigned int MAX_MENU_ITEMS = 20;
   class Menu;
   class MenuItem {
      char* m_content{};
      friend class Menu;
      MenuItem();
      MenuItem(const char* menuitemConent);
      MenuItem(MenuItem& toCopy) = delete;
      MenuItem& operator=(MenuItem& toAssg) = delete;
      ~MenuItem();
      operator bool() const;
      operator const char* () const;
      std::ostream& display(std::ostream& ostr)const;
   };

   //-------------------------------------------------------------------------//
   //------------------------------MENU---------------------------------------//

   class Menu {
      MenuItem m_title{};
      MenuItem* m_item[MAX_MENU_ITEMS]{};
      int m_size{};
      void setEmpty();
   public:
      Menu();
      Menu(const char* title);
      ~Menu();
      std::ostream& displayMenu(std::ostream& ostr = std::cout) const;
      std::ostream& displayTitle(std::ostream& ostr = std::cout) const;
      unsigned int run();
      unsigned int operator~();
      Menu& operator()(const char* title);
      Menu& operator<<(const char* menuitemConent);

      operator int() const;
      operator unsigned int() const;
      operator bool()const;
      const char* readtitle() const;
      const char* operator[](int index)const;
      friend std::ostream& operator<<(std::ostream& ostr, const Menu& menu);
   };
}

#endif // !SDDS_MENU_H
