#include "../include/FilteredVamanaIndex.h"
#include "../include/Filter.h"
#include "../include/acutest.h"


void test_filtered_vamana_get_filters(void) {

    std::set<CategoricalAttributeFilter> filters;
    CategoricalAttributeFilter filter1(1);
    CategoricalAttributeFilter filter2(2);
    CategoricalAttributeFilter filter3(3);

    filters.insert(filter1);
    filters.insert(filter2);
    filters.insert(filter3);

    FilteredVamanaIndex<BaseDataVector<float>> index(filters);

    std::set<CategoricalAttributeFilter> computedFilters = index.getFilters();

    TEST_CHECK(computedFilters == filters);

}

TEST_LIST = {
    { "filtered_vamana_get_filters", test_filtered_vamana_get_filters },
    { NULL, NULL }
};
