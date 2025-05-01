#include "../include/cpp_core.hpp" // For custom container, algorithms, and I/O operations
#include <fstream>                 // For std::ifstream
#include <sstream>                 // For std::istringstream
#include <fmt/core.h>              // For fmt::print
#include <tuple>                   // For std::tuple

namespace cpp_core::io_operations
{
    // Function: sendFileDataToAPI
    // Reads a file in chunks, processes the data line by line, and sends each line to an API.
    // Handles cases where lines span across chunks by using a leftover buffer.
    void sendFileDataToAPI(std::string_view filename)
    {
        constexpr std::size_t chunk_size{512};
        cont::Array<char, chunk_size> buffer;

        // Open the file in binary mode
        std::ifstream file(filename.data(), std::ios::binary);
        if (!file.is_open())
        {
            fmt::print(stderr, "Failed to open the file: {}\n", filename);
            return;
        }

        std::string leftover; // Buffer to hold incomplete lines from the previous chunk

        // Read the file in chunks
        while (file.read(buffer.data(), chunk_size) || file.gcount() > 0)
        {
            std::size_t bytes_read{static_cast<std::size_t>(file.gcount())}; // Number of bytes read in the current chunk
            fmt::print("Read {} bytes:\n", bytes_read);

            // Combine leftover data with the current chunk
            std::string chunk_data{leftover + std::string(buffer.data(), bytes_read)};
            leftover.clear();

            // Process the chunk line by line
            for (auto [chunk_stream, line] = std::tuple{std::istringstream(chunk_data), std::string{}};
                 std::getline(chunk_stream, line);)
            {
                // If the line is incomplete (no newline at the end), save it to leftover
                if (chunk_stream.eof() && !chunk_data.empty() && chunk_data.back() != '\n')
                {
                    leftover = line;
                }
                else
                {
                    // Send the complete line to the API
                    fmt::print("Sending to API-> {}\n", line);
                }
            }
        }

        // Send any remaining data in the leftover buffer
        if (!leftover.empty())
        {
            fmt::print("Sending to API-> {}\n", leftover);
        }

        file.close(); // Close the file
    }
}
