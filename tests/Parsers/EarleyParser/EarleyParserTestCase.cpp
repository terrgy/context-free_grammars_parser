#include "GeneralEarleyParserTestCase.h"
#include "EarleyParser.h"

void GeneralEarleyParserTestCase::SetUp() {
    parser = new EarleyParser();
}

TEST_F(GeneralEarleyParserTestCase, FileTest1) {
    TestFromFile("../tests/Parsers/tests/CorrectParserTests/1");
}

TEST_F(GeneralEarleyParserTestCase, FileTest2) {
    TestFromFile("../tests/Parsers/tests/CorrectParserTests/2");
}

TEST_F(GeneralEarleyParserTestCase, FileTest3) {
    TestFromFile("../tests/Parsers/tests/CorrectParserTests/3");
}

TEST_F(GeneralEarleyParserTestCase, FileTest4) {
    TestFromFile("../tests/Parsers/tests/CorrectParserTests/4");
}

TEST_F(GeneralEarleyParserTestCase, FileTest5) {
    TestFromFile("../tests/Parsers/tests/CorrectParserTests/5");
}

TEST_F(GeneralEarleyParserTestCase, FileTest6) {
    TestFromFile("../tests/Parsers/tests/CorrectParserTests/6");
}

TEST_F(GeneralEarleyParserTestCase, FileTest7) {
    TestFromFile("../tests/Parsers/tests/CorrectParserTests/7");
}

TEST_F(GeneralEarleyParserTestCase, FileTest8) {
    TestFromFile("../tests/Parsers/tests/CorrectParserTests/8");
}

TEST_F(GeneralEarleyParserTestCase, FileTest9) {
    TestFromFile("../tests/Parsers/tests/CorrectParserTests/9");
}

