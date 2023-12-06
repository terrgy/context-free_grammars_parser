#pragma once

#include <string>
#include "Grammar.h"

class BaseParser {
protected:
    Grammar grammar;

public:
    virtual void fit(Grammar) = 0;
    virtual bool predict(const std::string&) = 0;

    virtual ~BaseParser() = default;
};