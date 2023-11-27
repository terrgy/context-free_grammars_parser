#include "EarleyParser.h"
#include <tuple>

EarleyParser::Situation::Situation(int rule_left_part, const std::vector<int> &rule_right_part, size_t word_idx,
                                   size_t right_part_idx) :
                                   rule_left_part(rule_left_part), rule_right_part(&rule_right_part),
                                   right_part_idx(right_part_idx), word_idx(word_idx) {}

bool EarleyParser::Situation::isFinished() const {
    return right_part_idx == rule_right_part->size();
}

int EarleyParser::Situation::getNext() const {
    return (*rule_right_part)[right_part_idx];
}

void EarleyParser::Situation::makeStep() {
    ++right_part_idx;
}

bool EarleyParser::Situation::operator==(const EarleyParser::Situation &other) const {
    return tie(rule_left_part, rule_right_part, word_idx, right_part_idx) ==
    tie(other.rule_left_part, other.rule_right_part, other.word_idx, other.right_part_idx);
}

template<class T>
void EarleyParser::SituationHash::hash_combine(std::size_t &seed, const T &v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

size_t EarleyParser::SituationHash::operator()(const EarleyParser::Situation &situation) const {
    size_t result = 0;
    hash_combine(result, situation.rule_left_part);
    hash_combine(result, situation.rule_right_part);
    hash_combine(result, situation.word_idx);
    hash_combine(result, situation.right_part_idx);
    return result;
}

void EarleyParser::fit(Grammar grammar) {
    this->grammar = std::move(grammar);
    int new_start = this->grammar.addNonTerminal();
    this->grammar.addRule(new_start, {this->grammar.start_symbol});
    this->grammar.changeStartSymbol(new_start);
}

bool EarleyParser::predict(const std::string &str) {
    std::vector<int> encoded_str;
    try {
        encoded_str = grammar.encodeStr(str);
    } catch (std::runtime_error&) {
        return false;
    }

    Situation start_situation = initParser(encoded_str.size());
    mainCycle(encoded_str);
    start_situation.makeStep();
    bool res = getVerdict(start_situation);
    clear();

    return res;
}

bool EarleyParser::getVerdict(const Situation& end_situation) {
    return fast_contain_check.back().contains(end_situation);
}

EarleyParser::Situation EarleyParser::initParser(size_t word_size) {
    Situation start_situation(grammar.start_symbol,
                                          grammar.rules[grammar.start_symbol].front(), 0);
    parsing_lists.resize(word_size + 1);
    fast_contain_check.resize(word_size + 1);
    tryToAdd(start_situation, 0);
    return start_situation;
}

void EarleyParser::mainCycle(const std::vector<int>& word) {
    for (size_t i = 0; i <= word.size(); ++i) {
        for (auto& situation : parsing_lists[i]) {
            if (situation.isFinished()) {
                complete(situation, i);
            } else {
                if (grammar.isNonTerminal(situation.getNext())) {
                    predict(situation, i);
                } else {
                    scan(situation, i, word);
                }
            }
        }
    }
}

void EarleyParser::predict(const Situation& situation, size_t i) {
    int next_symbol = situation.getNext();
    for (auto& right_part : grammar.rules[next_symbol]) {
        tryToAdd(Situation(next_symbol, right_part, i), i);
    }
}

void EarleyParser::scan(const Situation& situation, size_t i, const std::vector<int>& word) {
    int next_symbol = situation.getNext();
    if ((situation.word_idx < word.size()) && (i < word.size()) && (next_symbol == word[i])) {
        Situation new_situation = situation;
        new_situation.makeStep();
        tryToAdd(new_situation, i + 1);
    }
}

void EarleyParser::complete(const Situation& situation, size_t i) {
    for (auto& parent_situation : parsing_lists[situation.word_idx]) {
        if (parent_situation.isFinished() || (parent_situation.getNext() != situation.rule_left_part)) {
            continue;
        }
        Situation new_situation = parent_situation;
        new_situation.makeStep();
        tryToAdd(new_situation, i);
    }
}

void EarleyParser::tryToAdd(const Situation &situation, size_t parsing_list_idx) {
    if (!fast_contain_check[parsing_list_idx].contains(situation)) {
        parsing_lists[parsing_list_idx].push_back(situation);
        fast_contain_check[parsing_list_idx].insert(situation);
    }
}

void EarleyParser::clear() {
    parsing_lists.clear();
    fast_contain_check.clear();
}