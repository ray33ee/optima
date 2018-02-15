#include "calculatordialog.h"
#include "ui_calculatordialog.h"

#include "mainwindow.h"

CalculatorDialog::CalculatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalculatorDialog)
{
    ui->setupUi(this);

}

CalculatorDialog::~CalculatorDialog()
{
    delete ui;
}

void CalculatorDialog::accept()
{
    QDialog::accept();
}

void CalculatorDialog::calculate()
{
    //Convert expression to complex number
    auto inputformula = parseFormula::processString(ui->txtInput->text(), true);
    auto input = ((MainWindow*)parent())->evaluate(Complex{0.0,0.0}, TokenList{inputformula.data(), inputformula.size()});


    auto formula = parseFormula::processString(ui->txtFormula->toPlainText(), false);

    auto ans = ((MainWindow*)parent())->evaluate(input, TokenList{formula.data(), formula.size()});

    ui->txtOutput->setText(QString::number(ans.real()) + " " + QString::number(ans.imag()));


}
