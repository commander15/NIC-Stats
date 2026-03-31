#include "error.h"

#include <utility>

class ErrorData : public QSharedData
{
public:
    int code = 0;
    QString string;
};

Error::Error(int code, const QString &str)
    : data(new ErrorData)
{
    data->code = code;
    data->string = str;
}

Error::Error(const Error &rhs)
    : data{rhs.data}
{}

Error::Error(Error &&rhs)
    : data{std::move(rhs.data)}
{}

Error &Error::operator=(const Error &rhs)
{
    if (this != &rhs)
        data = rhs.data;
    return *this;
}

Error &Error::operator=(Error &&rhs)
{
    if (this != &rhs)
        data = std::move(rhs.data);
    return *this;
}

Error::~Error() {}

int Error::code() const
{
    return data->code;
}

QString Error::errorString() const
{
    return data->string;
}
