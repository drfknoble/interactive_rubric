#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <cmath>

#include <QMainWindow>

#include <QDebug>

#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>
#include <QRegExp>
#include <QRegExpValidator>
#include <QTextStream>
#include <QVBoxLayout>
#include <QVector>

#include "rubric.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addRubric(Rubric* rubric);

private slots:    
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionExit_triggered();

    void on_lineEditStudentName_editingFinished();

    void on_lineEditStudentID_editingFinished();

    void on_pushButtonAddRubric_clicked();

    void calculateMark();

    void on_pushButtonReset_clicked();

    void on_pushButtonGenerate_clicked();

    void on_textEdit_textChanged();

    void on_pushButtonExport_clicked();

    void on_lineEditFinalMark_editingFinished();

private:
    Ui::MainWindow *ui;

    QString* studentName;
    QString* studentID;

    QVector<Rubric*>* rubrics;

    QVBoxLayout* vLayoutRubrics;

    QString* comment;

    double finalMark;

    QString* report;
    QFile* reportFile;
    QString* reportFileName;

    QRegExpValidator* studentNameValidator;

    QRegExpValidator* studentIDValidator;

    QRegExpValidator* finalMarkValidator;
};
#endif // MAINWINDOW_H
