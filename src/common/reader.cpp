#include "reader.h"

#include <common/error.h>

#include <xlsxdocument.h>

Reader::~Reader()
{
}

QXlsx::Document *Reader::openFile(const QString &fileName, Error *error)
{
    QXlsx::Document *doc = new QXlsx::Document(fileName);

    if (!doc->load()) {
        if (error != nullptr)
            *error = Error(1, "Can't open file for reading");
        delete doc;
        return nullptr;
    }

    if (error != nullptr)
        *error = Error(0, "No error");

    return doc;
}

void Reader::closeFile(QXlsx::Document *document, bool autoSave)
{
    if (autoSave && document->property("write").toBool())
        document->save();
    delete document;
}
