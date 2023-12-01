#pragma once

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

#include <algorithm>
#include <cctype>
#include <locale>

class Grammar {
private:
    int last_symbol_code;

    static inline void ltrim(std::string &s);
    static inline void rtrim(std::string &s);
    static inline void trim(std::string &s);

    void readNonTerminals(size_t count, std::istream& in = std::cin);
    void readTerminals(size_t count, std::istream& in = std::cin);
    void readRules(size_t count, std::istream& in = std::cin);
    void readStartSymbol(std::istream& in = std::cin);

    void printSymbols(const std::unordered_set<int>& storage, std::ostream& out = std::cout) const;
    void printRules(std::ostream& out = std::cout) const;
    void printStartSymbol(std::ostream& out = std::cout) const;
public:
    Grammar();

    static const std::unordered_set<char> read_allowed_non_terminals;
    static const std::unordered_set<char> read_allowed_terminals;

    std::unordered_set<int> non_terminals;
    std::unordered_set<int> terminals;
    std::unordered_map<int, std::vector< std::vector<int> > > rules;
    int start_symbol;

    std::unordered_map<int, char> symbol_to_char;
    std::unordered_map<char, int> char_to_symbol;

    void read(std::istream& in = std::cin);
    void print(std::ostream& out = std::cout) const;
    size_t getRulesCount() const;
    int encodeSymbol(char symbol) const;
    std::vector<int> encodeStr(const std::string& str) const;
    std::string decodeSymbol(int symbol_code) const;
    std::string decodeStr(const std::vector<int>& word) const;
    int addNonTerminal();
    int addSymbolicNonTerminal(char symbol);
    void addRule(int left_part, const std::vector<int>& right_part);
    void changeStartSymbol(int new_start);
    bool isNonTerminal(int symbol_code) const;
    void makeAugmented();
    int addTerminal();
    int addSymbolicTerminal(char symbol);
    std::unordered_set<int> getAlphabet() const;
    size_t alphabetSize() const;
};