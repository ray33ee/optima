#include "drawdialog.h"
#include "ui_drawdialog.h"

#include "mainwindow.h"

DrawDialog::DrawDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DrawDialog)
{
    ui->setupUi(this);

    list.append(Token{1, {0.0,0.0}});

    minimum = Complex{-1.0,-1.0};
    maximum = Complex{1.0,1.0};

    ui->txtMinimum->setText("-1-i");
    ui->txtMaximum->setText("1+i");

    ui->txtFormula->setPlainText("z");
}

DrawDialog::~DrawDialog()
{
    delete ui;
}

void DrawDialog::accept()
{
    setFormula(ui->txtFormula->toPlainText());

    auto minList = parseFormula::processString(ui->txtMinimum->text());
    auto maxList = parseFormula::processString(ui->txtMaximum->text());

    minimum = ((MainWindow*)parent())->evaluate(TokenList{minList.data(), minList.size()});
    maximum = ((MainWindow*)parent())->evaluate(TokenList{maxList.data(), maxList.size()});

    QDialog::accept();
}


QString DrawDialog::getFormula() const
{
    return ui->txtFormula->toPlainText();
}

void DrawDialog::setFormula(const QString &form)
{

    try
    {
        list = parseFormula::processString(form);
    }
    catch (const std::exception& e)
    {
        qDebug() << e.what();
    }

    ui->txtFormula->setPlainText(form);
}

void DrawDialog::setMin(const Complex &min)
{
    minimum = min;
    ui->txtMinimum->setText(QString::number(minimum.real()) + (minimum.imag() < 0 ? " - " : " + ") + QString::number(fabs(minimum.imag())) + "*i");
}

void DrawDialog::setMax(const Complex &max)
{
    maximum = max;
    ui->txtMaximum->setText(QString::number(maximum.real()) + (maximum.imag() < 0 ? " - " : " + ") + QString::number(fabs(maximum.imag())) + "*i");
}
