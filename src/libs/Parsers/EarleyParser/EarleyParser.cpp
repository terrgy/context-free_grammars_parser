#include "EarleyParser.h"
#include <tuple>
#include <algorithm>

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

bool EarleyParser::Situation::operator==(const EarleyParser::Situation &other) const {
    return tie(rule_left_part, rule_right_part, word_idx, right_part_idx) ==
    tie(other.rule_left_part, other.rule_right_part, other.word_idx, other.right_part_idx);
}

void EarleyParser::fit(Grammar grammar) {
    this->grammar = std::move(grammar);
    int new_start = this->grammar.addNonTerminal();
    this->grammar.addRule(new_start, {this->grammar.start_symbol});
    this->grammar.changeStartSymbol(new_start);
}

bool EarleyParser::predict(const std::string &str) {
    std::vector<int> encoded_str = grammar.encodeStr(str);

    initParser(encoded_str.size());
    mainCycle(encoded_str);
    bool res = get_verdict();
    clear();

    return res;
}

bool EarleyParser::get_verdict() {
    return find(parsing_lists.back().begin(), parsing_lists.back().end(),
                Situation(grammar.start_symbol, grammar.rules[grammar.start_symbol].front(), 0, 1))
                != parsing_lists.back().end();
}

void EarleyParser::initParser(size_t word_size) {
    parsing_lists.resize(word_size + 1);
    tryToAdd(Situation(grammar.start_symbol,
                       grammar.rules[grammar.start_symbol].front(), 0),
             0);
}

void EarleyParser::mainCycle(const std::vector<int>& word) {
    for (size_t i = 0; i <= word.size(); ++i) {
        for (size_t situation_idx = 0; situation_idx < parsing_lists[i].size(); ++situation_idx) {
            if (parsing_lists[i][situation_idx].isFinished()) {
                complete(parsing_lists[i][situation_idx], i);
            } else {
                if (grammar.isNonTerminal(parsing_lists[i][situation_idx].getNext())) {
                    predict(parsing_lists[i][situation_idx], i);
                } else {
                    scan(parsing_lists[i][situation_idx], i, word);
                }
            }
        }
    }
}

void EarleyParser::predict(Situation situation, size_t i) {
    int next_symbol = situation.getNext();
    for (auto& right_part : grammar.rules[next_symbol]) {
        tryToAdd(Situation(next_symbol, right_part, i), i);
    }
}

void EarleyParser::scan(EarleyParser::Situation situation, size_t i, const std::vector<int>& word) {
    int next_symbol = situation.getNext();
    if ((situation.word_idx < word.size()) && (i < word.size()) && (next_symbol == word[i])) {
        Situation new_situation = situation;
        ++new_situation.right_part_idx;
        tryToAdd(new_situation, i + 1);
    }
}

void EarleyParser::complete(EarleyParser::Situation situation, size_t i) {
    for (int situation_idx = 0; situation_idx < parsing_lists[situation.word_idx].size(); ++situation_idx) {
        if (parsing_lists[situation.word_idx][situation_idx].isFinished() ||
            (parsing_lists[situation.word_idx][situation_idx].getNext() != situation.rule_left_part)) {
            continue;
        }
        Situation new_situation = parsing_lists[situation.word_idx][situation_idx];
        ++new_situation.right_part_idx;
        tryToAdd(new_situation, i);
    }
}

void EarleyParser::tryToAdd(const EarleyParser::Situation &situation, size_t parsing_list_idx) {
    if (std::find(parsing_lists[parsing_list_idx].begin(),
                  parsing_lists[parsing_list_idx].end(), situation) ==
                  parsing_lists[parsing_list_idx].end()) {
        parsing_lists[parsing_list_idx].push_back(situation);
    }
}

void EarleyParser::clear() {
    parsing_lists.clear();
}