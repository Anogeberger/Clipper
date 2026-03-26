#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>

#define VERSION_MAJOR 2
#define VERSION_MINOR 0

class Config {
public:
    void load(const std::string& filename);
    void save(const std::string& filename);
    void setRules(const std::unordered_map<std::string, std::string>& rules);
    std::unordered_map<std::string, std::string> getRules() const;

private:
    std::unordered_map<std::string, std::string> rules;
};

#endif