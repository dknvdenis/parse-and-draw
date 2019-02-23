#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QFile>
#include "../qcustomplot/qcustomplot.h"
#include "appExceptions.h"
#include "helpDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    _workerThread(new QThread()),
    _worker(new Worker(nullptr)),
    _busy(false)
{
    ui->setupUi(this);

    init();
    initLinks();

    _workerThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;

    _workerThread->requestInterruption();
    _workerThread->quit();
    if (!_workerThread->wait(5000))
        _workerThread->terminate();

    delete _workerThread;
    _workerThread = nullptr;
}

void MainWindow::init()
{
    ui->openAction->setEnabled(false);
    ui->updateAction->setEnabled(false);
    ui->lbConsole->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->plot->xAxis->setLabel("x");
    ui->plot->yAxis->setLabel("y");

    _worker->moveToThread(_workerThread);
}

void MainWindow::initLinks()
{
    connect(ui->openAction, &QAction::triggered,
            this, &MainWindow::openFile);

    connect(ui->updateAction, &QAction::triggered,
            this, &MainWindow::updateFile);

    connect(ui->closeAction, &QAction::triggered,
            this, &MainWindow::close);

    connect(ui->helpAction, &QAction::triggered,
            this, &MainWindow::openHelp);

    connect(_workerThread, &QThread::started,
            this, [this] () { ui->openAction->setEnabled(true); });

    connect(_workerThread, &QThread::finished,
            [this] () { delete _worker; _worker = nullptr; });

    connect(_worker, &Worker::finished,
            this, &MainWindow::workerFinished);

    connect(_worker, &Worker::progress,
            this, &MainWindow::workerProgress);

    connect(_worker, &Worker::error,
            this, &MainWindow::workerError);
}

void MainWindow::readfile()
{
    ui->lbConsole->clear();
    ui->lbConsole->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    removeAllGraph(true);

    addToConsole(QString("Чтение файла %1").arg(_fileFullPath));

    QFile file(_fileFullPath);
    if (file.open(QFile::ReadOnly))
    {
        WorkerInput in;

        in.expression = QString::fromUtf8(file.readLine());
        if (in.expression.endsWith("\r\n"))
            in.expression.chop(2); // remove \r\n
        if (in.expression.endsWith('\n'))
            in.expression.chop(1);

        _lastExpression = in.expression;
        if (!in.expression.isEmpty())
        {
            in.delta = 0.01;

            QString bounds = QString::fromUtf8(file.readLine());
            if (parseBounds(bounds, in.x1, in.y1, in.x2, in.y2)
                    && checkBounds(in))
            {
                addToConsole(QString("Построение графика %1...").arg(in.expression));
                _busy = true;

                ui->plot->xAxis->setRange(in.x1, in.x2);
                ui->plot->yAxis->setRange(in.y2, in.y1);

                QMetaObject::invokeMethod(_worker, "run", Qt::QueuedConnection,
                                          Q_ARG(WorkerInput, in));
            }
            else
                addToConsole("Ошибка. Неправильный формат области графика");
        }
        else
            addToConsole("Ошибка. Выражение не задано");

        file.close();
    }
    else
        addToConsole("Ошибка. Не удалось открыть файл");
}

void MainWindow::addToConsole(const QString &text)
{
    ui->lbConsole->setText(ui->lbConsole->text() + text + '\n');
}

bool MainWindow::parseBounds(const QString &str, int &outX1, int &outY1,
                             int &outX2, int &outY2) const
{
    QStringList points = str.split(";", QString::SkipEmptyParts);
    if (points.size() != 2)
        return false;

    QStringList v1 = points.first().split(",", QString::SkipEmptyParts);
    if (v1.size() != 2)
        return false;

    QStringList v2 = points.last().split(",", QString::SkipEmptyParts);
    if (v2.size() != 2)
        return false;

    bool ok = false;
    outX1 = v1[0].toInt(&ok);
    if (!ok) return false;

    outY1 = v1[1].toInt(&ok);
    if (!ok) return false;

    outX2 = v2[0].toInt(&ok);
    if (!ok) return false;

    outY2 = v2[1].toInt(&ok);
    if (!ok) return false;

    return true;
}

void MainWindow::removeAllGraph(bool replot)
{
    for (int i = 0; i < ui->plot->graphCount(); i++)
        ui->plot->removeGraph(i);

    if (replot)
        ui->plot->replot();
}

bool MainWindow::checkBounds(const WorkerInput &in) const
{
    return (in.x1 < in.x2 && in.y1 > in.y2);
}

void MainWindow::openFile()
{
    if (_busy)
    {
        QMessageBox::information(this, "Plot",
                                 "Построение предыдущего графика ещё не закончено");
        return;
    }

    QString file =
            QFileDialog::getOpenFileName(this, "Открыть файл",
                                         _fileFullPath,
                                         "Текстовые файлы (*.txt) (*.txt);;"
                                                        "Все файлы (*.*)");
    if (file.isNull())
        return;

    _fileFullPath = file;
    QFileInfo fileInfo(file);
    setWindowTitle(QString("%1 - Plot").arg(fileInfo.fileName()));
    ui->updateAction->setEnabled(true);

    readfile();
}

void MainWindow::updateFile()
{
    if (_busy)
    {
        QMessageBox::information(this, "Plot",
                                 "Построение предыдущего графика ещё не закончено");
        return;
    }

    if (!_fileFullPath.isEmpty())
        readfile();
}

void MainWindow::openHelp()
{
    HelpDialog dlg(this);
    dlg.exec();
}

void MainWindow::workerFinished(const WorkerOutput &result)
{
    for (int i = 0; i < result.graphs.size(); i++)
    {
        ui->plot->addGraph();
        ui->plot->graph(i)->setData(result.graphs[i].x, result.graphs[i].y);
    }
    ui->plot->replot();

    addToConsole(QString("График построен за %1 миллисекунд").arg(result.elapsedTime));
    _busy = false;
}

void MainWindow::workerProgress(int value)
{
    addToConsole(QString("%1%").arg(value));
}

void MainWindow::workerError(std::exception_ptr excPtr)
{
    removeAllGraph(true);

    try
    {
        if (excPtr)
            std::rethrow_exception(excPtr);
    }
    catch (const TokenException &exc)
    {
        addToConsole(QString("Ошибка. Токен: \"%1\". %2")
                     .arg(exc.token().name.c_str()).arg(exc.what()));

        int pos = exc.token().posInLine;
        if (pos < _lastExpression.size())
        {
            const char *const cursor = "^<--";
            const int cursorLen = strlen(cursor);

            QByteArray errorCursor(pos + cursorLen, ' ');
            memcpy(errorCursor.data() + pos, cursor, cursorLen);

            addToConsole(_lastExpression);
            addToConsole(QString::fromUtf8(errorCursor));
        }
    }
    catch (const std::exception &exc)
    {
        addToConsole(QString("Ошибка. %1").arg(exc.what()));
    }
    catch (...)
    {
        addToConsole("Ошибка");
    }

    _busy = false;
}
