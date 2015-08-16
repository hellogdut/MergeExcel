#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTextStream>
#include <QVector>
#include <QFile>
#include <QPair>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
#include <QProcess>
#include <QDesktopServices>
#include <algorithm>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tempFolder = QDir(QDir::current().filePath("xxx_temp"));

}

MainWindow::~MainWindow()
{
    QFileInfo html_file(html_path);
    if(html_file.exists())
        QFile::remove(html_path);
    delete ui;
}

void MainWindow::runVbsFinished()
{
    EnumExcel(tempFolder.path());
    writeToHtml();
    tempFolder = QDir(QDir::current().filePath("xxx_temp"));
    html_path = QDir::current().filePath("1.html");
    QFile file(html_path);
    if (!file.open(QIODevice::ReadWrite))
        ;
    QTextStream out(&file);
    out << result;
    file.close();
    QDesktopServices::openUrl(QUrl(html_path));
    if(tempFolder.exists())
        tempFolder.removeRecursively();
    qDebug() << result;
    ui->pushButton_3->setEnabled(true);


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
       // qDebug() << "xx : " + vec[i][0];
        if(vec[i][0].indexOf("问题、风险\n/总结分享") == 0)
        {
            break;
        }

        person.thisWeek.push_back(qMakePair(vec[i][1],vec[i][2]));
        i++;
    }
    /* 底栏的问题 */
    if(i < n && vec[i][0].indexOf("问题、风险\n/总结分享") == 0)
    {
        while(i < n)
        {
            if(vec[i][1].isEmpty())
                break;
            person.share.push_back(vec[i][1]);
            i++;
        }
    }

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
    QDateTime time = QDateTime::currentDateTime();// 获取系统现在的时间
    QString day = time.toString("yyyyMMdd"); // 设置显示格式
    QDir dir;
    QDir today(QDir(dir.currentPath()).filePath(day));
    QStringList names = {"3D麻将","斗地主","斗牛","公共"};
    for(int i = 0;i < names.size();++i)
    {
        QDir folder(today.filePath(names[i]));
        if(!folder.exists())
        {
            qDebug() << folder.path();
            dir.mkpath(folder.path());
        }

    }


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
                <th></th>\
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
        {
            last_week += s.first + "</br>";
            if(!s.second.trimmed().isEmpty())
                share += s.second + "</br>";
        }
        QString feed_back = "";
        for(auto s : person.thisWeek)
        {
            this_week += s.first + "</br>";
            if(!s.second.trimmed().isEmpty())
                feed_back += s.second + "</br>";
        }
        if(!feed_back.trimmed().isEmpty() && !share.trimmed().isEmpty())
            share = share +  "---------------------------------------" + "</br>" + feed_back;
        feed_back = "";
        for(auto s : person.share)
        {
            if(!s.trimmed().isEmpty())
               feed_back += s + "</br>";
        }
        if(!feed_back.trimmed().isEmpty() && !share.trimmed().isEmpty())
            share = share +  "---------------------------------------" + "</br>" + feed_back;

        body += "\t<tr>\n";
        body += "\t\t<td>" + (index == 0 ? (g.name + "( " + QString::number(g.members.count()) + "人" +  " )") : " ") + "</td>\n";
        body += "\t\t<td>" + person.name + "</td>\n";
        body += "\t\t<td>" + last_week + "</td>\n";
        body += "\t\t<td>" + this_week + "</td>\n";
        body += "\t\t<td>" + share + "</td></tr>\n";
        index++;
     }
    }
    QString end = "\n</table><br><br></body></html>";
    QString html = css + table_head + body + end;
    //qDebug() << html;
    //ui->textEdit->setPlainText(html);
    result = html;
}


void MainWindow::on_pushButton_2_clicked()
{
    if(tempFolder.exists())
        tempFolder.removeRecursively();
    tempFolder.mkpath(tempFolder.path());

    QFile f1(tempFolder.filePath("1.vbs"));
    if(!f1.open(QIODevice::WriteOnly))
    {
        qDebug() << "can't open " << tempFolder.filePath("1.vbs");
        //return;
    }
    QFile f2(":/1.vbs");
    if(!f2.open(QIODevice::ReadOnly))
    {
        qDebug() << "can't open" << "qrc:/1.vbs";
        return;
    }
    QTextStream in(&f2);
    QTextStream out(&f1);
    out << in.readAll();
    f1.close();
    f2.close();

    QString path = ui->textEdit->toPlainText();
    copyDirectoryFiles(path,tempFolder.path(),true);

    QProcess* proc = new QProcess(this);
    connect(proc,SIGNAL(aboutToClose()),SLOT(runVbsFinished()));
    QStringList param;
    QString vbs_path = tempFolder.filePath("1.vbs");
    param  << "/c" << vbs_path << tempFolder.path();

    //proc->setWorkingDirectory(tempFolder.path());
    proc->start("cmd.exe",param);
    proc->waitForFinished();
   // proc->waitForFinished();
    //ui->textEdit->setPlainText(QString::fromLocal8Bit(proc->readAllStandardOutput()));
    runVbsFinished();
}

bool MainWindow::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{

    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if(fromDir.isEmpty() || !sourceDir.exists())
        return false;
    if(!targetDir.exists()){    /** 如果目标目录不存在，则进行创建 */
        if(!targetDir.mkdir(targetDir.path()))
        {
            qDebug() << "mkdir " + targetDir.filePath(sourceDir.dirName()) + " failed";
        }
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();

    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir())
        {   /**< 当为目录时，递归的进行 copy */
            qDebug() << fileInfo.filePath() << "-> " << targetDir.filePath(fileInfo.fileName());
            copyDirectoryFiles(fileInfo.filePath(),targetDir.filePath(fileInfo.fileName()),coverFileIfExist);
        }
        else
        {
            // 同步文件
            QFileInfo fileA(fileInfo.filePath());
            QFileInfo fileB(targetDir.filePath(fileInfo.fileName()));
            if(fileB.exists())
                QFile::remove(fileB.filePath());
            if(!QFile::copy(fileA.filePath(),fileB.filePath()))
                qDebug() << "can't copy" << fileA.filePath() << " to " << fileB.filePath();
        }
    }
    return true;
}

void MainWindow::on_textEdit_textChanged()
{
    QString url = ui->textEdit->toPlainText();
    bool rst = QDir(url).exists() && !url.isEmpty();
    ui->pushButton_2->setEnabled(rst);
}

void MainWindow::on_pushButton_3_clicked()
{
    QVector<QString> vec;
    for(auto s : Group::allMemberNames)
        vec.push_back(s);
    std::sort(vec.begin(),vec.end());

    QString str;
    int j = 1;
    for(int i = 0;i < vec.size();++i)
    {
        str += vec[i].leftJustified(12,' ') + "\t\t";
        if(j % 3 == 0)
            str += "\n";
        j++;
    }
    QMessageBox::information(0, "值周表", str);

}
