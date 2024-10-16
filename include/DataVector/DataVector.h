#ifndef DATA_VECTOR_H
#define DATA_VECTOR_H

#include <iostream>
#include <vector>

template <typename dvector_t> class DataVector {

private:
    dvector_t* data;
    unsigned int dimension;

public:
    DataVector(void) {}

    DataVector(unsigned int dimension_): dimension(dimension_) {
        this->data = new dvector_t[dimension_];
    }

    ~DataVector(void) {
        delete [] this->data;
    }

    void setDataAtIndex(const dvector_t data, const unsigned int index) {
        this->data[index] = data;
    }

    dvector_t getDataAtIndex(const unsigned int index) const {
        return this->data[index];
    }

};

#endif /* DATA_VECTOR_H */
