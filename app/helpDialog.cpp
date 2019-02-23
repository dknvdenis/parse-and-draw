#include "helpDialog.h"
#include "ui_helpDialog.h"
#include <QFontDatabase>

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    ui->lbHelp->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->lbHelp->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    ui->lbHelp->setText(
R"(Программа для построения графика функции, заданной в виде математического выражения.

Формат файла:
Первая строка содержит выражение.
  Разрешённые символы: числа, скобки, операторы (+ - * / ^),
  функции (sin, cos, tan, cot, abs), переменная 'x'.
  Разделитель целой и дробной частей зависит от текущей локали системы.
  Для русской - запятая, для американской - точка.

Вторая строка содержит координаты ограничивающего прямоугольника, в котором будет выведен график.
  Формат строки: "X1,Y1;X2,Y2",
  где X1,Y1 - координаты верхней левой вершины, X2,Y2 - координаты нижней правой вершины.
  (X1,Y1)+--------------+
         |              |
         |              |
         |              |
         +--------------+ (X2,Y2)

Примеры:
-15+x^2
-10,20;10,-10

1/x
-10,10;10,-10

sin(x^0,5)
0,2;1000,-2
)"
);
}

HelpDialog::~HelpDialog()
{
    delete ui;
}
