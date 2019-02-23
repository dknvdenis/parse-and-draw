#pragma once

#include <stack>
#include <unordered_map>
#include <functional>
#include "rpn.h"

using EvalStack_t = std::stack<double>;
using EvalFn_t = std::function<bool (EvalStack_t&)>;

class RPNEvaluator
{
public:
    void setVariable(const std::string &name, double value);
    double eval(const RPN_t &rpn);

private:
    std::unordered_map<std::string, double> _vars;
    static std::unordered_map<std::string, EvalFn_t> _evalFns;
};
