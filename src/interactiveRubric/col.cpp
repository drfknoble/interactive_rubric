#include "col.h"
#include "ui_col.h"

Col::Col(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Col)
{
    ui->setupUi(this);

    heading = new QString {};
    subHeading = new QString {};
    body = new QString {};
}

Col::~Col()
{
    delete ui;
}

QString Col::getHeading() const
{
    return *heading;
}

void Col::setHeading(const QString &heading)
{
    *this->heading = heading;
    ui->lineEditHeading->setText(heading);
}

QString Col::getSubHeading() const
{
    return *subHeading;
}

void Col::setSubHeading(const QString &subHeading)
{
    *this->subHeading = subHeading;
    ui->lineEditSubHeading->setText(subHeading);
}

QString Col::getBody() const
{
    return *body;
}

void Col::setBody(const QString &body)
{
    *this->body = body;
    ui->plainTextEditBody->setPlainText(body);
}

void Col::on_lineEditHeading_editingFinished()
{
    *heading = ui->lineEditHeading->text();
}

void Col::on_lineEditSubHeading_editingFinished()
{
    *subHeading = ui->lineEditSubHeading->text();
}

void Col::on_plainTextEditBody_textChanged()
{
    *body = ui->plainTextEditBody->toPlainText();
}
