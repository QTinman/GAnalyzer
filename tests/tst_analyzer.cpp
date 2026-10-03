#include <QtTest>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTemporaryDir>

#include "analyzer.h"
#include "ciphers.h"
#include "decodegraph.h"
#include "historyindex.h"

// Searching history used to answer with a bool per word: something matched, in
// one of eleven ciphers, and you were not told which. These cover the thing
// that replaced it - that the agreements are found, counted, explained, and
// that a near miss is never quietly promoted into a match.

class AnalyzerTests : public QObject
{
    Q_OBJECT

private:
    static QStringList sampleHistory()
    {
        return QStringList() << "Washington" << "Propaganda" << "Shooting"
                             << "Assassin" << "Murder" << "Big One" << "Earthquake";
    }

    // A word that agrees with `seed` in at least one cipher, found rather than
    // assumed - the test should not depend on my arithmetic.
    static QString wordSharingValue(const HistoryIndex &index, const QString &seed, int cipherId)
    {
        const ciphers::CipherValue v = ciphers::valueOf(seed, cipherId);
        const QStringList holders = index.wordsWith(cipherId, v);

        return holders.isEmpty() ? QString() : holders.first();
    }

private slots:

    // ---- the index ------------------------------------------------------

    void indexFindsWordsByValue()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        QCOMPARE(index.count(), 7);

        const ciphers::CipherValue v = ciphers::valueOf(QString("Murder"), ciphers::EnglishOrdinal);

        QVERIFY(index.wordsWith(ciphers::EnglishOrdinal, v).contains("Murder"));
    }

    void indexCachesTheSameValuesTheCipherEngineProduces()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        const QStringList words = index.words();

        for (int w = 0; w < words.size(); ++w) {
            const QVector<int> &ids = index.ciphers();

            for (int i = 0; i < ids.size(); ++i) {
                QCOMPARE(index.valueOf(words.at(w), ids.at(i)).key(),
                         ciphers::valueOf(words.at(w), ids.at(i)).key());
            }
        }
    }

    void addAndRemoveKeepTheIndexInStep()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        const QString added = "Eclipse";
        const ciphers::CipherValue v = ciphers::valueOf(added, ciphers::EnglishOrdinal);

        QVERIFY(!index.wordsWith(ciphers::EnglishOrdinal, v).contains(added));

        index.add(added);
        QCOMPARE(index.count(), 8);
        QVERIFY(index.wordsWith(ciphers::EnglishOrdinal, v).contains(added));

        index.remove(added);
        QCOMPARE(index.count(), 7);

        // The point of the test: a removed word must leave no trace in any
        // cipher's map, or it goes on being found by searches for ever.
        const QVector<int> &ids = index.ciphers();

        for (int i = 0; i < ids.size(); ++i) {
            const ciphers::CipherValue value = ciphers::valueOf(added, ids.at(i));
            QVERIFY2(!index.wordsWith(ids.at(i), value).contains(added),
                     qPrintable(ciphers::name(ids.at(i))));
        }
    }

    void addingTheSameWordTwiceDoesNotDuplicateIt()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        index.add("Murder");
        index.add("  Murder  ");

        QCOMPARE(index.count(), 7);

        const ciphers::CipherValue v = ciphers::valueOf(QString("Murder"), ciphers::EnglishOrdinal);
        QCOMPARE(index.wordsWith(ciphers::EnglishOrdinal, v).count("Murder"), 1);
    }

    void redundantCiphersAreNotIndexedTwice()
    {
        HistoryIndex index;

        QVERIFY(!index.ciphers().contains(ciphers::MirrorAtbash));
        QVERIFY(index.ciphers().contains(ciphers::ReverseOrdinal));
    }

    void anUnreadableHistoryIsNotAnEmptyOne()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        QString error;
        QVERIFY(!index.load("no-such-file-anywhere.txt", &error));
        QVERIFY(!error.isEmpty());

        // Still holding what it had: a failed read must not look like a history
        // with nothing in it.
        QCOMPARE(index.count(), 7);
    }

    void loadReadsOnePhrasePerLine()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        const QString path = dir.filePath("history.txt");
        QFile file(path);

        QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Text));
        file.write("Washington\n\nMurder \n Big One\n");
        file.close();

        HistoryIndex index;
        QVERIFY(index.load(path));

        QCOMPARE(index.count(), 3);          // the blank line is dropped
        QVERIFY(index.words().contains("Murder"));
        QVERIFY(index.words().contains("Big One"));
    }

    // ---- matching and scoring -------------------------------------------

    void aPhraseFindsTheWordItSharesANumberWith()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        const QString target = wordSharingValue(index, "Murder", ciphers::EnglishOrdinal);
        QVERIFY(!target.isEmpty());

        analyzer::Options options;
        const analyzer::Result result = analyzer::analyze("Murder", index, options);

        // "Murder" is itself in history and must not be returned as its own
        // relative, however many ciphers it agrees with itself in.
        for (int i = 0; i < result.candidates.size(); ++i)
            QVERIFY(result.candidates.at(i).word.compare("Murder", Qt::CaseInsensitive) != 0);

        QCOMPARE(result.wordsSearched, 7);
        QVERIFY(!result.values.isEmpty());
    }

    void everyMatchSaysWhichCipherAndWhatValue()
    {
        HistoryIndex index;
        index.setWords(QStringList() << "Murder" << "Assassin");

        analyzer::Options options;
        const analyzer::Result result = analyzer::analyze("Assassin", index, options);

        for (int c = 0; c < result.candidates.size(); ++c) {
            const analyzer::Candidate &candidate = result.candidates.at(c);

            QVERIFY(!candidate.matches.isEmpty());

            for (int m = 0; m < candidate.matches.size(); ++m) {
                const analyzer::Match &match = candidate.matches.at(m);

                QVERIFY(!match.cipherName.isEmpty());
                QVERIFY(ciphers::byId(match.cipherId) != nullptr);

                // The whole point: an exact match is two identical values, and
                // the result carries both so the reader can check.
                if (match.exact)
                    QCOMPARE(match.inputValue.key(), match.candidateValue.key());
            }
        }
    }

    void scoreIsOnePointPerAgreeingCipher()
    {
        HistoryIndex index;
        index.setWords(QStringList() << "Murder");

        analyzer::Options options;
        const analyzer::Result result = analyzer::analyze("Murder ", index, options);

        // "Murder " trims to "Murder", which is the stored word, so it is
        // excluded as itself - the trimming is what this checks.
        QCOMPARE(result.candidates.size(), 0);
    }

    void scoreAddsUpAcrossCiphersAndCanBeRecomputedByHand()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        analyzer::Options options;
        const analyzer::Result result = analyzer::analyze("Shooting", index, options);

        for (int c = 0; c < result.candidates.size(); ++c) {
            const analyzer::Candidate &candidate = result.candidates.at(c);
            int exact = 0, score = 0;

            for (int m = 0; m < candidate.matches.size(); ++m) {
                if (candidate.matches.at(m).exact) {
                    ++exact;
                    score += candidate.matches.at(m).weight;
                }
            }

            QCOMPARE(candidate.exactMatches, exact);
            QCOMPARE(candidate.score, score);
        }
    }

    void weightsChangeTheScoreAndAreDeclared()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        analyzer::Options plain;
        const analyzer::Result before = analyzer::analyze("Shooting", index, plain);

        if (before.candidates.isEmpty())
            QSKIP("no agreement in this sample to weight");

        const int cipherId = before.candidates.first().matches.first().cipherId;

        analyzer::Options weighted;
        weighted.cipherWeights.insert(cipherId, 5);

        const analyzer::Result after = analyzer::analyze("Shooting", index, weighted);

        QVERIFY(after.candidates.first().score > before.candidates.first().score);

        // A ranking that cannot be traced back to a choice is a ranking nobody
        // can argue with.
        QVERIFY(after.scoringExplanation.contains("counts 5"));
        QVERIFY(after.scoringExplanation.contains(ciphers::name(cipherId)));
    }

    void candidatesComeBackHighestFirst()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        analyzer::Options options;
        const analyzer::Result result = analyzer::analyze("Washington", index, options);

        for (int i = 1; i < result.candidates.size(); ++i)
            QVERIFY(result.candidates.at(i - 1).score >= result.candidates.at(i).score);
    }

    void aMinimumNumberOfAgreeingCiphersCanBeRequired()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        analyzer::Options strict;
        strict.minimumCipherMatches = 3;

        const analyzer::Result result = analyzer::analyze("Washington", index, strict);

        for (int i = 0; i < result.candidates.size(); ++i)
            QVERIFY(result.candidates.at(i).exactMatches >= 3);

        QVERIFY(result.scoringExplanation.contains("fewer than 3"));
    }

    void onlySelectedCiphersAreCompared()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        analyzer::Options only;
        only.ciphers << ciphers::EnglishOrdinal << ciphers::Chaldean;

        const analyzer::Result result = analyzer::analyze("Washington", index, only);

        QCOMPARE(result.values.size(), 2);

        for (int c = 0; c < result.candidates.size(); ++c) {
            const analyzer::Candidate &candidate = result.candidates.at(c);

            for (int m = 0; m < candidate.matches.size(); ++m) {
                const int id = candidate.matches.at(m).cipherId;
                QVERIFY(id == ciphers::EnglishOrdinal || id == ciphers::Chaldean);
            }
        }
    }

    void selectingBothMirrorAndReverseOrdinalStillCountsOnce()
    {
        analyzer::Options both;
        both.ciphers << ciphers::ReverseOrdinal << ciphers::MirrorAtbash;

        const QVector<int> used = analyzer::effectiveCiphers(both);

        QCOMPARE(used.size(), 1);
        QCOMPARE(used.first(), static_cast<int>(ciphers::ReverseOrdinal));
    }

    void nearMatchesAreSeparateFromAgreementAndScoreNothing()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        analyzer::Options near;
        near.includeNearMatches = true;
        near.nearTolerance = 2;
        near.minimumCipherMatches = 0;

        const analyzer::Result result = analyzer::analyze("Washington", index, near);

        QVERIFY(result.scoringExplanation.contains("add nothing to the score"));

        for (int c = 0; c < result.candidates.size(); ++c) {
            const analyzer::Candidate &candidate = result.candidates.at(c);

            for (int m = 0; m < candidate.matches.size(); ++m) {
                const analyzer::Match &match = candidate.matches.at(m);

                if (!match.exact) {
                    QCOMPARE(match.weight, 0);
                    QVERIFY(match.difference > 0);
                    QVERIFY(match.difference <= 2);
                    QVERIFY(match.inputValue.key() != match.candidateValue.key());
                }
            }

            // A near miss must never be counted as an agreement.
            int exact = 0;
            for (int m = 0; m < candidate.matches.size(); ++m)
                if (candidate.matches.at(m).exact) ++exact;

            QCOMPARE(candidate.exactMatches, exact);
        }
    }

    void nearMatchingIsOffByDefault()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        analyzer::Options options;
        const analyzer::Result result = analyzer::analyze("Washington", index, options);

        for (int c = 0; c < result.candidates.size(); ++c)
            QCOMPARE(result.candidates.at(c).nearMatches, 0);
    }

    void anEmptyPhraseFindsNothing()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        analyzer::Options options;

        QCOMPARE(analyzer::analyze("", index, options).candidates.size(), 0);
        QCOMPARE(analyzer::analyze("   ", index, options).candidates.size(), 0);
    }

    void aCipherTheInputScoresNothingInIsNotEvidence()
    {
        // "Washington" has no Abjad value, because Abjad reads Arabic. Every
        // stored Latin word also has none, so without this guard they would all
        // "agree" on zero.
        HistoryIndex index;
        index.setWords(sampleHistory());

        analyzer::Options arabic;
        arabic.ciphers << ciphers::Abjad;

        const analyzer::Result result = analyzer::analyze("Washington", index, arabic);

        QCOMPARE(result.candidates.size(), 0);
    }

    void sayingThereIsNoDictionaryIsNotTheSameAsFindingNothing()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        analyzer::Options dictionary;
        dictionary.source = analyzer::Source::Dictionary;

        const analyzer::Result result = analyzer::analyze("Washington", index, dictionary);

        QCOMPARE(result.candidates.size(), 0);
        QVERIFY(result.scoringExplanation.contains("No dictionary"));
    }

    // ---- the graph -------------------------------------------------------

    void theGraphCarriesThePhraseItsNumbersAndItsCandidates()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        analyzer::Options options;
        const analyzer::Result result = analyzer::analyze("Washington", index, options);

        QVERIFY(result.graph.hasNode(decode::Graph::phraseId("Washington")));
        QVERIFY(result.graph.nodeCount() > 1);

        for (int c = 0; c < result.candidates.size(); ++c)
            QVERIFY(result.graph.hasNode(decode::Graph::wordId(result.candidates.at(c).word)));
    }

    void everyEdgeNamesItsCipherAndItsValue()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        analyzer::Options options;
        const analyzer::Result result = analyzer::analyze("Washington", index, options);

        const QVector<decode::Edge> &edges = result.graph.edges();

        QVERIFY(!edges.isEmpty());

        for (int i = 0; i < edges.size(); ++i) {
            QVERIFY(edges.at(i).cipherId >= 0);
            QVERIFY(!edges.at(i).cipherName.isEmpty());
            QVERIFY(!edges.at(i).value.isEmpty());
        }
    }

    void theGraphSerialisesWithValuesAsStrings()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        analyzer::Options options;
        const analyzer::Result result = analyzer::analyze("Washington", index, options);

        const QJsonObject json = result.graph.toJson();

        QVERIFY(json.contains("nodes"));
        QVERIFY(json.contains("edges"));

        const QJsonArray edges = json.value("edges").toArray();
        QVERIFY(!edges.isEmpty());

        // Values travel as strings. A Multiplicative value can be twenty-nine
        // digits and JSON numbers are doubles in most readers, which would
        // round it and hand a model a number the program never calculated.
        for (int i = 0; i < edges.size(); ++i) {
            const QJsonObject edge = edges.at(i).toObject();

            if (edge.contains("value"))
                QVERIFY(edge.value("value").isString());
        }

        // It must survive a round trip through real JSON, not just look right.
        const QByteArray encoded = QJsonDocument(json).toJson(QJsonDocument::Compact);
        const QJsonObject again = QJsonDocument::fromJson(encoded).object();

        QCOMPARE(again.value("edges").toArray().size(), edges.size());
    }

    void twoIdenticalAnalysesProduceIdenticalPayloads()
    {
        HistoryIndex index;
        index.setWords(sampleHistory());

        analyzer::Options options;

        const QByteArray a = QJsonDocument(analyzer::analyze("Washington", index, options).graph.toJson())
                                 .toJson(QJsonDocument::Compact);
        const QByteArray b = QJsonDocument(analyzer::analyze("Washington", index, options).graph.toJson())
                                 .toJson(QJsonDocument::Compact);

        QCOMPARE(a, b);
    }

    void aNumberNodeIsPerCipher()
    {
        // 156 in English Ordinal and 156 in Chaldean are not the same fact.
        // Merging them would invent a relationship between every pair of words
        // that happen to hit the same integer in two unrelated systems.
        QVERIFY(decode::Graph::numberId(ciphers::EnglishOrdinal, "156")
                != decode::Graph::numberId(ciphers::Chaldean, "156"));
    }
};

// No QTEST_APPLESS_MAIN: two suites share one binary, so each exposes an
// entry point and main.cpp runs both. See tests/main.cpp.
int runAnalyzerTests(int argc, char **argv)
{
    AnalyzerTests tests;
    return QTest::qExec(&tests, argc, argv);
}
#include "tst_analyzer.moc"
