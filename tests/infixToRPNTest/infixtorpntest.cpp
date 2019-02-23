#include <QtTest>

#include "../../app/infixToRPN.h"
#include "../../app/appExceptions.h"

class LexerMock : public ILexer
{
public:
    LexerMock(const QVector<Token> &tokens)
        : ILexer(),
          _tokens(tokens),
          _pos(0)
    {}

public:
    Token nextToken() override
    {
        if (isDone())
            return Token();

        return _tokens[_pos++];
    }

    bool isDone() const override
    {
        return (_pos >= _tokens.size());
    }

    void reset() override
    {
        _pos = 0;
    }

private:
    QVector<Token> _tokens;
    int _pos;
};

class InfixToRPNTest : public QObject
{
    Q_OBJECT

private:
    void test(const RPN_t &rpn, const QVector<Token> &tokens)
    {
        RPN_t q = rpn;
        int i = 0;
        while (q.size() > 0)
        {
            Token t = q.front();
            q.pop();

            QVERIFY2(i < tokens.size(), "Error: i >= tokens.size()");

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
        auto l = new LexerMock({
                                   {"3", TokenType_t::number},
                                   {"^", TokenType_t::op},
                                   {"(", TokenType_t::leftParen},
                                   {"2", TokenType_t::number},
                                   {"+", TokenType_t::op},
                                   {"5", TokenType_t::number},
                                   {")", TokenType_t::rightParen},
                                   {"/", TokenType_t::op},
                                   {"7", TokenType_t::number},
                               });

        InfixToRPN cnv{std::shared_ptr<ILexer>(l)};
        test(cnv.convert(), {
                 {"3", TokenType_t::number},
                 {"2", TokenType_t::number},
                 {"5", TokenType_t::number},
                 {"+", TokenType_t::op},
                 {"^", TokenType_t::op},
                 {"7", TokenType_t::number},
                 {"/", TokenType_t::op},
             });
    }

    void test_case2()
    {
        auto l = new LexerMock({
                                   {"1", TokenType_t::number},
                                   {"+", TokenType_t::op},
                                   {"1", TokenType_t::number},
                                   {"\n", TokenType_t::unknown},
                               });

        InfixToRPN cnv{std::shared_ptr<ILexer>(l)};
        QVERIFY_EXCEPTION_THROWN(test(cnv.convert(), {
                                          {"1", TokenType_t::number},
                                          {"1", TokenType_t::number},
                                          {"+", TokenType_t::op},
                                          {"\n", TokenType_t::unknown},
                                      }), TokenException);
    }

    void test_case3()
    {
        auto l = new LexerMock({
                                   {"(", TokenType_t::leftParen},
                                   {"1", TokenType_t::number},
                                   {"+", TokenType_t::op},
                                   {"1", TokenType_t::number},
                                   {")", TokenType_t::rightParen},
                                   {")", TokenType_t::rightParen},
                               });

        InfixToRPN cnv{std::shared_ptr<ILexer>(l)};
        QVERIFY_EXCEPTION_THROWN(test(cnv.convert(), {
                                          {"1", TokenType_t::number},
                                          {"1", TokenType_t::number},
                                          {"+", TokenType_t::op},
                                      }), TokenException);
    }

    void test_case4()
    {
        auto l = new LexerMock({
                                   {"1", TokenType_t::number},
                                   {"+", TokenType_t::op},
                                   {"sin", TokenType_t::func},
                                   {"(", TokenType_t::leftParen},
                                   {"x", TokenType_t::var},
                                   {"^", TokenType_t::op},
                                   {"2", TokenType_t::number},
                                   {")", TokenType_t::rightParen},
                               });

        InfixToRPN cnv{std::shared_ptr<ILexer>(l)};
        test(cnv.convert(), {
                 {"1", TokenType_t::number},
                 {"x", TokenType_t::var},
                 {"2", TokenType_t::number},
                 {"^", TokenType_t::op},
                 {"sin", TokenType_t::func},
                 {"+", TokenType_t::op},
             });
    }
};

QTEST_APPLESS_MAIN(InfixToRPNTest)

#include "infixtorpntest.moc"
