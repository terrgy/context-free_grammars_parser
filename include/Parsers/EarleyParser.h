#pragma once

#include "BaseParser.h"

class EarleyParser : public BaseParser {
    struct Situation {
        int rule_left_part;
        const std::vector<int>* rule_right_part;
        size_t word_idx;
        size_t right_part_idx;


        Situation(int rule_left_part, const std::vector<int>& rule_right_part,
                  size_t word_idx, size_t right_part_idx = 0);

        bool operator==(const Situation& other) const;

        bool isFinished() const;
        int getNext() const;
    };

    std::vector< std::vector<Situation> > parsing_lists;

    void initParser(size_t word_size);
    void mainCycle(const std::vector<int>& word);

    void tryToAdd(const EarleyParser::Situation &situation, size_t parsing_list_idx);

    void clear();

    void predict(Situation situation, size_t i);
    void scan(Situation situation, size_t i, const std::vector<int>& word);
    void complete(Situation situation, size_t i);

    bool get_verdict();

public:
    void fit(Grammar grammar) override;
    bool predict(const std::string& str) override;
};