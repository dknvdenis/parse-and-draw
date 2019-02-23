#pragma once

#include <QString>
#include <QVector>

struct WorkerInput
{
    QString expression;

    int x1;
    int y1;
    int x2;
    int y2;

    double delta;
};

struct GraphData
{
    QVector<double> x;
    QVector<double> y;
};

struct WorkerOutput
{
    QVector<GraphData> graphs;
    qint64 elapsedTime;
};
