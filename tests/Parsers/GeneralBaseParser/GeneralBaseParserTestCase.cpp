#include "GeneralBaseParserTestCase.h"
#include <fstream>

void GeneralBaseParserTestCase::TearDown() {
    delete parser;
}

void GeneralBaseParserTestCase::TestFromFile(const std::string &filename) {
    std::ifstream fin(filename + "/in.txt");
    grammar.read(fin);

    parser->fit(grammar);

    size_t n;
    fin >> n;
    fin.get();
    std::ifstream res_fin(filename + "/out.txt");
    for (int i = 0; i < n; ++i) {
        std::string test, result, true_result;
        getline(fin, test);
        getline(res_fin, true_result);
        result = (parser->predict(test) ? "Yes" : "No");
        ASSERT_EQ(result, true_result);
    }
    fin.close();
    res_fin.close();
}