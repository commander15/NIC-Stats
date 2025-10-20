#ifndef ENVELOPPEGENERATOR_H
#define ENVELOPPEGENERATOR_H

#include <generators/filegenerator.h>

#include <data/enveloppe.h>

#include <QMap>

class EnveloppeGenerator : public FileGenerator
{
public:
    EnveloppeGenerator();

    void generate(const QString &folder, const Enveloppe &enveloppe, Error *error = nullptr);

private:
    int generateKitBasedTable(const QRegularExpression &kit, const Enveloppe &enveloppe, QXlsx::Document *doc, int startRow = 1);

    QString kitName(const QString &enrollmentNumber, const Enveloppe &enveloppe) const;

    QMap<QString, QString> m_kits;
};

#endif // ENVELOPPEGENERATOR_H
