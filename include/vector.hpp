#pragma once

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <ranges>

namespace tinystl {

template <typename T>
class vector_iterator {
public:
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = T&;

    vector_iterator(T* ptr)
        : m_ptr{ptr} {}

    reference operator*() const {
        return *m_ptr;
    }

    vector_iterator& operator++() {
        ++m_ptr;
        return *this;
    }

    vector_iterator operator++(int) {
        vector_iterator it{*this};
        ++*this;
        return it;
    }

    vector_iterator& operator--() {
        --m_ptr;
        return *this;
    }

    vector_iterator operator--(int) {
        vector_iterator it{*this};
        --*this;
        return it;
    }

    vector_iterator& operator+=(std::ptrdiff_t n) {
        std::next(m_ptr, n);
        return *this;
    }

    vector_iterator operator+(std::ptrdiff_t n) const {
        vector_iterator it{*this};
        it += n;
        return it;
    }

    friend vector_iterator operator+(std::ptrdiff_t n, const vector_iterator& it) {
        return it + n;
    }

    vector_iterator& operator-=(std::ptrdiff_t n) {
        std::prev(m_ptr, n);
        return *this;
    }

    vector_iterator operator-(std::ptrdiff_t n) const {
        vector_iterator it{*this};
        it -= n;
        return it;
    }

    friend vector_iterator operator-(const vector_iterator& first, const vector_iterator& last) {
        return vector_iterator{last.m_ptr - first.m_ptr};
    }

    reference operator[](std::ptrdiff_t n) {
        return *(m_ptr + n);
    }

    //  隐式预置 operator==
    friend auto operator<=>(const vector_iterator& first, const vector_iterator& last) = default;
private:
    T* m_ptr{};
};

template <typename T>
class vector {
public:
    using value_type = T;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using reference = value_type&;
    using const_reference = const value_type&;

    using pointer = value_type*;
    using const_pointer = const value_type*;

    using iterator = vector_iterator<T>;
    using const_iterator = vector_iterator<const T>;
    
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    vector() = default;

    explicit vector(std::size_t size)
        : m_data{new T[size]{}}, m_size{size}, m_capacity{size} {}

    vector(std::size_t size, const T& value)
        : m_data{new T[size]}, m_size{size}, m_capacity{size}
    {
        std::uninitialized_fill(m_data, m_data + size, value);
    }

    template <std::input_iterator InputIter>
    vector(InputIter first, InputIter last)
        : m_data{new T[std::distance(first, last)]}, m_size{std::distance(first, last)}, m_capacity{m_size}
    {
        std::uninitialized_copy(first, last, m_data);
    }

    template <std::ranges::input_range Range>
    vector(Range&& range)
        : vector(std::ranges::begin(range), std::ranges::end(range)) {}

    vector(const vector& other)
        : m_data{new T[other.m_size]}, m_size{other.m_size}, m_capacity{other.m_capacity}
    {
        std::uninitialized_copy(other.m_data, other.m_data + other.m_size, m_data);
    }

    vector(vector&& other) noexcept
        : m_data{other.m_data}, m_size{other.m_size}, m_capacity{other.m_capacity}
    {
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    vector(std::initializer_list<T> init_list)
        : m_data{new T[init_list.size()]}, m_size{init_list.size()}, m_capacity{init_list.size()}
    {
        std::uninitialized_copy(init_list.begin(), init_list.end(), m_data);
    }

    ~vector() {
        delete[] m_data;
    }

    vector& operator=(const vector& other) {
        delete[] m_data;

        m_data = new T[other.m_size];
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        std::uninitialized_copy(other.m_data, other.m_data + other.m_size, m_data);
        return *this;
    }

    vector& operator=(vector&& other) noexcept {
        delete[] m_data;

        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;

        return *this;
    }

    vector& operator=(std::initializer_list<T> init_list) {
        delete[] m_data;

        m_data = new T[init_list.size()];
        m_size = init_list.size();
        m_capacity = init_list.size();

        std::uninitialized_copy(init_list.begin(), init_list.end(), m_data);
        return *this;
    }

    iterator begin() const {
        return iterator{m_data};
    }

    iterator end() const {
        return iterator{m_data + m_size};
    }
private:
    T* m_data{};
    std::size_t m_size{};
    std::size_t m_capacity{};
};

} // namespace tinystl