#include "GrammarTestCase.h"
#include "Grammar.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <string>


TEST_F(GrammarTestCase, SimpleInputOutputTest) {
    std::istringstream input_stream("1 2 2\n"
                                    "S\n"
                                    "ab\n"
                                    "S->aSbS\n"
                                    "S->\n"
                                    "S");
    std::stringstream output_stream;

    std::istream in(input_stream.rdbuf());
    std::ostream out(output_stream.rdbuf());

    Grammar grammar;
    grammar.read(in);
    grammar.print(out);

    std::string str;
    getline(output_stream, str);
    ASSERT_EQ(str, "1 2 2");
    getline(output_stream, str);
    ASSERT_EQ(str, "S");
    getline(output_stream, str);
    ASSERT_TRUE((str == "ab") || (str == "ba"));
    getline(output_stream, str);
    std::string str2;
    getline(output_stream, str2);
    str += str2;
    ASSERT_TRUE((str == "S->aSbSS->") || (str == "S->S->aSbS"));
    getline(output_stream, str);
    ASSERT_EQ(str, "S");
}

TEST_F(GrammarTestCase, WrongInputTest) {
    std::vector<std::string> cases = {
            "1 0 0\n"
            "a\n"
            "a\n",

            "2 0 0\n"
            "SS\n"
            "S\n",

            "1 2 1\n"
            "S\n"
            "ab\n"
            "ab\n"
            "S\n",

            "1 2 1\n"
            "S\n"
            "ab\n"
            "SS->a\n"
            "S\n",

            "1 2 1\n"
            "S\n"
            "ab\n"
            "a->a\n"
            "S\n",

            "1 2 1\n"
            "S\n"
            "ab\n"
            "S->c\n"
            "S\n",

            "1 2 1\n"
            "S\n"
            "ab\n"
            "S->a\n"
            "a\n",
    };

    for (auto& s : cases) {
        std::istringstream input_stream(s);
        std::istream in(input_stream.rdbuf());
        Grammar grammar;
        ASSERT_THROW(grammar.read(in), std::runtime_error);
    }
}