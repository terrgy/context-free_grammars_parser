#pragma once

#include <gtest/gtest.h>
#include <iostream>
#include "GeneralBaseParserTestCase.h"

class GeneralEarleyParserTestCase: public GeneralBaseParserTestCase {
protected:
    void SetUp() override;
};