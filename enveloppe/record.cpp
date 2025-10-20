#include "record.h"

class RecordData : public QSharedData
{
public:
    QString personalNumber;
    QString documentNumber;
    QString enrollmentNumber;
    QString name;
    QString firstName;
    QString sex;
    QString birthdate;
};

Record::Record()
    : data(new RecordData)
{
}

Record::Record(const Record &rhs)
    : data{rhs.data}
{}

Record::Record(Record &&rhs)
    : data{std::move(rhs.data)}
{}

Record &Record::operator=(const Record &rhs)
{
    if (this != &rhs)
        data = rhs.data;
    return *this;
}

Record &Record::operator=(Record &&rhs)
{
    if (this != &rhs)
        data = std::move(rhs.data);
    return *this;
}

Record::~Record() {}

QString Record::personalNumber() const
{
    return data->personalNumber;
}

void Record::setPersonalNumber(const QString &personalNumber)
{
    data->personalNumber = personalNumber;
}

QString Record::documentNumber() const
{
    return data->documentNumber;
}

void Record::setDocumentNumber(const QString &documentNumber)
{
    data->documentNumber = documentNumber;
}

QString Record::fullName(const QString &sep) const
{
    if (data->firstName.isEmpty())
        return data->name;
    else
        return data->name + sep + data->firstName;
}

QString Record::sex() const
{
    return data->sex;
}

void Record::setSex(const QString &sex)
{
    data->sex = sex;
}

QString Record::birthdate() const
{
    return data->birthdate;
}

void Record::setBirthdate(const QString &birthdate)
{
    data->birthdate = birthdate;
}

QString Record::enrollmentNumber() const
{
    return data->enrollmentNumber;
}

void Record::setEnrollmentNumber(const QString &enrollmentNumber)
{
    data->enrollmentNumber = enrollmentNumber;
}

QString Record::name() const
{
    return data->name;
}

void Record::setName(const QString &name)
{
    data->name = name;
}

QString Record::firstName() const
{
    return data->firstName;
}

void Record::setFirstName(const QString &firstName)
{
    data->firstName = firstName;
}
