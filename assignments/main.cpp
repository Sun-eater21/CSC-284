#include "LogAnalyzer.h"
#include <iostream>
#include <filesystem>
#include <thread>
#include <vector>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <log_directory>\n";
        return 1;
    }

    std::string folderPath = argv[1];
    LogAnalyzer analyzer;
    std::vector<std::string> keywords = {"[ERROR]", "[WARNING]", "[INFO]", /* TODO: Add the rest of the error levels */};
    std::vector<std::thread> threads;

    std::cout << "Analyzing folder: " << folderPath << "\n\n";

    // TODO: Use std::filesystem to iterate through all files in the folder.
    // For each file with ".log" extension:
    //   - Create a thread calling analyzer.analyzeFile(filename, keywords)
    //   - Store the thread in 'threads' vector.

    // TODO: Join all threads after launching.

    analyzer.printSummary();

    return 0;
}
