/**
 * @brief Specialized subclasses for reading and storing enhanced datasets 
 * used in phase 2 of the project development.
 * 
 * @details These subclasses provide structured storage for new vector datasets 
 * with additional attributes for categorical and timestamp data, enabling more 
 * advanced querying capabilities.
 * 
 * The datasets can be found at: 
 * https://zenodo.org/records/13998879
 * 
 * Further documentation of their functionality:
 * https://transactional.blog/sigmod-contest/2024
 */

#ifndef DATAVECTOR_H
#define DATAVECTOR_H
#include "DataVector.h"

/**
 * @brief Class that represents a data vector with additional attributes for 
 * categorical and timestamp information.
 * 
 * @details The BaseDataVector class is designed to store base vector information 
 * from the dataset. Each base vector has two defining characteristics:
 * 
 * - **Categorical attribute (C)**: A single value representing a category.
 * - **Timestamp (T)**: A normalized timestamp value representing the time aspect.
 * 
 * Additionally, the base vector contains the remaining vector dimensions required 
 * for representation.
 * 
 * @tparam dvector_t the type of the data stored in the vector
 */
template <typename dvector_t>
class BaseDataVector : public DataVector<dvector_t>{
private:
    unsigned int C; // Categorical attribute
    float T; // Timestamp attribut

public:
    // Constructor to initialize the BaseDataVector with data, category, and timestamp
    BaseDataVector(unsigned int dimension, unsigned int index, unsigned int category, float timestamp)
        : DataVector<dvector_t>(dimension, index), C(category), T(timestamp) {}

    // Getter for the categorical attribute (C)
    unsigned int getC() const {
        return C;
    }

    // Getter for the timestamp attribute (T)
    float getT() const {
        return T;
    }

    // Setter for the categorical attribute (C)
    void setC(unsigned int category) {
        C = category;
    }

    // Setter for the timestamp attribute (T)
    void setT(float timestamp) {
        T = timestamp;
    }
};

/**
 * @brief Class that represents a query vector with additional query-related 
 * attributes.
 * 
 * @details The QueryDataVector class is used to store query vector information 
 * with specific search constraints. Each query vector has four defining characteristics:
 * 
 * - **Query type (query_type)**: Specifies the type of search constraint applied.
 * - **Categorical attribute (v)**: Represents a specific categorical value; set to -1 if not queried.
 * - **Timestamp range (l and r)**: Defines a range for timestamps; set to -1 if not queried.
 * - **Vector dimensions**: The remaining 100 dimensions representing the query vector data.
 * 
 * This class is designed to enable queries on data with both categorical and temporal 
 * attributes, enhancing its utility in tasks that involve filtered data searches.
 * 
 * @tparam dvector_t the type of the data stored in the vector
 */
template <typename dvector_t>
class QueryDataVector : public DataVector<dvector_t>{
private:
    unsigned int query_type; // Query type (e.g., 0, 1, 2, or 3, to represent different query categories)
    unsigned int v; // Categorical value associated with this query
    float l; // Lower bound for timestamp range
    float r; // Upper bound for timestamp range

public:
    // Constructor to initialize the QueryDataVector with data and additional query attributes
    QueryDataVector(unsigned int dimension, unsigned int index, unsigned int q_type, unsigned int v_value,
                    float l_value, float r_value)
        : DataVector<dvector_t>(dimension, index), query_type(q_type), v(v_value), l(l_value), r(r_value) {}

    // Getter for query type
    unsigned int getQueryType() const {
        return query_type;
    }

    // Getter for categorical value (v)
    unsigned int getV() const {
        return v;
    }

    // Getter for lower bound of timestamp range (l)
    float getL() const {
        return l;
    }

    // Getter for upper bound of timestamp range (r)
    float getR() const {
        return r;
    }

    // Setter for query type
    void setQueryType(unsigned int q_type) {
        query_type = q_type;
    }

    // Setter for categorical value (v)
    void setV(unsigned int v_value) {
        v = v_value;
    }

    // Setter for lower bound of timestamp range (l)
    void setL(float l_value) {
        l = l_value;
    }

    // Setter for upper bound of timestamp range (r)
    void setR(float r_value) {
        r = r_value;
    }
};

#endif // DATAVECTOR_H
