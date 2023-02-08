/* Citation and Sources...
Final Project Milestone ?
Module: LibApp
Filename: LibApp.h
Version 1.0
Author	Hengyi Zhao
StudentID: 173484213
Revision History
-----------------------------------------------------------
Date      Reason
2022/7/11 MS2 Submission
-----------------------------------------------------------
I have done all the coding by myself and only copied the code
that my professor provided to complete my project milestones.
-----------------------------------------------------------*/
#ifndef SDDS_LIBAPP_H
#define SDDS_LIBAPP_H
#include "Menu.h"
#include "Lib.h"
#include "Publication.h"

namespace sdds {

   class LibApp {
      bool m_changed{};
      Menu m_mainMenu{};
      Menu m_exitMenu{};

      //MS5 File name
      char m_filename[SDDS_MAX_FILENAME]{};

      //MS5 Publication Pointers Array
      Publication * m_pulication[SDDS_LIBRARY_CAPACITY]{};

      //MS5 Number Of Loaded Publications
      int m_size{0};

      //MS5 Last Library Reference Number
      int m_LastRefNum{0};

      //MS5 Publication Type Menu
      Menu m_pubMenu{"Choose the type of publication:" };

      bool confirm(const char* message);
      void load();
      void save();
      void search();
      void returnPub();
      void newPublication();
      void removePublication();
      void checkOutput();

      //MS 5 
      void initiate();
      void prnPub(Publication* p[], int size, int ref);
      void prnAborted();
      Publication* getPub(int libRef);
   public:
      LibApp();
      void run();

      //MS 5 
      LibApp(const char* filename);
   };
}
#endif // !SDDS_LIBAPP_H
