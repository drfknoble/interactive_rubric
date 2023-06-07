#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , finalMark {0.0}
{
    ui->setupUi(this);

    studentName = new QString {};
    studentID = new QString {};

    rubrics = new QVector<Rubric*> {};

    vLayoutRubrics = new QVBoxLayout {};
    vLayoutRubrics->setAlignment(Qt::AlignTop);
    vLayoutRubrics->setMargin(0);
    vLayoutRubrics->setSpacing(0);

    comment = new QString {};
    ui->textEdit->setPlainText(*comment);

    report = new QString {};
    reportFile = new QFile {};
    reportFileName = new QString {};

    studentNameValidator = new QRegExpValidator {};
    studentNameValidator->setRegExp(QRegExp {QString {"[A-Za-z ]*"}});
    ui->lineEditStudentName->setValidator(studentNameValidator);
    ui->lineEditStudentName->setText(*studentName);

    studentIDValidator = new QRegExpValidator {};
    studentIDValidator->setRegExp(QRegExp {QString {"[0-9]*"}});
    ui->lineEditStudentID->setValidator(studentIDValidator);
    ui->lineEditStudentID->setText(*studentID);

    finalMarkValidator = new QRegExpValidator {};
    finalMarkValidator->setRegExp(QRegExp {QString {"([0-9]{1,3})?(.[0-9]{1,2})"}});
    ui->lineEditFinalMark->setValidator(finalMarkValidator);
    ui->lineEditFinalMark->setText(QString::number(finalMark));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addRubric(Rubric *rubric)
{
    rubrics->push_back(rubric);
    this->connect(rubric, &Rubric::rubricChanged, this, &MainWindow::calculateMark);

    qDebug() << rubrics->size();

    vLayoutRubrics->addWidget(rubric);

    ui->scrollAreaWidgetContents->setLayout(vLayoutRubrics);
}

void MainWindow::on_actionNew_triggered()
{
    QLayoutItem* child;
    while ((child = (vLayoutRubrics)->takeAt(0)) != nullptr)
    {
        child->widget()->setParent(nullptr);
        vLayoutRubrics->removeWidget(child->widget());
        delete child;
    }

    delete rubrics;

    rubrics = new QVector<Rubric*> {};
}

void MainWindow::on_actionOpen_triggered()
{
    // Get file's name

    QString cwd {QDir::currentPath()};

    QString fileName {QFileDialog::getOpenFileName(this, QString {"Open File"}, cwd, QString {"Json (*.json)"})};

    if (fileName.isEmpty())
    {
        return;
    }

    // Clear widgets

    QLayoutItem* child;
    while ((child = (vLayoutRubrics)->takeAt(0)) != nullptr)
    {
        child->widget()->setParent(nullptr);
        vLayoutRubrics->removeWidget(child->widget());
        delete child;
    }

    delete rubrics;

    rubrics = new QVector<Rubric*> {};

    // Read file

    QFile file {fileName};

    file.open(QIODevice::ReadOnly);

    if(!file.isOpen())
        return;

    QString data {};
    QTextStream stream {&data};

    stream << file.readAll();

    file.close();

    // Parse JSON data

    QJsonParseError parseError;
    QJsonDocument doc {QJsonDocument::fromJson(data.toUtf8(), &parseError)};

    if (parseError.error != QJsonParseError::NoError)
    {
        qDebug() << "Parse error: " << parseError.errorString();
        return;
    }

    if (!doc.isObject())
    {
        qDebug() << "Document does not contain array";
        return;
    }

    QJsonObject docObj {doc.object()};

    QJsonArray rubricsArray {docObj.value("rubrics")};

    for(const auto& rubric : rubricsArray.at(0).toArray())
    {
        QString rubricName {rubric.toObject().value("name").toString()};
        double rubricWeight {rubric.toObject().value("weight").toDouble()};

        Rubric* newRubric = new Rubric {};
        newRubric->setName(rubricName);
        newRubric->setWeight(rubricWeight);

        addRubric(newRubric);

        QJsonArray rowsArray {rubric.toObject().value("rows").toArray()};

        for (const auto& row : rowsArray.at(0).toArray())
        {
            QString rowName {row.toObject().value("name").toString()};
            double rowWeight {row.toObject().value("weight").toDouble()};

            Row* newRow = new Row {};
            newRow->setName(rowName);
            newRow->setWeight(rowWeight);

            newRubric->addRow(newRow);

            QJsonArray colsArray {row.toObject().value("cols").toArray()};

            for (const auto& col : colsArray.at(0).toArray())
            {
                QString heading {col.toObject().value("heading").toString()};
                QString subHeading {col.toObject().value("subHeading").toString()};
                QString body {col.toObject().value("body").toString()};

                Col* newCol = new Col {};

                newCol->setHeading(heading);
                newCol->setSubHeading(subHeading);
                newCol->setBody(body);

                newRow->addCol(newCol);
            }
        }
    }
}


void MainWindow::on_actionSave_triggered()
{
    QString cwd {QDir::currentPath()};

    QString fileName {QFileDialog::getSaveFileName(this, QString {"Save File"}, cwd, QString {"Json (*.json)"})};

    if (fileName.isEmpty())
    {
        return;
    }

    QFile file {fileName};

    file.open(QIODevice::WriteOnly);

    if(!file.isOpen())
        return;

    // Save Rubric

    QJsonDocument doc {};

    QJsonObject docObj {};

    qDebug() << rubrics->size();

    QJsonArray rubricsArray {};

    for (int i {0}; i < rubrics->size(); i++)
    {
        QJsonObject rubric {};

        rubric.insert("name", rubrics->at(i)->getName());
        rubric.insert("weight", rubrics->at(i)->getWeight());

        QJsonArray rowsArray {};

        auto* rows = rubrics->at(i)->getRows();

        for(int j {0}; j < rows->size(); j++)
        {
            QJsonObject row {};

            row.insert("name", rows->at(j)->getName());
            row.insert("weight", rows->at(j)->getWeight());

            auto* cols = rows->at(j)->getCols();

            QJsonArray colsArray {};

            for (int k {0}; k < cols->size(); k++)
            {
                QJsonObject col {};

                col.insert("heading", cols->at(k)->getHeading());
                col.insert("subHeading", cols->at(k)->getSubHeading());
                col.insert("body", cols->at(k)->getBody());

                colsArray.append(col);
            }

            row.insert("cols", colsArray);

            rowsArray.append(row);

        }

        rubric.insert("rows", rowsArray);

        rubricsArray.append(rubric);
    }

    docObj.insert("rubrics", rubricsArray);

    doc.setObject(docObj);

    file.write(doc.toJson());

    file.close();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_lineEditStudentName_editingFinished()
{
    *studentName = ui->lineEditStudentName->text();
}

void MainWindow::on_lineEditStudentID_editingFinished()
{
    *studentID = ui->lineEditStudentID->text();
}

void MainWindow::on_pushButtonAddRubric_clicked()
{
    Rubric* rubric = new Rubric {};

    rubrics->push_back(rubric);
    this->connect(rubric, &Rubric::rubricChanged, this, &MainWindow::calculateMark);

    qDebug() << rubrics->size();

    vLayoutRubrics->addWidget(rubric);

    ui->scrollAreaWidgetContents->setLayout(vLayoutRubrics);

}

void MainWindow::calculateMark()
{
    double sum {0};

    for (int i {0}; i < rubrics->size(); i++)
    {
        double m {rubrics->at(i)->getMark()};
        double w {rubrics->at(i)->getWeight()};

        sum += m * w;
    }

    finalMark = std::round(sum * 10.0) / 10.0; // 1DP

    ui->lineEditFinalMark->setText(QString::number(finalMark, 'f', 1));
}


void MainWindow::on_pushButtonReset_clicked()
{
    *studentName = QString {};
    ui->lineEditStudentName->setText(*studentName);

    *studentID = QString {};
    ui->lineEditStudentID->setText(*studentID);

    *comment = QString {};
    ui->textEdit->setPlainText(*comment);

    for (int i {0}; i < rubrics->size(); i++)
    {
        rubrics->at(i)->reset();
    }

}

void MainWindow::on_pushButtonGenerate_clicked()
{
    *report = QString {};

    QTextStream stream {report};

    stream << "Dear" << " " << *studentName << ",";
    stream << Qt::endl;
    stream << "Thank you for your submission. ";

    stream << "Your final mark is " << QString::number(finalMark, 'f', 1) << ". ";

    if (finalMark <= 49.99)
    {
        stream << "Overall, your submission did not meet expectations.";
        stream << Qt::endl;
    } else if ((finalMark >= 50.00) && (finalMark <= 64.99))
    {
        stream << "Overall, your submission was below expectations.";
        stream << Qt::endl;
    } else if ((finalMark >= 65.00) && (finalMark <= 79.99))
    {
        stream << "Overall, your submission met expectations. Good work.";
        stream << Qt::endl;
    } else if ((finalMark >= 80.00) && (finalMark <= 100.00))
    {
        stream << "Overall, your submission exceeded expectations. Great work!";
        stream << Qt::endl;
    }

    for (int i {0}; i < rubrics->size(); i++)
    {
        Rubric* rubric {rubrics->at(i)};

        stream << "With regard to the " << (rubric->getName()).toLower() << " rubric: ";

        auto* rows = rubric->getRows();

        for (int j {0}; j < rows->size(); j++)
        {

            Row* row = rows->at(j);

            stream << "your " << row->getName() << " section ";

            double mark = row->getMark();

            if (mark <= 49.99)
            {
                stream << "did not meet expectations";
                stream << " (" << mark << ")";
            } else if ((mark >= 50.00) && (mark <= 64.99))
            {
                stream << "was below expectations";
                stream << " (" << mark << ")";
            } else if ((mark >= 65.00) && (mark <= 79.99))
            {
                stream << "met expectations";
                stream << " (" << mark << ")";
            } else if ((mark >= 80.00) && (mark <= 100.00))
            {
                stream << "exceeded expectations";
                stream << " (" << mark << ")";
            }

            if (j < (rows->size() - 1))
                stream << "; ";
            else
            {
                stream << ".";
                stream << Qt::endl;
            }


        }

        if (rows->size() > 2)
        {

            Row* minRow {rubric->getMinRow()};
            Row* maxRow {rubric->getMaxRow()};

            if ((minRow != nullptr) && (maxRow != nullptr) && (minRow != maxRow))
            {
                stream << "Your best section was the ";
                stream << (maxRow)->getName() << " section" << " (" << (maxRow)->getMark() << "). ";
                stream << "In the future, try and improve your ";
                stream << (minRow)->getName() << " section" << " (" << (minRow)->getMark() << ").";
                stream << Qt::endl;
            }

        }

    }

    stream << "Best regards, ";
    stream << Qt::endl;
    stream << "Frazer.";
    stream << Qt::endl;

    if (comment->size() > 0)
    {
        stream << "Also: ";
        stream << *comment;
        stream << Qt::endl;
    }

    stream << Qt::endl;

    QMessageBox::information(this, "Report", *report);

}

void MainWindow::on_textEdit_textChanged()
{
    *comment = ui->textEdit->toPlainText();
}

void MainWindow::on_pushButtonExport_clicked()
{
    if (!reportFile->isOpen())
    {
        QString fileName {QFileDialog::getSaveFileName(this, QString {"Save File"}, QDir::currentPath(), QString {"Text (*.txt)"})};
        reportFile->setFileName(fileName);
    }

    reportFile->open(QIODevice::Append);

    if (!reportFile->isOpen())
        return;

    QTextStream stream {reportFile};

    stream << *report;

    reportFile->close();
}

void MainWindow::on_lineEditFinalMark_editingFinished()
{
    finalMark = ui->lineEditFinalMark->text().toDouble();

    finalMark = std::round(finalMark * 10) / 10; // 1DP

    ui->lineEditFinalMark->setText(QString::number(finalMark));
}
