#include <iostream>  // For std::cout
#include <cstddef>   // For std::size_t
#include <expected>  // For std::expected / std::unexpected (C++23)
#include <array>     // For std::array in constructor
#include <algorithm> // For std::copy
#include <list>      // For std::list used in other tests
#include <iterator>  // For iterator tags like std::contiguous_iterator_tag

// Const iterator class for fixed-size Array
template <typename T, std::size_t Size>
class ArrayConstIterator
{
public:
    using iterator_category = std::contiguous_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T *;
    using const_reference = const T &;
    using reference = T &;

    constexpr ArrayConstIterator() noexcept = default;

    constexpr explicit ArrayConstIterator(pointer pt, size_t offset = 0) noexcept
        : m_ptr{pt}, m_offset{offset} {}

    [[nodiscard]] constexpr const_reference operator*() const noexcept
    {
        return m_ptr[m_offset];
    }

    constexpr ArrayConstIterator &operator++() noexcept
    {
        ++m_offset;
        return *this;
    }

    constexpr ArrayConstIterator operator++(int) noexcept
    {
        ArrayConstIterator temp = *this;
        ++(*this);
        return temp;
    }

    constexpr ArrayConstIterator &operator--() noexcept
    {
        --m_offset;
        return *this;
    }

    constexpr ArrayConstIterator operator--(int) noexcept
    {
        ArrayConstIterator temp = *this;
        --(*this);
        return temp;
    }

    constexpr ArrayConstIterator &operator+=(difference_type offset) noexcept
    {
        m_offset += offset;
        return *this;
    }

    [[nodiscard]] constexpr auto operator<=>(const ArrayConstIterator &other) const noexcept
    {
        return m_offset <=> other.m_offset;
    }

    [[nodiscard]] constexpr bool operator==(const ArrayConstIterator &other) const noexcept
    {
        return (*this <=> other) == 0;
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept
    {
        return m_ptr + m_offset;
    }

    [[nodiscard]] constexpr bool operator!=(const ArrayConstIterator &other) const noexcept
    {
        return !(*this == other);
    }

    [[nodiscard]] constexpr ArrayConstIterator operator+(difference_type offset) const noexcept
    {
        return ArrayConstIterator(m_ptr, m_offset + offset);
    }

    [[nodiscard]] constexpr ArrayConstIterator operator-(difference_type offset) const noexcept
    {
        return ArrayConstIterator(m_ptr, m_offset - offset);
    }

    constexpr ArrayConstIterator &operator-=(difference_type offset) noexcept
    {
        m_offset -= offset;
        return *this;
    }

    [[nodiscard]] constexpr ArrayConstIterator &operator-() noexcept
    {
        m_offset = -m_offset;
        return *this;
    }

    [[nodiscard]] constexpr difference_type operator-(const ArrayConstIterator &other) const noexcept
    {
        return static_cast<difference_type>(m_offset) - static_cast<difference_type>(other.m_offset);
    }

protected:
    pointer m_ptr{nullptr};
    size_t m_offset{0};
};

// Mutable iterator inheriting from ArrayConstIterator
template <typename T, std::size_t Size>
class ArrayIterator : public ArrayConstIterator<T, Size>
{
public:
    using MyBase = ArrayConstIterator<T, Size>;
    using typename MyBase::const_reference;
    using typename MyBase::difference_type;
    using typename MyBase::iterator_category;
    using typename MyBase::pointer;
    using typename MyBase::reference;
    using typename MyBase::value_type;

    constexpr ArrayIterator() noexcept = default;

    constexpr explicit ArrayIterator(pointer ptr, size_t offset = 0) noexcept
        : MyBase(ptr, offset) {}

    [[nodiscard]] constexpr reference operator*() const noexcept
    {
        return const_cast<reference>(MyBase::operator*());
    }

    constexpr ArrayIterator &operator++() noexcept
    {
        MyBase::operator++();
        return *this;
    }

    constexpr ArrayIterator operator++(int) noexcept
    {
        ArrayIterator temp = *this;
        MyBase::operator++();
        return temp;
    }

    constexpr ArrayIterator &operator--() noexcept
    {
        MyBase::operator--();
        return *this;
    }

    constexpr ArrayIterator operator--(int) noexcept
    {
        ArrayIterator temp = *this;
        MyBase::operator--();
        return temp;
    }

    constexpr ArrayIterator &operator+=(difference_type offset) noexcept
    {
        MyBase::operator+=(offset);
        return *this;
    }

    constexpr ArrayIterator &operator-=(difference_type offset) noexcept
    {
        MyBase::operator-=(offset);
        return *this;
    }

    [[nodiscard]] constexpr auto operator<=>(const ArrayIterator &other) const noexcept
    {
        return MyBase::operator<=>(other);
    }

    [[nodiscard]] constexpr bool operator==(const ArrayIterator &other) const noexcept
    {
        return MyBase::operator==(other);
    }

    [[nodiscard]] constexpr bool operator!=(const ArrayIterator &other) const noexcept
    {
        return !(*this == other);
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept
    {
        return this->m_ptr + this->m_offset;
    }

    [[nodiscard]] constexpr ArrayIterator operator+(difference_type offset) const noexcept
    {
        return ArrayIterator(this->m_ptr, this->m_offset + offset);
    }

    [[nodiscard]] constexpr ArrayIterator &operator-() noexcept
    {
        this->m_offset = -this->m_offset;
        return *this;
    }

    [[nodiscard]] constexpr ArrayIterator operator-(difference_type offset) const noexcept
    {
        return ArrayIterator(this->m_ptr, this->m_offset - offset);
    }

    [[nodiscard]] constexpr difference_type operator-(const ArrayIterator &other) const noexcept
    {
        return static_cast<difference_type>(this->m_offset) - static_cast<difference_type>(other.m_offset);
    }
};

// Fixed-size array container
template <typename T, std::size_t Size>
class Array
{
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T &;
    using const_reference = const T &;
    using pointer = T *;
    using const_iterator = ArrayConstIterator<T, Size>;
    using iterator = ArrayIterator<T, Size>;

    Array() = default;

    template <typename... Values>
    constexpr Array(Values... values)
    {
        static_assert(sizeof...(Values) == Size, "Number of arguments must match array size.");
        static_assert((std::is_convertible_v<Values, T> && ...), "All Values must be T");
        const std::array<T, sizeof...(Values)> temp = {values...};
        std::copy(temp.begin(), temp.end(), m_elements);
    }

    [[nodiscard]] constexpr size_type size() const noexcept { return Size; }

    constexpr bool empty() const noexcept { return Size == 0; }

    [[nodiscard]] constexpr reference operator[](size_type pos) noexcept { return m_elements[pos]; }

    [[nodiscard]] constexpr const_reference operator[](size_type pos) const noexcept { return m_elements[pos]; }

    const_iterator begin() const noexcept { return const_iterator(m_elements); }
    const_iterator end() const noexcept { return const_iterator(m_elements, Size); }

    iterator begin() noexcept { return iterator(m_elements); }
    iterator end() noexcept { return iterator(m_elements, Size); }

private:
    T m_elements[Size]{};
};

// FIXED VERSION: Finds iterator to the maximum element
template <typename It>
constexpr It max_element(It begin, It end) noexcept
{
    if (begin == end)
        return end; // guard for empty range

    auto max_it = begin;
    for (auto it = std::next(begin); it != end; ++it)
    {
        if (*it > *max_it)
        {
            max_it = it;
        }
    }
    return max_it;
}

int main()
{
    Array<float, 5> ages = {12.2f, 15.0f, 17.0f, 19.4f, 23.0f};

    const auto res = max_element(ages.begin(), ages.end());
    if (res != ages.end())
        std::cout << "MAX : " << *res << "\n";
    else
        std::cout << "Array was empty.\n";

    return 0;
}
