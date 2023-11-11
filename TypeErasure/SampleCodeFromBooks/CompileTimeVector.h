#pragma once

#include <vector>
#include <ranges>
#include <algorithm>
#include <numeric>

template<std::ranges::input_range T>
consteval auto modifiedAvg(const T& rg)
{
    using elemType = std::ranges::range_value_t<T>;

    // initialize compile-time vector with passed elements:
    std::vector<elemType> v{ std::ranges::begin(rg),
                            std::ranges::end(rg) };
    // perform several modifications:
    v.push_back(elemType{});
    std::ranges::sort(v);
    auto newEnd = std::unique(v.begin(), v.end());
    //...

    // return average of modified vector:
    auto sum = std::accumulate(v.begin(), newEnd,
        elemType{});
    //return v;
    return sum / static_cast<double>(v.size());
}

#if 0
template<std::ranges::input_range T>
consteval auto callModifiedAvg(T&& rg)
{
    constexpr auto vec = modifiedAvg(rg);
    using elemType = std::ranges::range_value_t<T>;

    auto sum = std::accumulate(vec.begin(), vec.end(),
        elemType{});

    return sum / (double)(vec.size());
}
#endif


template<std::ranges::input_range T>
consteval auto mergeValues(T rg, auto...vals)
{
    std::vector<std::ranges::range_value_t<T>> v{ std::ranges::begin(rg), std::ranges::end(rg) };
    (..., v.push_back(vals)); // merge all passed parameters

    std::ranges::sort(v);

    constexpr auto sz = std::ranges::size(rg) + sizeof...(vals);

    std::array<std::ranges::range_value_t<T>, sz> arr{};
    std::ranges::copy(v, arr.begin());
    return arr;
}

