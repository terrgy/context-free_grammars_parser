#include "Grammar.h"

const std::unordered_set<char> Grammar::read_allowed_non_terminals = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
                                                                      'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
                                                                      'W', 'X', 'Y', 'Z'};
const std::unordered_set<char> Grammar::read_allowed_terminals = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
                                                                  'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                                                                  'y', 'z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
                                                                  '(', ')', '[', ']', '{', '}', '+', '-', '=', '*', '\\'};

Grammar::Grammar() : last_symbol_code(0) {}

void Grammar::read(std::istream& in) {
    size_t non_terminals_count, terminals_count, rules_count;
    in >> non_terminals_count >> terminals_count >> rules_count;

    readSymbols(non_terminals_count, non_terminals, non_terminal_to_char,
                char_to_non_terminal, read_allowed_non_terminals, in);
    readSymbols(terminals_count, terminals, terminal_to_char,
                char_to_terminal, read_allowed_terminals, in);
    readRules(rules_count, in);
    readStartSymbol(in);
}

void Grammar::readSymbols(size_t count, std::unordered_set<int>& storage,
                          std::unordered_map<int, char>& symbol_to_char, std::unordered_map<char, int>& char_to_symbol,
                          const std::unordered_set<char>& allowed, std::istream& in) {
    for (size_t i = 0; i < count; ++i) {
        char symbol;
        in >> symbol;
        if (!allowed.contains(symbol)) {
            throw std::runtime_error("Not allowed symbol - '" + std::to_string(symbol) + "'");
        }
        if (char_to_symbol.contains(symbol)) {
            throw std::runtime_error("The symbol occurs several times - '" + std::to_string(symbol) + "'");
        }
        int symbol_code = last_symbol_code++;
        storage.insert(symbol_code);
        symbol_to_char[symbol_code] = symbol;
        char_to_symbol[symbol] = symbol_code;
    }
}

int Grammar::encodeSymbol(char symbol) const {
    if (char_to_non_terminal.contains(symbol)) {
        return char_to_non_terminal.at(symbol);
    }
    if (char_to_terminal.contains(symbol)) {
        return char_to_terminal.at(symbol);
    }
    throw std::runtime_error("No such symbol to encode - '" + std::to_string(symbol) + "'");
}

std::vector<int> Grammar::encodeStr(const std::string& str) const {
    std::vector<int> result;
    result.resize(str.size());
    for (size_t i = 0; i < str.size(); ++i) {
        result[i] = encodeSymbol(str[i]);
    }
    return result;
}

std::string Grammar::decodeSymbol(int symbol_code) const {
    if (non_terminal_to_char.contains(symbol_code)) {
        return {non_terminal_to_char.at(symbol_code)};
    }
    if (terminal_to_char.contains(symbol_code)) {
        return {terminal_to_char.at(symbol_code)};
    }
    if (non_terminals.contains(symbol_code)) {
        return "#(" + std::to_string(symbol_code) + ")";
    }
    if (terminals.contains(symbol_code)) {
        return "##(" + std::to_string(symbol_code) + ")";
    }
    throw std::runtime_error("No such symbol to decode - " + std::to_string(symbol_code));
}

std::string Grammar::decodeStr(const std::vector<int>& word) const {
    std::string result;
    result.reserve(word.size());
    for (int i : word) {
        result += decodeSymbol(i);
    }
    return result;
}

void Grammar::readRules(size_t count, std::istream& in) {
    for (size_t i = 0; i < count; ++i) {
        std::string str;
        in >> str;

        size_t del_pos = str.find("->");
        if (del_pos == std::string::npos) {
            throw std::runtime_error("No delimeter ('->') in the rule");
        }

        std::string left_part, right_part;
        left_part = str.substr(0, del_pos);
        right_part = str.substr(del_pos + 2);

        if (left_part.size() != 1) {
            throw std::runtime_error("Left part must be one symbol only");
        }
        char left_symbol = left_part[0];

        if (!char_to_non_terminal.contains(left_symbol)) {
            throw std::runtime_error("Left part must be one non-terminal symbol");
        }

        for (char right_symbol : right_part) {
            if (!char_to_non_terminal.contains(right_symbol) && !char_to_terminal.contains(right_symbol)) {
                throw std::runtime_error("Right part must consist only of non-terminals and terminals");
            }
        }

        int left_symbol_code = encodeSymbol(left_symbol);
        std::vector<int> encoded_right_part = encodeStr(right_part);

        rules[left_symbol_code].push_back(encoded_right_part);
    }
}

void Grammar::readStartSymbol(std::istream &in) {
    char symbol;
    in >> symbol;
    if (!char_to_non_terminal.contains(symbol)) {
        throw std::runtime_error("Start symbol must be non-terminal");
    }
    start_symbol = char_to_non_terminal[symbol];
}

void Grammar::print(std::ostream& out) const{
    out << non_terminals.size() << ' ' << terminals.size() << ' ' << getRulesCount() << '\n';
    printSymbols(non_terminals, out);
    printSymbols(terminals, out);
    printRules(out);
    printStartSymbol(out);
}

void Grammar::printSymbols(const std::unordered_set<int>& storage, std::ostream& out) const {
    for (int symbol_code : storage) {
        out << decodeSymbol(symbol_code);
    }
    out << '\n';
}

void Grammar::printRules(std::ostream& out) const {
    for (auto& rule : rules) {
        for (auto& right_part : rule.second) {
            out << decodeSymbol(rule.first) << "->" << decodeStr(right_part) << '\n';
        }
    }
}

void Grammar::printStartSymbol(std::ostream& out) const {
    out << decodeSymbol(start_symbol) << '\n';
}

size_t Grammar::getRulesCount() const {
    size_t result = 0;
    for (auto& rule : rules) {
        result += rule.second.size();
    }
    return result;
};