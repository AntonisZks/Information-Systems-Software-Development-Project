#ifndef DATA_VECTOR_H
#define DATA_VECTOR_H

#include <iostream>
#include <vector>

template <typename dvector_t>
class DataVector {

private:
    dvector_t* data;
    unsigned int dimension;

public:
    // Default constructor
    DataVector(void) : data(nullptr), dimension(0) {}

    // Constructor with dimension
    DataVector(unsigned int dimension_) : dimension(dimension_) {
        this->data = new dvector_t[dimension_];
    }

    // Destructor
    ~DataVector(void) {
        delete[] this->data;
    }

    // Copy constructor (to handle deep copy)
    DataVector(const DataVector& other) : dimension(other.dimension) {
        if (other.data) {
            this->data = new dvector_t[other.dimension];
            std::copy(other.data, other.data + other.dimension, this->data);
        } else {
            this->data = nullptr;
        }
    }

    // Copy assignment operator (to handle deep copy)
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

    // Move constructor
    DataVector(DataVector&& other) noexcept : data(other.data), dimension(other.dimension) {
        other.data = nullptr;  // Prevent the original object from freeing the memory
        other.dimension = 0;
    }

    // Move assignment operator
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

    // Set data at a specific index
    void setDataAtIndex(const dvector_t data, const unsigned int index) {
        this->data[index] = data;
    }

    // Get data at a specific index
    dvector_t getDataAtIndex(const unsigned int index) const {
        return this->data[index];
    }

    // Get the dimension of the vector
    unsigned int getDimension() const {
        return dimension;
    }
};

#endif /* DATA_VECTOR_H */
