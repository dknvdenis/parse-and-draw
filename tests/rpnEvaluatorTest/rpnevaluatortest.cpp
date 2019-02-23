#include <QtTest>

#include "../../app/rpnEvaluator.h"
#include "../../app/appExceptions.h"

class RPNEvaluatorTest : public QObject
{
    Q_OBJECT

private:
    void test(const RPN_t &rpn,
              const std::vector<std::pair<std::string, double>> &vars,
              double result)
    {
        RPNEvaluator e;

        for (const auto &var : vars)
            e.setVariable(var.first, var.second);

        double actual = e.eval(rpn);
        QCOMPARE(actual, result);
    }

#define EMPTY_VARS std::vector<std::pair<std::string, double>>()

private slots:
    void test_case1()
    {
        test(RPN_t({
                       {"7", TokenType_t::number},
                       {"8", TokenType_t::number},
                       {"*", TokenType_t::op},
             }),
             EMPTY_VARS,
             56
             );
    }

    void test_case2()
    {
        test(RPN_t({
                       {"4", TokenType_t::number},
                       {"2", TokenType_t::number},
                       {"5", TokenType_t::number},
                       {"+", TokenType_t::op},
                       {"^", TokenType_t::op},
                       {"7", TokenType_t::number},
                       {"/", TokenType_t::op},
             }),
             EMPTY_VARS,
             2340.5714285714284
             );
    }

    void test_case3()
    {
        test(RPN_t({
                       {"x", TokenType_t::var},
                       {"cos", TokenType_t::func},
                       {"x", TokenType_t::var},
                       {"cos", TokenType_t::func},
                       {"*", TokenType_t::op},
                       {"x", TokenType_t::var},
                       {"sin", TokenType_t::func},
                       {"x", TokenType_t::var},
                       {"sin", TokenType_t::func},
                       {"*", TokenType_t::op},
                       {"+", TokenType_t::op},
             }),
             { {"x", 0.5} },
             1
             );
    }

    void test_case4()
    {
        test(RPN_t({
                       {"x", TokenType_t::var},
                       {"cos", TokenType_t::func},
                       {"2", TokenType_t::number},
                       {"^", TokenType_t::op},
                       {"x", TokenType_t::var},
                       {"sin", TokenType_t::func},
                       {"2", TokenType_t::number},
                       {"^", TokenType_t::op},
                       {"+", TokenType_t::op},
             }),
             { {"x", 0.5} },
             1
             );
    }

    void test_case5()
    {
        QVERIFY_EXCEPTION_THROWN(test(RPN_t({
                       {"x", TokenType_t::var},
                       {"sqrt", TokenType_t::func},
             }),
             { {"x", 9} },
             3
             ), TokenException);
    }

    void test_case6()
    {
        test(RPN_t({
                       {"x", TokenType_t::var},
                       {"cot", TokenType_t::func},
                       {"x", TokenType_t::var},
                       {"tan", TokenType_t::func},
                       {"x", TokenType_t::var},
                       {"sin", TokenType_t::func},
                       {"x", TokenType_t::var},
                       {"cos", TokenType_t::func},
                       {"+", TokenType_t::op},
                       {"+", TokenType_t::op},
                       {"+", TokenType_t::op},
             }),
             { {"x", 0.5} },
             3.7337983120508182
             );
    }

    void test_case7()
    {
        test(RPN_t({
                       {"x", TokenType_t::var},
                       {"abs", TokenType_t::func},
             }),
             { {"x", -5} },
             5
             );
    }
};

QTEST_APPLESS_MAIN(RPNEvaluatorTest)

#include "rpnevaluatortest.moc"
