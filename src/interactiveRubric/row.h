#ifndef ROW_H
#define ROW_H

#include <cmath>

#include <QWidget>

#include <QDebug>
#include <QRegExp>
#include <QRegExpValidator>
#include <QString>
#include <QHBoxLayout>
#include <QVector>

#include "col.h"

namespace Ui {
class Row;
}

class Row : public QWidget
{
    Q_OBJECT

public:
    explicit Row(QWidget *parent = nullptr);
    ~Row();

    void addCol(Col* col);

    void reset();

    QString getName() const;
    void setName(const QString& name);

    QVector<Col*>* getCols() const;
    void setCols(QVector<Col*>* cols);

    double getMark() const;
    void setMark(const double& mark);

    double getWeight() const;
    void setWeight(const double& weight);

private slots:
    void on_pushButtonAddCol_clicked();

    void on_lineEditName_editingFinished();

    void on_horizontalSlider_sliderMoved(int position);

    void on_lineEditMark_editingFinished();

    void on_lineEditWeight_editingFinished();


private:
    Ui::Row *ui;

    QString* name;

    QVector<Col*>* cols;

    double mark;
    double weight;

    QHBoxLayout* hLayoutCols;

    QRegExpValidator* markValidator;

    QRegExpValidator* weightValidator;

signals:
    void rowChanged();
};

#endif // ROW_H
