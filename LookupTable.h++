//
// Created by Nathan White on 11/1/22.
//

#ifndef TLAG_23_LOOKUPTABLE_H
#define TLAG_23_LOOKUPTABLE_H

#include <string>
#include <cstdint>
#include <array>
#include <utility>
#include <vector>
#include <thread>
#include <algorithm>
#include <numeric>
#include <execution>
#include <initializer_list>

#define ND [[nodiscard]]
#define MU [[maybe_unused]]

class LookupTable {
private:
    std::string                                 m_name;
    std::uint32_t                               m_id;
    std::vector< std::array< std::uint32_t, 2>> m_lookup_table;

public:
    LookupTable(std::string name, std::uint32_t id, std::vector< std::array< std::uint32_t, 2>> lookup_table) :
            m_name(std::move(name)), m_id(id), m_lookup_table(std::move(lookup_table)) {
        std::sort(std::execution::par, m_lookup_table.begin(), m_lookup_table.end(), [](
                const std::array< std::uint32_t, 2 > & a, const std::array< std::uint32_t, 2 > & b) {
            return a[0] < b[0];
        });
    }
    [[nodiscard]] std::string getName() const { return m_name; }
    [[nodiscard]] std::uint32_t getId() const { return m_id; }
    [[nodiscard]] std::vector< std::array< std::uint32_t, 2>> getLookupTable() const { return m_lookup_table; }

    // Constructor via initializer list
    LookupTable(std::string name, std::uint32_t id, std::initializer_list< std::array< std::uint32_t, 2>> lookup_table)
            :
            m_name(std::move(name)), m_id(id), m_lookup_table(lookup_table) {
        std::sort(m_lookup_table.begin(), m_lookup_table.end(), [](const std::array< std::uint32_t, 2 > & a
                                                                   , const std::array< std::uint32_t, 2 > & b) {
            return a[0] < b[0];
        });
    }


    // Parallel Lookup the value associated with the given key.
    [[nodiscard]] std::uint32_t parallelLookup(std::uint32_t key) const {
        auto it = std::find_if(std::execution::par, m_lookup_table.begin(), m_lookup_table.end(), [key](
                const std::array< std::uint32_t, 2 > & pair) { return pair[0] == key; });
        return it != m_lookup_table.end() ? (* it)[1] : 0;
    };

    // Sequential Lookup the value associated with the given key.
    [[nodiscard]] std::uint32_t sequentialLookup(std::uint32_t key) const {
        auto it = std::find_if(std::execution::seq, m_lookup_table.begin(), m_lookup_table.end(), [key](
                const std::array< std::uint32_t, 2 > & pair) { return pair[0] == key; });
        return it != m_lookup_table.end() ? (* it)[1] : 0;
    };


    // Reverse Lookup the key associated with the given value.
    [[nodiscard]] std::uint32_t reverseLookup(std::uint32_t value) const {
        auto it = std::find_if(std::execution::seq, m_lookup_table.begin(), m_lookup_table.end(), [value](
                const std::array< std::uint32_t, 2 > & pair) { return pair[1] == value; });
        return it != m_lookup_table.end() ? (* it)[0] : 0;
    };

    // Parallel reverse Lookup the key associated with the given value.
    [[nodiscard]] std::uint32_t parallelReverseLookup(std::uint32_t value) const {
        auto it = std::find_if(std::execution::par, m_lookup_table.begin(), m_lookup_table.end(), [value](
                const std::array< std::uint32_t, 2 > & pair) { return pair[1] == value; });
        return it != m_lookup_table.end() ? (* it)[0] : 0;
    };
};


class NesterdLookupTable {
private:
    std::string                m_name;
    std::uint32_t              m_id;
    std::vector< LookupTable > m_lookup_table;

public:
    NesterdLookupTable(std::string name, std::uint32_t id, std::vector< LookupTable > lookup_table) :
            m_name(std::move(name)), m_id(id), m_lookup_table(std::move(lookup_table)) {
        std::sort(std::execution::par, m_lookup_table.begin(), m_lookup_table.end(), [](
                const LookupTable & a, const LookupTable & b) {
            return a.getId() < b.getId();
        });
    }
    [[nodiscard]] std::string getName() const { return m_name; }
    [[nodiscard]] std::uint32_t getId() const { return m_id; }
    [[nodiscard]] std::vector< LookupTable > getLookupTable() const { return m_lookup_table; }

    // Constructor via initializer list
    NesterdLookupTable(std::string name, std::uint32_t id, std::initializer_list< LookupTable > lookup_table)
            :
            m_name(std::move(name)), m_id(id), m_lookup_table(lookup_table) {
        std::sort(m_lookup_table.begin(), m_lookup_table.end(), [](const LookupTable & a
                                                                   , const LookupTable & b) {
            return a.getId() < b.getId();
        });
    }

    // Parallel Lookup the value associated with the given key.
    [[nodiscard]] std::uint32_t parallelLookup(std::uint32_t key) const {
        auto it = std::find_if(std::execution::par, m_lookup_table.begin(), m_lookup_table.end(), [key](
                const LookupTable & pair) { return pair.getId() == key; });
        return it != m_lookup_table.end() ? it->parallelLookup(key) : 0;
    };

    // Sequential Lookup the value associated with the given key.
    [[nodiscard]] std::uint32_t sequentialLookup(std::uint32_t key) const {
        auto it = std::find_if(std::execution::seq, m_lookup_table.begin(), m_lookup_table.end(), [key](
                const LookupTable & pair) { return pair.getId() == key; });
        return it != m_lookup_table.end() ? it->sequentialLookup(key) : 0;
    };

    // Reverse Lookup the key associated with the given value.
    [[nodiscard]] std::uint32_t reverseLookup(std::uint32_t value) const {
        auto it = std::find_if(std::execution::seq, m_lookup_table.begin(), m_lookup_table.end(), [value](
                const LookupTable & pair) { return pair.getId() == value; });
        return it != m_lookup_table.end() ? it->reverseLookup(value) : 0;
    };


    // Parallel reverse Lookup the key associated with the given value.
    [[nodiscard]] std::uint32_t parallelReverseLookup(std::uint32_t value) const {
        auto it = std::find_if(std::execution::par, m_lookup_table.begin(), m_lookup_table.end(), [value](
                const LookupTable & pair) { return pair.getId() == value; });
        return it != m_lookup_table.end() ? it->parallelReverseLookup(value) : 0;
    };


};


template < typename N >
struct Hash {
    std::hash< N > hash;

    std::size_t operator ()(const N & n) const {
        return std::hash(Hash::hash(n));
    }
};

template < typename LHS, typename RHS >
struct Equals {

    std::hash< LHS > lhs_hash;
    std::hash< RHS > rhs_hash;

    bool operator ()(const LHS & lhs, const RHS & rhs) const {
        return std::equal_to(Equals::lhs_hash(lhs), Equals::rhs_hash(rhs));
    }
};


template < typename I >
class SafeArithmetic {

    static_assert(std::is_integral_v< I >, "SafeArithmetic requires an integral type.");

public:

    bool additive_overflow(I lhs, I rhs) {
        return (rhs > 0 && lhs > (std::numeric_limits< I >::max() - rhs)) ||
               (rhs < 0 && lhs < (std::numeric_limits< I >::min() - rhs));
    }

    bool subtractive_overflow(I lhs, I rhs) {
        return (rhs < 0 && lhs > (std::numeric_limits< I >::max() + rhs)) ||
               (rhs > 0 && lhs < (std::numeric_limits< I >::min() + rhs));
    }

    bool multiplicative_overflow(I lhs, I rhs) {
        if (lhs > 0) {
            if (rhs > 0) {
                return lhs > (std::numeric_limits< I >::max() / rhs);
            } else if (rhs < -1) {
                return lhs < (std::numeric_limits< I >::min() / rhs);
            }
        } else if (lhs < -1) {
            if (rhs > 0) {
                return lhs < (std::numeric_limits< I >::min() / rhs);
            } else if (rhs < -1) {
                return lhs > (std::numeric_limits< I >::max() / rhs);
            }
        }
        return false;
    }

    bool division_overflow(I lhs, I rhs) {
        return lhs == std::numeric_limits< I >::min() && rhs == -1;
    }

    bool modulus_overflow(I lhs, I rhs) {
        return false;
    }

    bool left_shift_overflow(I lhs, I rhs) {
        return rhs < 0 || rhs >= std::numeric_limits< I >::digits || lhs > (std::numeric_limits< I >::max() >> rhs);
    }

    bool right_shift_overflow(I lhs, I rhs) {
        return rhs < 0 || rhs >= std::numeric_limits< I >::digits;
    }

    bool bitwise_and_overflow(I lhs, I rhs) {
        return false;
    }

    bool bitwise_or_overflow(I lhs, I rhs) {
        return false;
    }

    bool bitwise_xor_overflow(I lhs, I rhs) {
        return false;
    }

    bool bitwise_not_overflow(I lhs) {
        return false;
    }

    bool bitwise_left_shift_overflow(I lhs, I rhs) {
        return rhs < 0 || rhs >= std::numeric_limits< I >::digits || lhs > (std::numeric_limits< I >::max() >> rhs);
    }

    bool bitwise_right_shift_overflow(I lhs, I rhs) {
        return rhs < 0 || rhs >= std::numeric_limits< I >::digits;
    }

    bool bitwise_complement_overflow(I lhs) {
        return false;
    }

    bool bitwise_and_assignment_overflow(I lhs, I rhs) {
        return false;
    }

    bool bitwise_or_assignment_overflow(I lhs, I rhs) {
        return false;
    }

    bool bitwise_xor_assignment_overflow(I lhs, I rhs) {
        return false;
    }

    bool bitwise_left_shift_assignment_overflow(I lhs, I rhs) {
        return rhs < 0 || rhs >= std::numeric_limits< I >::digits || lhs > (std::numeric_limits< I >::max() >> rhs);
    }

    bool bitwise_right_shift_assignment_overflow(I lhs, I rhs) {
        return rhs < 0 || rhs >= std::numeric_limits< I >::digits;
    }

    bool bitwise_complement_assignment_overflow(I lhs) {
        return false;
    }

    bool additive_underflow(I lhs, I rhs) {
        return (rhs > 0 && lhs < (std::numeric_limits< I >::min() + rhs)) ||
               (rhs < 0 && lhs > (std::numeric_limits< I >::max() + rhs));
    }

    bool subtractive_underflow(I lhs, I rhs) {
        return (rhs < 0 && lhs < (std::numeric_limits< I >::min() - rhs)) ||
               (rhs > 0 && lhs > (std::numeric_limits< I >::max() - rhs));
    }

    bool multiplicative_underflow(I lhs, I rhs) {
        if (lhs > 0) {
            if (rhs > 0) {
                return lhs < (std::numeric_limits< I >::min() / rhs);
            } else if (rhs < -1) {
                return lhs > (std::numeric_limits< I >::max() / rhs);
            }
        } else if (lhs < -1) {
            if (rhs > 0) {
                return lhs > (std::numeric_limits< I >::max() / rhs);
            } else if (rhs < -1) {
                return lhs < (std::numeric_limits< I >::min() / rhs);
            }
        }
        return false;
    }

    bool division_underflow(I lhs, I rhs) {
        return false;
    }

    bool modulus_underflow(I lhs, I rhs) {
        return false;
    }

    bool left_shift_underflow(I lhs, I rhs) {
        return rhs < 0 || rhs >= std::numeric_limits< I >::digits || lhs < (std::numeric_limits< I >::min() >> rhs);
    }
    

};


template < typename N >
struct FloatingPointZero {

    N default_tolerance = 1e-10;

    bool operator ()(const N & n) const {
        return std::abs(n) < default_tolerance;
    }

    bool operator ()(const N & n, const N & tolerance) const {
        return std::abs(n) < tolerance;
    }
};

template < typename N >
struct FloatingPointEquals {

    N default_tolerance = 1e-10;

    bool operator ()(const N & lhs, const N & rhs) const {
        return FloatingPointZero< N >::operator ()(lhs - rhs, default_tolerance);
    }

    bool operator ()(const N & lhs, const N & rhs, const N & tolerance) const {
        return FloatingPointZero< N >::operator ()(lhs - rhs, tolerance);
    }

};


template < typename K, typename V >
class SwitchCase {
private:
    std::unordered_map< K, V > m_map;

public:
    explicit SwitchCase(std::unordered_map< K, V > map) : m_map(std::move(map)) {}

    ND V operator ()(K key) const {

        // Find the key in the map
        auto it = m_map.find(key);

        // If the key is found, return the value
        if (it != m_map.end()) {

            return it->second;
        }
        return V();
    }
};


template < typename K, typename V, std::size_t N, typename FallthroughPolicy, typename DefaultCasePolicy >
class ND MU LUTSwitch {

private: /* Policies */
    MU static FallthroughPolicy policy_fallthrough;
    MU static DefaultCasePolicy policy_default;

private: /* Private Data */

    /* The lookup table */
    std::array< SwitchCase< K, V >, N + 1 > m_lookup_table = nullptr;

    /* Stores the number of times each case is executed */
    std::array< std::size_t, N > m_num_triggers = {0x0};

    /* The number of cases that the user has inserted */
    std::size_t m_cases = 0x0;

    /* Error Message if the user tries to access a case that does not exist */
    std::string m_error_message = "Case does not exist";

public:

    explicit LUTSwitch(std::array< SwitchCase< K, V >, N > lookup_table) :
            m_lookup_table(std::move(lookup_table)) {
        m_cases = m_lookup_table.size();
    }

    void add_case(SwitchCase< K, V > switch_case) {
        m_lookup_table = switch_case;
        m_cases++;
    }

    LUTSwitch() = default;

    LUTSwitch(const LUTSwitch & other) = default;

    LUTSwitch(LUTSwitch && other) noexcept = default;

    LUTSwitch & operator =(const LUTSwitch & other) = default;

    LUTSwitch & operator =(LUTSwitch && other) noexcept = default;

    virtual ~LUTSwitch() = default;

    V scan(K key) const {

        // Find the key in the map
        for (auto i = 0; i < m_cases; i++) {

            V value = m_lookup_table[i](key);

            if (value != V()) {

                m_num_triggers[i]++;


                if constexpr (std::is_same_v< FallthroughPolicy, typename FallthroughPolicy::NoFallthrough >) {
                    return value;
                } else {
                    continue;
                }
            }
        }

        return policy_default(m_error_message);
    }
};


template < typename K, typename V, std::size_t N >
class ND MU MutableLUTSwitch {

private:

    /* The lookup table */
    std::vector< SwitchCase< K, V >> m_lookup_table;

    /* Stores the number of times each case is executed */
    std::array< std::size_t, N > m_num_triggers = {0x0};

    /* The number of cases that the user has inserted */
    std::size_t m_cases = 0x0;


public:

    explicit MutableLUTSwitch(std::vector< SwitchCase< K, V >> lookup_table) :
            m_lookup_table(std::move(lookup_table)) {
        m_cases = m_lookup_table.size();
    }

    void add_case(SwitchCase< K, V > switch_case) {
        m_lookup_table.push_back(switch_case);
        m_cases++;
    }

    void add_case(std::initializer_list< SwitchCase< K, V >> switch_case) {
        m_lookup_table.insert(m_lookup_table.end(), switch_case);
        m_cases = m_lookup_table.size();
    }

    void add_case(std::vector< SwitchCase< K, V >> switch_case) {
        m_lookup_table.insert(m_lookup_table.end(), switch_case.begin(), switch_case.end());
        m_cases = m_lookup_table.size();
    }


};

};

#endif