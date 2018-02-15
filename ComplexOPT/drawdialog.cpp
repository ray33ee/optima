#include "drawdialog.h"
#include "ui_drawdialog.h"

#include "mainwindow.h"

DrawDialog::DrawDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DrawDialog)
{
    ui->setupUi(this);

    setModal(true);

    setMin(Complex{-1.0,-1.0});
    setMax(Complex{1.0,1.0});

    setFormula("z");
}

DrawDialog::~DrawDialog()
{
    delete ui;
}

void DrawDialog::accept()
{
    try
    {
        setFormula(ui->txtFormula->toPlainText());
        auto minList = parseFormula::processString(ui->txtMinimum->text(), true);
        auto maxList = parseFormula::processString(ui->txtMaximum->text(), true);

        minimum = ((MainWindow*)parent())->evaluate(Complex{0.0, 0.0}, TokenList{minList.data(), minList.size()});
        maximum = ((MainWindow*)parent())->evaluate(Complex{0.0, 0.0}, TokenList{maxList.data(), maxList.size()});

        QDialog::accept();
    }
    catch (const std::exception &e)
    {
        QMessageBox::warning(this, "Optima", "Formula error - " + QString(e.what()), QMessageBox::Ok);
    }
}

QString DrawDialog::getFormula() const
{
    return ui->txtFormula->toPlainText();
}

void DrawDialog::setFormula(const QString &form)
{
    list = parseFormula::processString(form, false);
    ui->txtFormula->setPlainText(form);
}

void DrawDialog::setMin(const Complex &min)
{
    minimum = min;
    ui->txtMinimum->setText(complex_to_string(minimum));
}

void DrawDialog::setMax(const Complex &max)
{
    maximum = max;
    ui->txtMaximum->setText(complex_to_string(maximum));
}
