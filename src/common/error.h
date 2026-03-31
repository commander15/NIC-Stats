#ifndef ERROR_H
#define ERROR_H

#include <QSharedDataPointer>

class ErrorData;

class Error
{
public:
    Error(int code = 0, const QString &str = QString());
    Error(const Error &);
    Error(Error &&);
    Error &operator=(const Error &);
    Error &operator=(Error &&);
    ~Error();

    int code() const;
    QString errorString() const;

private:
    QSharedDataPointer<ErrorData> data;
};

#endif // ERROR_H
