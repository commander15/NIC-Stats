#ifndef FILEGENERATOR_H
#define FILEGENERATOR_H

namespace QXlsx {
class Document;
}

class QString;

class Error;

class FileGenerator
{
public:
    virtual ~FileGenerator();

protected:
    QXlsx::Document *openFile(const QString &fileName, Error *error = nullptr);
    void closeFile(QXlsx::Document *document);
};

#endif // FILEGENERATOR_H
