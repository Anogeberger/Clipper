#ifndef CLIPPER_H
#define CLIPPER_H

#include <string>
#include <unordered_map>

class Clipper {
public:
    Clipper();
    void addRule(const std::string& original, const std::string& replacement);
    void setRules(const std::unordered_map<std::string, std::string>& rules);
    std::string processAddress(const std::string& address);
    std::unordered_map<std::string, std::string> getRules() const;

private:
    std::unordered_map<std::string, std::string> rules;
    bool isCryptoAddress(const std::string& addr);
    std::string normalizeAddress(const std::string& addr);
};

#endif