#ifndef AIPROVIDER_H
#define AIPROVIDER_H

#include "analyzer.h"

#include <QByteArray>
#include <QJsonObject>
#include <QString>
#include <functional>

// Optional interpretation of an analysis, by a model, with the calculation
// left strictly alone.
//
// The rule this file exists to enforce: the numbers are the program's, and the
// model never supplies one. It is given the finished graph - phrases, stored
// words, the ciphers that agreed and the values they agreed on - and asked what
// it notices. Anything it says is labelled interpretation and shown apart from
// the calculated result. If it is disabled, unconfigured, unreachable, slow or
// returns nonsense, the analysis is exactly what it was before: the AI is an
// addition to the output, never a step in producing it.

namespace ai {

// Which service. Three of these speak the same wire format; they are listed
// separately because what they imply about privacy is not the same.
enum class Provider {
    Anthropic,
    OpenAI,
    OpenAICompatible,   // a local or self-hosted endpoint speaking the OpenAI API
    Ollama
};

struct Settings
{
    Settings();

    bool     enabled;
    Provider provider;
    QString  model;
    QString  endpoint;       // empty: the provider's default

    // The NAME of an environment variable, never the key itself. A key in a
    // settings file is a key in a backup, in a screenshot and in a support
    // email.
    QString  apiKeyVariable;

    // Whether to send a temperature at all, and what it is.
    //
    // Off by default, and deliberately a separate flag rather than a magic
    // value of the number. Sending it unconditionally was wrong: the current
    // Anthropic models refuse the request outright with "`temperature` is
    // deprecated for this model", so a setting nobody had chosen - it was
    // simply the default of 0.2 - stopped the feature working at all.
    bool     sendTemperature;
    double   temperature;

    // How many candidates may be described to the model. Everything beyond this
    // is simply not sent - the whole history never is.
    int      maximumCandidates;

    int      timeoutMs;

    static Settings fromSettings();     // QSettings, same group as the rest
    void save() const;

    static QString providerName(Provider provider);
    static Provider providerFromName(const QString &name, bool *ok = nullptr);

    // True when the endpoint is on this machine, which is the only case where
    // nothing leaves it.
    bool isLocal() const;

    QString defaultEndpoint() const;
    QString defaultModel() const;

    // The key itself, read from the environment at the moment it is needed and
    // never stored, logged or put in the payload.
    QString apiKey() const;

    // Why this cannot run, or an empty string when it can. See usability(),
    // below, for which kind of trouble it is.
    QString unusableReason() const;
};

enum class Status {
    Ok,
    Disabled,            // the user has not turned it on
    NotConfigured,       // no model, endpoint or key variable
    NoCredentials,       // the environment variable is unset or empty
    NothingToSend,       // the analysis found nothing worth interpreting
    TransportFailed,     // unreachable, refused, timed out
    BadResponse          // reached it, could not make sense of the reply
};

// Whether these settings can be used, and if not, which kind of not. Ok when
// there is nothing wrong with them.
//
// This returns a Status rather than a message because classifying the trouble
// by looking for words in the message was how it was first written, and it was
// wrong: "no variable is named for the key" and "the named variable is empty"
// both contain the words "API key", so a substring test called them the same
// thing. They are not. One is a setting nobody filled in; the other is an
// environment this program cannot fix, and the user needs to be told to set it.
Status usability(const Settings &settings, QString *reason = nullptr);

struct Analysis
{
    Analysis();

    Status  status;
    QString text;        // the model's words. Interpretation, never values.
    QString error;       // why, when status is not Ok
    QString providerUsed;
    QString modelUsed;
    bool    stayedLocal;

    bool isOk() const { return status == Status::Ok; }

    // One line to put above the text in the UI, so nobody mistakes it for
    // calculation.
    QString disclaimer() const;
};

// Exactly what would be sent, so it can be shown to the user before it is.
//
// Built from the analysis alone: the input phrase, its cipher values, and the
// top candidates with the ciphers and values that matched them. Not the history
// file, not the settings, not the key.
QJsonObject buildPayload(const analyzer::Result &result, const Settings &settings);

// What the user is told before anything leaves the machine. Names the provider,
// the endpoint, how many candidates and whether it stays local.
QString privacyNotice(const analyzer::Result &result, const Settings &settings);

// The prompt. Stated here rather than buried in a request builder because what
// the model is forbidden to do is part of the design, not a detail.
QString systemPrompt();

// How the request is actually sent. Swappable so the failure paths - timeout,
// refusal, malformed reply - can be tested without a network, which is the only
// way to be sure the analyzer survives them.
struct TransportResult
{
    bool       ok;
    QByteArray body;
    QString    error;
};

typedef std::function<TransportResult (const QString &url,
                                       const QByteArray &body,
                                       const QList<QPair<QByteArray, QByteArray> > &headers,
                                       int timeoutMs)> Transport;

// The default transport: QNetworkAccessManager, with the timeout applied.
Transport networkTransport();

class Client
{
public:
    explicit Client(const Settings &settings);

    // Tests inject their own; production leaves it alone.
    void setTransport(const Transport &transport);

    // Never throws, never blocks longer than the timeout, and never reports
    // anything but Ok as though it had worked.
    Analysis interpret(const analyzer::Result &result) const;

    // The pieces, exposed for testing and for showing the user.
    QString requestUrl() const;
    QList<QPair<QByteArray, QByteArray> > requestHeaders() const;
    QByteArray requestBody(const analyzer::Result &result) const;

    // Pulls the model's text out of a provider's reply. Returns false and sets
    // error for anything that is not the shape expected - including valid JSON
    // of the wrong shape, which is the failure a naive parser turns into an
    // empty string and presents as an answer.
    static bool parseReply(Provider provider, const QByteArray &body,
                           QString *text, QString *error);

private:
    Settings  _settings;
    Transport _transport;
};

} // namespace ai

#endif // AIPROVIDER_H
