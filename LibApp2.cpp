/* Citation and Sources...
Final Project Milestone ?
Module: LibApp
Filename: LibApp.cpp
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
   
   int PPA{};

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

   void LibApp::load(){
      cout << "Loading Data" <<endl;
      ifstream filein(m_filename);
      char pType{};
      for (int row = 1; filein; row++) {
         filein >> pType;
         if (filein) {
            if (pType == 'B') {
               filein.ignore();
               Book newBook{};
               newBook = getNextBRec(filein);
               if (filein) {
                  m_pulication[m_size] = new Book(newBook);
                  m_size++;
               }
            }
            else if (pType == 'P') {
               filein.ignore();
               Publication newPub{};
               newPub = getNextPRec(filein);
               if (filein) {
                  m_pulication[m_size] = new Publication(newPub);
                  m_size++;
               }
            }
         };
      }
      filein.close();

      //Get the last reference number
      m_LastRefNum = m_pulication[m_size-1]->getRef();

      //the last read position. save as global variable
      PPA = m_size;
   }

   void LibApp::save(){
      cout << "Saving Data" << endl;
      ofstream fileout(m_filename, ios::app);
      for (int i = PPA ; i < m_size; i++) {
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
            cout << *p[i] << endl;
            i = size; // Exit the loop.
         }
      }
   }

   void LibApp::prnAborted(){
      cout << "Aborted!" << endl
           << "-------------------------------------------------------" << endl;
   }

   Publication* LibApp::getPub(int libRef){
      Publication* res = nullptr;
      for (int i = 0; i < m_size; i++) {
         if (libRef == m_pulication[i]->getRef()) res = m_pulication[i];
      }
      return res;
   }

   void LibApp::search(){
      //int s{0};
      //bool done = false;
      //char* userInput{ nullptr };
      //PublicationSelector ps("Select one of the following found matches:", 15);

      cout << "Checkout publication from the library" << endl;

      //do{
      //   s = m_pubMenu.run();
      //   if (s != 0) {

      //      cout << "Publication Title: ";
      //      userInput = ut.getDynCstr(cin, '\n');

      //      //Book collection
      //      if (s == 1) {
      //         for (int i = 0; i < m_size; i++) {
      //            if (m_pulication[i]->type() == 'B' && ut.strstr(*m_pulication[i], userInput)) ps << *m_pulication[i];
      //         }
      //      }
      //      //Publication collection
      //      if (s == 2) {
      //         for (int i = 0; i < m_size; i++) {
      //            if (m_pulication[i]->type() == 'P' && ut.strstr(*m_pulication[i], userInput)) ps << *m_pulication[i];
      //         }
      //      }

      //      //
      //      if (ps) {
      //         ps.sort(); // sort them based on date and title
      //         int ref = ps.run(); // display the publications and select one
      //         if (ref) {
      //            cout << "Selected Library Reference Number: " << ref << endl;
      //            prnPub(m_pulication, 100, ref);
      //         }
      //         else {
      //            cout << "Aborted by user!";
      //            done = true;
      //         }
      //      }
      //      else {
      //         cout << "No matches found!" << endl;
      //      }
      //   }
      //   else {
      //      done = true;
      //      prnAborted();
      //   }
      //} while (done == false);

   }

   void LibApp::returnPub(){
      search();
      cout << "Returning publication" << endl;
      cout << "Publication returned" << endl;
      m_changed = true;
   }

   void LibApp::newPublication(){
      if (m_size == SDDS_LIBRARY_CAPACITY) cout << "Library is at its maximum capacity!" << endl;
      else { // start to read
         int s{ -1 };
         cout << "Adding new publication to the library" << endl;

         // read the user input
         s = m_pubMenu.run();
         Publication* pd = nullptr;

         if (s == 0) {// if s == 0, aborted
            prnAborted();
            cin.clear();
            cin.ignore(1000, '\n');
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
                  if (pd!= nullptr) { //pd has value
                     m_changed = true;
                     m_LastRefNum++; //last reference number ++
                     pd->setRef(m_LastRefNum);

                     m_pulication[m_size] = pd;
                     m_size++;
                     cout << "Publication added" << endl;
                     
                  }
                  else {
                     cout << "Failed to add publication!" << endl;
                     delete pd;// clear memory
                  }
               }
            }
         }
      }
   }

   void LibApp::removePublication(){
      cout << "Removing publication from library" << endl;
      search();
      if (confirm("Remove this publication from the library?")) {
         m_changed = true;
         cout << "Publication removed" << endl;
      }
   }

   void LibApp::checkOutput(){
      search();
      if (confirm("Check out publication?")) {
         m_changed = true;
         cout << "Publication checked out" << endl;
      }
   }

   void LibApp::initiate(){
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

   LibApp::LibApp(){
      initiate();
   }

   LibApp::LibApp(const char* filename){
      if(filename!= nullptr) ut.strcpy(m_filename,filename);
      initiate();

   }

   void LibApp::run(){
      unsigned int s{};
      bool done = false;
      do {
         s = m_mainMenu.run();
         if (s == 1) newPublication();
         else if (s == 2) removePublication();
         else if (s == 3) checkOutput();
         else if (s == 4) returnPub();
         else {
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
         }
         cout << endl;
      } while (done == false);

      cout << "-------------------------------------------" << endl
           << "Thanks for using Seneca Library Application" << endl;
   }
}