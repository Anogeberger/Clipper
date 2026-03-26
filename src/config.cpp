#include "config.h"
#include <fstream>
#include <iostream>
#include <sstream>

void Config::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    std::string line;
    while (std::getline(file, line)) {
        auto pos = line.find('=');
        if (pos != std::string::npos) {
            std::string orig = line.substr(0, pos);
            std::string repl = line.substr(pos + 1);
            rules[orig] = repl;
        }
    }
    file.close();
}

void Config::save(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot write to " << filename << std::endl;
        return;
    }
    for (const auto& pair : rules) {
        file << pair.first << "=" << pair.second << std::endl;
    }
    file.close();
}

void Config::setRules(const std::unordered_map<std::string, std::string>& newRules) {
    rules = newRules;
}

std::unordered_map<std::string, std::string> Config::getRules() const {
    return rules;
}