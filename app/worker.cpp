#include "worker.h"

#include <cmath>
#include <QThread>
#include <QElapsedTimer>
#include <QLocale>
#include <QDebug>
#include "lexer.h"
#include "infixToRPN.h"
#include "rpnEvaluator.h"
#include "appExceptions.h"

Worker::Worker(QObject *parent)
    : QObject(parent)
{}

void Worker::run(const WorkerInput &data)
{
    try
    {
        QElapsedTimer timer;
        timer.start();

        emit progress(0, QPrivateSignal());

        char decimalPoint = QLocale::system().decimalPoint().toLatin1();
        auto lexer = std::make_shared<Lexer>(data.expression.toStdString(),
                                             decimalPoint);
        InfixToRPN cnv(lexer);
        auto rpn = cnv.convert();

        RPNEvaluator evaluator;
        std::string varX("x");

        WorkerOutput result;
        GraphData gData;

        std::vector<double> parts(10);
        double part = double(data.x2 - data.x1) / parts.size();
        for (size_t i = 0; i < parts.size(); i++)
            parts[i] = part * (i + 1);

        int counter = 0;
        bool intersect = false;
        for (double x = data.x1; x < data.x2; x += data.delta)
        {
            if (counter++ > 1000)
            {
                counter = 0;
                if (thread()->isInterruptionRequested())
                    return;
            }

            for (int i = parts.size() - 1; i >= 0; i--)
            {
                if (parts[i] < x)
                {
                    parts[i] = std::numeric_limits<double>::max();
                    emit progress((i + 1) * 10, QPrivateSignal());
                    break;
                }
            }

            evaluator.setVariable(varX, x);
            double y = evaluator.eval(rpn);

            if (std::isinf(y) || std::isnan(y))
            {
                result.graphs.push_back(gData);
                gData.x.clear();
                gData.y.clear();
                continue;
            }

            gData.x.push_back(x);
            gData.y.push_back(y);

            if (!intersect && intersection(data, x, y))
                intersect = true;
        }

        result.elapsedTime = timer.elapsed();
        emit progress(100, QPrivateSignal());

        if (!intersect)
            throw NoIntersectionException("График не попал в заданную область");

        if (!gData.x.isEmpty())
            result.graphs.push_back(gData);

        emit finished(result, QPrivateSignal());
    }
    catch (...)
    {
        emit error(std::current_exception(), QPrivateSignal());
    }
}

bool Worker::intersection(const WorkerInput &in, double x, double y) const
{
    return (x >= in.x1 && x <= in.x2 && y >= in.y2 && y <= in.y1);
}
