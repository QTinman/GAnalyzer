#include <QtTest>
#include <QSettings>

#include "cipherselection.h"
#include "ciphers.h"

// The selection owns the seven globals the rest of the program reads directly.
// The thing that must not break is the promise that nothing downstream has to
// know that: printword(), findword() and searchwords() go on reading
// single_r_on and the rest, and must see exactly what they would have seen.

extern bool single_r_on, francis_on, satanic_on, jewish_on, sumerian_on,
            rev_sumerian_on, fibonacci_on;

// cipherselection reads this to decide where its settings live.
extern QString appgroup;

namespace {
const char *TestGroup = "GAnalyzerTests";
}

class SelectionTests : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        // Somewhere of this suite's own. Without it these tests would read and
        // then overwrite the real cipher selection of whoever ran them - which
        // is exactly the kind of thing a test should never do to the machine it
        // is running on.
        appgroup = TestGroup;
    }

    void init()
    {
        QSettings store("QTinman", TestGroup);

        store.beginGroup(TestGroup);
        store.remove("ciphers");
        store.endGroup();

        cipherselection::load();
    }

    void theFourOriginalsCannotBeTurnedOff()
    {
        QVERIFY(cipherselection::isAlwaysOn(ciphers::EnglishOrdinal));
        QVERIFY(cipherselection::isAlwaysOn(ciphers::FullReduction));
        QVERIFY(cipherselection::isAlwaysOn(ciphers::ReverseOrdinal));
        QVERIFY(cipherselection::isAlwaysOn(ciphers::ReverseFullReduction));

        QVERIFY(!cipherselection::isAlwaysOn(ciphers::Satanic));
        QVERIFY(!cipherselection::isAlwaysOn(ciphers::Chaldean));

        // Even asked for nothing, they are on.
        cipherselection::setEnabled(QVector<int>());

        QVERIFY(cipherselection::isEnabled(ciphers::EnglishOrdinal));
        QVERIFY(cipherselection::isEnabled(ciphers::ReverseFullReduction));
    }

    void nothingNewIsOnUntilSomebodyAsks()
    {
        // The upgrade promise: a user who has never opened the dialog sees the
        // columns they saw yesterday, not nineteen more.
        for (size_t i = 0; i < ciphers::all().size(); ++i) {
            const int id = ciphers::all()[i].id;

            if (!cipherselection::isAlwaysOn(id))
                QVERIFY2(!cipherselection::isEnabled(id), qPrintable(ciphers::name(id)));
        }
    }

    void theLegacyGlobalsFollowTheSelection()
    {
        cipherselection::setEnabled(QVector<int>() << ciphers::Satanic << ciphers::Fibonacci);

        QVERIFY(satanic_on);
        QVERIFY(fibonacci_on);
        QVERIFY(!single_r_on);
        QVERIFY(!francis_on);
        QVERIFY(!jewish_on);
        QVERIFY(!sumerian_on);
        QVERIFY(!rev_sumerian_on);

        cipherselection::setEnabled(QVector<int>() << ciphers::SingleReduction);

        QVERIFY(single_r_on);
        QVERIFY2(!satanic_on, "a cipher dropped from the selection must switch its global off");
        QVERIFY(!fibonacci_on);
    }

    void aSelectionSurvivesBeingReloaded()
    {
        cipherselection::setEnabled(QVector<int>()
                                    << ciphers::Chaldean << ciphers::Primes << ciphers::Jewish);

        cipherselection::load();

        QVERIFY(cipherselection::isEnabled(ciphers::Chaldean));
        QVERIFY(cipherselection::isEnabled(ciphers::Primes));
        QVERIFY(cipherselection::isEnabled(ciphers::Jewish));
        QVERIFY(jewish_on);

        QVERIFY(!cipherselection::isEnabled(ciphers::Scrabble));
    }

    void theSelectionComesBackInTableOrder()
    {
        // Added in a jumble; the output columns must not reshuffle between runs.
        cipherselection::setEnabled(QVector<int>()
                                    << ciphers::Scrabble << ciphers::Chaldean << ciphers::Primes);

        const QVector<int> enabled = cipherselection::enabled();

        for (int i = 1; i < enabled.size(); ++i)
            QVERIFY(enabled.at(i - 1) < enabled.at(i));
    }

    void anUnknownCipherIdIsIgnored()
    {
        cipherselection::setEnabled(QVector<int>() << ciphers::Chaldean << 9999 << -1);

        QVERIFY(cipherselection::isEnabled(ciphers::Chaldean));
        QVERIFY(!cipherselection::isEnabled(9999));
        QVERIFY(!cipherselection::isEnabled(-1));
    }

    void cleanupTestCase()
    {
        QSettings store("QTinman", TestGroup);

        store.beginGroup(TestGroup);
        store.remove("ciphers");
        store.endGroup();

        appgroup.clear();
    }
};

// No QTEST_APPLESS_MAIN: the suites share one binary. See tests/main.cpp.
int runSelectionTests(int argc, char **argv)
{
    SelectionTests tests;
    return QTest::qExec(&tests, argc, argv);
}

#include "tst_selection.moc"
