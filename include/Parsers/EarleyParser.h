#pragma once

#include "BaseParser.h"
#include <functional>
#include <tuple>
#include <list>

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
        void makeStep();
    };

    struct SituationHash {
        template<class T>
        static inline void hash_combine(std::size_t& seed, const T& v);
        size_t operator()(const Situation& situation) const;
    };

    std::vector< std::list<Situation> > parsing_lists;
    std::vector< std::unordered_set<Situation, SituationHash> > fast_contain_check;

    Situation initParser(size_t word_size);
    void mainCycle(const std::vector<int>& word);

    void tryToAdd(const Situation &situation, size_t parsing_list_idx);

    void clear();

    void predict(const Situation& situation, size_t i);
    void scan(const Situation& situation, size_t i, const std::vector<int>& word);
    void complete(const Situation& situation, size_t i);

    bool getVerdict(const Situation& end_situation);

public:
    void fit(Grammar grammar) override;
    bool predict(const std::string& str) override;
};