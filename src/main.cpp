#include <fmt/core.h>              // For fmt::print
#include "../include/cpp_core.hpp" // For custom container, algorithms, and I/O operations

int main()
{
    {
        // Test the custom Array container and algorithms
        constexpr cont::Array<float, 6> ages{12.2f, 15.0f, 17.0f, 19.4f, 23.32f, 5.4f};

        // Print the values of the ages array
        fmt::print("Custom array of Ages: \n");
        for (const auto &age : ages)
        {
            fmt::print("{} ", age);
        }
        fmt::print("\n\n");

        // Test max_element
        const auto max_it{algo::max_element(ages.begin(), ages.end())};
        if (max_it != ages.end())
            fmt::print("Max: {}\n", *max_it);
        else
            fmt::print("No maximum element found.\n");

        // Test min_element
        const auto min_it{algo::min_element(ages.begin(), ages.end())};
        if (min_it != ages.end())
            fmt::print("Min: {}\n", *min_it);
        else
            fmt::print("No minimum element found.\n");

        // Test find_if: find first age > 18
        const auto found{algo::find_if(ages.begin(), ages.end(), [](float val)
                                       { return val > 18.0f; })};
        if (found != ages.end())
            fmt::print("First age > 18: {}\n", *found);
        else
            fmt::print("No age > 18 found.\n");

        // Test count: how many are >= 15
        const auto count_result{algo::count(ages.begin(), ages.end(), [](float val)
                                            { return val >= 15.0f; })};
        if (count_result > 0)
            fmt::print("Count of ages >= 15: {}\n", count_result);
        else
            fmt::print("No ages >= 15 found.\n");
    }
    // Test the custom I/O operations

    // Test sendFileDataToAPI
    fmt::print("\nReading from test_file.txt:\n");
    io::sendFileDataToAPI("/home/user/qt_c++/array/test_file.txt");

    // Test sendFileDataToAPI with test_file2.txt
    fmt::print("\nReading from test_file2.txt:\n");
    io::sendFileDataToAPI("/home/user/qt_c++/array/test_file2.txt");

    return 0;
}
