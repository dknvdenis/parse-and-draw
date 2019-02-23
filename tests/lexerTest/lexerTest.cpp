#include <QtTest>

#include "../../app/lexer.h"

class LexerTest : public QObject
{
    Q_OBJECT
    char _dp;

public:
    LexerTest()
    {
        _dp = QLocale::system().decimalPoint().toLatin1();
    }

private:
    void test(Lexer &l, const QVector<Token> &tokens)
    {
        int i = 0;

        l.reset();
        while (!l.isDone())
        {
            QVERIFY2(i < tokens.size(), "Error: i >= tokens.size()");

            Token t = l.nextToken();
            QVERIFY2((t.name == tokens[i].name && t.type == tokens[i].type),
                     (QString("Error: %1 != %2 || %3 != %4")
                     .arg(t.name.c_str())
                     .arg(tokens[i].name.c_str())
                     .arg(static_cast<int>(t.type))
                     .arg(static_cast<int>(tokens[i].type))).toLocal8Bit());

            i++;
        }
    }

private slots:
    void test_case1()
    {
        Lexer l(std::string("1 99") + _dp + "0+-*/^();", _dp);
        test(l, {
                 {"1", TokenType_t::number},
                 {" ", TokenType_t::whitespace},
                 {std::string("99") + _dp + "0", TokenType_t::number},
                 {"+", TokenType_t::op},
                 {"-", TokenType_t::op},
                 {"*", TokenType_t::op},
                 {"/", TokenType_t::op},
                 {"^", TokenType_t::op},
                 {"(", TokenType_t::leftParen},
                 {")", TokenType_t::rightParen},
                 {";", TokenType_t::unknown},
             });
    }

    void test_case2()
    {
        Lexer l("1+1*2/3", _dp);
        test(l, {
                 {"1", TokenType_t::number},
                 {"+", TokenType_t::op},
                 {"1", TokenType_t::number},
                 {"*", TokenType_t::op},
                 {"2", TokenType_t::number},
                 {"/", TokenType_t::op},
                 {"3", TokenType_t::number},
             });
    }

    void test_case3()
    {
        Lexer l("3^(2+5)/7\n", _dp);
        test(l, {
                 {"3", TokenType_t::number},
                 {"^", TokenType_t::op},
                 {"(", TokenType_t::leftParen},
                 {"2", TokenType_t::number},
                 {"+", TokenType_t::op},
                 {"5", TokenType_t::number},
                 {")", TokenType_t::rightParen},
                 {"/", TokenType_t::op},
                 {"7", TokenType_t::number},
                 {"\n", TokenType_t::unknown},
             });
    }

    void test_case4()
    {
        Lexer l("sin(x)+cos(5)*tan(y)/cot(z)", _dp);
        test(l, {
                 {"sin", TokenType_t::func},
                 {"(", TokenType_t::leftParen},
                 {"x", TokenType_t::var},
                 {")", TokenType_t::rightParen},
                 {"+", TokenType_t::op},
                 {"cos", TokenType_t::func},
                 {"(", TokenType_t::leftParen},
                 {"5", TokenType_t::number},
                 {")", TokenType_t::rightParen},
                 {"*", TokenType_t::op},
                 {"tan", TokenType_t::func},
                 {"(", TokenType_t::leftParen},
                 {"y", TokenType_t::var},
                 {")", TokenType_t::rightParen},
                 {"/", TokenType_t::op},
                 {"cot", TokenType_t::func},
                 {"(", TokenType_t::leftParen},
                 {"z", TokenType_t::var},
                 {")", TokenType_t::rightParen},
             });
    }
};

QTEST_MAIN(LexerTest)

#include "lexerTest.moc"
