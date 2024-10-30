
#ifndef DATA_VECTOR_H
#define DATA_VECTOR_H

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

/**
 * @brief Class that represents the main Vector in which the data are going to
 * be placed. Specifically it is used to store the data we are reading from the
 * data sets that are provided at: http://corpus-texmex.irisa.fr/
*/
template <typename dvector_t> class DataVector {

private:
    dvector_t* data;
    unsigned int dimension;

public:
    
    /**
     * @brief Default Constructor of the DataVector. Exists just to avoid errors.
     * Sets the data to NULL and the dimension of the vector to 0.
    */
    DataVector(void) : data(nullptr), dimension(0) {}

    /**
     * @brief Constructor of the DataVector. Here all the properties of the Vector are
     * being initialized. The dimension is being set and memory is being allocated for the
     * vector data, according to the given dimension.
     * 
     * @param dimension_ the dimension of the vector.
    */
    DataVector(unsigned int dimension_) : dimension(dimension_) {
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
    DataVector(const DataVector& other) : dimension(other.dimension) {
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
    DataVector(DataVector&& other) noexcept : data(other.data), dimension(other.dimension) {
        other.data = nullptr;
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

    // Constructor using std::vector
    DataVector(const std::vector<dvector_t>& vec) : dimension(vec.size()) {
        this->data = new dvector_t[dimension];
        std::copy(vec.begin(), vec.end(), this->data);
    }

    // Method to calculate Euclidean distance
    double euclideanDistance(const DataVector<dvector_t>& other) const {
        double sum = 0.0;
        for (size_t i = 0; i < dimension; ++i) {
            sum += std::pow(data[i] - other.data[i], 2);
        }
        return std::sqrt(sum);
    }

    // Comparison operator for priority queue
    bool operator<(const DataVector<dvector_t>& other) const {
        for (size_t i = 0; i < dimension; ++i) {
            if (data[i] < other.data[i]) return true;
            if (data[i] > other.data[i]) return false;
        }
        return false;
    }
};

/**
 * @brief Operator << overloading for printing a DataVector object. Specifically it prints the 
 * first and last 10 items of the vector.
 * 
 * @param out the output stream object
 * @param vector the DataVector object to print
 * 
 * @return the output stream
*/
template <typename dvector_t> std::ostream& operator<<(std::ostream& out, const DataVector<dvector_t>& vector) {
    
    // Print the first 10 items inside the vector
    out << "[";
    for (unsigned int i = 0; i < 10 && i < vector.getDimension(); i++) {
        out << vector.getDataAtIndex(i);
        if (i != 9 && i != vector.getDimension() - 1) {
            out << ", ";
        }
    }

    if (vector.getDimension() > 10) {
        out << "... ";

        // Print the last 10 items inside the vector
        for (unsigned int i = vector.getDimension() - 10; i < vector.getDimension() - 1; i++) {
            out << vector.getDataAtIndex(i);
            if (i != vector.getDimension() - 2) {
                out << ", ";
            }
        }
        out << vector.getDataAtIndex(vector.getDimension() - 1);
    }
    out << "]";

    return out;
}

#endif /* DATA_VECTOR_H */
