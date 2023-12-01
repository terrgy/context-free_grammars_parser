#include "Grammar.h"

const std::unordered_set<char> Grammar::read_allowed_non_terminals = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
                                                                      'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
                                                                      'W', 'X', 'Y', 'Z'};
const std::unordered_set<char> Grammar::read_allowed_terminals = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
                                                                  'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                                                                  'y', 'z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
                                                                  '(', ')', '[', ']', '{', '}', '+', '-', '=', '*', '\\'};

Grammar::Grammar() : last_symbol_code(0) {}


void Grammar::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void Grammar::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void Grammar::trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

void Grammar::read(std::istream& in) {
    size_t non_terminals_count, terminals_count, rules_count;
    in >> non_terminals_count >> terminals_count >> rules_count;

    readNonTerminals(non_terminals_count, in);
    readTerminals(terminals_count, in);
    readRules(rules_count, in);
    readStartSymbol(in);
}

void Grammar::readNonTerminals(size_t count, std::istream& in) {
    for (size_t i = 0; i < count; ++i) {
        char symbol;
        in >> symbol;
        addSymbolicNonTerminal(symbol);
    }
}

void Grammar::readTerminals(size_t count, std::istream& in) {
    for (size_t i = 0; i < count; ++i) {
        char symbol;
        in >> symbol;
        addSymbolicTerminal(symbol);
    }
}

int Grammar::encodeSymbol(char symbol) const {
    if (char_to_symbol.contains(symbol)) {
        return char_to_symbol.at(symbol);
    }
    throw std::runtime_error("No such symbol in grammar - '" + std::string(1, symbol) + "'");
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
    if (symbol_to_char.contains(symbol_code)) {
        return {symbol_to_char.at(symbol_code)};
    }
    if (non_terminals.contains(symbol_code)) {
        return "#(" + std::to_string(symbol_code) + ")";
    }
    if (terminals.contains(symbol_code)) {
        return "##(" + std::to_string(symbol_code) + ")";
    }
    throw std::runtime_error("No such symbol in grammar - " + std::to_string(symbol_code));
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
    in.get();
    for (size_t i = 0; i < count; ++i) {
        std::string str;
        getline(in, str);

        size_t del_pos = str.find("->");
        if (del_pos == std::string::npos) {
            throw std::runtime_error("No delimeter ('->') in the rule");
        }

        std::string left_part, right_part;
        left_part = str.substr(0, del_pos);
        right_part = str.substr(del_pos + 2);
        trim(left_part);
        trim(right_part);

        if (left_part.size() != 1) {
            throw std::runtime_error("Left part must be one symbol only");
        }
        char left_symbol = left_part[0];

        int left_symbol_code = encodeSymbol(left_symbol);
        if (!non_terminals.contains(left_symbol_code)) {
            throw std::runtime_error("Left part must be one non-terminal symbol");
        }
        std::vector<int> encoded_right_part = encodeStr(right_part);
        rules[left_symbol_code].push_back(encoded_right_part);
    }
}

void Grammar::readStartSymbol(std::istream &in) {
    char symbol;
    in >> symbol;
    if (!char_to_symbol.contains(symbol) || !non_terminals.contains(char_to_symbol[symbol])) {
        throw std::runtime_error("Start symbol must be non-terminal");
    }
    start_symbol = char_to_symbol[symbol];
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
}

int Grammar::addNonTerminal() {
    int new_code = last_symbol_code++;
    non_terminals.insert(new_code);
    return new_code;
}

int Grammar::addSymbolicNonTerminal(char symbol) {
    if (!read_allowed_non_terminals.contains(symbol)) {
        throw std::runtime_error("Not allowed non-terminal - '" + std::string(1, symbol) + "'");
    }
    if (char_to_symbol.contains(symbol)) {
        throw std::runtime_error("The non-terminal occurs several times - '" + std::string(1, symbol) + "'");
    }
    int new_code = addNonTerminal();
    symbol_to_char[new_code] = symbol;
    char_to_symbol[symbol] = new_code;
    return new_code;
}

void Grammar::addRule(int left_part, const std::vector<int>& right_part) {
    rules[left_part].push_back(right_part);
}

void Grammar::changeStartSymbol(int new_start) {
    start_symbol = new_start;
}

bool Grammar::isNonTerminal(int symbol_code) const {
    return non_terminals.contains(symbol_code);
}

void Grammar::makeAugmented() {
    int new_start = addNonTerminal();
    addRule(new_start, {start_symbol});
    changeStartSymbol(new_start);
}

int Grammar::addTerminal() {
    int new_code = last_symbol_code++;
    terminals.insert(new_code);
    return new_code;
}

int Grammar::addSymbolicTerminal(char symbol) {
    if (!read_allowed_terminals.contains(symbol)) {
        throw std::runtime_error("Not allowed terminal - '" + std::string(1, symbol) + "'");
    }
    if (char_to_symbol.contains(symbol)) {
        throw std::runtime_error("The terminal occurs several times - '" + std::string(1, symbol) + "'");
    }
    int new_code = addTerminal();
    symbol_to_char[new_code] = symbol;
    char_to_symbol[symbol] = new_code;
    return new_code;
}

std::unordered_set<int> Grammar::getAlphabet() const {
    std::unordered_set<int> result = terminals;
    result.insert(non_terminals.begin(), non_terminals.end());
    return result;
}

size_t Grammar::alphabetSize() const {
    return last_symbol_code;
}