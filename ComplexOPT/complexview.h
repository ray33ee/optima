#ifndef COMPLEXVIEW_H
#define COMPLEXVIEW_H

#include <QtGui>
#include <QGraphicsView>

#include "mainwindow.h"
#include "complex.h"

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

                auto adjustedMovement =  Complex((m_Parent->getDialog()->getMax().real() - m_Parent->getDialog()->getMin().real()) / width() * movementVector.x(), (m_Parent->getDialog()->getMin().imag() - m_Parent->getDialog()->getMax().imag()) / height() * movementVector.y() );
                m_Parent->getDialog()->setMin(m_Parent->getDialog()->getMin() - adjustedMovement);
                m_Parent->getDialog()->setMax(m_Parent->getDialog()->getMax() - adjustedMovement);}

                break;
            case 1: //zoom
                {auto min = Complex(m_Parent->getDialog()->getMin().real(), m_Parent->getDialog()->getMax().imag());
                auto max = Complex(m_Parent->getDialog()->getMax().real(), m_Parent->getDialog()->getMin().imag());

                auto newMin = Complex((max.real() - min.real()) / width() * pressPos.x() + min.real(), (max.imag() - min.imag()) / height() * pressPos.y() + min.imag());
                auto newMax = Complex((max.real() - min.real()) / width() * event->pos().x() + min.real(), (max.imag() - min.imag()) / height() * event->pos().y() + min.imag());

                m_Parent->getDialog()->setMin(Complex(newMin.real() < newMax.real() ? newMin.real() : newMax.real(), newMin.imag() < newMax.imag() ? newMin.imag() : newMax.imag()));

                m_Parent->getDialog()->setMax(Complex(newMin.real() > newMax.real() ? newMin.real() : newMax.real(), newMin.imag() > newMax.imag() ? newMin.imag() : newMax.imag()));}
                break;
            case 2: //newton
                {
                    auto min = Complex(m_Parent->getDialog()->getMin().real(), m_Parent->getDialog()->getMax().imag());

                    auto max = Complex(m_Parent->getDialog()->getMax().real(), m_Parent->getDialog()->getMin().imag());

                    auto diff = max - min;

                    Complex z = {
                            min.real() + diff.real() * (pressPos.x()) / width(),
                            min.imag() + diff.imag() * (pressPos.y()) / height()
                        };

                    m_Parent->find_root(z);


                }
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
