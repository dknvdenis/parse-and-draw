#include "rpnEvaluator.h"
#include <string>
#include <cmath>
#include "appExceptions.h"

namespace {
double popFromStack(EvalStack_t &stack)
{
    double v = stack.top();
    stack.pop();

    return v;
}

}

std::unordered_map<std::string, EvalFn_t> RPNEvaluator::_evalFns = {
    {"+", [] (EvalStack_t &stack) {
         if (stack.size() < 2) return false;
         double op1 = popFromStack(stack);
         double op2 = popFromStack(stack);
         stack.push(op1 + op2);
         return true;
    }},

    {"-", [] (EvalStack_t &stack) {
         if (stack.size() < 2) return false;
         double op1 = popFromStack(stack);
         double op2 = popFromStack(stack);
         stack.push(op2 - op1);
         return true;
    }},

    {"*", [] (EvalStack_t &stack) {
         if (stack.size() < 2) return false;
         double op1 = popFromStack(stack);
         double op2 = popFromStack(stack);
         stack.push(op1 * op2);
         return true;
    }},

    {"/", [] (EvalStack_t &stack) {
         if (stack.size() < 2) return false;
         double op1 = popFromStack(stack);
         double op2 = popFromStack(stack);
         stack.push(op2 / op1);
         return true;
    }},

    {"^", [] (EvalStack_t &stack) {
         if (stack.size() < 2) return false;
         double op1 = popFromStack(stack);
         double op2 = popFromStack(stack);
         stack.push(pow(op2, op1));
         return true;
    }},

    {"minus", [] (EvalStack_t &stack) {
         if (stack.size() < 1) return false;
         double op = popFromStack(stack);
         stack.push(-op);
         return true;
    }},

    {"sin", [] (EvalStack_t &stack) {
         if (stack.size() < 1) return false;
         double op = popFromStack(stack);
         stack.push(sin(op));
         return true;
    }},

    {"cos", [] (EvalStack_t &stack) {
         if (stack.size() < 1) return false;
         double op = popFromStack(stack);
         stack.push(cos(op));
         return true;
    }},

    {"tan", [] (EvalStack_t &stack) {
         if (stack.size() < 1) return false;
         double op = popFromStack(stack);
         stack.push(tan(op));
         return true;
    }},

    {"cot", [] (EvalStack_t &stack) {
         if (stack.size() < 1) return false;
         double op = popFromStack(stack);
         stack.push(1. / tan(op));
         return true;
    }},

    {"abs", [] (EvalStack_t &stack) {
         if (stack.size() < 1) return false;
         double op = popFromStack(stack);
         stack.push(std::abs(op));
         return true;
    }},
};

void RPNEvaluator::setVariable(const std::string &name, double value)
{
    _vars[name] = value;
}

double RPNEvaluator::eval(const RPN_t &rpn)
{
    RPN_t q = rpn;
    std::stack<double> stack;

    while (!q.empty())
    {
        Token token = q.front();
        q.pop();

        switch (token.type)
        {
        case TokenType_t::number: {
            try
            {
                stack.push(std::stod(token.name));
            }
            catch (...)
            {
                throw TokenException(token, "Не удалось конвертировать токен в число");
            }
            break;
        }

        case TokenType_t::var: {
            if (_vars.find(token.name) == _vars.cend())
                throw TokenException(token, "Переменная не определена");

            stack.push(_vars[token.name]);
            break;
        }

        case TokenType_t::op:
        case TokenType_t::func: {
            if (_evalFns.find(token.name) == _evalFns.cend())
                throw TokenException(token, "Функция не определена");

            EvalFn_t &fn = _evalFns[token.name];
            if (!fn(stack))
                throw TokenException(token, "Неверное количество операндов");
            break;
        }

        default:
            throw TokenException(token, "Неизвестный токен");
        }
    }

    if (stack.size() != 1)
        throw TokenException(Token(), "Некорректное состояние стека");

    return stack.top();
}
