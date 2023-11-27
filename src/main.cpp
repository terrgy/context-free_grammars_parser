//#define _GLIBCXX_DEBUG 1

#include <iostream>
#include "Grammar.h"
#include "EarleyParser.h"


int main() {
    Grammar grammar;
    grammar.read();

    EarleyParser parser;
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
