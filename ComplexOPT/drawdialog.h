#ifndef DRAWDIALOG_H
#define DRAWDIALOG_H

#include <QDialog>

#include "qdebug.h"
#include "complex.h"
#include "infix.h"

namespace Ui {
class DrawDialog;
}

class DrawDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::DrawDialog *ui;

    infix* formula;
    TokenList* list;

    Complex minimum;
    Complex maximum;

public:
    explicit DrawDialog(Complex min, Complex max, Token *tokens, int count, QWidget *parent = 0);
    ~DrawDialog();

    const Complex& getMin() const { return minimum; }
    const Complex& getMax() const { return maximum; }
    QString getFormula() const;

    void setMin(const Complex &min);
    void setMax(const Complex &max);
    void setFormula(const QString &form);

    TokenList* getList() const { return list; }

public slots:
    void accept();


};

#endif // DRAWDIALOG_H
