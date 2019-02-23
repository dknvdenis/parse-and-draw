#pragma once

#include <exception>
#include <QMainWindow>
#include <QThread>
#include "worker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QThread *_workerThread;
    Worker *_worker;
    QString _fileFullPath;
    QString _lastExpression;
    bool _busy;

private:
    void init();
    void initLinks();
    void readfile();
    void addToConsole(const QString &text);
    bool parseBounds(const QString &str, int &outX1, int &outY1,
                     int &outX2, int &outY2) const;
    void removeAllGraph(bool replot);
    bool checkBounds(const WorkerInput &in) const;

private slots:
    void openFile();
    void updateFile();
    void openHelp();
    void workerFinished(const WorkerOutput &result);
    void workerProgress(int value);
    void workerError(std::exception_ptr excPtr);
};
