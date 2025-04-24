# cpp_core_array

## Overview

A modern C++ library providing custom fixed-size containers, STL-compatible iterators, and efficient algorithms. Designed for seamless integration into larger projects, it also includes robust file processing utilities with chunked reading capabilities, making it ideal for performance-critical applications.

### Features

1. **Custom Containers**:
   - `Array`: A fixed-size array container similar to `std::array`, with support for:
     - Random-access storage.
     - Custom iterators (`ArrayIterator` and `ArrayConstIterator`) for mutable and read-only access.
     - STL compatibility for algorithms like `std::sort`, `std::copy`, etc.

2. **Custom Algorithms**:
   - `max_element`: Finds the maximum element in a range.
   - `min_element`: Finds the minimum element in a range.
   - `find_if`: Finds the first element in a range that satisfies a predicate.
   - `count`: Counts the number of elements in a range that satisfy a predicate.

3. **Custom I/O Operations**:
   - `sendFileDataToAPI`: Reads a file in chunks, processes it line by line, and simulates sending each line to an API. Handles cases where lines span across chunks.

---

## Build Instructions

1. **Prerequisites**:
   - C++ compiler with C++23 support (e.g., GCC 13 or later, Clang 16 or later).
   - CMake version 3.28 or later.
   - `fmt` library installed on the system.

2. **Steps to Build**:

   ```bash
   # Create a build directory
   cmake -B build

   # Build the project
   cmake --build build
   ```

3. **Executable**:
   The compiled executable will be located at `build/cpp_core_executable`.

---

## Run Instructions

1. **Run the Executable**:

   ```bash
   ./build/cpp_core_executable
   ```

2. **Expected Output**:
   The program demonstrates the following:
   - Testing the custom `Array` container and algorithms.
   - Reading and processing files using `sendFileDataToAPI`.

   Example output:

   ```bash
   Custom array of Ages: 
   12.2 15 17 19.4 23.32 5.4 

   Max: 23.32
   Min: 5.4
   First age > 18: 19.4
   Count of ages >= 15: 4

   Reading from test_file.txt:
   Read 18 bytes:
   Sending to API-> Line1
   Sending to API-> Line2
   Sending to API-> Line3

   Reading from test_file2.txt:
   Read 512 bytes:
   Sending to API-> Line 1: This is a test line to fill the file.
   Sending to API-> Line 2: Adding more content to exceed 512 bytes.
   Sending to API-> Line 3: Each line contributes to the total size.
   Sending to API-> Line 4: This file is used for testing purposes.
   Sending to API-> Line 5: The goal is to trigger multiple iterations.
   Sending to API-> Line 6: Let's add more lines to ensure the size grows.
   Sending to API-> Line 7: Testing the chunked reading functionality.
   Sending to API-> Line 8: This line is part of the test data.
   Sending to API-> Line 9: Adding more lines to reach the required size.
   Sending to API-> Line 10: This should be enough to exceed 512 bytes.
   Read 135 bytes:
   Sending to API-> Line 11: Adding extra lines just to be sure.
   Sending to API-> Line 12: Testing the API simulation with this data.
   Sending to API-> Line 13: This is the final line of the test file.
   ```

---

## Detailed Explanation

### Custom Containers

- **`Array`**:
  - A fixed-size container with compile-time size.
  - Provides:
    - `begin()` and `end()` for iterators.
    - `data()` for raw pointer access.
    - `operator[]` for element access.
    - STL compatibility with custom iterators.

### Custom Algorithms

- **`max_element`**:
  - Finds the maximum element in a range `[begin, end)`.
  - Returns an iterator to the maximum element or `end` if the range is empty.

- **`min_element`**:
  - Finds the minimum element in a range `[begin, end)`.
  - Returns an iterator to the minimum element or `end` if the range is empty.

- **`find_if`**:
  - Finds the first element in a range `[begin, end)` that satisfies a predicate.
  - Returns an iterator to the matching element or `end` if no match is found.

- **`count`**:
  - Counts the number of elements in a range `[begin, end)` that satisfy a predicate.
  - Returns the count as a `std::size_t`.

### Custom I/O Operations

- **`sendFileDataToAPI`**:
  - Reads a file in chunks of 512 bytes.
  - Processes the data line by line.
  - Handles cases where lines span across chunks using a leftover buffer.
  - Simulates sending each line to an API.

---

## Optimal Chunk Size for File Reading

The default chunk size of 512 bytes used in this project is based on historical disk sector sizes. However, modern systems and storage devices often benefit from larger chunk sizes. Below is a detailed explanation:

### Modern Systems (e.g., SSDs, Advanced HDDs)

- **Recommended Chunk Size**: 4 KiB (4096 bytes) or larger.
- **Why?**:
  - Modern drives use 4 KiB physical sectors (Advanced Format), and operating systems typically use 4 KiB or larger file system block sizes.
  - Reading smaller chunks (e.g., 512 bytes) causes the drive to internally read the full 4 KiB anyway, leading to unnecessary overhead.
  - Aligning the chunk size with the file system block size (typically 4 KiB(4096 bytes)) ensures that one read operation corresponds to one block, improving efficiency.
  - [https://github.com/ajitm722/DiskOpsAnalyzer](https://github.com/ajitm722/DiskOpsAnalyzer)

### Constrained Embedded Systems

- **Recommended Chunk Size**: 512 bytes.
- **Why?**:
  - Some embedded systems and older storage devices still use 512-byte sectors as the hardware-enforced minimum.
  - For systems with limited memory or processing power, smaller chunks may be necessary to avoid resource constraints.

### Customization for Your Project

This project can be easily customized to use different chunk sizes based on the target environment:

1. **For Modern Systems**:
   - Increase the chunk size to 4 KiB or larger depending on filesystem and usecase (e.g., 8 KiB, 16 KiB, 64 KiB).
   - Modify the buffer size in the `sendFileDataToAPI` function to reflect the new chunk size.

2. **For Embedded Systems**:
   - Retain the 512-byte chunk size for compatibility with constrained environments.

By default, the project uses 512 bytes for demonstration purposes with custom array.

---

## Acknowledgments

This project was inspired by lectures from the [C++ Global Masterclass](https://cppmasterclass.com.br/En/) by Herik Lima.
