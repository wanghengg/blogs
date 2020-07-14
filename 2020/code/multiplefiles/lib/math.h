#include <limits>
#include "../definition/error_code.h"

template<typename ValueType>
ValueType ValueTypeMax(ValueType) {
    return std::numeric_limits<ValueType>::max();
}

template<typename ValueType>
int safeAdd(ValueType &sum) {
    return exit_success;
}

template<typename ValueType, typename ...ValueTypes>
int safeAdd(ValueType &sum, const ValueType &value, const ValueTypes &...other_values) {
    int ret_val = safeAdd<ValueType>(sum, other_values...);
    if (ret_val != exit_success) {
        return ret_val;
    }

    if (sum > ValueTypeMax(value) - value) {
        sum = ValueTypeMax(value);
        return error_data_overflow;
    }

    sum += value;
    return exit_success;
}