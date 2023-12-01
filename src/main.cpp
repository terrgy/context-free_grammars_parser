#include <iostream>
#include "Grammar.h"
#include "EarleyParser.h"
#include "LR1Parser.h"

int main() {
    Grammar grammar;
    grammar.read();

    LR1Parser parser;
    parser.fit(grammar);

    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::string word;
        std::cin >> word;
        std::cout << (parser.predict(word) ? "Yes" : "No") << '\n';
    }
    return 0;
}
