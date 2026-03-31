#ifndef READER_H
#define READER_H

#include <QString>

namespace QXlsx {
class Document;
}

class Error;

class Reader
{
public:
    virtual ~Reader();

protected:
    QXlsx::Document *openFile(const QString &fileName, Error *error = nullptr);
    void closeFile(QXlsx::Document *document, bool autoSave = true);
};

#endif // READER_H
