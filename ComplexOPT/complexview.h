#ifndef COMPLEXVIEW_H
#define COMPLEXVIEW_H

#include <QtGui>
#include <QGraphicsView>

#include "mainwindow.h"

#include <complex>

using Complex = std::complex<double>;

/* Widget responsible for drawing on canvas */
class ComplexView : public QGraphicsView
{
private:
    QPoint pressPos;

    MainWindow* m_Parent;

public:
    ComplexView(QWidget* parent = 0) : QGraphicsView(parent) {}


    void setWindowParent(MainWindow* parent) { m_Parent = parent; }


protected:

    void mousePressEvent(QMouseEvent * event)
    {
        pressPos = event->pos();
    }

    void mouseReleaseEvent(QMouseEvent * event)
    {
        switch (m_Parent->checkedButton())
        {
            case 0: //pan
                {auto movementVector = event->pos() - pressPos;

                auto adjustedMovement =  Complex((m_Parent->getDialog()->getMax().real() - m_Parent->getDialog()->getMin().real()) / width() * movementVector.x(), (m_Parent->getDialog()->getMax().imag() - m_Parent->getDialog()->getMin().imag()) / height() * movementVector.y() );
                m_Parent->getDialog()->setMin(m_Parent->getDialog()->getMin() - adjustedMovement);
                m_Parent->getDialog()->setMax(m_Parent->getDialog()->getMax() - adjustedMovement);}

                break;
            case 1: //zoom
                {auto min = m_Parent->getDialog()->getMin();
                auto max = m_Parent->getDialog()->getMax();

                m_Parent->getDialog()->setMin({(max.real() - min.real()) / width() * pressPos.x() + min.real(), (max.imag() - min.imag()) / height() * pressPos.y() + min.imag()});

                m_Parent->getDialog()->setMax({(max.real() - min.real()) / width() * event->pos().x() + min.real(), (max.imag() - min.imag()) / height() * event->pos().y() + min.imag()});}
                break;
            case 2: //newton

                break;
        }
        m_Parent->buttonNew();
    }

    void mouseMoveEvent(QMouseEvent *event)
    {
        m_Parent->retrace(event->pos());
    }


};

#endif // COMPLEXVIEW_H
