#ifndef DRAWDIALOG_H
#define DRAWDIALOG_H

#include <QDialog>
#include <QMessageBox>

#include "qdebug.h"
#include "parseformula.h"
#include "complex.h"

class MainWindow;

namespace Ui {
class DrawDialog;
}

class DrawDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::DrawDialog *ui;

    QVector<Token> list;

    Complex minimum;
    Complex maximum;

public:
    explicit DrawDialog(QWidget *parent = 0);
    ~DrawDialog();

    const Complex& getMin() const { return minimum; }
    const Complex& getMax() const { return maximum; }
    QString getFormula() const;

    void setMin(const Complex &min);
    void setMax(const Complex &max);
    void setFormula(const QString &form);

    TokenList getList() { return TokenList{list.data(), list.size()}; }

public slots:
    void accept();


};

#endif // DRAWDIALOG_H
