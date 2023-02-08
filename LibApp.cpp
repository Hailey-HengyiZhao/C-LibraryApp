#include <iostream>
#include <iomanip>
#include <fstream>
#include "LibApp.h"
#include "Utils.h"
#include "Book.h"
#include "Publication.h"
#include "PublicationSelector.h"

using namespace std;

namespace sdds {

   Book readBook(istream& istr) {
      Book P;
      istr >> P;
      return P;
   };
   Book getNextBRec(ifstream& ifstr) {
      Book P;
      ifstr >> P;
      ifstr.ignore(1000, '\n');
      return P;
   };
   Publication readPublication(istream& istr) {
      Publication P;
      istr >> P;
      return P;
   };
   Publication getNextPRec(ifstream& ifstr) {
      Publication P;
      ifstr >> P;
      ifstr.ignore(1000, '\n');
      return P;
   };

   bool LibApp::confirm(const char* message)
   {
      Menu confirm(message);
      confirm << "Yes";
      return confirm.run();
   }

   void LibApp::load() {
      std::cout << "Loading Data" << endl;
      ifstream filein(m_filename);
      char pType{};
      for (int row = 1; filein; row++) {
         filein >> pType;
         if (filein) {
            if (pType == 'B') {
               filein.ignore();
               Book pd;
               pd = getNextBRec(filein);
               if (filein) {
                  m_pulication[m_size] = new Book(pd);
                  m_size++;
               }
               else {
                  delete pd;
               }
            }
            else if (pType == 'P') {
               filein.ignore();
               Publication pd;
               pd = getNextPRec(filein);
               if (filein) {
                  m_pulication[m_size] = new Publication(pd);
                  m_size++;
               }
               else{
                  delete pd;
               }
            }
         };
      }
      filein.close();
      //Get the last reference number
      m_LastRefNum = m_pulication[m_size - 1]->getRef();
   }

   void LibApp::save() {
      std::cout << "Saving Data" << endl;
      ofstream fileout(m_filename);
      for (int i = 0; i < m_size; i++) {
         if (m_pulication[i]->getRef() != 0) {
            fileout << *m_pulication[i] << endl;
         }
      };
      fileout.close();
   }

   void LibApp::prnPub(Publication* p[], int size, int ref) {
      int i;
      for (i = 0; i < size; i++) {
         if (ref == p[i]->getRef()) {
            std::cout << *p[i] << endl;
            i = size; // Exit the loop.
         }
      }
   }

   void LibApp::prnAborted() {
      std::cout << "Aborted!" << endl
         << "-------------------------------------------------------" << endl;
   }

   Publication* LibApp::getPub(int libRef) {
      Publication* res = nullptr;
      for (int i = 0; i < m_size; i++) {
         if (libRef == m_pulication[i]->getRef()) res = m_pulication[i];
      }
      return res;
   }

   int LibApp::search(char pType, int sType) {
      int ref{0};
      char userInput[256]{};

      PublicationSelector ps("Select one of the following found matches:", 15);
      std::cout << "Publication Title: ";
      cin.getline(userInput, 256,'\n');
         
      switch (sType) {
      case 1://All 
         //Search and add to ps
         for (int i = 0; i < m_size; i++) {
            if (m_pulication[i]->type() == pType
               && ut.strstr(*m_pulication[i], userInput)
               && m_pulication[i]->getRef() != 0) {
               ps << m_pulication[i];
            }
         }
         break;
      case 2: //onloan
         //Search and add to ps
         for (int i = 0; i < m_size; i++) {
            if (m_pulication[i]->type() == pType
               && ut.strstr(*m_pulication[i], userInput)
               && m_pulication[i]->getRef() != 0
               && m_pulication[i]->onLoan()) ps << m_pulication[i];
         }
         break;
      case 3: // !onloan
         //Search and add to ps
         for (int i = 0; i < m_size; i++) {
            if (m_pulication[i]->type() == pType
               && ut.strstr(*m_pulication[i], userInput)
               && m_pulication[i]->getRef() != 0
               && !m_pulication[i]->onLoan()) ps << m_pulication[i];
         }
         break;
      }

      if (ps) {
         ps.sort(); // sort them based on date and title
         ref = ps.run(); // display the publications and select one
         if (ref) prnPub(m_pulication, 100, ref);
         else {
            std::cout << "Aborted!" << endl << endl;
         }
      }
      else std::cout << "No matches found!" << endl;
      

      return ref;
      
   }

   void LibApp::returnPub() {
      int getTpye{};
      int getRef{};
      int overDays{};
      std::cout << "Return publication to the library" << endl;

      getTpye = m_pubMenu.run();
      if (getTpye != 1 && getTpye != 2) prnAborted();
      else {

         switch (getTpye) {
         case 1:
            getRef = search('B', 2);
            break;
         case 2:
            getRef = search('P', 2);
            break;
         }

         if (getRef && confirm("Return Publication?")) {
            Date D1(getPub(getRef)->checkoutDate());
            Date D2{D2.setCurrentDay()};

            m_changed = true;
            getPub(getRef)->set(0);
            overDays = D2 - D1 - 15;
            
            if (overDays) {
               cout << "Please pay $";
               cout.setf(ios::fixed);
               cout << setprecision(2);
               cout << overDays*0.5
                    << " penalty for being "
                    << overDays 
                    << " days late!" << endl;
            }
            std::cout << "Publication returned" << endl << endl;
         }
      }
      
      m_changed = true;
   }

   void LibApp::newPublication() {
      if (m_size == SDDS_LIBRARY_CAPACITY) {
         std::cout << "Library is at its maximum capacity!" << endl;

      }
      else { // start to read
         int s{ -1 };
         std::cout << "Adding new publication to the library" << endl;

         // read the user input
         s = m_pubMenu.run();
         Publication* pd = nullptr;

         if (s == 0) {// if s == 0, aborted
            cin.clear();
            std::cout << "Aborted!" << endl;
         }
         else {// if s == 1 or 2
            if (s == 1) {
               pd = new Book;
               cin >> *pd;
            }
            else if (s == 2) {
               pd = new Publication;
               cin >> *pd;
            }

            if (!cin) { // flush the cin
               prnAborted();
               cin.clear();
               cin.ignore(1000, '\n');
            }
            else {
               if (confirm("Add this publication to the library?")) { // input ==1
                  if (pd != nullptr) { //pd has value
                     m_changed = true;
                     m_LastRefNum++; //last reference number ++
                     pd->setRef(m_LastRefNum);

                     m_pulication[m_size] = pd;
                     m_pulication[m_size] = pd;
                     m_size++;
                     std::cout << "Publication added" << endl;

                  }
                  else {
                     std::cout << "Failed to add publication!" << endl;
                     delete pd;// clear memory
                  }
               }
            }
         }
      }
      std::cout << endl;
   }

   void LibApp::removePublication() {
      int getTpye{};
      int getRef{};
      std::cout << "Removing publication from the library" << endl;

      getTpye = m_pubMenu.run();
      if (getTpye != 1 && getTpye != 2) prnAborted();
      else {
         switch (getTpye) {
         case 1:
            getRef = search('B');
            break;
         case 2:
            getRef = search('P');
            break;
         }
         if (getRef && confirm("Remove this publication from the library?")) {
            m_changed = true;
            getPub(getRef)->setRef(0);
            std::cout << "Publication removed" << endl;
         }
         
      };

   }

   void LibApp::checkOutput() {
      int getTpye{};
      int getRef{};
      int readMember{};
      std::cout << "Checkout publication from the library" << endl;
      getTpye = m_pubMenu.run();
      if (getTpye != 1 && getTpye != 2) prnAborted();
      else {
         switch (getTpye) {
         case 1:
            getRef = search('B', 3);
            break;
         case 2:
            getRef = search('P', 3);
            break;
         }
         if (getRef && confirm("Check out publication?")) {
            std::cout << "Enter Membership number: ";
            do {
               std::cin >> readMember;
               if (cin && readMember < 100000 && readMember > 9999) {
                  getPub(getRef)->set(readMember);
                  m_changed = true;
                  std::cout << "Publication checked out" << endl;
               }
               else {
                  std::cout << "Invalid membership number, try again: " ;
               }
            } while (m_changed == false);
         }
      };
      std::cout << endl;
   }

   void LibApp::initiate() {
      m_mainMenu("Seneca Library Application");
      m_mainMenu << "Add New Publication"
         << "Remove Publication"
         << "Checkout publication from library"
         << "Return publication to library";
      m_pubMenu << "Book" << "Publication";
      m_exitMenu("Changes have been made to the data, what would you like to do?");
      m_exitMenu << "Save changes and exit"
         << "Cancel and go back to the main menu";

      m_changed = false;
      load();
   }

   LibApp::LibApp() {
      initiate();
   }

   LibApp::LibApp(const char* filename) {
      if (filename != nullptr) ut.strcpy(m_filename, filename);
      initiate();

   }

   void LibApp::run() {
      unsigned int s{};
      bool done = false;
      do {
         s = m_mainMenu.run();
         switch (s)
         {
         case 1:
            newPublication();
            break;
         case 2:
            removePublication();
            break;
         case 3:
            checkOutput();
            break;
         case 4:
            returnPub();
            break;
         default:
            if (m_changed) {
               s = m_exitMenu.run();
               if (s == 1) {
                  save();
                  done = true;
               }
               else if (s == 2) done = false;
               else {
                  if (confirm("This will discard all the changes are you sure?")) done = true;
               }
            }
            else done = true;

            std::cout << endl;
            break;
         }
      } while (done == false);

      std::cout << "-------------------------------------------" << endl
                << "Thanks for using Seneca Library Application" << endl;


      //free the allocated memory
      for (int i = 0;i < m_size; i++) {  
         delete m_pulication[i];
      }
   }
}