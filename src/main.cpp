#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
#include "clipper.h"
#include "config.h"

void printHelp() {
    std::cout << "Crypto Address Clipper v" << VERSION_MAJOR << "." << VERSION_MINOR << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  add <original> <replacement>  - Add address replacement rule" << std::endl;
    std::cout << "  list                          - Show all rules" << std::endl;
    std::cout << "  save                          - Save rules to file" << std::endl;
    std::cout << "  start                         - Start clipboard monitoring" << std::endl;
    std::cout << "  exit                          - Exit program" << std::endl;
}

int main() {
    Config config;
    Clipper clipper;
    config.load("addresses.ini");
    clipper.setRules(config.getRules());

    bool monitoring = false;
    std::thread monitorThread;
    auto stopMonitoring = [&]() {
        if (monitoring) {
            monitoring = false;
            if (monitorThread.joinable())
                monitorThread.join();
        }
    };

    printHelp();

    std::string command;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        if (command == "exit") break;
        else if (command == "start") {
            if (monitoring) {
                std::cout << "Already monitoring." << std::endl;
                continue;
            }
            monitoring = true;
            monitorThread = std::thread([&]() {
                while (monitoring) {
                    if (OpenClipboard(nullptr)) {
                        HANDLE hData = GetClipboardData(CF_TEXT);
                        if (hData) {
                            char* pszText = static_cast<char*>(GlobalLock(hData));
                            if (pszText) {
                                std::string original(pszText);
                                std::string replaced = clipper.processAddress(original);
                                if (replaced != original) {
                                    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, replaced.size() + 1);
                                    if (hMem) {
                                        memcpy(GlobalLock(hMem), replaced.c_str(), replaced.size() + 1);
                                        GlobalUnlock(hMem);
                                        EmptyClipboard();
                                        SetClipboardData(CF_TEXT, hMem);
                                        std::cout << "[!] Replaced: " << original << " -> " << replaced << std::endl;
                                    }
                                }
                                GlobalUnlock(hData);
                            }
                        }
                        CloseClipboard();
                    }
                    Sleep(500);
                }
            });
            std::cout << "Monitoring started." << std::endl;
        }
        else if (command == "list") {
            auto rules = clipper.getRules();
            if (rules.empty()) {
                std::cout << "No rules." << std::endl;
            } else {
                for (const auto& rule : rules) {
                    std::cout << rule.first << " -> " << rule.second << std::endl;
                }
            }
        }
        else if (command.rfind("add ", 0) == 0) {
            std::string params = command.substr(4);
            size_t space = params.find(' ');
            if (space == std::string::npos) {
                std::cout << "Usage: add <original> <replacement>" << std::endl;
                continue;
            }
            std::string orig = params.substr(0, space);
            std::string repl = params.substr(space + 1);
            clipper.addRule(orig, repl);
            std::cout << "Rule added." << std::endl;
        }
        else if (command == "save") {
            config.setRules(clipper.getRules());
            config.save("addresses.ini");
            std::cout << "Rules saved to addresses.ini" << std::endl;
        }
        else {
            std::cout << "Unknown command. Type 'help' for commands." << std::endl;
        }
    }

    stopMonitoring();
    return 0;
}