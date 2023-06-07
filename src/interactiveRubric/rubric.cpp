#include "rubric.h"
#include "ui_rubric.h"

Rubric::Rubric(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Rubric),
    mark {0},
    weight {1.0}
{
    ui->setupUi(this);

    name = new QString {};
    *name = "Rubric";

    rows = new QVector<Row*> {};

    vLayoutRows = new QVBoxLayout {};
    vLayoutRows->setAlignment(Qt::AlignTop);

    markValidator = new QRegExpValidator {};
    markValidator->setRegExp(QRegExp {QString {"([0-9]{1,3})?(.[0-9]{1,2})"}});
    ui->lineEditMark->setValidator(markValidator);
    ui->lineEditMark->setText(QString::number(mark));

    weightValidator = new QRegExpValidator {};
    weightValidator->setRegExp(QRegExp {QString {"([0-9]{1,3})?(.[0-9]{1,2})"}});
    ui->lineEditWeight->setValidator(weightValidator);
    ui->lineEditWeight->setText(QString::number(weight));
}

Rubric::~Rubric()
{
    delete ui;
}

void Rubric::addRow(Row* row)
{
    rows->push_back(row);
    this->connect(row, &Row::rowChanged, this, &Rubric::calculateMark);

    vLayoutRows->addWidget(row);

    ui->widget->setLayout(vLayoutRows);
}

Row* Rubric::getMinRow()
{
    double min {1000};
    Row* minRow {nullptr};

    for (int i {0}; i < rows->size(); i++)
    {
        double mark {rows->at(i)->getMark()};
        if (mark < min)
        {
            min = mark;
            minRow = rows->at(i);
        }
    }

    return minRow;
}

Row* Rubric::getMaxRow()
{
    double max {-1000};
    Row* maxRow {nullptr};

    for (int i {0}; i < rows->size(); i++)
    {
        double mark {rows->at(i)->getMark()};
        if (mark > max)
        {
            max = mark;
            maxRow = rows->at(i);
        }
    }

    return maxRow;
}

void Rubric::reset()
{
    for (int i {0}; i < rows->size(); i++)
    {
        rows->at(i)->reset();
    }
}

QString Rubric::getName() const
{
    return *name;
}

void Rubric::setName(const QString &name)
{
    *this->name = name;
    ui->lineEditName->setText(name);
}

QVector<Row*>* Rubric::getRows() const
{
    return rows;
}

void Rubric::setRows(QVector<Row *> *rows)
{
    this->rows = rows;
}

double Rubric::getMark() const
{
    return mark;
}

void Rubric::setMark(const double& mark)
{
    this->mark = mark;

    ui->lineEditMark->setText(QString::number(mark));

    emit rubricChanged();
}

double Rubric::getWeight() const
{
    return weight;
}

void Rubric::setWeight(const double& weight)
{
    this->weight = weight;

    ui->lineEditWeight->setText(QString::number(weight));

    emit rubricChanged();
}

void Rubric::on_pushButtonAddRow_clicked()
{
    Row* row = new Row {};

    rows->push_back(row);
    this->connect(row, &Row::rowChanged, this, &Rubric::calculateMark);

    vLayoutRows->addWidget(row);

    ui->widget->setLayout(vLayoutRows);
}

void Rubric::on_lineEditName_editingFinished()
{
    *name = ui->lineEditName->text();
}

void Rubric::on_lineEditMark_editingFinished()
{
    mark = ui->lineEditMark->text().toDouble();

    emit rubricChanged();
}

void Rubric::on_lineEditWeight_editingFinished()
{
    weight = ui->lineEditWeight->text().toDouble();

    emit rubricChanged();
}

void Rubric::calculateMark()
{
    double sum {};

    for (int i {0}; i < rows->size(); i++)
    {
        double m {rows->at(i)->getMark()};
        double w {rows->at(i)->getWeight()};

        sum += m * w;
    }

    mark = sum;

    ui->lineEditMark->setText(QString::number(mark));

    emit rubricChanged();
}
