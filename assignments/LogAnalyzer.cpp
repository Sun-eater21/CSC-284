#include "LogAnalyzer.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <algorithm>

void LogAnalyzer::analyzeFile(const std::string& filename, const std::vector<std::string>& keywords) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    std::cout << "Processing: " << filename << std::endl;

    std::string line;

    // TODO: Read each line.
    // For each keyword in 'keywords', check if it exists in the line.
    // If found, increment keywordCounts[keyword] (inside a mutex lock).
}

void LogAnalyzer::printSummary() const {
    std::cout << "\n--- Keyword Summary ---\n";

    // TODO: Use std::for_each and a lambda to print keyword counts.

    std::cout << "-----------------------\n";
}
