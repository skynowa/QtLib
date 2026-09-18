#include "Utils.h"

#include <QTest>
#include <QTimeZone>

class TestHumanFormats : public QObject
{
    Q_OBJECT

private slots:
    void fileSizeHuman();
    void dateTimeHuman();
    void dateTimeHumanDetailed();
};

void
TestHumanFormats::fileSizeHuman()
{
    const QString bytes = qtlib::Utils::fileSizeHuman(999);
    const QString kilobytes = qtlib::Utils::fileSizeHuman(1500);

    QCOMPARE(bytes, QStringLiteral("999 B"));
    QCOMPARE(kilobytes, QStringLiteral("1.5 KB"));
}

void
TestHumanFormats::dateTimeHuman()
{
    const QDateTime dateTime(
        QDate(2026, 9, 18), QTime(20, 15, 47), QTimeZone::systemTimeZone());
    const QString formatted = qtlib::Utils::dateTimeHuman(dateTime);
    const QString invalid = qtlib::Utils::dateTimeHuman({});

    QCOMPARE(formatted, QStringLiteral("2026-sep-18 20:15"));
    QVERIFY(invalid.isEmpty());
}

void
TestHumanFormats::dateTimeHumanDetailed()
{
    const QDateTime dateTime(
        QDate(2026, 9, 18), QTime(20, 15, 47), QTimeZone::systemTimeZone());
    const QString formatted = qtlib::Utils::dateTimeHumanDetailed(dateTime);
    const QString expected = QStringLiteral("2026-sep-18 20:15:47 %1")
        .arg(dateTime.timeZoneAbbreviation());
    const QString invalid = qtlib::Utils::dateTimeHumanDetailed({});

    QCOMPARE(formatted, expected);
    QVERIFY(invalid.isEmpty());
}

QTEST_MAIN(TestHumanFormats)

#include "TestHumanFormats.moc"
