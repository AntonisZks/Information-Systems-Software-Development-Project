#ifndef FILTER_H
#define FILTER_H


#include <iostream>

struct CategoricalAttributeFilter {

private:
  unsigned int c_value;

public:

  CategoricalAttributeFilter() : c_value(0) {}

  CategoricalAttributeFilter(unsigned int C_value) : c_value(C_value) {}

  inline unsigned int getC() const { 
    return c_value;
  }

  inline void setC(unsigned int c_value) { 
    this->c_value = c_value;
  }

  bool operator==(const CategoricalAttributeFilter& other) const {
    return c_value == other.c_value;
  }

  bool operator!=(const CategoricalAttributeFilter& other) const {
    return c_value != other.c_value;
  }

  bool operator<(const CategoricalAttributeFilter& other) const {
    return c_value < other.c_value;
  }

  bool operator>(const CategoricalAttributeFilter& other) const {
    return c_value > other.c_value;
  }

};

// std::ostream& operator<<(std::ostream& out, const CategoricalAttributeFilter& filter) {
//   out << filter.getC();
//   return out;
// }


#endif /* FILTER_H */