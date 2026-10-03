#include "aiprovider.h"
#include "ciphers.h"

#include <QCoreApplication>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QTimer>
#include <QUrl>

extern QString appgroup;   // the QSettings group the rest of the program uses

namespace ai {

// ---------------------------------------------------------------- Settings

Settings::Settings()
    : enabled(false),
      provider(Provider::Anthropic),
      temperature(0.2),
      maximumCandidates(25),
      timeoutMs(30000)
{
}

QString Settings::providerName(Provider provider)
{
    switch (provider) {
    case Provider::Anthropic:        return QStringLiteral("Anthropic");
    case Provider::OpenAI:           return QStringLiteral("OpenAI");
    case Provider::OpenAICompatible: return QStringLiteral("OpenAI-compatible");
    case Provider::Ollama:           return QStringLiteral("Ollama");
    }

    return QStringLiteral("Anthropic");
}

Provider Settings::providerFromName(const QString &name, bool *ok)
{
    const QString lower = name.trimmed().toLower();

    if (ok != nullptr)
        *ok = true;

    if (lower == "anthropic")          return Provider::Anthropic;
    if (lower == "openai")             return Provider::OpenAI;
    if (lower == "openai-compatible")  return Provider::OpenAICompatible;
    if (lower == "ollama")             return Provider::Ollama;

    if (ok != nullptr)
        *ok = false;

    return Provider::Anthropic;
}

QString Settings::defaultEndpoint() const
{
    switch (provider) {
    case Provider::Anthropic:        return QStringLiteral("https://api.anthropic.com/v1/messages");
    case Provider::OpenAI:           return QStringLiteral("https://api.openai.com/v1/chat/completions");
    case Provider::OpenAICompatible: return QStringLiteral("http://localhost:8000/v1/chat/completions");
    case Provider::Ollama:           return QStringLiteral("http://localhost:11434/api/chat");
    }

    return QString();
}

QString Settings::defaultModel() const
{
    switch (provider) {
    case Provider::Anthropic:        return QStringLiteral("claude-sonnet-5");
    case Provider::OpenAI:           return QStringLiteral("gpt-4o-mini");
    case Provider::OpenAICompatible: return QString();
    case Provider::Ollama:           return QStringLiteral("llama3");
    }

    return QString();
}

bool Settings::isLocal() const
{
    const QString target = endpoint.isEmpty() ? defaultEndpoint() : endpoint;
    const QString host = QUrl(target).host().toLower();

    return host == QLatin1String("localhost")
        || host == QLatin1String("127.0.0.1")
        || host == QLatin1String("::1")
        || host.isEmpty();
}

QString Settings::apiKey() const
{
    if (apiKeyVariable.trimmed().isEmpty())
        return QString();

    // Read now, used now, kept nowhere.
    return QString::fromLocal8Bit(qgetenv(apiKeyVariable.trimmed().toLocal8Bit()));
}

Status usability(const Settings &settings, QString *reason)
{
    const auto say = [reason](const QString &text) {
        if (reason != nullptr)
            *reason = text;
    };

    say(QString());

    if (!settings.enabled) {
        say(QStringLiteral("AI analysis is switched off."));
        return Status::Disabled;
    }

    const QString target = settings.endpoint.isEmpty() ? settings.defaultEndpoint()
                                                       : settings.endpoint;

    if (target.isEmpty()) {
        say(QStringLiteral("No endpoint is configured for this provider."));
        return Status::NotConfigured;
    }

    if ((settings.model.isEmpty() ? settings.defaultModel() : settings.model).isEmpty()) {
        say(QStringLiteral("No model is configured."));
        return Status::NotConfigured;
    }

    // A local model needs no key; a remote one does, and the difference should
    // not be a surprise at the moment of sending.
    if (!settings.isLocal() && settings.provider != Provider::Ollama) {
        if (settings.apiKeyVariable.trimmed().isEmpty()) {
            // Nobody filled the setting in: a configuration problem, fixable here.
            say(QStringLiteral("No environment variable is named for the API key."));
            return Status::NotConfigured;
        }

        if (settings.apiKey().isEmpty()) {
            // The setting is fine; the environment is not. Different problem,
            // different person, different fix.
            say(QString("The environment variable %1 is not set, so there is no API key.")
                    .arg(settings.apiKeyVariable.trimmed()));
            return Status::NoCredentials;
        }
    }

    return Status::Ok;
}

QString Settings::unusableReason() const
{
    QString reason;

    return usability(*this, &reason) == Status::Ok ? QString() : reason;
}

Settings Settings::fromSettings()
{
    Settings s;

    // The same place MainWindow keeps everything else: QSettings("QTinman",
    // appgroup), then a group of the same name. The default constructor would
    // write elsewhere and the settings would seem not to stick.
    QSettings store(QStringLiteral("QTinman"),
                    appgroup.isEmpty() ? QStringLiteral("GAnalyzer") : appgroup);

    store.beginGroup(appgroup.isEmpty() ? QStringLiteral("GAnalyzer") : appgroup);
    store.beginGroup(QStringLiteral("ai"));

    s.enabled = store.value("enabled", false).toBool();
    s.provider = providerFromName(store.value("provider", "Anthropic").toString());
    s.model = store.value("model", QString()).toString();
    s.endpoint = store.value("endpoint", QString()).toString();
    s.apiKeyVariable = store.value("apiKeyVariable", QString()).toString();
    s.temperature = store.value("temperature", 0.2).toDouble();
    s.maximumCandidates = store.value("maximumCandidates", 25).toInt();
    s.timeoutMs = store.value("timeoutMs", 30000).toInt();

    store.endGroup();
    store.endGroup();

    return s;
}

void Settings::save() const
{
    QSettings store(QStringLiteral("QTinman"),
                    appgroup.isEmpty() ? QStringLiteral("GAnalyzer") : appgroup);

    store.beginGroup(appgroup.isEmpty() ? QStringLiteral("GAnalyzer") : appgroup);
    store.beginGroup(QStringLiteral("ai"));

    store.setValue("enabled", enabled);
    store.setValue("provider", providerName(provider));
    store.setValue("model", model);
    store.setValue("endpoint", endpoint);

    // The variable's name. Never its contents - a key written here would be in
    // the registry, in every backup of it, and in any screenshot of this page.
    store.setValue("apiKeyVariable", apiKeyVariable);

    store.setValue("temperature", temperature);
    store.setValue("maximumCandidates", maximumCandidates);
    store.setValue("timeoutMs", timeoutMs);

    store.endGroup();
    store.endGroup();
}

// ---------------------------------------------------------------- Analysis

Analysis::Analysis()
    : status(Status::Disabled), stayedLocal(false)
{
}

QString Analysis::disclaimer() const
{
    if (stayedLocal) {
        return QString("Interpretation by %1 (%2), produced on this computer. "
                       "The numbers above were calculated by GAnalyzer; everything below is "
                       "commentary and may be wrong.")
                   .arg(modelUsed, providerUsed);
    }

    return QString("Interpretation by %1 (%2). The numbers above were calculated by "
                   "GAnalyzer; everything below is commentary and may be wrong.")
               .arg(modelUsed, providerUsed);
}

// ---------------------------------------------------------------- payload

QString systemPrompt()
{
    return QStringLiteral(
        "You are given the finished output of a gematria calculator as a graph of nodes and "
        "edges. Nodes are phrases, stored words and numbers. Each edge records that two things "
        "share a value, and names the cipher and the exact value that produced it.\n"
        "\n"
        "Your task is to describe what is in this data:\n"
        "  - which numerical agreements are strongest, and why\n"
        "  - which ciphers recur across several candidates\n"
        "  - any clusters or repeated structure among the candidates\n"
        "\n"
        "Rules you must follow:\n"
        "  - Every number you mention must appear verbatim in the data you were given. "
        "Do not compute, derive, estimate or recall any gematria value.\n"
        "  - If you are unsure whether something is in the data, say so rather than supplying it.\n"
        "  - Separate what the data states from what you are suggesting. Mark the second as "
        "speculation.\n"
        "  - An agreement in several ciphers is more notable than one in a single cipher. Say so "
        "plainly; do not imply that any agreement means anything beyond itself.\n"
        "  - Be brief.");
}

QJsonObject buildPayload(const analyzer::Result &result, const Settings &settings)
{
    QJsonObject payload;

    payload.insert("phrase", result.phrase);
    payload.insert("scoring", result.scoringExplanation);
    payload.insert("wordsSearched", result.wordsSearched);

    QJsonArray values;

    for (int i = 0; i < result.values.size(); ++i) {
        QJsonObject v;

        v.insert("cipher", ciphers::name(result.values.at(i).first));
        v.insert("value", result.values.at(i).second.toString());
        values.append(v);
    }

    payload.insert("inputValues", values);

    // Only the candidates that will be described, newest-scoring first. The
    // history file itself is never sent: a candidate is here because this
    // analysis found it, not because it exists.
    const int limit = settings.maximumCandidates > 0
                          ? qMin(settings.maximumCandidates, result.candidates.size())
                          : result.candidates.size();

    QJsonArray candidates;

    for (int i = 0; i < limit; ++i) {
        const analyzer::Candidate &candidate = result.candidates.at(i);

        QJsonObject c;
        c.insert("word", candidate.word);
        c.insert("score", candidate.score);
        c.insert("exactMatches", candidate.exactMatches);
        c.insert("nearMatches", candidate.nearMatches);

        QJsonArray matches;

        for (int m = 0; m < candidate.matches.size(); ++m) {
            const analyzer::Match &match = candidate.matches.at(m);

            QJsonObject om;
            om.insert("cipher", match.cipherName);
            om.insert("exact", match.exact);

            // Strings, so a twenty-nine digit Multiplicative value is not
            // rounded into something the program never calculated.
            om.insert("inputValue", match.inputValue.toString());
            om.insert("candidateValue", match.candidateValue.toString());

            if (!match.exact)
                om.insert("difference", static_cast<double>(match.difference));

            matches.append(om);
        }

        c.insert("matches", matches);
        candidates.append(c);
    }

    payload.insert("candidates", candidates);
    payload.insert("candidatesSent", limit);
    payload.insert("candidatesFound", result.candidates.size());
    payload.insert("graph", result.graph.toJson());

    return payload;
}

QString privacyNotice(const analyzer::Result &result, const Settings &settings)
{
    const QString target = settings.endpoint.isEmpty() ? settings.defaultEndpoint() : settings.endpoint;
    const int sending = settings.maximumCandidates > 0
                            ? qMin(settings.maximumCandidates, result.candidates.size())
                            : result.candidates.size();

    QString text;

    if (settings.isLocal()) {
        text = QString("This stays on your computer. The analysis is sent to %1 at %2.\n\n")
                   .arg(Settings::providerName(settings.provider), target);
    } else {
        text = QString("This leaves your computer. The analysis is sent to %1 at %2, over the "
                       "internet.\n\n")
                   .arg(Settings::providerName(settings.provider), target);
    }

    text += QString("What is sent:\n"
                    "  the phrase \"%1\"\n"
                    "  its value in %2 cipher(s)\n"
                    "  %3 of the %4 candidate(s) found, with the ciphers and values that matched\n\n"
                    "What is not sent:\n"
                    "  your history file - only the candidates listed above\n"
                    "  your settings, your API key, or anything else on this computer")
                .arg(result.phrase)
                .arg(result.values.size())
                .arg(sending)
                .arg(result.candidates.size());

    return text;
}

// ---------------------------------------------------------------- transport

Transport networkTransport()
{
    return [](const QString &url, const QByteArray &body,
              const QList<QPair<QByteArray, QByteArray> > &headers, int timeoutMs) {
        TransportResult result;
        result.ok = false;

        QNetworkAccessManager manager;
        QNetworkRequest request((QUrl(url)));

        for (int i = 0; i < headers.size(); ++i)
            request.setRawHeader(headers.at(i).first, headers.at(i).second);

        QNetworkReply *reply = manager.post(request, body);

        // A reply that never arrives must not hang the program. Qt's own
        // transfer timeout does not cover a server that accepts the connection
        // and then says nothing.
        QEventLoop loop;
        QTimer timer;

        timer.setSingleShot(true);
        QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

        timer.start(timeoutMs > 0 ? timeoutMs : 30000);
        loop.exec();

        if (!reply->isFinished()) {
            reply->abort();
            result.error = QString("No reply within %1 seconds.").arg((timeoutMs > 0 ? timeoutMs : 30000) / 1000);
            reply->deleteLater();
            return result;
        }

        if (reply->error() != QNetworkReply::NoError) {
            result.error = reply->errorString();
            result.body = reply->readAll();   // often carries the provider's own message
            reply->deleteLater();
            return result;
        }

        result.ok = true;
        result.body = reply->readAll();
        reply->deleteLater();

        return result;
    };
}

// ---------------------------------------------------------------- Client

Client::Client(const Settings &settings)
    : _settings(settings), _transport(networkTransport())
{
}

void Client::setTransport(const Transport &transport)
{
    _transport = transport;
}

QString Client::requestUrl() const
{
    return _settings.endpoint.isEmpty() ? _settings.defaultEndpoint() : _settings.endpoint;
}

QList<QPair<QByteArray, QByteArray> > Client::requestHeaders() const
{
    QList<QPair<QByteArray, QByteArray> > headers;

    headers.append(qMakePair(QByteArray("Content-Type"), QByteArray("application/json")));

    const QByteArray key = _settings.apiKey().toLocal8Bit();

    switch (_settings.provider) {
    case Provider::Anthropic:
        if (!key.isEmpty()) {
            headers.append(qMakePair(QByteArray("x-api-key"), key));
            headers.append(qMakePair(QByteArray("anthropic-version"), QByteArray("2023-06-01")));
        }
        break;

    case Provider::OpenAI:
    case Provider::OpenAICompatible:
        if (!key.isEmpty())
            headers.append(qMakePair(QByteArray("Authorization"), QByteArray("Bearer ") + key));
        break;

    case Provider::Ollama:
        break;   // local, no credential
    }

    return headers;
}

QByteArray Client::requestBody(const analyzer::Result &result) const
{
    const QString model = _settings.model.isEmpty() ? _settings.defaultModel() : _settings.model;
    const QJsonObject payload = buildPayload(result, _settings);

    const QString userText =
        QString("Here is the analysis as JSON. Describe what is in it, following the rules.\n\n")
        + QString::fromUtf8(QJsonDocument(payload).toJson(QJsonDocument::Compact));

    QJsonObject body;

    if (_settings.provider == Provider::Anthropic) {
        QJsonObject message;
        message.insert("role", "user");
        message.insert("content", userText);

        QJsonArray messages;
        messages.append(message);

        body.insert("model", model);
        body.insert("max_tokens", 1024);
        body.insert("temperature", _settings.temperature);
        body.insert("system", systemPrompt());
        body.insert("messages", messages);
    } else if (_settings.provider == Provider::Ollama) {
        QJsonArray messages;

        QJsonObject system;
        system.insert("role", "system");
        system.insert("content", systemPrompt());
        messages.append(system);

        QJsonObject user;
        user.insert("role", "user");
        user.insert("content", userText);
        messages.append(user);

        body.insert("model", model);
        body.insert("stream", false);
        body.insert("messages", messages);
    } else {
        QJsonArray messages;

        QJsonObject system;
        system.insert("role", "system");
        system.insert("content", systemPrompt());
        messages.append(system);

        QJsonObject user;
        user.insert("role", "user");
        user.insert("content", userText);
        messages.append(user);

        body.insert("model", model);
        body.insert("temperature", _settings.temperature);
        body.insert("messages", messages);
    }

    return QJsonDocument(body).toJson(QJsonDocument::Compact);
}

bool Client::parseReply(Provider provider, const QByteArray &body, QString *text, QString *error)
{
    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(body, &parseError);

    if (document.isNull() || !document.isObject()) {
        if (error != nullptr) {
            *error = QString("The reply was not JSON (%1).")
                         .arg(parseError.errorString());
        }
        return false;
    }

    const QJsonObject root = document.object();

    // A provider reporting its own error returns valid JSON of a different
    // shape; saying "empty answer" would hide what it actually said.
    if (root.contains("error")) {
        if (error != nullptr) {
            const QJsonValue e = root.value("error");
            *error = e.isObject() ? e.toObject().value("message").toString()
                                  : e.toVariant().toString();

            if (error->isEmpty())
                *error = QStringLiteral("The provider returned an error.");
        }
        return false;
    }

    QString found;

    switch (provider) {
    case Provider::Anthropic: {
        const QJsonArray content = root.value("content").toArray();

        for (int i = 0; i < content.size(); ++i) {
            const QJsonObject part = content.at(i).toObject();

            if (part.value("type").toString() == QLatin1String("text"))
                found += part.value("text").toString();
        }
        break;
    }

    case Provider::Ollama:
        found = root.value("message").toObject().value("content").toString();
        break;

    case Provider::OpenAI:
    case Provider::OpenAICompatible: {
        const QJsonArray choices = root.value("choices").toArray();

        if (!choices.isEmpty())
            found = choices.at(0).toObject().value("message").toObject().value("content").toString();
        break;
    }
    }

    if (found.trimmed().isEmpty()) {
        if (error != nullptr) {
            // Valid JSON of the wrong shape is the failure a naive parser turns
            // into an empty string and then presents as an answer.
            *error = QStringLiteral("The reply was JSON, but held no message where one was expected.");
        }
        return false;
    }

    if (text != nullptr)
        *text = found.trimmed();

    return true;
}

Analysis Client::interpret(const analyzer::Result &result) const
{
    Analysis analysis;

    analysis.providerUsed = Settings::providerName(_settings.provider);
    analysis.modelUsed = _settings.model.isEmpty() ? _settings.defaultModel() : _settings.model;
    analysis.stayedLocal = _settings.isLocal();

    // Disabled, misconfigured and missing-credentials are told apart here so
    // the UI can say "set OPENAI_API_KEY" rather than "something went wrong".
    QString reason;
    const Status usable = usability(_settings, &reason);

    if (usable != Status::Ok) {
        analysis.status = usable;
        analysis.error = reason;
        return analysis;
    }

    if (result.candidates.isEmpty()) {
        analysis.status = Status::NothingToSend;
        analysis.error = QStringLiteral("The analysis found no candidates, so there is nothing to interpret.");
        return analysis;
    }

    TransportResult reply;

    if (_transport) {
        reply = _transport(requestUrl(), requestBody(result), requestHeaders(), _settings.timeoutMs);
    } else {
        reply.ok = false;
        reply.error = QStringLiteral("No transport is configured.");
    }

    if (!reply.ok) {
        analysis.status = Status::TransportFailed;
        analysis.error = reply.error.isEmpty()
                             ? QStringLiteral("The request failed.")
                             : reply.error;

        // Some providers put a useful message in the body of a failed request.
        QString fromBody;
        if (!reply.body.isEmpty() && parseReply(_settings.provider, reply.body, &fromBody, nullptr))
            analysis.error += " " + fromBody;

        return analysis;
    }

    QString text;
    QString error;

    if (!parseReply(_settings.provider, reply.body, &text, &error)) {
        analysis.status = Status::BadResponse;
        analysis.error = error;
        return analysis;
    }

    analysis.status = Status::Ok;
    analysis.text = text;

    return analysis;
}

} // namespace ai
