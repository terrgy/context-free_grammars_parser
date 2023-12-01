#include "LR1Parser.h"
#include <queue>
#include <cstdint>

LR1Parser::Situation::Situation(int rule_left_part, const std::vector<int>& rule_right_part, int next_terminal,
                                size_t right_part_idx) : rule(rule_left_part, rule_right_part),
                                                        right_part_idx(right_part_idx), next_terminal(next_terminal) {}

bool LR1Parser::Situation::operator==(const LR1Parser::Situation &other) const {
    return tie(rule.left_part, rule.right_part, next_terminal, right_part_idx) ==
           tie(other.rule.left_part, other.rule.right_part, other.next_terminal, other.right_part_idx);
}

bool LR1Parser::Situation::isFinished() const {
    return right_part_idx == rule.right_part->size();
}

int LR1Parser::Situation::getNext() const {
    return (*rule.right_part)[right_part_idx];
}

void LR1Parser::Situation::makeStep() {
    ++right_part_idx;
}

template<class T>
void LR1Parser::hash_combine(std::size_t& seed, const T &v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

size_t LR1Parser::SituationHash::operator()(const LR1Parser::Situation& situation) const {
    size_t result = 0;
    RuleHash hasher;
    hash_combine(result, hasher(situation.rule));
    hash_combine(result, situation.right_part_idx);
    hash_combine(result, situation.next_terminal);
    return result;
}

size_t LR1Parser::SituationSetHash::operator()(const LR1Parser::situation_set& st) const {
    size_t result = 0;
    LR1Parser::SituationHash hasher;
    for (auto& situation : st) {
        LR1Parser::hash_combine(result, hasher(situation));
    }
    return result;
}

LR1Parser::TableEntry::TableEntry() : TableEntry(Type::Reject) {}
LR1Parser::TableEntry::TableEntry(LR1Parser::TableEntry::Type type) : type(type) {}
LR1Parser::TableEntry::TableEntry(LR1Parser::TableEntry::Type type, std::optional<size_t> idx) : type(type), idx(idx) {}

bool LR1Parser::TableEntry::operator==(const LR1Parser::TableEntry &other) const {
    return (type == other.type) && (idx == other.idx);
}

LR1Parser::Rule::Rule(int left_part, const std::vector<int>& right_part) : left_part(left_part), right_part(&right_part) {}

size_t LR1Parser::RuleHash::operator()(const LR1Parser::Rule &rule) const {
    size_t result = 0;
    LR1Parser::hash_combine(result, rule.left_part);
    LR1Parser::hash_combine(result, rule.right_part);
    return result;
}

bool LR1Parser::Rule::operator==(const LR1Parser::Rule &other) const {
    return (left_part == other.left_part) && (right_part == other.right_part);
}

std::vector<int> LR1Parser::word_suffix(const std::vector<int> &word, size_t pos) {
    std::vector<int> result(word.size() - pos);
    std::copy(word.begin() + pos, word.end(), result.begin());
    return result;
}

std::unordered_set<int> LR1Parser::getFirst(const std::vector<int> &word) {
    std::unordered_set<int> result;
    bool last_contained_end_terminal = true;
    for (int symbol : word) {
        if (!first[symbol].contains(end_terminal)) {
            result.insert(first[symbol].begin(), first[symbol].end());
            last_contained_end_terminal = false;
            break;
        } else {
            for (int terminal : first[symbol]) {
                if (terminal == end_terminal) {
                    continue;
                }
                result.insert(terminal);
            }
            last_contained_end_terminal = true;
        }
    }
    if (last_contained_end_terminal) {
        result.insert(end_terminal);
    }
    return result;
}

LR1Parser::situation_set LR1Parser::closure(const situation_set& situations) {
    situation_set result = situations;
    std::queue<Situation> q;
    for (auto& situation : result) {
        q.push(situation);
    }

    while (!q.empty()) {
        Situation situation = q.front();
        q.pop();
        if (situation.isFinished() || !grammar.isNonTerminal(situation.getNext())) {
            continue;
        }
        int next_non_terminal = situation.getNext();
        auto first_word = word_suffix(*situation.rule.right_part, situation.right_part_idx + 1);
        first_word.push_back(situation.next_terminal);
        auto first_terminals = getFirst(first_word);
        for (auto& right_part : grammar.rules[next_non_terminal]) {
            for (int terminal : first_terminals) {
                Situation new_situation(next_non_terminal, right_part, terminal);
                if (!result.contains(new_situation)) {
                    result.insert(new_situation);
                    q.push(new_situation);
                }
            }
        }
    }
    return result;
}

LR1Parser::situation_set LR1Parser::getGoRaw(const situation_set& situations, int symbol) {
    situation_set result;
    for (auto& situation : situations) {
        if (situation.isFinished() || (situation.getNext() != symbol)) {
            continue;
        }
        Situation new_situation = situation;
        new_situation.makeStep();
        result.insert(new_situation);
    }
    return closure(result);
}

size_t LR1Parser::getGo(size_t closure_idx, int symbol) {
    if (!go[closure_idx].contains(symbol)) {
        auto new_closure = getGoRaw(situation_closures[closure_idx], symbol);
        if (closure_to_idx.contains(new_closure)) {
            go[closure_idx][symbol] = closure_to_idx[new_closure];
        } else if (new_closure.empty()) {
            go[closure_idx][symbol] = SIZE_MAX;
        } else {
            go[closure_idx][symbol] = addClosure(new_closure);
        }
    }
    return go[closure_idx][symbol];
}

size_t LR1Parser::addClosure(const LR1Parser::situation_set& situations) {
    situation_closures.push_back(situations);
    go.emplace_back();
    closure_to_idx[situations] = situation_closures.size() - 1;
    return situation_closures.size() - 1;
}

void LR1Parser::fitGrammar(Grammar &grammar) {
    this->grammar = grammar;
    this->grammar.makeAugmented();
    end_terminal = this->grammar.addTerminal();
}

void LR1Parser::fitFirstNonTerminals() {
    for (int terminal : grammar.terminals) {
        first[terminal];
        first[terminal].insert(terminal);
    }

    while (true) {
        bool changes = false;
        for (auto& [left_part, right_parts] : grammar.rules) {
            for (auto& right_part : right_parts) {
                auto add_terminals = getFirst(right_part);
                size_t prev_size = first[left_part].size();
                first[left_part].insert(add_terminals.begin(), add_terminals.end());
                changes |= (prev_size != first[left_part].size());
            }
        }
        if (!changes) {
            break;
        }
    }
}

void LR1Parser::fitSituationClosures() {
    Situation start_situation(grammar.start_symbol,
                              grammar.rules[grammar.start_symbol].front(), end_terminal);
    addClosure(closure({start_situation}));
    auto alphabet = grammar.getAlphabet();
    for (size_t closure_idx = 0; closure_idx < situation_closures.size(); ++closure_idx) {
        for (int symbol : alphabet) {
            getGo(closure_idx, symbol);
        }
    }
}

void LR1Parser::setTableCell(size_t closure_idx, int symbol, const LR1Parser::TableEntry& entry) {
    if (table[closure_idx].contains(symbol) && !(table[closure_idx][symbol] == entry)) {
        throw std::runtime_error("Grammar is not LR(1)");
    }
    table[closure_idx][symbol] = entry;
}

void LR1Parser::fitRulesNumbering() {
    size_t rules_count = grammar.getRulesCount();
    idx_to_rule.reserve(rules_count);
    rule_to_idx.reserve(rules_count);
    for (auto& [left_part, right_parts] : grammar.rules) {
        for (auto& right_part : right_parts) {
            idx_to_rule.emplace_back(left_part, right_part);
            rule_to_idx[idx_to_rule.back()] = idx_to_rule.size() - 1;
        }
    }
}

void LR1Parser::fitTable() {
    table.resize(situation_closures.size());
    auto alphabet = grammar.getAlphabet();
    for (size_t i = 0; i < situation_closures.size(); ++i) {
        for (auto symbol : alphabet) {
            size_t current_go = getGo(i, symbol);
            if (current_go == SIZE_MAX) {
                continue;
            }
            setTableCell(i, symbol, TableEntry(TableEntry::Type::Shift, current_go));
        }
        for (auto& situation : situation_closures[i]) {
            if (!situation.isFinished()) {
                continue;
            }
            if (situation.rule.left_part == grammar.start_symbol) {
                setTableCell(i, end_terminal, TableEntry(TableEntry::Type::Accept));
            } else {
                setTableCell(i, situation.next_terminal,
                             TableEntry(TableEntry::Type::Reduce, rule_to_idx[situation.rule]));
            }
        }
    }

}

void LR1Parser::clearFitData() {
    first.clear();
    situation_closures.clear();
    go.clear();
    closure_to_idx.clear();
    rule_to_idx.clear();
}

void LR1Parser::fit(Grammar grammar) {
    fitGrammar(grammar);
    fitFirstNonTerminals();
    fitSituationClosures();
    fitRulesNumbering();
    fitTable();
    clearFitData();
}

bool LR1Parser::predictCycle(const std::vector<int>& word) {
    std::vector<int> stack;
    stack.push_back(0);

    size_t word_idx = 0;
    while (true) {
        int symbol = word[word_idx];
        size_t closure_idx = stack.back();
        TableEntry entry = table[closure_idx][symbol];
        switch (entry.type) {
            case TableEntry::Type::Shift:
                stack.push_back(symbol);
                ++word_idx;
                stack.push_back((int)entry.idx.value());
                break;
            case TableEntry::Type::Reduce:
                {
                    auto& rule = idx_to_rule[entry.idx.value()];
                    for (int i = (int)rule.right_part->size() - 1; i >= 0; --i) {
                        stack.pop_back();
                        if (stack.empty() || (stack.back() != rule.right_part->at(i))) {
                            return false;
                        }
                        stack.pop_back();
                    }
                    entry = table[stack.back()][rule.left_part];
                    if (entry.type != TableEntry::Type::Shift) {
                        return false;
                    }
                    stack.push_back(rule.left_part);
                    stack.push_back((int)entry.idx.value());
                }
                break;
            case TableEntry::Type::Accept:
                return true;
            case TableEntry::Type::Reject:
                return false;
        }
    }
}

bool LR1Parser::predict(const std::string &str) {
    std::vector<int> encoded_str;
    try {
        encoded_str = grammar.encodeStr(str);
    } catch (std::runtime_error&) {
        return false;
    }

    encoded_str.push_back(end_terminal);
    return predictCycle(encoded_str);
}