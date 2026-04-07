#include <map>
#include <string>
#include <vector>
#include <mutex>

class LogAnalyzer {
private:
    std::map<std::string, int> keywordCounts;
    std::mutex countMutex;

public:
    void analyzeFile(const std::string& filename, const std::vector<std::string>& keywords);
    void printSummary() const;
};
