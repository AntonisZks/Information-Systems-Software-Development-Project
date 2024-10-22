#ifndef READFUNCTION_H
#define READFUNCTION_H

#include <vector>
#include <string>
#include "DataVector/DataVectorNew.h"  // Include your DataVector class

// Function declaration
std::vector<DataVector<float>> ReadVectorFile(const std::string& filename);

#endif /* READFUNCTION_H */
