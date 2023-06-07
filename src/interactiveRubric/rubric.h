#ifndef RUBRIC_H
#define RUBRIC_H

#include <QWidget>

#include <QRegExp>
#include <QRegExpValidator>
#include <QString>
#include <QVBoxLayout>
#include <QVector>

#include "row.h"

namespace Ui {
class Rubric;
}

class Rubric : public QWidget
{
    Q_OBJECT

public:
    explicit Rubric(QWidget *parent = nullptr);
    ~Rubric();

    void addRow(Row* row);

    Row* getMinRow();
    Row* getMaxRow();

    void reset();

    QString getName() const;
    void setName(const QString& name);

    QVector<Row*>* getRows() const;
    void setRows(QVector<Row*>* rows);

    double getMark() const;
    void setMark(const double& mark);

    double getWeight() const;
    void setWeight(const double& weight);

private slots:
    void on_pushButtonAddRow_clicked();

    void on_lineEditName_editingFinished();

    void on_lineEditMark_editingFinished();

    void on_lineEditWeight_editingFinished();

    void calculateMark();

private:
    Ui::Rubric *ui;

    QString* name;

    QVector<Row*>* rows;

    double mark;
    double weight;

    QVBoxLayout* vLayoutRows;

    QRegExpValidator* markValidator;

    QRegExpValidator* weightValidator;

signals:
    void rubricChanged();
};

#endif // RUBRIC_H
