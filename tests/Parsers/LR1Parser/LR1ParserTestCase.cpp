#include "LR1ParserTestCase.h"
#include "LR1Parser.h"

void GeneralLR1ParserTestCase::SetUp() {
    parser = new LR1Parser();
}

TEST_F(GeneralLR1ParserTestCase, FileTest1) {
    TestFromFile("../tests/Parsers/tests/CorrectParserTests/1");
}

TEST_F(GeneralLR1ParserTestCase, FileTest2) {
    TestFromFile("../tests/Parsers/tests/CorrectParserTests/2");
}

TEST_F(GeneralLR1ParserTestCase, FileTest3) {
    TestFromFile("../tests/Parsers/tests/CorrectParserTests/3");
}

TEST_F(GeneralLR1ParserTestCase, FileTest4) {
    ASSERT_THROW(TestFromFile("../tests/Parsers/tests/CorrectParserTests/4"), std::runtime_error);
}

TEST_F(GeneralLR1ParserTestCase, FileTest5) {
    ASSERT_THROW(TestFromFile("../tests/Parsers/tests/CorrectParserTests/5"), std::runtime_error);
}

TEST_F(GeneralLR1ParserTestCase, FileTest6) {
    ASSERT_THROW(TestFromFile("../tests/Parsers/tests/CorrectParserTests/6"), std::runtime_error);
}

TEST_F(GeneralLR1ParserTestCase, FileTest7) {
    TestFromFile("../tests/Parsers/tests/CorrectParserTests/7");
}

TEST_F(GeneralLR1ParserTestCase, FileTest8) {
    TestFromFile("../tests/Parsers/tests/CorrectParserTests/8");
}

TEST_F(GeneralLR1ParserTestCase, FileTest9) {
    TestFromFile("../tests/Parsers/tests/CorrectParserTests/9");
}