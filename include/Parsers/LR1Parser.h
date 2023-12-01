#pragma once

#include "BaseParser.h"
#include <unordered_set>

class LR1Parser : public BaseParser {
private:
    template<class T>
    static inline void hash_combine(std::size_t& seed, const T& v);

    struct Rule {
        int left_part;
        const std::vector<int>* right_part;

        Rule(int left_part, const std::vector<int>& right_part);
        bool operator==(const Rule& other) const;
    };

    struct RuleHash {
        size_t operator()(const Rule& rule) const;
    };

    struct Situation {
        Rule rule;
        size_t right_part_idx;
        int next_terminal;

        Situation(int rule_left_part, const std::vector<int>& rule_right_part,
                  int next_terminal, size_t right_part_idx = 0);

        bool operator==(const Situation& other) const;
        bool isFinished() const;
        int getNext() const;
        void makeStep();
    };

    struct SituationHash {
        size_t operator()(const Situation& situation) const;
    };

    typedef std::unordered_set<Situation, SituationHash> situation_set;

    struct SituationSetHash {
        size_t operator()(const situation_set& st) const;
    };

    struct TableEntry {
        enum class Type {Shift, Reduce, Accept, Reject};

        Type type;
        std::optional<size_t> idx;

        TableEntry();
        explicit TableEntry(Type type);
        TableEntry(Type type, std::optional<size_t> idx);

        bool operator==(const TableEntry& other) const;
    };

    static std::vector<int> word_suffix(const std::vector<int>& word, size_t pos = 0);

    std::vector< std::unordered_map<int, TableEntry> > table;
    std::vector<Rule> idx_to_rule;
    int end_terminal;

    std::unordered_map<int, std::unordered_set<int> > first;
    std::vector<situation_set> situation_closures;
    std::vector< std::unordered_map<int, size_t> > go;
    std::unordered_map<situation_set, size_t, SituationSetHash> closure_to_idx;
    std::unordered_map<Rule, size_t, RuleHash> rule_to_idx;

    std::unordered_set<int> getFirst(const std::vector<int>& word);
    situation_set closure(const situation_set& situations);
    situation_set getGoRaw(const situation_set& situations, int symbol);
    size_t getGo(size_t closure_idx, int symbol);
    size_t addClosure(const situation_set& situations);
    void setTableCell(size_t closure_idx, int symbol, const TableEntry& entry);

    void fitGrammar(Grammar& grammar);
    void fitFirstNonTerminals();
    void fitSituationClosures();
    void fitRulesNumbering();
    void fitTable();
    void clearFitData();

    bool predictCycle(const std::vector<int>& word);

public:
    void fit(Grammar grammar) override;
    bool predict(const std::string& str) override;
};