#pragma once

#include <gtest/gtest.h>
#include <iostream>
#include "GeneralBaseParserTestCase.h"

class GeneralLR1ParserTestCase: public GeneralBaseParserTestCase {
protected:
    void SetUp() override;
};