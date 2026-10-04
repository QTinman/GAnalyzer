#include <QtTest>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "aiprovider.h"
#include "analyzer.h"
#include "ciphers.h"
#include "historyindex.h"

// The AI layer is optional, and these exist to prove it. Every failure a model
// or a network can produce has to leave the calculated analysis exactly as it
// was: switched off, misconfigured, no key, unreachable, slow, or answering
// with something that is not an answer.

class AiTests : public QObject
{
    Q_OBJECT

private:
    // An analysis that definitely found something.
    //
    // The first version of this searched for "Eclipse" in a list of unrelated
    // words, found nothing, and so returned NothingToSend before any transport
    // was reached - which quietly made six of the tests below assert nothing at
    // all. An anagram cannot fail to agree: every cipher here that sums its
    // letters gives "Listen" and "Silent" the same total, by construction
    // rather than by my arithmetic.
    static analyzer::Result sampleAnalysis()
    {
        HistoryIndex index;
        index.setWords(QStringList() << "Silent" << "Enlist" << "Tinsel"
                                     << "Washington" << "Murder");

        analyzer::Options options;
        const analyzer::Result result = analyzer::analyze("Listen", index, options);

        Q_ASSERT(!result.candidates.isEmpty());

        return result;
    }

    static ai::Settings workingSettings()
    {
        ai::Settings s;

        s.enabled = true;
        s.provider = ai::Provider::Ollama;          // local: needs no key
        s.model = "llama3";
        s.endpoint = "http://localhost:11434/api/chat";
        s.timeoutMs = 1000;

        return s;
    }

    static ai::Transport replying(const QByteArray &body)
    {
        return [body](const QString &, const QByteArray &,
                      const QList<QPair<QByteArray, QByteArray> > &, int) {
            ai::TransportResult r;
            r.ok = true;
            r.body = body;
            return r;
        };
    }

    static ai::Transport failing(const QString &error)
    {
        return [error](const QString &, const QByteArray &,
                       const QList<QPair<QByteArray, QByteArray> > &, int) {
            ai::TransportResult r;
            r.ok = false;
            r.error = error;
            return r;
        };
    }

private slots:

    // ---- switched off and unconfigured -----------------------------------

    void disabledIsReportedAsDisabledNotAsFailure()
    {
        ai::Settings off;
        off.enabled = false;

        const ai::Analysis analysis = ai::Client(off).interpret(sampleAnalysis());

        QCOMPARE(analysis.status, ai::Status::Disabled);
        QVERIFY(!analysis.isOk());
        QVERIFY(analysis.text.isEmpty());
    }

    void aMissingApiKeyIsItsOwnAnswer()
    {
        // Named apart from "misconfigured" so the UI can say which environment
        // variable to set instead of "something went wrong".
        ai::Settings remote;
        remote.enabled = true;
        remote.provider = ai::Provider::OpenAI;
        remote.model = "gpt-4o-mini";
        remote.apiKeyVariable = "GANALYZER_TEST_KEY_THAT_IS_NOT_SET";

        const ai::Analysis analysis = ai::Client(remote).interpret(sampleAnalysis());

        QCOMPARE(analysis.status, ai::Status::NoCredentials);
        QVERIFY(analysis.error.contains("GANALYZER_TEST_KEY_THAT_IS_NOT_SET"));
    }

    void noKeyVariableNamedAtAllIsMisconfiguration()
    {
        ai::Settings remote;
        remote.enabled = true;
        remote.provider = ai::Provider::Anthropic;
        remote.model = "claude-sonnet-5";

        const ai::Analysis analysis = ai::Client(remote).interpret(sampleAnalysis());

        QCOMPARE(analysis.status, ai::Status::NotConfigured);
    }

    void aLocalProviderNeedsNoKey()
    {
        const ai::Settings local = workingSettings();

        QVERIFY(local.isLocal());
        QVERIFY(local.unusableReason().isEmpty());
    }

    void localEndpointsAreRecognised()
    {
        ai::Settings s = workingSettings();

        s.endpoint = "http://localhost:11434/api/chat";   QVERIFY(s.isLocal());
        s.endpoint = "http://127.0.0.1:8000/v1/chat";     QVERIFY(s.isLocal());
        s.endpoint = "https://api.openai.com/v1/chat";    QVERIFY(!s.isLocal());
        s.endpoint = "https://api.anthropic.com/v1/x";    QVERIFY(!s.isLocal());
    }

    // ---- failures of every kind ------------------------------------------

    void aTransportFailureIsReportedNotSwallowed()
    {
        ai::Client client(workingSettings());
        client.setTransport(failing("Connection refused"));

        const ai::Analysis analysis = client.interpret(sampleAnalysis());

        QCOMPARE(analysis.status, ai::Status::TransportFailed);
        QVERIFY(analysis.error.contains("Connection refused"));
        QVERIFY(analysis.text.isEmpty());
    }

    void aTimeoutIsATransportFailure()
    {
        ai::Client client(workingSettings());
        client.setTransport(failing("No reply within 1 seconds."));

        const ai::Analysis analysis = client.interpret(sampleAnalysis());

        QCOMPARE(analysis.status, ai::Status::TransportFailed);
        QVERIFY(!analysis.isOk());
    }

    void rubbishThatIsNotJsonIsABadResponse()
    {
        ai::Client client(workingSettings());
        client.setTransport(replying("<html>502 Bad Gateway</html>"));

        const ai::Analysis analysis = client.interpret(sampleAnalysis());

        QCOMPARE(analysis.status, ai::Status::BadResponse);
        QVERIFY(analysis.text.isEmpty());
    }

    void validJsonOfTheWrongShapeIsAlsoABadResponse()
    {
        // The failure a naive parser turns into an empty string and then shows
        // the user as though the model had answered.
        ai::Client client(workingSettings());
        client.setTransport(replying("{\"something\":\"else\"}"));

        const ai::Analysis analysis = client.interpret(sampleAnalysis());

        QCOMPARE(analysis.status, ai::Status::BadResponse);
        QVERIFY(analysis.error.contains("no message"));
    }

    void aProvidersOwnErrorIsPassedOnInItsOwnWords()
    {
        ai::Client client(workingSettings());
        client.setTransport(replying("{\"error\":{\"message\":\"model not found\"}}"));

        const ai::Analysis analysis = client.interpret(sampleAnalysis());

        QCOMPARE(analysis.status, ai::Status::BadResponse);
        QVERIFY(analysis.error.contains("model not found"));
    }

    void anEmptyAnalysisIsNotSentAtAll()
    {
        HistoryIndex empty;
        analyzer::Options options;
        const analyzer::Result nothing = analyzer::analyze("Eclipse", empty, options);

        bool called = false;
        ai::Client client(workingSettings());

        client.setTransport([&called](const QString &, const QByteArray &,
                                      const QList<QPair<QByteArray, QByteArray> > &, int) {
            called = true;
            ai::TransportResult r;
            r.ok = true;
            r.body = "{}";
            return r;
        });

        const ai::Analysis analysis = client.interpret(nothing);

        QCOMPARE(analysis.status, ai::Status::NothingToSend);
        QVERIFY2(!called, "nothing should have left the machine");
    }

    // ---- a reply that works ----------------------------------------------

    void eachProvidersReplyShapeIsUnderstood()
    {
        struct Case { ai::Provider provider; const char *body; };

        const Case cases[] = {
            { ai::Provider::Anthropic,
              "{\"content\":[{\"type\":\"text\",\"text\":\"Three ciphers agree.\"}]}" },
            { ai::Provider::OpenAI,
              "{\"choices\":[{\"message\":{\"content\":\"Three ciphers agree.\"}}]}" },
            { ai::Provider::OpenAICompatible,
              "{\"choices\":[{\"message\":{\"content\":\"Three ciphers agree.\"}}]}" },
            { ai::Provider::Ollama,
              "{\"message\":{\"content\":\"Three ciphers agree.\"}}" },
        };

        for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
            QString text, error;

            QVERIFY2(ai::Client::parseReply(cases[i].provider, cases[i].body, &text, &error),
                     qPrintable(error));
            QCOMPARE(text, QString("Three ciphers agree."));
        }
    }

    void aGoodReplyIsMarkedAsInterpretation()
    {
        ai::Client client(workingSettings());
        client.setTransport(replying("{\"message\":{\"content\":\"Three ciphers agree.\"}}"));

        const ai::Analysis analysis = client.interpret(sampleAnalysis());

        QCOMPARE(analysis.status, ai::Status::Ok);
        QCOMPARE(analysis.text, QString("Three ciphers agree."));

        // Nobody should be able to mistake it for calculation.
        QVERIFY(analysis.disclaimer().contains("Interpretation"));
        QVERIFY(analysis.disclaimer().contains("calculated by GAnalyzer"));
        QVERIFY(analysis.stayedLocal);
        QVERIFY(analysis.disclaimer().contains("on this computer"));
    }

    // ---- privacy ----------------------------------------------------------

    void thePayloadHoldsOnlyTheCandidatesThisAnalysisFound()
    {
        const analyzer::Result result = sampleAnalysis();

        ai::Settings settings = workingSettings();
        settings.maximumCandidates = 2;

        const QJsonObject payload = ai::buildPayload(result, settings);
        const QJsonArray candidates = payload.value("candidates").toArray();

        QVERIFY(candidates.size() <= 2);
        QCOMPARE(payload.value("candidatesSent").toInt(), candidates.size());

        // The history file is never sent. Every word in the payload must be one
        // this analysis actually matched.
        QSet<QString> allowed;
        for (int i = 0; i < result.candidates.size(); ++i)
            allowed.insert(result.candidates.at(i).word);

        for (int i = 0; i < candidates.size(); ++i)
            QVERIFY(allowed.contains(candidates.at(i).toObject().value("word").toString()));
    }

    void thePayloadNeverCarriesTheApiKey()
    {
        qputenv("GANALYZER_TEST_SECRET", "sk-do-not-send-me");

        ai::Settings settings;
        settings.enabled = true;
        settings.provider = ai::Provider::OpenAI;
        settings.model = "gpt-4o-mini";
        settings.apiKeyVariable = "GANALYZER_TEST_SECRET";

        QCOMPARE(settings.apiKey(), QString("sk-do-not-send-me"));

        const ai::Client client(settings);
        const QByteArray body = client.requestBody(sampleAnalysis());

        QVERIFY2(!body.contains("sk-do-not-send-me"), "the key must travel in a header, not the body");
        QVERIFY2(!body.contains("GANALYZER_TEST_SECRET"), "not even the variable's name belongs in the body");

        // It does belong in the header, once.
        bool inHeader = false;
        const QList<QPair<QByteArray, QByteArray> > headers = client.requestHeaders();

        for (int i = 0; i < headers.size(); ++i)
            if (headers.at(i).second.contains("sk-do-not-send-me"))
                inHeader = true;

        QVERIFY(inHeader);
        qunsetenv("GANALYZER_TEST_SECRET");
    }

    void noTemperatureIsSentUnlessItIsAskedFor()
    {
        // The current Anthropic models refuse a request that carries one at
        // all - "`temperature` is deprecated for this model" - so sending it by
        // default broke the feature for a setting nobody had chosen.
        ai::Settings quiet = workingSettings();
        quiet.sendTemperature = false;

        const QByteArray without = ai::Client(quiet).requestBody(sampleAnalysis());
        QVERIFY2(!without.contains("temperature"), without.left(200).constData());

        ai::Settings loud = workingSettings();
        loud.sendTemperature = true;
        loud.temperature = 0.7;

        // Ollama nests it under options; Anthropic and OpenAI take it at the
        // top level. Checked for each, because "not sent" looked identical to
        // "this provider ignores it" until one of them did.

        const ai::Provider providers[] = { ai::Provider::Anthropic, ai::Provider::OpenAI,
                                           ai::Provider::OpenAICompatible, ai::Provider::Ollama };

        for (size_t i = 0; i < sizeof(providers) / sizeof(providers[0]); ++i) {
            ai::Settings off = loud;
            off.provider = providers[i];
            off.model = "a-model";
            off.sendTemperature = false;

            ai::Settings on = off;
            on.sendTemperature = true;

            const QByteArray quiet = ai::Client(off).requestBody(sampleAnalysis());
            const QByteArray noisy = ai::Client(on).requestBody(sampleAnalysis());

            QVERIFY2(!quiet.contains("temperature"),
                     qPrintable(ai::Settings::providerName(providers[i])));
            QVERIFY2(noisy.contains("temperature"),
                     qPrintable(ai::Settings::providerName(providers[i])));
            QVERIFY2(noisy.contains("0.7"),
                     qPrintable(ai::Settings::providerName(providers[i])));
        }
    }

    void sendingTemperatureIsOffByDefault()
    {
        QVERIFY(!ai::Settings().sendTemperature);
    }

    void valuesTravelAsStringsSoNothingIsRounded()
    {
        const analyzer::Result result = sampleAnalysis();
        const QJsonObject payload = ai::buildPayload(result, workingSettings());
        const QJsonArray values = payload.value("inputValues").toArray();

        QVERIFY(!values.isEmpty());

        for (int i = 0; i < values.size(); ++i)
            QVERIFY(values.at(i).toObject().value("value").isString());
    }

    void theUserIsToldWhatLeavesTheMachineAndWhatDoesNot()
    {
        const analyzer::Result result = sampleAnalysis();

        ai::Settings remote;
        remote.enabled = true;
        remote.provider = ai::Provider::OpenAI;
        remote.endpoint = "https://api.openai.com/v1/chat/completions";

        const QString away = ai::privacyNotice(result, remote);

        QVERIFY(away.contains("leaves your computer"));
        QVERIFY(away.contains("api.openai.com"));
        QVERIFY(away.contains("your history file"));
        QVERIFY(away.contains("API key"));

        const QString local = ai::privacyNotice(result, workingSettings());

        QVERIFY(local.contains("stays on your computer"));
    }

    void thePromptForbidsInventingValues()
    {
        const QString prompt = ai::systemPrompt();

        QVERIFY(prompt.contains("must appear verbatim"));
        QVERIFY(prompt.contains("Do not compute"));
        QVERIFY(prompt.contains("speculation"));
    }

    // ---- the deterministic analyzer is untouched by any of this -----------

    void theAnalysisIsIdenticalWhateverTheAiDoes()
    {
        const analyzer::Result before = sampleAnalysis();

        ai::Client broken(workingSettings());
        broken.setTransport(failing("the world has ended"));
        broken.interpret(before);

        ai::Client rubbish(workingSettings());
        rubbish.setTransport(replying("not json at all"));
        rubbish.interpret(before);

        const analyzer::Result after = sampleAnalysis();

        QCOMPARE(after.candidates.size(), before.candidates.size());
        QCOMPARE(after.scoringExplanation, before.scoringExplanation);
        QCOMPARE(QJsonDocument(after.graph.toJson()).toJson(QJsonDocument::Compact),
                 QJsonDocument(before.graph.toJson()).toJson(QJsonDocument::Compact));
    }

    void providerNamesRoundTrip()
    {
        const ai::Provider providers[] = { ai::Provider::Anthropic, ai::Provider::OpenAI,
                                           ai::Provider::OpenAICompatible, ai::Provider::Ollama };

        for (size_t i = 0; i < sizeof(providers) / sizeof(providers[0]); ++i) {
            bool ok = false;
            const QString name = ai::Settings::providerName(providers[i]);

            QCOMPARE(ai::Settings::providerFromName(name, &ok), providers[i]);
            QVERIFY(ok);
        }

        bool ok = true;
        ai::Settings::providerFromName("something else", &ok);
        QVERIFY(!ok);
    }
};

// No QTEST_APPLESS_MAIN: the suites share one binary. See tests/main.cpp.
int runAiTests(int argc, char **argv)
{
    AiTests tests;
    return QTest::qExec(&tests, argc, argv);
}

#include "tst_ai.moc"
