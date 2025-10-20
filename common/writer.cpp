#include "writer.h"

#include <common/error.h>

#include <xlsxdocument.h>

Writer::~Writer()
{
}

QXlsx::Document *Writer::openFile(const QString &fileName, QIODeviceBase::OpenMode mode, Error *error)
{
    QXlsx::Document *doc = new QXlsx::Document(fileName);

    if (mode.testFlag(QIODevice::ReadOnly) && mode.testFlag(QIODevice::WriteOnly)) {
        if (!doc->save()) {
            if (error != nullptr)
                *error = Error(1, "Can't open file for writing");
            delete doc;
            return nullptr;
        } else {
            doc->setProperty("write", true);
        }
    } else if (mode.testFlag(QIODevice::WriteOnly)) {
        //deleteAllSheets(doc);
        if (!doc->save()) {
            if (error != nullptr)
                *error = Error(1, "Can't open file for writing");
            delete doc;
            return nullptr;
        } else {
            doc->setProperty("write", true);
        }
    } else if (mode.testFlag(QIODevice::ReadOnly)) {
        if (!doc->load()) {
            if (error != nullptr)
                *error = Error(2, "Can't open file for reading");
            delete doc;
            return nullptr;
        }
    }

    if (error != nullptr)
        *error = Error(0, "No error");

    return doc;
}

void Writer::closeFile(QXlsx::Document *document, bool autoSave)
{
    if (autoSave && document->property("write").toBool())
        document->save();
    delete document;
}

void Writer::deleteAllSheets(QXlsx::Document *document)
{
    const QStringList sheetNames = document->sheetNames();
    for (const QString &sheet : sheetNames)
        document->deleteSheet(sheet);

    // Cleaning the remaining sheet, cause it don't get deleted (QXlsx bug ?)
    auto dimension = document->dimension();
    for (int i(1); i <= dimension.rowCount(); ++i)
        for (int j(1); j <= dimension.columnCount(); ++j)
            document->write(i, j, QVariant());
}
