#ifndef CUSTOMEDIT_H
#define CUSTOMEDIT_H

#include <QWidget>
#include <QTextEdit>
class CustomEdit : public QTextEdit
{
public:
    CustomEdit(QWidget* parent = 0);

    ~CustomEdit();
public:
    virtual void dragEnterEvent(QDragEnterEvent * e);
    virtual void dropEvent(QDropEvent * e);
};

#endif // CUSTOMEDIT_H
