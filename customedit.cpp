#include "customedit.h"
#include <QUrl>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QFileInfo>
#include <QDebug>
CustomEdit::CustomEdit(QWidget* parent)
    :QTextEdit(parent)
{

}

CustomEdit::~CustomEdit()
{

}

void CustomEdit::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

void CustomEdit::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    for(auto url : urls)
    {
        QString fileName = url.toLocalFile();
        if(fileName.startsWith("file:///"))  // OSX
        {
            fileName = fileName.mid(7,fileName.length() - 7);
        }
        else // Windows
        {
            QFileInfo file(fileName);
            if(file.exists() && file.isSymLink())
            {
                fileName = file.symLinkTarget();
            }
        }
        if (fileName.isEmpty())
            return;
        this->append(fileName);
    }
}

