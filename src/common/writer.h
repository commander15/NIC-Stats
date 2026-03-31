#ifndef WRITER_H
#define WRITER_H

#include <QString>
#include <QIODevice>

namespace QXlsx {
class Document;
}

class Error;

class Writer
{
public:
    virtual ~Writer();

protected:
    QXlsx::Document *openFile(const QString &fileName, QIODevice::OpenMode mode, Error *error = nullptr);
    void closeFile(QXlsx::Document *document, bool autoSave = true);

    void deleteAllSheets(QXlsx::Document *document);
};

#endif // WRITER_H
