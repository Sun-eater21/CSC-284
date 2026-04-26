#include "LogAnalyzer.h"
#include <iostream>
#include <vector>
#include <thread>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: LogAnalyzer <folder_path>\n";
        return 1;
    }

    std::string folderPath = argv[1];
    std::cout << "Analyzing folder: " << folderPath << std::endl;

    LogAnalyzer analyzer;

    std::vector<std::string> keywords = {
        "[WARN]", "[WARNING]", "[ERROR]", "[FATAL]", "[INFO]", "[DEBUG]", "[TRACE]"
    };

    std::vector<std::thread> threads;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.path().extension() == ".log") {
            std::string filename = entry.path().string();

            std::cout << "Processing: " << filename << std::endl;

            threads.emplace_back([&analyzer, filename, &keywords]() {
                analyzer.analyzeFile(filename, keywords);
            });
        }
    }

    for (auto& t : threads) {
        t.join();
    }

    analyzer.printSummary();

    return 0;
}