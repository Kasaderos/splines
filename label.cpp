#include "label.h"

#include <QPainter>

Label::Label(QWidget *parent) :
    QLabel(parent)
{
    n = 0;
    pix = QPixmap(500, 500);
}

void Label :: draw(QVector<double> &x1, QVector<double> &y1)
{
    pix.fill(Qt::white);
    QPainter painter(&pix);
    painter.setBrush(Qt::blue);
    for (int i = 0; i < X.size(); i++) {
        painter.drawEllipse(X[i] - 5, Y[i] - 5, 10, 10);
    }
    painter.setBrush(Qt::black);
    for (int i = 1; i < x1.size(); i++)
        painter.drawLine(x1[i-1], y1[i-1], x1[i], y1[i]);

    setPixmap(pix);
}

void Label :: mousePressEvent(QMouseEvent *ev)
{
    double xx = ev->x();// - this->pos().x();
    double yy = ev->y();// - this->pos().y();
    X.push_back(xx);
    Y.push_back(yy);
    QVector <double> x1, y1;
    n++;
    if (n == 1) { draw(x1, y1); return; }
    QVector< double > Ax(n), Bx(n), Cx(n), Dx(n);
    QVector< double > Ay(n), By(n), Cy(n), Dy(n);
    calculate(X, Ax, Bx, Cx, Dx);
    calculate(Y, Ay, By, Cy, Dy);
    for (int i = 0; i < n-1; i++) {
        for (double t = 0.0; t < 1.0; t += 0.001) {
            x1.push_back(Ax[i] *(t) * (t) * (t)
                        + Bx[i]*(t) * (t)
                        + Cx[i] * (t) + Dx[i]);
            y1.push_back(Ay[i] *(t) * (t) * (t)
                        + By[i]*(t) * (t)
                        + Cy[i] * (t) + Dy[i]);
        }

    }
    draw(x1, y1);
}

void Label :: calculate(const QVector<int> &Z,
                             QVector<double> &A,
                             QVector<double> &B,
                             QVector<double> &C,
                             QVector<double> &D)
{
    QVector <double> a(this->n), b(this->n), c(this->n), f(this->n);
    int n = Z.size() - 1;

    for (int i = 0; i < n; i++) {
            a[i] = 1;
            b[i] = 4;
            c[i] = 1;
    }
    c[0] = 0;
    for (int i = 1; i < n; i++) {
        f[i] = 3 * (Z[i - 1] - 2 * Z[i] + Z[i + 1]);
    }
    sweep(a, b, c, f, B);
    D[0] = Z[0];
    B[0] = 0;
    A[0] = (B[1] - B[0]) / 3;
    //C[0] = Z[1] - Z[0] - A[0] - B[0];
    C[0] = (B[0] - B[1]) / 3 + Z[1] - Z[0];
    for (int i = 1 ; i < n; i++) {
        D[i] = Z[i];
        A[i] = (B[i + 1] - B[i]) / 3;
        C[i] = (2 * B[i] + B[i - 1]) / 3
                + (Z[i] - Z[i - 1]);
    }

}

void Label :: sweep(const QVector<double> &a,
                         const QVector<double> &b,
                         const QVector<double> &c,
                         const QVector<double> &f,
                         QVector<double> &z)
{
    int i, n = f.size();
    double coef;
    QVector <double> alpha(n), beta(n);
    alpha[0] = 0;
    beta[0] = 0;
    for (i = 1; i < n - 1; i++) {
        coef = a[i] * alpha[i - 1] + b[i];
        alpha[i] = (1 / coef) * (-c[i]);
        beta[i] = (1 / coef) * (f[i] - a[i]
                                * beta[i - 1]);
     }
    if (n > 2)
        z[n - 1] = (f[n - 1] - a[n - 2] * beta[n - 2])
             / (a[n - 2] * alpha[n - 2] + b[n - 1]);
    else
        z[1] = 0;
    for (i = n - 2; i > -1; i--) {
        z[i] = alpha[i] * z[i + 1] + beta[i];
    }
}


