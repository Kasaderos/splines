#ifndef LABEL_H
#define LABEL_H

#include <QLabel>
#include <QPaintEvent>
#include <QMouseEvent>

class Label : public QLabel
{
    Q_OBJECT

    QPixmap pix;

public:
    QVector <int> X, Y;
    int n;
    Label(QWidget *parent = 0);
    void draw(QVector<double> &x1, QVector<double> &y1);
    void sweep(const QVector <double> &a,
                   const QVector <double> &b,
                   const QVector <double> &c,
                   const QVector <double> &f,
                   QVector <double> &z);
    void calculate(const QVector <int> &Z,
                       QVector <double> &A,
                       QVector <double> &B,
                       QVector <double> &C,
                       QVector <double> &D);
    void mousePressEvent(QMouseEvent *ev);
signals:

public slots:
};


#endif // LABEL_H
