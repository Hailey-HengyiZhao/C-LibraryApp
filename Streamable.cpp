
#include <iostream>
#include "Streamable.h"

using namespace std;

namespace sdds {

   ostream& operator<<(ostream& os, const Streamable& S){
      if(S.operator bool()) S.write(os);
      return os;
   }
   istream& operator>>(istream& is, Streamable& S){
      if(S.operator bool()) S.read(is);
      return is;
   }
}