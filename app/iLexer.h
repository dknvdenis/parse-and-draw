#pragma once

#include "token.h"

class ILexer
{
public:
    virtual ~ILexer() = default;

public:
    virtual Token nextToken() = 0;
    virtual bool isDone() const = 0;
    virtual void reset() = 0;
};
