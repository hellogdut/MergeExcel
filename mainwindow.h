#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <person.h>
#include <QMap>
#include <QTextStream>
#include <QStringList>
#include <QDir>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void runVbsFinished();
private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

    void on_textEdit_textChanged();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
private:
    bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
    Person getPersonInfo(QString path);
    void EnumExcel(QString path);
    void writeToHtml();

    QDir tempFolder;
    QString vbs;
    QString result;
    QString html_path;
private:
    QMap<QString,Group> groups;

};

#endif // MAINWINDOW_H
