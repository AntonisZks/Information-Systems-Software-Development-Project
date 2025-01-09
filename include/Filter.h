#ifndef FILTER_H
#define FILTER_H


#include <iostream>

// Enum to define different filter types based on the query type
enum FilterType {

  NO_FILTER = 0,                   // For query_type = 0, no filter is applied (only the vector is used).
  C_EQUALS_v = 1,                  // For query_type = 1, filter for C = v (categorical attribute).
  l_LEQ_T_LEQ_r = 2,               // For query_type = 2, filter for l ≤ T ≤ r (timestamp constraint).
  C_EQUALS_v_AND_l_LEQ_T_LEQ_r = 3 // For query_type = 3, filter for both C = v and l ≤ T ≤ r (combined constraints).

};

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