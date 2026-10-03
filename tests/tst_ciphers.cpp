#include <QtTest>
#include <string>

#include "ciphers.h"
#include "tools.h"

// tools.cpp reaches into gcalc.cpp for these two. Linking gcalc.cpp in would
// pull most of the program behind it, so the test binary supplies its own. No
// cipher calculation touches either: if one ever did, these would make it fail
// loudly here rather than quietly read a real user's settings.
QVariant loadsettings(QString)
{
    return QVariant();
}

QString Qtotable(QString str, int, int, int, int)
{
    return str;
}

// The globals tools.cpp expects. The cipher engine does not read them, but the
// translation unit will not link without them.
QString phrase, labeltext, tmpstring, filesource, appgroup;
int year = 0, dd = 0, mm = 0, ns = 0, d2 = 0, m2 = 0, y2 = 0, filter = 0, hmempos = 0;
bool single_r_on = false, francis_on = false, satanic_on = false, jewish_on = false;
bool sumerian_on = false, rev_sumerian_on = false, fibonacci_on = false;
bool nightmode = false;
int lunar_filter = 0;
int zerodays[8][250] = {{0}}, lunardays[8][500] = {{0}}, linenumbers = 0;
QString hmem[10];
std::vector<int> primes;

class CipherTests : public QObject
{
    Q_OBJECT

private slots:

    // ---- the three ciphers given letter by letter ------------------------
    //
    // Checked against the specification as written, not against whatever the
    // code happens to produce. Each one is spelled out because a cipher that is
    // quietly wrong does not crash: it produces plausible numbers that are
    // wrong everywhere downstream, for as long as nobody notices.

    void reverseSatanic_data()
    {
        QTest::addColumn<QString>("letter");
        QTest::addColumn<int>("expected");

        QTest::newRow("A") << "A" << 61;
        QTest::newRow("B") << "B" << 60;
        QTest::newRow("M") << "M" << 49;
        QTest::newRow("N") << "N" << 48;
        QTest::newRow("Y") << "Y" << 37;
        QTest::newRow("Z") << "Z" << 36;
    }

    void reverseSatanic()
    {
        QFETCH(QString, letter);
        QFETCH(int, expected);

        QCOMPARE(ciphers::value(letter, ciphers::ReverseSatanic), expected);
    }

    void reverseSatanicIsSixtyTwoMinusOrdinal()
    {
        // The table is the statement of intent; this is the proof that the
        // twenty-six numbers written out in ciphers.cpp contain no typo.
        for (char c = 'A'; c <= 'Z'; ++c) {
            const int ordinal = c - 'A' + 1;
            QCOMPARE(ciphers::value(std::string(1, c), ciphers::ReverseSatanic), 62 - ordinal);
        }
    }

    void primes_data()
    {
        QTest::addColumn<QString>("letter");
        QTest::addColumn<int>("expected");

        QTest::newRow("A") << "A" << 2;
        QTest::newRow("B") << "B" << 3;
        QTest::newRow("C") << "C" << 5;
        QTest::newRow("E") << "E" << 11;
        QTest::newRow("M") << "M" << 41;
        QTest::newRow("T") << "T" << 71;
        QTest::newRow("Z") << "Z" << 101;
    }

    void primes()
    {
        QFETCH(QString, letter);
        QFETCH(int, expected);

        QCOMPARE(ciphers::value(letter, ciphers::Primes), expected);
    }

    void primesAddUp()
    {
        QCOMPARE(ciphers::value(QString("ABC"), ciphers::Primes), 10);   // 2 + 3 + 5
    }

    void primesAreActuallyPrime()
    {
        for (char c = 'A'; c <= 'Z'; ++c) {
            const int v = ciphers::value(std::string(1, c), ciphers::Primes);

            QVERIFY2(v > 1, qPrintable(QString("%1 = %2").arg(c).arg(v)));

            for (int d = 2; d * d <= v; ++d)
                QVERIFY2(v % d != 0, qPrintable(QString("%1 = %2 is divisible by %3").arg(c).arg(v).arg(d)));
        }
    }

    void chaldean_data()
    {
        QTest::addColumn<QString>("letter");
        QTest::addColumn<int>("expected");

        QTest::newRow("A") << "A" << 1;
        QTest::newRow("F") << "F" << 8;
        QTest::newRow("G") << "G" << 3;
        QTest::newRow("O") << "O" << 7;
        QTest::newRow("U") << "U" << 6;
        QTest::newRow("W") << "W" << 6;
        QTest::newRow("X") << "X" << 5;
        QTest::newRow("Z") << "Z" << 7;
    }

    void chaldean()
    {
        QFETCH(QString, letter);
        QFETCH(int, expected);

        QCOMPARE(ciphers::value(letter, ciphers::Chaldean), expected);
    }

    void chaldeanIsNotARepeatingPattern()
    {
        // The whole instruction about this cipher was not to replace it with a
        // generic 1-8 cycle. G would be 7 under such a cycle and is 3 here;
        // nothing is ever 9.
        QCOMPARE(ciphers::value(QString("G"), ciphers::Chaldean), 3);
        QCOMPARE(ciphers::value(QString("X"), ciphers::Chaldean), 5);

        for (char c = 'A'; c <= 'Z'; ++c)
            QVERIFY(ciphers::value(std::string(1, c), ciphers::Chaldean) != 9);
    }

    void chaldeanWholeWords()
    {
        // ECLIPSE: 5 + 3 + 3 + 1 + 8 + 3 + 5
        QCOMPARE(ciphers::value(QString("ECLIPSE"), ciphers::Chaldean), 28);

        // WASHINGTON: 6 + 1 + 3 + 5 + 1 + 5 + 3 + 4 + 7 + 5
        QCOMPARE(ciphers::value(QString("WASHINGTON"), ciphers::Chaldean), 40);
    }

    // ---- text handling ---------------------------------------------------

    void caseIsIgnored()
    {
        const int ids[] = { ciphers::ReverseSatanic, ciphers::Primes, ciphers::Chaldean,
                            ciphers::Scrabble, ciphers::Qwerty, ciphers::BuildingValue };

        for (size_t i = 0; i < sizeof(ids) / sizeof(ids[0]); ++i) {
            QCOMPARE(ciphers::value(QString("eclipse"), ids[i]),
                     ciphers::value(QString("ECLIPSE"), ids[i]));
            QCOMPARE(ciphers::value(QString("EcLiPsE"), ids[i]),
                     ciphers::value(QString("ECLIPSE"), ids[i]));
        }
    }

    void punctuationAndSpacesCarryNoValue()
    {
        QCOMPARE(ciphers::value(QString("ab c"), ciphers::Primes),
                 ciphers::value(QString("abc"), ciphers::Primes));
        QCOMPARE(ciphers::value(QString("a,b.c!"), ciphers::Primes),
                 ciphers::value(QString("abc"), ciphers::Primes));
        QCOMPARE(ciphers::value(QString("  abc  "), ciphers::Chaldean),
                 ciphers::value(QString("abc"), ciphers::Chaldean));
    }

    void emptyInputIsZero()
    {
        for (size_t i = 0; i < ciphers::all().size(); ++i)
            QCOMPARE(ciphers::value(QString(""), ciphers::all()[i].id), 0);
    }

    void digitsCarryNoValueInTheNewCiphers()
    {
        // The original engine adds the face value of digits 1-9, and keeps doing
        // so. A Scrabble set has no tile for "7", so the new ciphers skip them.
        QCOMPARE(ciphers::value(QString("abc7"), ciphers::Scrabble),
                 ciphers::value(QString("abc"), ciphers::Scrabble));
    }

    // ---- the added-but-unsurprising ciphers ------------------------------

    void scrabble()
    {
        QCOMPARE(ciphers::value(QString("A"), ciphers::Scrabble), 1);
        QCOMPARE(ciphers::value(QString("Q"), ciphers::Scrabble), 10);
        QCOMPARE(ciphers::value(QString("Z"), ciphers::Scrabble), 10);
        QCOMPARE(ciphers::value(QString("QUIZ"), ciphers::Scrabble), 22);  // 10+1+1+10
    }

    void qwertyFollowsTheKeyboardRows()
    {
        const QString rows = "QWERTYUIOPASDFGHJKLZXCVBNM";

        for (int i = 0; i < rows.size(); ++i)
            QCOMPARE(ciphers::value(QString(rows.at(i)), ciphers::Qwerty), i + 1);
    }

    void buildingValueSumsTheRunningTotals()
    {
        // A, A+B, A+B+C = 1 + 3 + 6
        QCOMPARE(ciphers::value(QString("ABC"), ciphers::BuildingValue), 10);
        QCOMPARE(ciphers::value(QString("A"), ciphers::BuildingValue), 1);

        // Reverse ordinal: Z=1, Y=2, X=3 -> 1 + 3 + 6
        QCOMPARE(ciphers::value(QString("ZYX"), ciphers::ReverseBuildingValue), 10);
    }

    void wordReductionFoldsToOneDigit()
    {
        // WASHINGTON is 130 in English Ordinal
        // (23+1+19+8+9+14+7+20+15+14); 1+3+0 = 4.
        QCOMPARE(ciphers::value(QString("WASHINGTON"), ciphers::EnglishOrdinal), 130);
        QCOMPARE(ciphers::value(QString("WASHINGTON"), ciphers::WordReduction), 4);

        for (int n = 0; n < 200; ++n)
            QVERIFY(ciphers::digitalRoot(n) >= 0 && ciphers::digitalRoot(n) <= 9);
    }

    void digitalRootIsNotTheLetterLevelReduce()
    {
        // tools.cpp's reduce() subtracts 9 or 18 and is only meaningful for one
        // letter. Applied to a word total it returns nonsense, which is why the
        // word-level fold is its own function.
        QCOMPARE(ciphers::digitalRoot(130), 4);
        QVERIFY(reduce(130) != 4);
    }

    // ---- nothing that already existed has moved --------------------------

    void legacyCiphersAreUnchanged()
    {
        // The table must not have altered a single existing number. Checked by
        // delegation rather than by re-implementation: these ids call straight
        // into getwordnumericvalue(), and this asserts the wiring is right for
        // every one of them.
        struct Expect { int id; int reduced; int reversed; int type; };

        const Expect legacy[] = {
            { ciphers::EnglishOrdinal,       0, 0, 0 },
            { ciphers::FullReduction,        1, 0, 0 },
            { ciphers::ReverseOrdinal,       0, 1, 0 },
            { ciphers::ReverseFullReduction, 1, 1, 0 },
            { ciphers::SingleReduction,      0, 0, 1 },
            { ciphers::FrancisBacon,         0, 0, 2 },
            { ciphers::Satanic,              0, 0, 3 },
            { ciphers::Jewish,               0, 0, 4 },
            { ciphers::Sumerian,             0, 0, 5 },
            { ciphers::ReverseSumerian,      0, 1, 5 },
            { ciphers::Fibonacci,            0, 0, 6 },
        };

        const char *words[] = {
            "Washington", "Freddie Mercury", "Propaganda", "Shooting", "Assassin",
            "Murder", "Big One", "Earthquake", "eclipse", "a", "z", "", "The Quick Brown Fox"
        };

        for (size_t w = 0; w < sizeof(words) / sizeof(words[0]); ++w) {
            for (size_t i = 0; i < sizeof(legacy) / sizeof(legacy[0]); ++i) {
                const std::string word(words[w]);
                const int viaTable = ciphers::value(word, legacy[i].id);
                const int viaEngine = getwordnumericvalue(word, legacy[i].reduced,
                                                          legacy[i].reversed, legacy[i].type);

                QVERIFY2(viaTable == viaEngine,
                         qPrintable(QString("%1 of '%2': table %3, engine %4")
                                    .arg(ciphers::name(legacy[i].id))
                                    .arg(words[w]).arg(viaTable).arg(viaEngine)));
            }
        }
    }

    void existingCipherNamesAreUnchanged()
    {
        QCOMPARE(ciphers::name(ciphers::EnglishOrdinal), QString("English Ordinal"));
        QCOMPARE(ciphers::name(ciphers::FullReduction), QString("Full Reduction"));
        QCOMPARE(ciphers::name(ciphers::ReverseOrdinal), QString("Reverse Ordinal"));
        QCOMPARE(ciphers::name(ciphers::ReverseFullReduction), QString("Reverse Full Reduction"));
        QCOMPARE(ciphers::name(ciphers::SingleReduction), QString("Single Reduction"));
        QCOMPARE(ciphers::name(ciphers::FrancisBacon), QString("Francis Bacon"));
        QCOMPARE(ciphers::name(ciphers::Satanic), QString("Satanic"));
        QCOMPARE(ciphers::name(ciphers::Jewish), QString("Jewish"));
        QCOMPARE(ciphers::name(ciphers::Sumerian), QString("Sumerian"));
        QCOMPARE(ciphers::name(ciphers::ReverseSumerian), QString("Reverse Sumerian"));
        QCOMPARE(ciphers::name(ciphers::Fibonacci), QString("Fibonacci"));
    }

    void everyIdIsUniqueAndPresent()
    {
        // The ids reach settings and saved analyses, so a duplicate or a gap
        // would quietly re-point a user's selection at the wrong cipher.
        const std::vector<ciphers::Cipher> &all = ciphers::all();

        QCOMPARE(static_cast<int>(all.size()), static_cast<int>(ciphers::CipherCount));

        for (size_t i = 0; i < all.size(); ++i) {
            QCOMPARE(all[i].id, static_cast<int>(i));
            QVERIFY(ciphers::byId(all[i].id) != nullptr);
            QVERIFY(!ciphers::name(all[i].id).isEmpty());
        }

        QVERIFY(ciphers::byId(-1) == nullptr);
        QVERIFY(ciphers::byId(ciphers::CipherCount) == nullptr);
    }
};

QTEST_APPLESS_MAIN(CipherTests)
#include "tst_ciphers.moc"
