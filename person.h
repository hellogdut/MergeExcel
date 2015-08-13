#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QVector>
#include <QPair>
#include <QSet>
#include <QObject>
class Person
{
public:
    Person();
    ~Person();
    QString name;
    QVector<QPair<QString,QString>> lastWeek;
    QVector<QPair<QString,QString>> thisWeek;
    QVector<QString> share;
};
class Group
{
public:
    QString name;
    QSet<QString> memberNames;
    QVector<Person> members;
    static QSet<QString> allMemberNames;
};

#endif // PERSON_H
