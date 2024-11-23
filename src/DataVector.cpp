#include "../include/DataVector.h"

/**
 * @brief Default Constructor of the DataVector. Exists just to avoid errors.
 * Sets the data to NULL and the dimension of the vector to 0.
*/
template <typename dvector_t> DataVector<dvector_t>::DataVector(void) 
  : data(nullptr), dimension(0), graphIndex(0) {

    this->data = new dvector_t[0];
}

/**
 * @brief Constructor of the DataVector. Here all the properties of the Vector are
 * being initialized. The dimension is being set and memory is being allocated for the
 * vector data, according to the given dimension.
 * 
 * @param dimension_ the dimension of the vector.
*/
template <typename dvector_t> DataVector<dvector_t>::DataVector(unsigned int dimension_, unsigned int index_) 
  : dimension(dimension_), graphIndex(index_) {

    this->data = new dvector_t[dimension_];
}

/**
 * @brief Destructor of the DataVector. Here all the memory allocated for the data of the 
 * vector, is being deleted.
*/
template <typename dvector_t> DataVector<dvector_t>::~DataVector(void) {
    delete[] this->data;
}

/**
 * @brief Copy Constructor of the DataVector. Ensures that all the properties and data of
 * the one vector are successfully copied to another vector.
 * 
 * @param other the other vector to copy the data to
*/
template <typename dvector_t> DataVector<dvector_t>::DataVector(const DataVector& other) 
  : dimension(other.dimension), graphIndex(other.graphIndex) {
  
  if (other.data) {
    this->data = new dvector_t[other.dimension];
    std::copy(other.data, other.data + other.dimension, this->data);
  } 
  else {
    this->data = nullptr;
  }

}

/**
 * @brief Operator = overloading for the DataVector class. Here we ensure that all the data
 * of one vector are successfully transferred to another vector, in order to have the same
 * data and properties like the first one.
 * 
 * @param other the other vector to transfer the data to
 * 
 * @return the vector itself
*/
template <typename dvector_t> DataVector<dvector_t>& DataVector<dvector_t>::operator=(const DataVector& other) {
  if (this == &other) return *this;  // Self-assignment check

  delete[] this->data;  // Free the existing resource

  this->dimension = other.dimension;
  this->graphIndex = other.graphIndex;
  if (other.data) {
    this->data = new dvector_t[other.dimension];
    std::copy(other.data, other.data + other.dimension, this->data);
  } 
  else {
    this->data = nullptr;
  }

  return *this;
}

/**
 * @brief Move Constructor of the DataVector. Transfers ownership of the data from 
 * one vector to another without allocating new memory. Ensures that the original
 * vector releases its memory to prevent double deletion.
 * 
 * @param other the other vector to transfer the data from
*/
template <typename dvector_t> DataVector<dvector_t>::DataVector(DataVector&& other) noexcept 
  : data(other.data), dimension(other.dimension), graphIndex(other.graphIndex) {
  
  other.data = nullptr;  // Prevent the original object from freeing the memory
  other.dimension = 0;

}

/**
 * @brief Move Assignment Operator for the DataVector. Transfers ownership of the data
 * from one vector to another, ensuring that the data and properties of the target vector 
 * are correctly replaced.
 * 
 * @param other the other vector to transfer the data from
 * 
 * @return the vector itself
*/
template <typename dvector_t> DataVector<dvector_t>& DataVector<dvector_t>::operator=(DataVector&& other) noexcept {

  if (this != &other) {
    delete[] this->data;  // Free the existing resource

    this->data = other.data;
    this->dimension = other.dimension;

    other.data = nullptr;
    other.dimension = 0;
  }

  return *this;

}

/**
 * @brief Calculates the Euclidean norm (magnitude) of the DataVector. Specifically it represents 
 * the vector's length in n-dimensional space. Useful for comparing vectors based on their magnitudes.
 * The Euclidean norm is calculated as:
 * 
 *   norm = sqrt(sum(data[i]^2)) for all elements in the vector
 * 
 * @return A floating-point number representing the Euclidean norm of the vector.
 */
template <typename dvector_t> float DataVector<dvector_t>::magnitude(void) const {

  // Calculate the euclidean norm of the vector
  float sum = 0.0;
  for (unsigned int i = 0; i < this->dimension; i++) {
    sum += this->data[i] * this->data[i];
  }

  return std::sqrt(sum);

}

/**
 * @brief Overloads the less-than operator to compare two DataVector objects based on their Euclidean 
 * norm (magnitude).
 * 
 * @param other The DataVector to compare with the current object.
 * @return True if the magnitude of the current DataVector is less than that 
 *         of the other DataVector; otherwise, false.
 */
template <typename dvector_t> bool DataVector<dvector_t>::operator<(const DataVector& other) const {

  if (this->dimension != other.dimension) {
    throw std::invalid_argument("Vectors must have the same dimension for comparison");
  }

  // Compare magnitudes first
  float thisMagnitude = this->magnitude();
  float otherMagnitude = other.magnitude();

  if (thisMagnitude != otherMagnitude) {
    return thisMagnitude < otherMagnitude;
  }

  // If magnitudes are equal, perform lexicographical comparison
  for (unsigned int i = 0; i < this->dimension; ++i) {
    if (this->data[i] != other.data[i]) {
      return this->data[i] < other.data[i];
    }
  }

  // If all elements are equal, return false
  return false;

}

/**
 * @brief Overloads the greater-than operator to compare two DataVector objects based on their Euclidean 
 * norm (magnitude).
 * 
 * @param other The DataVector to compare with the current object.
 * @return True if the magnitude of the current DataVector is less than that 
 *         of the other DataVector; otherwise, false.
 */
template <typename dvector_t> bool DataVector<dvector_t>::operator>(const DataVector& other) const {
    
  if (this->dimension != other.dimension) {
    throw std::invalid_argument("Vectors must have the same dimension for comparison");
  }

  // Compare magnitudes first
  float thisMagnitude = this->magnitude();
  float otherMagnitude = other.magnitude();

  if (thisMagnitude != otherMagnitude) {
    return thisMagnitude > otherMagnitude;
  }

  // If magnitudes are equal, perform lexicographical comparison
  for (unsigned int i = 0; i < this->dimension; ++i) {
    if (this->data[i] != other.data[i]) {
      return this->data[i] > other.data[i];
    }
  }

  // If all elements are equal, return false
  return false;

}

/**
 * @brief Overloading the == operator for the DataVector class.
 * 
 * @param other the other DataVector to compare against
 * @return true if both vectors are equal (same dimension and same data), false otherwise
 */
template <typename dvector_t> bool DataVector<dvector_t>::operator==(const DataVector& other) const {

  // First, check if dimensions are the same
  if (this->dimension != other.dimension) {
    return false;
  }

  // Then, check each element for equality
  for (unsigned int i = 0; i < this->dimension; ++i) {
    if (this->data[i] != other.data[i]) {
      return false; // Found a difference, so they are not equal
    }
  }

  // If all checks pass, the vectors are equal
  return true;

}

/**
 * @brief Sets the dimension of the vector. This method is used to change the dimension of the vector.
 * 
 * @param dimension the new dimension of the vector
 */
template <typename dvector_t> void DataVector<dvector_t>::setDimension(const unsigned int dimension) {

    this->dimension = dimension;
    delete [] this->data;
    this->data = new dvector_t[dimension];

}


template class DataVector<int>;
template class DataVector<float>;
template class DataVector<double>;


// /**
//  * @brief Operator << overloading for printing a DataVector object. Speicifically it prints the 
//  * first and last 10 items of the vector.
//  * 
//  * @param out the output stream object
//  * @param vector the DataVector object to print
//  * 
//  * @return the putput stream
// */
// template <typename dvector_t> std::ostream& operator<<(std::ostream& out, const DataVector<dvector_t> vector) {

//     if (vector.getDimension() > 3) {

//         // Print the first 3 items inside the vector
//         out << "[";
//         for (unsigned int i = 0; i < 3; i++) {
//             out << vector.getDataAtIndex(i);
//             out << ", ";
//         }

//         out << "... ";

//         // Print the last 3 items inside the vector
//         for (unsigned int i = vector.getDimension() - 3; i < vector.getDimension() -1; i++) {
//             out << vector.getDataAtIndex(i);
//             out << ", ";
//         }
//         out << vector.getDataAtIndex(vector.getDimension() - 1) << "]";

//     } else {
        
//         // Print the whole vector
//         out << "[";
//         for (unsigned int i = 0; i < vector.getDimension(); i++) {
//             out << vector.getDataAtIndex(i);
//             if (i < vector.getDimension() - 1) {
//                 out << ", ";
//             }
//         }
//         out << "] ";

//     }

//     return out;

// }
