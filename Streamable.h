
#ifndef SDDS_STREAMABLE_H
#define SDDS_STREAMABLE_H

using namespace std;

namespace sdds {
   class Streamable {
   public:
      virtual ostream& write(ostream& os = std::cout) const = 0;
      virtual istream& read(istream& is = std::cin) = 0;
      virtual bool conIO(ios& io) const = 0;
      virtual operator bool() const = 0;
      virtual ~Streamable(){};
   };
   ostream& operator<<(ostream& os, const Streamable& S);
   istream& operator>>(istream& is, Streamable& S);
   
}
#endif // !SDDS_STREAMABLE_H
