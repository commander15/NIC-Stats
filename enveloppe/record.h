#ifndef RECORD_H
#define RECORD_H

#include <QSharedDataPointer>

class RecordData;

class Record
{
public:
    Record();
    Record(const Record &);
    Record(Record &&);
    Record &operator=(const Record &);
    Record &operator=(Record &&);
    ~Record();

    // personalNumber
    QString personalNumber() const;
    void setPersonalNumber(const QString& personalNumber);

    // documentNumber
    QString documentNumber() const;
    void setDocumentNumber(const QString& documentNumber);

    // enrollmentNumber
    QString enrollmentNumber() const;
    void setEnrollmentNumber(const QString& enrollmentNumber);

    // name
    QString name() const;
    void setName(const QString& name);

    // firstName
    QString firstName() const;
    void setFirstName(const QString& firstName);

    // fullName
    QString fullName(const QString &sep = " ") const;

    // sex
    QString sex() const;
    void setSex(const QString& sex);

    // birthdate
    QString birthdate() const;
    void setBirthdate(const QString& birthdate);

private:
    QSharedDataPointer<RecordData> data;
};

#endif // RECORD_H
