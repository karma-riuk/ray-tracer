#include <chrono>
#include <iostream>
#include <string>

class profiler {
    std::string label;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;

  public:
    profiler(const std::string& label)
        : label(label),
          start(std::chrono::high_resolution_clock::now()) {}

    ~profiler() {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        if (label == "")
            std::cerr << elapsed.count() << ",";
        else
            std::cerr << label << ": " << elapsed.count() << " ms ("
                      << elapsed.count() / 1000 << " s)\n";
    }
};
