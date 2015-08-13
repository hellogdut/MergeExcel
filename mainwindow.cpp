#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTextStream>
#include <QVector>
#include <QFile>
#include <QPair>
#include <QDir>
#include <QFileInfo>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QString path = "file:///Users/milu/Desktop/欢乐前台开发组周报_aaronwang_20150801.txt ";
    //QString path = "C:\\Users\\milu\\Desktop\\周报\\20150803(大周会)\\斗地主\\2.txt";
    //getPersonInfo(path);
}

MainWindow::~MainWindow()
{
    delete ui;
}

Person MainWindow::getPersonInfo(QString path)
{
    Person person;
    QVector<QStringList> vec;
    QFile file(path);
    if (!file.open(QIODevice::ReadWrite))
        qDebug() << "Cannot open file : " + path;
    QTextStream in(&file);
    QString text = in.readAll();
    text = text.replace("\"","");
    QStringList lines = text.split("\r\n");
    for(auto line : lines)
    {
        //qDebug() << "line:" + line;
        QStringList tabs = line.split('\t');
        QString s;
        for(auto i:tabs)
            s += "+" + i;
        //qDebug() << s;
        if(tabs.size() < 4)
            continue;

        QString work = tabs[1];
        QString plus = tabs[3];
        QStringList wk = work.split('\n');
        QStringList ps = plus.split('\n');
        int i = 0;
        int j = 0;
        bool firstLine = true;
        while(1)
        {
            QStringList list;
            QString w = i < wk.size() ? wk[i] : "";
            QString p = j < ps.size() ? ps[i] : "";
            if(i >= wk.size() && j >= ps.size())
                break;
            if(firstLine)
            {
                list.push_back(tabs[0]);
                firstLine = false;
            }
            else
            {
                list.push_back(QString());
            }
            list.push_back(w);
            list.push_back(p);
            vec.push_back(list);
            i++;
            j++;
        }
    }
    for(QVector<QStringList>::iterator it = vec.begin();it != vec.end();)
    {
        if((*it)[0].isEmpty() && (*it)[1].isEmpty() && (*it)[2].isEmpty())
            it = vec.erase(it);
        else
            ++it;
    }

    int i = 0;
    int n = vec.size();
    bool lastWeek = false;
    while(i < n)
    {
        if(!lastWeek && vec[i][0] != "上周工作")
        {
            i++;
            continue;
        }
        else
            lastWeek = true;
        if(vec[i][0] == "本周计划")
        {
            i++;
            break;
        }
        if(vec[i][0] != "上周工作")
        {
            person.lastWeek.push_back(qMakePair(vec[i][1],vec[i][2]));
        }

        i++;
    }
    while(i < n)
    {
        qDebug() << "xx : " + vec[i][0];
        if(vec[i][0].indexOf("\"问题") == 0)
        {
            break;
        }

        person.thisWeek.push_back(qMakePair(vec[i][1],vec[i][2]));
        i++;
    }
    /* 底栏的问题 */
    if(i < n && vec[i][0].indexOf("xx") == 0)
        {
        return person;
        }

//    qDebug() << "lastWeek";
//    for(auto p:person.lastWeek)
//    {
//        qDebug() << p.first << p.second;
//    }
//    qDebug() << "thisWeek";
//    for(auto p:person.thisWeek)
//    {
//        qDebug() << p.first << p.second;
//    }

    return person;
}

void MainWindow::EnumExcel(QString path)
{
    QDir targetDir(path);
    qDebug() << path;
    QFileInfoList folders = targetDir.entryInfoList();

    for(auto folder : folders)
    {

        if(folder.fileName() == "." || folder.fileName() == "..")
            continue;
        QDir folderDir(folder.filePath());
        QString groupName = folderDir.dirName();
        groups[groupName].name = groupName;

        QFileInfoList files = folderDir.entryInfoList();
        for(auto file : files)
        {
            if(file.suffix() != "txt")
                continue;
            Person person = getPersonInfo(file.filePath());

            auto i = file.fileName().indexOf("_");
            auto j = file.fileName().indexOf("_",i+1);
            QString personName = file.fileName().mid(i + 1,j - i - 1);
            person.name = personName;
            groups[groupName].memberNames.insert(personName);
            groups[groupName].members.push_back(person);
            Group::allMemberNames.insert(personName);
        }

    }
}



void MainWindow::on_pushButton_clicked()
{
    QString path = ui->textEdit->toPlainText();
    EnumExcel(path);
    writeToHtml();
//    qDebug() << "person Count:" << Group::allMemberNames.count();
//    for(auto g : groups)
//    {
//        qDebug() << g.name;
//        for(auto person:g.members)
//        {

//            qDebug() << person.name;
//            qDebug() << "lastWeek";
//            for(auto last : person.lastWeek)
//            {
//                qDebug() << last.first << last.second;
//            }
//            qDebug() << "thisWeek";
//            for(auto tw:person.thisWeek)
//            {
//                qDebug() << tw.first << tw.second;
//            }
//        }
//    }

}
//void MainWindow::writeToHtml()
//{
//    QString css = "<!DOCTYPE html><html><head><title>Practical CSS3 tables with rounded corners - demo</title><style>body { margin: 40px auto;    font-family: 'trebuchet MS', 'Lucida sans', Arial;    font-size: 14px;    color: #444;}table {    *border-collapse: collapse;     border-spacing: 0;    width: 100%;    }.bordered {    border: solid #ccc 1px;    -moz-border-radius: 6px;    -webkit-border-radius: 6px;    border-radius: 6px;    -webkit-box-shadow: 0 1px 1px #ccc;     -moz-box-shadow: 0 1px 1px #ccc;     box-shadow: 0 1px 1px #ccc;         }.bordered tr:hover {    background: #fbf8e9;    -o-transition: all 0.1s ease-in-out;    -webkit-transition: all 0.1s ease-in-out;    -moz-transition: all 0.1s ease-in-out;    -ms-transition: all 0.1s ease-in-out;    transition: all 0.1s ease-in-out;     }        .bordered td, .bordered th {    max-width: 300px;border-left: 1px solid #ccc;    border-top: 1px solid #ccc;    padding: 10px;    text-align: left;    }.bordered th {    background-color: #dce9f9;    background-image: -webkit-gradient(linear, left top, left bottom, from(#ebf3fc), to(#dce9f9));    background-image: -webkit-linear-gradient(top, #ebf3fc, #dce9f9);    background-image:    -moz-linear-gradient(top, #ebf3fc, #dce9f9);    background-image:     -ms-linear-gradient(top, #ebf3fc, #dce9f9);    background-image:      -o-linear-gradient(top, #ebf3fc, #dce9f9);    background-image:         linear-gradient(top, #ebf3fc, #dce9f9);    -webkit-box-shadow: 0 1px 0 rgba(255,255,255,.8) inset;     -moz-box-shadow:0 1px 0 rgba(255,255,255,.8) inset;      box-shadow: 0 1px 0 rgba(255,255,255,.8) inset;            border-top: none;    text-shadow: 0 1px 0 rgba(255,255,255,.5); }.bordered td:first-child, .bordered th:first-child {    border-left: none;}.bordered th:first-child {    -moz-border-radius: 6px 0 0 0;    -webkit-border-radius: 6px 0 0 0;    border-radius: 6px 0 0 0;}.bordered th:last-child {    -moz-border-radius: 0 6px 0 0;    -webkit-border-radius: 0 6px 0 0;    border-radius: 0 6px 0 0;}.bordered th:only-child{    -moz-border-radius: 6px 6px 0 0;    -webkit-border-radius: 6px 6px 0 0;    border-radius: 6px 6px 0 0;}.bordered tr:last-child td:first-child {    -moz-border-radius: 0 0 0 6px;    -webkit-border-radius: 0 0 0 6px;    border-radius: 0 0 0 6px;}.bordered tr:last-child td:last-child {    -moz-border-radius: 0 0 6px 0;    -webkit-border-radius: 0 0 6px 0;    border-radius: 0 0 6px 0;}.zebra td, .zebra th {    padding: 10px;    border-bottom: 1px solid #f2f2f2;    }.zebra tbody tr:nth-child(even) {    background: #f5f5f5;    -webkit-box-shadow: 0 1px 0 rgba(255,255,255,.8) inset;     -moz-box-shadow:0 1px 0 rgba(255,255,255,.8) inset;      box-shadow: 0 1px 0 rgba(255,255,255,.8) inset;        }.zebra th {    text-align: left;    text-shadow: 0 1px 0 rgba(255,255,255,.5);     border-bottom: 1px solid #ccc;    background-color: #eee;    background-image: -webkit-gradient(linear, left top, left bottom, from(#f5f5f5), to(#eee));    background-image: -webkit-linear-gradient(top, #f5f5f5, #eee);    background-image:    -moz-linear-gradient(top, #f5f5f5, #eee);    background-image:     -ms-linear-gradient(top, #f5f5f5, #eee);    background-image:      -o-linear-gradient(top, #f5f5f5, #eee);     background-image:         linear-gradient(top, #f5f5f5, #eee);}.zebra th:first-child {    -moz-border-radius: 6px 0 0 0;    -webkit-border-radius: 6px 0 0 0;    border-radius: 6px 0 0 0;  }.zebra th:last-child {    -moz-border-radius: 0 6px 0 0;    -webkit-border-radius: 0 6px 0 0;    border-radius: 0 6px 0 0;}.zebra th:only-child{    -moz-border-radius: 6px 6px 0 0;    -webkit-border-radius: 6px 6px 0 0;    border-radius: 6px 6px 0 0;}.zebra tfoot td {    border-bottom: 0;    border-top: 1px solid #fff;    background-color: #f1f1f1;  }.zebra tfoot td:first-child {    -moz-border-radius: 0 0 0 6px;    -webkit-border-radius: 0 0 0 6px;    border-radius: 0 0 0 6px;}.zebra tfoot td:last-child {    -moz-border-radius: 0 0 6px 0;    -webkit-border-radius: 0 0 6px 0;    border-radius: 0 0 6px 0;}.zebra tfoot td:only-child{    -moz-border-radius: 0 0 6px 6px;    -webkit-border-radius: 0 0 6px 6px    border-radius: 0 0 6px 6px}</style></head><body>";
//    QString table_head = "<table class='bordered'><thead><tr>\
//                <th>项目组</th>\
//                <th>姓名</th>\
//                <th>上周工作重点</th>\
//                <th>本周工作重点</th>\
//                <th>问题&风险</th>\</thead>\n";
//    QString body = "";

//    for(auto g:groups)
//    {
//        bool begin_of_group = true;
//        for(auto person:g.members)
//        {
//            if(person.name == "gerrysun")
//            {
//                break;
//            }
//            int n_last = person.lastWeek.size();
//            int n_this = person.thisWeek.size();
//            int n_share = person.share.size();
//            int i = 0;
//            int j = 0;
//            int k = 0;
//            while(i < n_last || j < n_this || k < n_share)
//             {
//                body += "\t<tr>\n";
//                body += "\t\t<td>" + (begin_of_group ? g.name : " ") + "</td>\n";
//                body += "\t\t<td>" + (i == 0 ? person.name : " ") + "</td>\n";
//                body += "\t\t<td>" + (i < n_last ? person.lastWeek[i].first : " ") + "</td>\n";
//                body += "\t\t<td>" + (j < n_this ? person.thisWeek[j].first : " ") + "</td>\n";
//                body += "\t\t<td>" + (k < n_share ? person.share[k] : " ") + "</td></tr>\n";
//                i++;
//                j++;
//                k++;
//                if(begin_of_group)
//                    begin_of_group = false;
//             }
//        }
//    }
//    QString end = "\n</table><br><br></body></html>";
//    QString html = css + table_head + body + end;
//    qDebug() << html;

//}
void MainWindow::writeToHtml()
{
    QString css = "<!DOCTYPE html><html><head><title>Practical CSS3 tables with rounded corners - demo</title><style>body { margin: 40px auto;    font-family: 'trebuchet MS', 'Lucida sans', Arial;    font-size: 14px;    color: #444;}table {    *border-collapse: collapse;     border-spacing: 0;    width: 100%;    }.bordered {    border: solid #ccc 1px;    -moz-border-radius: 6px;    -webkit-border-radius: 6px;    border-radius: 6px;    -webkit-box-shadow: 0 1px 1px #ccc;     -moz-box-shadow: 0 1px 1px #ccc;     box-shadow: 0 1px 1px #ccc;         }.bordered tr:hover {    background: #fbf8e9;    -o-transition: all 0.1s ease-in-out;    -webkit-transition: all 0.1s ease-in-out;    -moz-transition: all 0.1s ease-in-out;    -ms-transition: all 0.1s ease-in-out;    transition: all 0.1s ease-in-out;     }        .bordered td, .bordered th {    max-width: 300px;border-left: 1px solid #ccc;    border-top: 1px solid #ccc;    padding: 10px;    text-align: left;    }.bordered th {    background-color: #dce9f9;    background-image: -webkit-gradient(linear, left top, left bottom, from(#ebf3fc), to(#dce9f9));    background-image: -webkit-linear-gradient(top, #ebf3fc, #dce9f9);    background-image:    -moz-linear-gradient(top, #ebf3fc, #dce9f9);    background-image:     -ms-linear-gradient(top, #ebf3fc, #dce9f9);    background-image:      -o-linear-gradient(top, #ebf3fc, #dce9f9);    background-image:         linear-gradient(top, #ebf3fc, #dce9f9);    -webkit-box-shadow: 0 1px 0 rgba(255,255,255,.8) inset;     -moz-box-shadow:0 1px 0 rgba(255,255,255,.8) inset;      box-shadow: 0 1px 0 rgba(255,255,255,.8) inset;            border-top: none;    text-shadow: 0 1px 0 rgba(255,255,255,.5); }.bordered td:first-child, .bordered th:first-child {    border-left: none;}.bordered th:first-child {    -moz-border-radius: 6px 0 0 0;    -webkit-border-radius: 6px 0 0 0;    border-radius: 6px 0 0 0;}.bordered th:last-child {    -moz-border-radius: 0 6px 0 0;    -webkit-border-radius: 0 6px 0 0;    border-radius: 0 6px 0 0;}.bordered th:only-child{    -moz-border-radius: 6px 6px 0 0;    -webkit-border-radius: 6px 6px 0 0;    border-radius: 6px 6px 0 0;}.bordered tr:last-child td:first-child {    -moz-border-radius: 0 0 0 6px;    -webkit-border-radius: 0 0 0 6px;    border-radius: 0 0 0 6px;}.bordered tr:last-child td:last-child {    -moz-border-radius: 0 0 6px 0;    -webkit-border-radius: 0 0 6px 0;    border-radius: 0 0 6px 0;}.zebra td, .zebra th {    padding: 10px;    border-bottom: 1px solid #f2f2f2;    }.zebra tbody tr:nth-child(even) {    background: #f5f5f5;    -webkit-box-shadow: 0 1px 0 rgba(255,255,255,.8) inset;     -moz-box-shadow:0 1px 0 rgba(255,255,255,.8) inset;      box-shadow: 0 1px 0 rgba(255,255,255,.8) inset;        }.zebra th {    text-align: left;    text-shadow: 0 1px 0 rgba(255,255,255,.5);     border-bottom: 1px solid #ccc;    background-color: #eee;    background-image: -webkit-gradient(linear, left top, left bottom, from(#f5f5f5), to(#eee));    background-image: -webkit-linear-gradient(top, #f5f5f5, #eee);    background-image:    -moz-linear-gradient(top, #f5f5f5, #eee);    background-image:     -ms-linear-gradient(top, #f5f5f5, #eee);    background-image:      -o-linear-gradient(top, #f5f5f5, #eee);     background-image:         linear-gradient(top, #f5f5f5, #eee);}.zebra th:first-child {    -moz-border-radius: 6px 0 0 0;    -webkit-border-radius: 6px 0 0 0;    border-radius: 6px 0 0 0;  }.zebra th:last-child {    -moz-border-radius: 0 6px 0 0;    -webkit-border-radius: 0 6px 0 0;    border-radius: 0 6px 0 0;}.zebra th:only-child{    -moz-border-radius: 6px 6px 0 0;    -webkit-border-radius: 6px 6px 0 0;    border-radius: 6px 6px 0 0;}.zebra tfoot td {    border-bottom: 0;    border-top: 1px solid #fff;    background-color: #f1f1f1;  }.zebra tfoot td:first-child {    -moz-border-radius: 0 0 0 6px;    -webkit-border-radius: 0 0 0 6px;    border-radius: 0 0 0 6px;}.zebra tfoot td:last-child {    -moz-border-radius: 0 0 6px 0;    -webkit-border-radius: 0 0 6px 0;    border-radius: 0 0 6px 0;}.zebra tfoot td:only-child{    -moz-border-radius: 0 0 6px 6px;    -webkit-border-radius: 0 0 6px 6px    border-radius: 0 0 6px 6px}</style></head><body>";
    QString table_head = "<table class='bordered'><thead><tr>\
                <th>项目组</th>\
                <th>姓名</th>\
                <th>上周工作重点</th>\
                <th>本周工作重点</th>\
                <th>问题&风险</th>\</thead>\n";
    QString body = "";
    for(auto g:groups)
    {
     bool index = 0;
     for(auto person:g.members)
     {

        QString last_week = "";
        QString this_week = "";
        QString share = "";
        for(auto s : person.lastWeek)
            last_week += s.first + "</br>";
        for(auto s : person.thisWeek)
            this_week += s.first + "</br>";
        for(auto s : person.share)
            share += s + "</br>";
        body += "\t<tr>\n";
        body += "\t\t<td>" + (index == 0 ? g.name : " ") + "</td>\n";
        body += "\t\t<td>" + person.name + "</td>\n";
        body += "\t\t<td>" + last_week + "</td>\n";
        body += "\t\t<td>" + this_week + "</td>\n";
        body += "\t\t<td>" + share + "</td></tr>\n";
        index++;
     }
    }
    QString end = "\n</table><br><br></body></html>";
    QString html = css + table_head + body + end;
    qDebug() << html;
}
