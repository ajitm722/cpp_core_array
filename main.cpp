#include <iostream>   // For std::cout
#include <cstddef>    // For std::size_t, std::ptrdiff_t
#include <iterator>   // For std::next, contiguous_iterator_tag
#include <fstream>    // For file I/O
#include <string>     // For std::string
#include <sstream>    // For std::istringstream
#include <fmt/core.h> // For fmt::print
#include <tuple>      // For std::tuple

// Namespace for custom container implementations
namespace cpp_core::container
{

    // Const iterator class for fixed-size Array
    // Implements a read-only, contiguous iterator compatible with STL algorithms and iterator traits.
    // Allows iteration over elements in a fixed-size container (like std::array).
    template <typename T, std::size_t Size>
    class ArrayConstIterator
    {
    public:
        // Type aliases required for STL iterator compliance
        using iterator_category = std::contiguous_iterator_tag; // (C++20) Indicates that elements are laid out contiguously in memory
        using value_type = T;                                   // Type of the elements the iterator refers to
        using difference_type = std::ptrdiff_t;                 // Used for expressing the distance between iterators
        using pointer = const T *;                              // Raw pointer to const elements
        using const_reference = const T &;                      // Reference type used for read-only access
        using reference = T &;                                  // Writable reference (not used in this const iterator)

        // Default constructor: creates an invalid iterator (null pointer, offset 0)
        constexpr ArrayConstIterator() noexcept = default;

        // Constructor: initializes iterator to point to a specific offset within an array
        constexpr explicit ArrayConstIterator(pointer pt, size_t offset = 0) noexcept
            : m_ptr{pt}, m_offset{offset} {}

        // Dereference operator: returns a const reference to the current element
        [[nodiscard]] constexpr const_reference operator*() const noexcept
        {
            return m_ptr[m_offset];
        }

        // Prefix increment: advances iterator to next element, returns *this
        constexpr ArrayConstIterator &operator++() noexcept
        {
            ++m_offset;
            return *this;
        }

        // Postfix increment: returns copy before advancing
        constexpr ArrayConstIterator operator++(int) noexcept
        {
            ArrayConstIterator temp = *this;
            ++(*this);
            return temp;
        }

        // Prefix decrement: moves iterator to previous element, returns *this
        constexpr ArrayConstIterator &operator--() noexcept
        {
            --m_offset;
            return *this;
        }

        // Postfix decrement: returns copy before retreating
        constexpr ArrayConstIterator operator--(int) noexcept
        {
            ArrayConstIterator temp = *this;
            --(*this);
            return temp;
        }

        // Compound addition assignment: move forward by offset
        constexpr ArrayConstIterator &operator+=(difference_type offset) noexcept
        {
            m_offset += offset;
            return *this;
        }

        // Spaceship operator: enables ordering comparisons (e.g., <, >, ==) using C++20 three-way comparison
        [[nodiscard]] constexpr auto operator<=>(const ArrayConstIterator &other) const noexcept
        {
            return m_offset <=> other.m_offset;
        }

        // Equality operator: true if two iterators point to the same logical position
        [[nodiscard]] constexpr bool operator==(const ArrayConstIterator &other) const noexcept
        {
            return (*this <=> other) == 0;
        }

        // Arrow operator: returns pointer to current element (to access member fields)
        [[nodiscard]] constexpr pointer operator->() const noexcept
        {
            return m_ptr + m_offset;
        }

        // Inequality operator: logical negation of equality
        [[nodiscard]] constexpr bool operator!=(const ArrayConstIterator &other) const noexcept
        {
            return !(*this == other);
        }

        // Addition: returns a new iterator advanced by given offset
        [[nodiscard]] constexpr ArrayConstIterator operator+(difference_type offset) const noexcept
        {
            return ArrayConstIterator(m_ptr, m_offset + offset);
        }

        // Subtraction: returns a new iterator retreated by given offset
        [[nodiscard]] constexpr ArrayConstIterator operator-(difference_type offset) const noexcept
        {
            return ArrayConstIterator(m_ptr, m_offset - offset);
        }

        // Compound subtraction assignment: move backward by offset
        constexpr ArrayConstIterator &operator-=(difference_type offset) noexcept
        {
            m_offset -= offset;
            return *this;
        }

        // Unary minus: negates current offset (rarely used, potentially unsafe in real usage)
        [[nodiscard]] constexpr ArrayConstIterator &operator-() noexcept
        {
            m_offset = -m_offset;
            return *this;
        }

        // Difference between two iterators: returns distance (this - other)
        [[nodiscard]] constexpr difference_type operator-(const ArrayConstIterator &other) const noexcept
        {
            return static_cast<difference_type>(m_offset) - static_cast<difference_type>(other.m_offset);
        }

    protected:
        pointer m_ptr{nullptr}; // Pointer to the base of the container’s data
        size_t m_offset{0};     // Logical offset from base pointer
    };

    // Mutable iterator inheriting from ArrayConstIterator
    // Allows element modification and full STL compatibility (e.g., std::copy, std::sort).
    // All inherited traits and operations are reused, with writable dereference logic added.
    template <typename T, std::size_t Size>
    class ArrayIterator : public ArrayConstIterator<T, Size>
    {
    public:
        using MyBase = ArrayConstIterator<T, Size>; // Alias to simplify references to the base class

        // Inherit STL-compliant iterator traits from the base
        using typename MyBase::const_reference;   // Read-only reference to element
        using typename MyBase::difference_type;   // Type used for pointer arithmetic (ptrdiff_t)
        using typename MyBase::iterator_category; // Declares this a contiguous iterator (C++20)
        using typename MyBase::pointer;           // Raw pointer type (T*)
        using typename MyBase::reference;         // Writable reference to element (T&)
        using typename MyBase::value_type;        // Element type (T)

        // Bring protected base class data members into scope to avoid use of `this->`
        using MyBase::m_offset; // Logical offset from base pointer
        using MyBase::m_ptr;    // Base pointer to array storage

        // Default constructor: creates an uninitialized iterator
        constexpr ArrayIterator() noexcept = default;

        // Construct iterator from raw pointer and offset
        constexpr explicit ArrayIterator(pointer ptr, size_t offset = 0) noexcept
            : MyBase(ptr, offset) {}

        // Dereference operator: allows writing to the pointed element
        // Casts away const-ness because base class only provides read access
        [[nodiscard]] constexpr reference operator*() const noexcept
        {
            return const_cast<reference>(MyBase::operator*());
        }

        // Pre-increment: advances iterator by one element
        constexpr ArrayIterator &operator++() noexcept
        {
            MyBase::operator++();
            return *this;
        }

        // Post-increment: returns current iterator, then advances
        constexpr ArrayIterator operator++(int) noexcept
        {
            ArrayIterator temp = *this;
            MyBase::operator++();
            return temp;
        }

        // Pre-decrement: moves iterator one element backward
        constexpr ArrayIterator &operator--() noexcept
        {
            MyBase::operator--();
            return *this;
        }

        // Post-decrement: returns current iterator, then decrements
        constexpr ArrayIterator operator--(int) noexcept
        {
            ArrayIterator temp = *this;
            MyBase::operator--();
            return temp;
        }

        // Compound addition: moves iterator forward by given offset
        constexpr ArrayIterator &operator+=(difference_type offset) noexcept
        {
            MyBase::operator+=(offset);
            return *this;
        }

        // Compound subtraction: moves iterator backward by given offset
        constexpr ArrayIterator &operator-=(difference_type offset) noexcept
        {
            MyBase::operator-=(offset);
            return *this;
        }

        // Three-way comparison: enables ordered comparisons (e.g., sort)
        [[nodiscard]] constexpr auto operator<=>(const ArrayIterator &other) const noexcept
        {
            return MyBase::operator<=>(other);
        }

        // Equality check: true if both iterators point to the same element
        [[nodiscard]] constexpr bool operator==(const ArrayIterator &other) const noexcept
        {
            return MyBase::operator==(other);
        }

        // Inequality check: logical negation of equality
        [[nodiscard]] constexpr bool operator!=(const ArrayIterator &other) const noexcept
        {
            return !(*this == other);
        }

        // Arrow operator: allows pointer-style access to members of the element
        [[nodiscard]] constexpr pointer operator->() const noexcept
        {
            return m_ptr + m_offset;
        }

        // Binary addition: returns a new iterator advanced by offset
        [[nodiscard]] constexpr ArrayIterator operator+(difference_type offset) const noexcept
        {
            return ArrayIterator(m_ptr, m_offset + offset);
        }

        // Unary minus: negates offset (uncommon; use with care)
        [[nodiscard]] constexpr ArrayIterator &operator-() noexcept
        {
            m_offset = -m_offset;
            return *this;
        }

        // Binary subtraction (iterator - offset): returns a new iterator moved back
        [[nodiscard]] constexpr ArrayIterator operator-(difference_type offset) const noexcept
        {
            return ArrayIterator(m_ptr, m_offset - offset);
        }

        // Iterator difference: computes number of elements between two iterators
        [[nodiscard]] constexpr difference_type operator-(const ArrayIterator &other) const noexcept
        {
            return static_cast<difference_type>(m_offset) - static_cast<difference_type>(other.m_offset);
        }
    };

    // Fixed-size array container similar to std::array
    // Provides random-access storage and supports both const and mutable iterators
    // Designed to be STL-compatible using custom iterator types
    template <typename T, std::size_t Size>
    class Array
    {
    public:
        // --- Standard container type aliases for consistency with STL ---

        using value_type = T;                               // Type of elements stored in the array
        using size_type = std::size_t;                      // Type used to express array size and indices
        using reference = T &;                              // Reference to element (for modification)
        using const_reference = const T &;                  // Read-only reference to element
        using pointer = T *;                                // Raw pointer to element
        using const_pointer = const T *;                    // Const raw pointer to element
        using const_iterator = ArrayConstIterator<T, Size>; // Custom const iterator
        using iterator = ArrayIterator<T, Size>;            // Custom mutable iterator

        // --- Constructors ---

        // Default constructor: value-initializes all elements (zero or default constructed)
        Array() = default;

        // Variadic constructor for in-place initialization of elements
        // Accepts exactly `Size` arguments that are convertible to T
        template <typename... Values>
        constexpr Array(Values... values)
        {
            static_assert(sizeof...(Values) == Size, "Number of arguments must match array size.");
            static_assert((std::is_convertible_v<Values, T> && ...), "All values must be convertible to T");

            // Use a fold expression to initialize the internal array
            size_t index = 0;
            ((m_elements[index++] = values), ...);
        }
        // --- Element Access ---

        // Returns number of elements (known at compile time)
        [[nodiscard]] constexpr size_type size() const noexcept { return Size; }

        // Returns true if array has zero elements (valid for Array<T, 0>)
        constexpr bool empty() const noexcept { return Size == 0; }

        // Subscript operator (non-const): read/write access
        [[nodiscard]] constexpr reference operator[](size_type pos) noexcept
        {
            return m_elements[pos];
        }

        // Subscript operator (const): read-only access
        [[nodiscard]] constexpr const_reference operator[](size_type pos) const noexcept
        {
            return m_elements[pos];
        }

        // --- Iterator Access ---

        // Returns const iterator to beginning of array
        const_iterator begin() const noexcept { return const_iterator(m_elements); }

        // Returns const iterator to one-past-the-end
        const_iterator end() const noexcept { return const_iterator(m_elements, Size); }

        // Returns mutable iterator to beginning of array
        iterator begin() noexcept { return iterator(m_elements); }

        // Returns mutable iterator to one-past-the-end
        iterator end() noexcept { return iterator(m_elements, Size); }

        // Returns a mutable pointer to the underlying array storage
        [[nodiscard]] constexpr pointer data() noexcept { return m_elements; }

        // Returns a const pointer to the underlying array storage
        [[nodiscard]] constexpr const_pointer data() const noexcept { return m_elements; }

    private:
        // Internal storage array: fixed-size, statically allocated
        T m_elements[Size]{}; // Value-initialized to zero/default
    };
}

// Namespace for custom algorithm implementations
namespace cpp_core::algorithms
{
    // Finds an iterator to the maximum element in the range [begin, end)
    // - Returns end if the range is empty
    // - Uses iterator dereferencing and comparisons (requires operator>)
    template <typename It>
    constexpr It max_element(const It begin, const It end) noexcept
    {
        if (begin == end)
            return end; // Guard clause: return early if range is empty

        auto max_it = begin; // Initialize max to the first element
        for (auto it = std::next(begin); it != end; ++it)
        {
            if (*it > *max_it) // Compare current element to current max
            {
                max_it = it; // Update max if current is greater
            }
        }
        return max_it;
    }
    // Finds an iterator to the minimum element in [begin, end)
    template <typename It>
    constexpr It min_element(It begin, It end) noexcept
    {
        if (begin == end)
            return end;

        auto min_it = begin;
        for (auto it = std::next(begin); it != end; ++it)
        {
            if (*it < *min_it)
            {
                min_it = it;
            }
        }
        return min_it;
    }

    // Finds the first element matching a predicate in [begin, end)
    template <typename It, typename Predicate>
    constexpr It find_if(It begin, It end, Predicate pred) noexcept
    {
        for (auto it = begin; it != end; ++it)
        {
            if (pred(*it))
            {
                return it;
            }
        }
        return end;
    }

    // Counts how many elements match a predicate in [begin, end)
    template <typename It, typename Predicate>
    constexpr std::size_t count(It begin, It end, Predicate pred) noexcept
    {
        std::size_t cnt = 0;
        for (auto it = begin; it != end; ++it)
        {
            if (pred(*it))
            {
                ++cnt;
            }
        }
        return cnt;
    }
}

// Namespace for custom I/O implementations
namespace cpp_core::io_operations
{
    // Function to send file data to an API
    // - Reads the file in chunks of 512 bytes
    void sendFileDataToAPI(std::string_view filename)
    {
        // Define the chunk size (512 bytes, typical disk sector size)
        constexpr std::size_t chunk_size = 512;

        // Create a constexpr Array to hold a chunk of data (512 bytes)
        cpp_core::container::Array<char, chunk_size> buffer;

        // Open the file in binary mode
        std::ifstream file(filename.data(), std::ios::binary);
        if (!file.is_open())
        {
            fmt::print(stderr, "Failed to open the file: {}\n", filename);
            return;
        }

        std::string leftover; // To store partial lines between chunks

        // Read the file in chunks
        while (file.read(buffer.data(), chunk_size) || file.gcount() > 0)
        {
            // `file.gcount()`:
            // - Returns the number of bytes actually read into the buffer.
            // - This is useful for the last chunk, which may be smaller than `chunk_size`.

            std::size_t bytes_read = file.gcount();
            fmt::print("Read {} bytes:\n", bytes_read);

            // Combine leftover from the previous chunk with the current chunk
            std::string chunk_data = leftover + std::string(buffer.data(), bytes_read);
            leftover.clear(); // Clear leftover as it's now part of chunk_data

            // Convert the chunk into a string stream for line-by-line processing
            for (auto [chunk_stream, line] = std::tuple{std::istringstream(chunk_data), std::string{}};
                 std::getline(chunk_stream, line);)
            {
                if (chunk_stream.eof() && !chunk_data.empty() && chunk_data.back() != '\n')
                {
                    // If the line is incomplete (no newline at the end), save it as leftover
                    leftover = line;
                }
                else
                {
                    // Otherwise, process the complete line
                    fmt::print("Sending to API-> {}\n", line);
                }
            }
        }

        // Process any remaining leftover data as the last line
        if (!leftover.empty())
        {
            fmt::print("Sending to API-> {}\n", leftover);
        }

        // Close the file
        file.close();
    }
}
// Alias for custom container namespace
namespace cont = cpp_core::container;

// Alias for custom algorithms namespace
namespace algo = cpp_core::algorithms;

int main()
{
    constexpr cont::Array<float, 6> ages = {12.2f, 15.0f, 17.0f, 19.4f, 23.32f, 5.4f};

    // Test max_element
    const auto max_it = algo::max_element(ages.begin(), ages.end());
    if (max_it != ages.end())
        std::cout << "Max: " << *max_it << "\n";
    else
        std::cout << "No maximum element found.\n";

    // Test min_element
    const auto min_it = algo::min_element(ages.begin(), ages.end());
    if (min_it != ages.end())
        std::cout << "Min: " << *min_it << "\n";
    else
        std::cout << "No minimum element found.\n";

    // Test find_if: find first age > 18
    const auto found = algo::find_if(ages.begin(), ages.end(), [](float val)
                                     { return val > 18.0f; });
    if (found != ages.end())
        std::cout << "First age > 18: " << *found << "\n";
    else
        std::cout << "No age > 18 found.\n";

    // Test count: how many are >= 15
    std::size_t count_result = algo::count(ages.begin(), ages.end(), [](float val)
                                           { return val >= 15.0f; });
    if (count_result > 0)
        std::cout << "Count of ages >= 15: " << count_result << "\n";
    else
        std::cout << "No ages >= 15 found.\n";

    // Test sendFileDataToAPI
    cpp_core::io_operations::sendFileDataToAPI("test_file.txt");

    // Test sendFileDataToAPI with test_file2.txt
    cpp_core::io_operations::sendFileDataToAPI("test_file2.txt");

    return 0;
}
