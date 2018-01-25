#include "drawdialog.h"
#include "ui_drawdialog.h"

DrawDialog::DrawDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DrawDialog)
{
    ui->setupUi(this);

    list.append(Token{1, {0.0,0.0}});

    minimum = Complex{-1.0,-1.0};
    maximum = Complex{1.0,1.0};

    ui->txtMinReal->setText("-1");
    ui->txtMinImag->setText("-1");
    ui->txtMaxReal->setText("1");
    ui->txtMaxImag->setText("1");

    ui->txtFormula->setPlainText("z");
}

DrawDialog::~DrawDialog()
{
    delete ui;
}

void DrawDialog::accept()
{
    setFormula(ui->txtFormula->toPlainText());

    minimum = { ui->txtMinReal->text().toDouble(), ui->txtMinImag->text().toDouble() };
    maximum = { ui->txtMaxReal->text().toDouble(), ui->txtMaxImag->text().toDouble() };

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

    ui->txtMinReal->setText(QString::number(minimum.real()));
    ui->txtMinImag->setText(QString::number(minimum.imag()));
}

void DrawDialog::setMax(const Complex &max)
{
    maximum = max;

    ui->txtMaxReal->setText(QString::number(maximum.real()));
    ui->txtMaxImag->setText(QString::number(maximum.imag()));
}
