#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <person.h>
#include <QMap>
#include <QTextStream>
#include <QStringList>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_pushButton_clicked();


private:
    Ui::MainWindow *ui;
private:
    Person getPersonInfo(QString path);
    void EnumExcel(QString path);
    void writeToHtml();
private:
    QMap<QString,Group> groups;

};

#endif // MAINWINDOW_H
