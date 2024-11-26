#ifndef DATA_VECTOR_H
#define DATA_VECTOR_H

#include <iostream>
#include <vector>
#include <cmath>

/**
 * @brief Class that represents the main Vector in which the data are going to
 * be placed. Specifically it is used to store the data we are reading from the
 * data sets that are provided at: http://corpus-texmex.irisa.fr/
*/
template <typename dvector_t> class DataVector {

private:
    dvector_t* data;
    unsigned int dimension;
    unsigned int graphIndex;

public:

    /**
     * @brief Default Constructor of the DataVector. Exists just to avoid errors.
     * Sets the data to NULL and the dimension of the vector to 0.
    */
    DataVector(void);

    /**
     * @brief Constructor of the DataVector. Here all the properties of the Vector are
     * being initialized. The dimension is being set and memory is being allocated for the
     * vector data, according to the given dimension.
     * 
     * @param dimension_ the dimension of the vector.
    */
    DataVector(unsigned int dimension_, unsigned int index_=0);

    /**
     * @brief Destructor of the DataVector. Here all the memory allocated for the data of the 
     * vector, is being deleted.
    */
    ~DataVector(void);

    /**
     * @brief Copy Constructor of the DataVector. Ensures that all the properties and data of
     * the one vector are successfully copied to another vector.
     * 
     * @param other the other vector to copy the data to
    */
    DataVector(const DataVector& other);

    /**
     * @brief Operator = overloading for the DataVector class. Here we ensure that all the data
     * of one vector are successfully transferred to another vector, in order to have the same
     * data and properties like the first one.
     * 
     * @param other the other vector to transfer the data to
     * 
     * @return the vector itself
    */
    DataVector& operator=(const DataVector& other);

    /**
     * @brief Move Constructor of the DataVector. Transfers ownership of the data from 
     * one vector to another without allocating new memory. Ensures that the original
     * vector releases its memory to prevent double deletion.
     * 
     * @param other the other vector to transfer the data from
    */
    DataVector(DataVector&& other) noexcept;

    /**
     * @brief Move Assignment Operator for the DataVector. Transfers ownership of the data
     * from one vector to another, ensuring that the data and properties of the target vector 
     * are correctly replaced.
     * 
     * @param other the other vector to transfer the data from
     * 
     * @return the vector itself
    */
    DataVector& operator=(DataVector&& other) noexcept;

    /**
     * @brief Calculates the Euclidean norm (magnitude) of the DataVector. Specifically it represents 
     * the vector's length in n-dimensional space. Useful for comparing vectors based on their magnitudes.
     * The Euclidean norm is calculated as:
     * 
     *   norm = sqrt(sum(data[i]^2)) for all elements in the vector
     * 
     * @return A floating-point number representing the Euclidean norm of the vector.
     */
    float magnitude(void) const;

    /**
     * @brief Overloads the less-than operator to compare two DataVector objects based on their Euclidean 
     * norm (magnitude).
     * 
     * @param other The DataVector to compare with the current object.
     * @return True if the magnitude of the current DataVector is less than that 
     *         of the other DataVector; otherwise, false.
     */
    bool operator<(const DataVector& other) const;

    /**
     * @brief Overloads the greater-than operator to compare two DataVector objects based on their Euclidean 
     * norm (magnitude).
     * 
     * @param other The DataVector to compare with the current object.
     * @return True if the magnitude of the current DataVector is less than that 
     *         of the other DataVector; otherwise, false.
     */
    bool operator>(const DataVector& other) const;

    /**
     * @brief Overloading the == operator for the DataVector class.
     * 
     * @param other the other DataVector to compare against
     * @return true if both vectors are equal (same dimension and same data), false otherwise
     */
    bool operator==(const DataVector& other) const;


    /**
     * @brief Sets data at a specific index in the vector.
     * 
     * @param data the value to set at the specified index
     * @param index the position in the vector to set the data
    */
    inline void setDataAtIndex(const dvector_t data, const unsigned int index) { this->data[index] = data; }

    /**
     * @brief Retrieves data at a specific index in the vector.
     * 
     * @param index the position in the vector from which to retrieve the data
     * @return the data value at the specified index
    */
    inline dvector_t getDataAtIndex(const unsigned int index) const { return this->data[index]; }

    /**
     * @brief Retrieves the dimension of the vector.
     * 
     * @return the dimension (size) of the vector
    */
    inline unsigned int getDimension() const { return dimension;}

    /**
     * @brief Sets the dimension of the vector. This method is used to change the dimension of the vector.
     * 
     * @param dimension the new dimension of the vector
     */
    void setDimension(const unsigned int dimension);

    /**
     * @brief Sets the index of the vector in the graph.
     * 
     * @param index the index of the vector in the graph
     */
    inline void setIndex(const unsigned int& index) { this->graphIndex = index; }

    /**
     * @brief Retrieves the index of the vector in the graph.
     * 
     * @return the index of the vector in the graph
     */
    inline unsigned int getIndex(void) const { return this->graphIndex; }
    
};

/**
 * @brief Operator << overloading for printing a DataVector object. Speicifically it prints the 
 * first and last 10 items of the vector.
 * 
 * @param out the output stream object
 * @param vector the DataVector object to print
 * 
 * @return the putput stream
*/
template <typename dvector_t> std::ostream& operator<<(std::ostream& out, const DataVector<dvector_t>& vector) {

  out << vector.getDimension() << " " << vector.getIndex();
  for (unsigned int i = 0; i < vector.getDimension(); i++) {
    out << " " << vector.getDataAtIndex(i);
  }

  return out;

}

/**
 * @brief Operator >> overloading for reading a DataVector object. Specifically it reads the
 * dimension, index and data of the vector from the input stream.
 *
 * @param in the input stream object
 * @param vector the DataVector object to read into
 *
 * @return the input stream
 */
template <typename dvector_t> std::istream& operator>>(std::istream& in, DataVector<dvector_t>& vector) {

    unsigned int valuesCount, index;
    in >> valuesCount >> index;

    vector.setDimension(valuesCount);
    vector.setIndex(index);

    for (unsigned int i = 0; i < valuesCount; i++) {
        dvector_t currentValue;
        in >> currentValue;
        vector.setDataAtIndex(currentValue, i);
    }

    return in;

}


#endif /* DATA_VECTOR_H */
