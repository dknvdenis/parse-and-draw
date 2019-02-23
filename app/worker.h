#pragma once

#include <exception>
#include <QObject>
#include "workerData.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent);

public slots:
    void run(const WorkerInput &data);

private:
    bool intersection(const WorkerInput &in, double x, double y) const;

signals:
    void finished(const WorkerOutput &out, QPrivateSignal);
    void progress(int value, QPrivateSignal);
    void error(std::exception_ptr excPtr, QPrivateSignal);
};
