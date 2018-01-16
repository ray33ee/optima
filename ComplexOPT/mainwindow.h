#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//#include "structs.h"

#include <QtGui>
#include <QGraphicsView>
#include "infix.h"
#include "drawdialog.h"
#include <QAction>
#include "QLabel"
#include "QMessageBox"
#include "QFileDialog"

#include <QVector>

#include <complex>

#include "parseformula.h"

using Complex = std::complex<double>;

template <class T>
class LinearUndo
{
private:
    QVector<T> m_vHistory;                 //List of items in undo history
    typename QVector<T>::iterator m_iCurrent;    //Pointer to current item in list
    typename QVector<T>::iterator m_iTop;        //Pointer to last item in history



    LinearUndo() : m_iCurrent(nullptr), m_iTop(nullptr) {}

public:

    LinearUndo(const LinearUndo &) = delete;
    LinearUndo operator=(const LinearUndo &) = delete;

    static LinearUndo& instance()
    {
        static LinearUndo inst;
        return inst;
    }

    void append(const T &rhs)
    {
        qDebug() << rhs.m_Minimum.real() << rhs.m_Minimum.imag() << rhs.m_Maximum.real() << rhs.m_Maximum.imag() << rhs.m_Formula;
        if (m_iCurrent == m_iTop)
        {
            //Append rhs to history
            m_vHistory.append(rhs);
            //Set m_iTop and m_iCurrent to last item in vector
            m_iTop = m_iCurrent = m_vHistory.end() - 1;
        }
        else
        {
            //Increment m_iCurrent
            ++m_iCurrent;
            //latest item has resulted in a branch, rhs is now the top
            m_iTop = m_iCurrent;
            //Set *m_iCurrent to rhs
            *m_iCurrent = rhs;
        }
    }

    const T* undo()
    {
        //If this is not the last item
        if (m_iCurrent != m_vHistory.begin())
            //decrement m_iCurrent
            --m_iCurrent;

        //return *m_iCurrent
        return m_iCurrent;
    }

    const T* redo()
    {
        //If m_iCurrent != m_iTop
        if (m_iCurrent != m_iTop)
            //increment m_iCurrent
            ++m_iCurrent;

        //return *m_iCurrent
        return m_iCurrent;
    }
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    void retrace(const QPoint&);

private:
    Ui::MainWindow *ui;

    /* DLL functions */
    using DLLConstruct = int (*)(int);
    using DLLInitialise = int (*)(unsigned, unsigned, TokenList*, uchar*);
    using DLLDestruct = int (*)();
    using DLLCalculate = int (*)(Complex, Complex);
    using DLLTrace = void (*)(Complex, int&, Complex&, TokenList*, double&, double&);

    DLLConstruct constructor;
    DLLInitialise initialise;
    DLLCalculate calculate;
    DLLDestruct destructor;
    DLLTrace trace;

    QAction* newButton;
    QAction* saveButton;
    QAction* panButton;
    QAction* zoomButton;
    QAction* newtonButton;
    QAction* refreshButton;
    QAction* undoButton;
    QAction* redoButton;
    QAction* zinButton;
    QAction* zoutButton;

    QLabel* color;
    QLabel* inreal;
    QLabel* inimag;
    QLabel* outreal;
    QLabel* outimag;
    QLabel* outmod;
    QLabel* outarg;


    QImage* image;
    QGraphicsScene* scene;
    QLibrary* lib;

    DrawDialog* dialog;

    struct Canvas
    {
    public:
        Complex m_Minimum;
        Complex m_Maximum;
        QString m_Formula;
    };



public:


    DrawDialog* getDialog() const { return dialog; }
    int checkedButton() const
    {
        if (panButton->isChecked())
            return 0;
        else if (zoomButton->isChecked())
            return 1;
        else if (newtonButton->isChecked())
            return 2;
    }

    void centerZoom(double factor);


    void buttonNew();
    void buttonSave();
    void buttonRefresh();
    void buttonUndo();
    void buttonRedo();


protected:
    virtual void resizeEvent(QResizeEvent* event);
    void redraw(Complex, Complex, TokenList *list);
    void redraw(const Canvas &c)
    {
        dialog->setFormula(c.m_Formula);
        redraw(c.m_Minimum, c.m_Maximum, dialog->getList());
    }

public slots:

    void toolbarTriggered(QAction *);
    void menuTriggered(QAction *);

    void togglePan(bool);
    void toggleZoom(bool);
    void toggleNewton(bool);

};

#endif // MAINWINDOW_H
