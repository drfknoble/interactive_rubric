#include "row.h"
#include "ui_row.h"

Row::Row(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Row),
    mark {50.0},
    weight {1.0}
{
    ui->setupUi(this);

    name = new QString {};
    *name = "Row";

    cols = new QVector<Col*> {};

    hLayoutCols = new QHBoxLayout {};
    hLayoutCols->setAlignment(Qt::AlignLeft);
    hLayoutCols->setMargin(0);
    hLayoutCols->setSpacing(0);

    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(100);
    ui->horizontalSlider->setValue(static_cast<int>(mark));

    markValidator = new QRegExpValidator {};
    markValidator->setRegExp(QRegExp {QString {"([0-9]{1,3})?(.[0-9]{0,2})"}});
    ui->lineEditMark->setValidator(markValidator);
    ui->lineEditMark->setText(QString::number(mark));

    weightValidator = new QRegExpValidator {};
    weightValidator->setRegExp(QRegExp {QString {"([0-9]{1,3})?(.[0-9]{0,2})"}});
    ui->lineEditWeight->setValidator(weightValidator);
    ui->lineEditWeight->setText(QString::number(weight));
}

Row::~Row()
{
    delete ui;
}

QString Row::getName() const
{
    return *name;
}

void Row::setName(const QString& name)
{
    *this->name = name;
    ui->lineEditName->setText(name);
}

void Row::addCol(Col *col)
{
    cols->push_back(col);

    hLayoutCols->addWidget(col);

    ui->widget->setLayout(hLayoutCols);
}

void Row::reset()
{
//    ui->horizontalSlider->setValue(50);
    setMark(50.0);
}

QVector<Col*>* Row::getCols() const
{
    return cols;
}

void Row::setCols(QVector<Col *> *cols)
{
    this->cols = cols;
}

double Row::getMark() const
{
    return mark;
}

void Row::setMark(const double& mark)
{  
    this->mark = std::round(mark * 10) / 10;

    ui->lineEditMark->setText(QString::number(this->mark));
    ui->horizontalSlider->setValue(static_cast<int>(this->mark));

    emit rowChanged();
}

double Row::getWeight() const
{
    return weight;
}

void Row::setWeight(const double& weight)
{
    this->weight = weight;

    ui->lineEditWeight->setText(QString::number(weight));

    emit rowChanged();
}

void Row::on_pushButtonAddCol_clicked()
{
    Col* col = new Col {};

    cols->push_back(col);

    hLayoutCols->addWidget(col);

    ui->widget->setLayout(hLayoutCols);
}

void Row::on_horizontalSlider_sliderMoved(int position)
{
    mark = static_cast<double>(position);

    mark = std::round(mark * 10) / 10; // 1DP

    ui->lineEditMark->setText(QString::number(mark));

    emit rowChanged();
}

void Row::on_lineEditName_editingFinished()
{
    *name = ui->lineEditName->text();
}

void Row::on_lineEditMark_editingFinished()
{
    mark = ui->lineEditMark->text().toDouble();

    mark = std::round(mark * 10) / 10; // 1DP

    ui->lineEditMark->setText(QString::number(mark, 'f', 1));

    ui->horizontalSlider->setValue(static_cast<int>(mark));

    emit rowChanged();
}

void Row::on_lineEditWeight_editingFinished()
{
    weight = ui->lineEditWeight->text().toDouble();

    emit rowChanged();
}
