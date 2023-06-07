#ifndef COL_H
#define COL_H

#include <QWidget>

#include <QString>

namespace Ui {
class Col;
}

class Col : public QWidget
{
    Q_OBJECT

public:
    explicit Col(QWidget *parent = nullptr);
    ~Col();

    QString getHeading() const;
    void setHeading(const QString& heading);

    QString getSubHeading() const;
    void setSubHeading(const QString& subHeading);

    QString getBody() const;
    void setBody(const QString& body);

private slots:
    void on_lineEditHeading_editingFinished();

    void on_lineEditSubHeading_editingFinished();

    void on_plainTextEditBody_textChanged();

private:
    Ui::Col *ui;

    QString* heading;
    QString* subHeading;
    QString* body;
};

#endif // COL_H
