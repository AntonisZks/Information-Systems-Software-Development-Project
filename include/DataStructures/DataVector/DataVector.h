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
    DataVector(void) : data(nullptr), dimension(0), graphIndex(0) {
        this->data = new dvector_t[0];
    }

    /**
     * @brief Constructor of the DataVector. Here all the properties of the Vector are
     * being initialized. The dimension is being set and memory is being allocated for the
     * vector data, according to the given dimension.
     * 
     * @param dimension_ the dimension of the vector.
    */
    DataVector(unsigned int dimension_, unsigned int index_=0) : dimension(dimension_), graphIndex(index_) {
        this->data = new dvector_t[dimension_];
    }

    /**
     * @brief Destructor of the DataVector. Here all the memory allocated for the data of the 
     * vector, is being deleted.
    */
    ~DataVector(void) {
        delete[] this->data;
    }

    /**
     * @brief Copy Constructor of the DataVector. Ensures that all the properties and data of
     * the one vector are successfully copied to another vector.
     * 
     * @param other the other vector to copy the data to
    */
    DataVector(const DataVector& other) : dimension(other.dimension), graphIndex(other.graphIndex) {
        if (other.data) {
            this->data = new dvector_t[other.dimension];
            std::copy(other.data, other.data + other.dimension, this->data);
        } else {
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
    DataVector& operator=(const DataVector& other) {
        if (this == &other) return *this;  // Self-assignment check

        delete[] this->data;  // Free the existing resource

        this->dimension = other.dimension;
        this->graphIndex = other.graphIndex;
        if (other.data) {
            this->data = new dvector_t[other.dimension];
            std::copy(other.data, other.data + other.dimension, this->data);
        } else {
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
    DataVector(DataVector&& other) noexcept : data(other.data), dimension(other.dimension), graphIndex(other.graphIndex) {
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
    DataVector& operator=(DataVector&& other) noexcept {
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
    float magnitude(void) const {

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
    bool operator<(const DataVector& other) const {

        if (this->dimension != other.dimension) {
            std::cout << this->dimension << " " << other.dimension << std::endl;
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
    bool operator>(const DataVector& other) const {
        
        if (this->dimension != other.dimension) {
            std::cout << this->dimension << " " << other.dimension << std::endl;
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
    bool operator==(const DataVector& other) const {

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
     * @brief Sets data at a specific index in the vector.
     * 
     * @param data the value to set at the specified index
     * @param index the position in the vector to set the data
    */
    void setDataAtIndex(const dvector_t data, const unsigned int index) {
        this->data[index] = data;
    }

    /**
     * @brief Retrieves data at a specific index in the vector.
     * 
     * @param index the position in the vector from which to retrieve the data
     * @return the data value at the specified index
    */
    dvector_t getDataAtIndex(const unsigned int index) const {
        return this->data[index];
    }

    /**
     * @brief Retrieves the dimension of the vector.
     * 
     * @return the dimension (size) of the vector
    */
    unsigned int getDimension() const {
        return dimension;
    }

    void setDimension(const unsigned int dimension) {
        this->dimension = dimension;
        delete [] this->data;
        this->data = new dvector_t[dimension];
    }

    void setIndex(const unsigned int& index) {
        this->graphIndex = index;
    }

    unsigned int getIndex(void) const {
        return this->graphIndex;
    }
    
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

    out << vector.getDimension();
    for (unsigned int i = 0; i < vector.getDimension(); i++) {
        out << " " << vector.getDataAtIndex(i);
    }

    return out;

}

template <typename dvector_t> std::istream& operator>>(std::istream& in, DataVector<dvector_t>& vector) {

    unsigned int valuesCount;
    in >> valuesCount;

    vector.setDimension(valuesCount);
    for (unsigned int i = 0; i < valuesCount; i++) {
        dvector_t currentValue;
        in >> currentValue;
        vector.setDataAtIndex(currentValue, i);
    }

    return in;

}

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

#endif /* DATA_VECTOR_H */
