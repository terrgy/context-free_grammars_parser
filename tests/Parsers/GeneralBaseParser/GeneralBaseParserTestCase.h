#pragma once

#include <gtest/gtest.h>
#include <iostream>
#include "BaseParser.h"
#include "Grammar.h"

class GeneralBaseParserTestCase: public ::testing::Test{
protected:
    Grammar grammar;
    BaseParser* parser;

    void SetUp() override = 0;

    void TestFromFile(const std::string& filename);

    void TearDown() override;
};