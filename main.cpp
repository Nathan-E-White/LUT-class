#include <iostream>


#include <ctime>
#include <chrono>
#include <vector>
#include <map>
#include <optional>
#include <iomanip>
#include <utility>
#include <unordered_map>

class DateTimeObject {
private:

    std::optional<std::chrono::system_clock::time_point> m_time_point;
    std::optional<std::chrono::year_month_day> m_datestamp;
    std::optional<std::chrono::hh_mm_ss<std::chrono::seconds>> m_timestamp;

public:


};


template <typename T, typename V>
class TimeValuePair {
private:
    T m_time;
    V m_value;

public:
    TimeValuePair(T time, V value) : m_time(time), m_value(value) {}
    T getTime() const { return m_time; }
    V getValue() const { return m_value; }
};
}



template <typename T, typename V>
class TimeSeries {
private:
    std::unordered_map <
        T, V, std::hash<T>, std::equal_to<T>,
        std::pmr::polymorphic_allocator<std::pair<const T, V>>
    > m_pmr_umap2;

};

template <typename T, typename V, std::size_t D>
class TemporalData {
private:

    std::unordered_multimap <
        T, std::array<V, D>, std::hash<T>, std::equal_to<T>,
        std::pmr::polymorphic_allocator<std::pair<const T, std::array<V, D>>>
    > m_temporal_data;
};

template <typename T, std::size_t I, typename V, std::size_t D>
class TaggedTemporalData {

private:

    std::reference_wrapper<std::LookupTable<T, I>> m_lookup_table;
};



class DailyData {

};



int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
