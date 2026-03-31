#include "filegenerator.h"

#include <data/error.h>

#include <xlsxdocument.h>

FileGenerator::~FileGenerator()
{
}

QXlsx::Document *FileGenerator::openFile(const QString &fileName, Error *error)
{
    QXlsx::Document *doc = new QXlsx::Document(fileName + ".xlsx");
    if (!doc->load() && error != nullptr) {
        //
    }
    return doc;
}

void FileGenerator::closeFile(QXlsx::Document *document)
{
    //document->save();
    delete document;
}
