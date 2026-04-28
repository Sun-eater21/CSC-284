#include "LogAnalyzer.h"
#include <fstream>
#include <iostream>
#include <algorithm>

void LogAnalyzer::analyzeFile(const std::string& filename, const std::vector<std::string>& keywords) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {

        // Count each keyword safely using mutex
        if (line.find("[ERROR]") != std::string::npos) {
            std::lock_guard<std::mutex> lock(countMutex);
            keywordCounts["[ERROR]"]++;
        }
        if (line.find("[WARNING]") != std::string::npos) {
            std::lock_guard<std::mutex> lock(countMutex);
            keywordCounts["[WARNING]"]++;
        }
        if (line.find("[WARN]") != std::string::npos) {
            std::lock_guard<std::mutex> lock(countMutex);
            keywordCounts["[WARN]"]++;
        }
        if (line.find("[INFO]") != std::string::npos) {
            std::lock_guard<std::mutex> lock(countMutex);
            keywordCounts["[INFO]"]++;
        }
        if (line.find("[DEBUG]") != std::string::npos) {
            std::lock_guard<std::mutex> lock(countMutex);
            keywordCounts["[DEBUG]"]++;
        }
        if (line.find("[TRACE]") != std::string::npos) {
            std::lock_guard<std::mutex> lock(countMutex);
            keywordCounts["[TRACE]"]++;
        }
        if (line.find("[FATAL]") != std::string::npos) {
            std::lock_guard<std::mutex> lock(countMutex);
            keywordCounts["[FATAL]"]++;
        }
    }
}

void LogAnalyzer::printSummary() const {
    std::cout << "\n--- Keyword Summary ---\n";

    std::for_each(keywordCounts.begin(), keywordCounts.end(),
        [](const auto& pair) {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }
    );

    std::cout << "-----------------------\n";
}