#include "analyzer.h"
#include "ciphers.h"

#include <QHash>
#include <QSet>
#include <algorithm>

namespace analyzer {

Options::Options()
    : source(Source::History),
      includeNearMatches(false),
      nearTolerance(1),
      minimumCipherMatches(1),
      maximumCandidates(0)
{
}

QVector<int> effectiveCiphers(const Options &options)
{
    if (!options.ciphers.isEmpty()) {
        // Honour the selection, but still refuse to count a cipher twice. If
        // somebody ticks both Mirror and Reverse Ordinal they have selected one
        // piece of evidence, whatever the dialog let them do.
        QVector<int> kept;
        QSet<int> selected;

        for (int i = 0; i < options.ciphers.size(); ++i)
            selected.insert(options.ciphers.at(i));

        for (int i = 0; i < options.ciphers.size(); ++i) {
            const int id = options.ciphers.at(i);
            const ciphers::Cipher *cipher = ciphers::byId(id);

            if (cipher == nullptr)
                continue;

            if (cipher->redundantWith >= 0 && selected.contains(cipher->redundantWith))
                continue;

            if (!kept.contains(id))
                kept.append(id);
        }

        return kept;
    }

    QVector<int> everything;
    const std::vector<ciphers::Cipher> &all = ciphers::all();

    for (size_t i = 0; i < all.size(); ++i) {
        if (all[i].redundantWith < 0)
            everything.append(all[i].id);
    }

    return everything;
}

namespace {

int weightFor(const Options &options, int cipherId)
{
    return options.cipherWeights.value(cipherId, 1);
}

QString explain(const Options &options, const QVector<int> &used)
{
    QString text = QStringLiteral(
        "Score = one point for each cipher in which both phrases have exactly the same value.");

    if (!options.cipherWeights.isEmpty()) {
        QStringList weighted;

        for (QHash<int, int>::const_iterator it = options.cipherWeights.constBegin();
             it != options.cipherWeights.constEnd(); ++it) {
            if (it.value() != 1)
                weighted.append(QString("%1 counts %2").arg(ciphers::name(it.key())).arg(it.value()));
        }

        if (!weighted.isEmpty())
            text += " Weighted: " + weighted.join(", ") + ".";
    }

    text += QString(" %1 cipher(s) compared.").arg(used.size());

    if (options.minimumCipherMatches > 1)
        text += QString(" Candidates agreeing in fewer than %1 are not shown.")
                    .arg(options.minimumCipherMatches);

    if (options.includeNearMatches) {
        text += QString(" Near matches (within %1) are listed separately and add nothing to the score.")
                    .arg(options.nearTolerance);
    }

    return text;
}

bool higherScore(const Candidate &a, const Candidate &b)
{
    if (a.score != b.score)
        return a.score > b.score;

    if (a.exactMatches != b.exactMatches)
        return a.exactMatches > b.exactMatches;

    // Alphabetical last, so an identical score always orders the same way
    // rather than however the hash happened to iterate.
    return a.word.compare(b.word, Qt::CaseInsensitive) < 0;
}

} // namespace

Result analyze(const QString &phrase, const HistoryIndex &index, const Options &options)
{
    Result result;

    result.phrase = phrase.trimmed();
    result.wordsSearched = index.count();

    const QVector<int> used = effectiveCiphers(options);

    result.scoringExplanation = explain(options, used);

    if (result.phrase.isEmpty())
        return result;

    const QString phraseNode = decode::Graph::phraseId(result.phrase);

    decode::Node self;
    self.id = phraseNode;
    self.type = decode::NodeType::Phrase;
    self.label = result.phrase;
    result.graph.addNode(self);

    // ---- the phrase's own values ----------------------------------------

    QHash<int, ciphers::CipherValue> inputValues;

    for (int i = 0; i < used.size(); ++i) {
        const int cipherId = used.at(i);
        const ciphers::CipherValue value = ciphers::valueOf(result.phrase, cipherId);

        inputValues.insert(cipherId, value);
        result.values.append(qMakePair(cipherId, value));

        // A cipher the phrase scores nothing in - Abjad on Latin text - is not
        // evidence of anything, and every empty word would agree with it.
        if (value.isBig() || value.toLongLong() != 0) {
            const QString numberNode = decode::Graph::numberId(cipherId, value.key());

            decode::Node number;
            number.id = numberNode;
            number.type = decode::NodeType::Number;
            number.label = value.toString();
            number.detail = ciphers::name(cipherId);
            result.graph.addNode(number);

            decode::Edge edge;
            edge.from = phraseNode;
            edge.to = numberNode;
            edge.type = decode::EdgeType::HasValue;
            edge.cipherId = cipherId;
            edge.cipherName = ciphers::name(cipherId);
            edge.value = value.key();
            edge.difference = 0;
            result.graph.addEdge(edge);
        }
    }

    if (options.source == Source::Dictionary) {
        // Nothing to search: this program has no dictionary. Saying so through
        // an empty result would be indistinguishable from "no word matched".
        result.scoringExplanation +=
            QStringLiteral(" No dictionary is installed, so nothing was searched.");
        return result;
    }

    // ---- gather agreements ----------------------------------------------

    QHash<QString, Candidate> candidates;

    for (int i = 0; i < used.size(); ++i) {
        const int cipherId = used.at(i);
        const ciphers::CipherValue value = inputValues.value(cipherId);

        if (!value.isBig() && value.toLongLong() == 0)
            continue;

        const QStringList holders = index.wordsWith(cipherId, value);

        for (int h = 0; h < holders.size(); ++h) {
            const QString word = holders.at(h);

            // A phrase stored in history matches itself in every cipher, which
            // is true and useless.
            if (word.compare(result.phrase, Qt::CaseInsensitive) == 0)
                continue;

            Candidate &candidate = candidates[word];
            candidate.word = word;

            Match match;
            match.cipherId = cipherId;
            match.cipherName = ciphers::name(cipherId);
            match.inputValue = value;
            match.candidateValue = value;
            match.exact = true;
            match.difference = 0;
            match.weight = weightFor(options, cipherId);

            candidate.matches.append(match);
            candidate.score += match.weight;
            candidate.exactMatches += 1;
        }
    }

    // ---- near matches, kept apart ---------------------------------------

    if (options.includeNearMatches && options.nearTolerance > 0) {
        for (int i = 0; i < used.size(); ++i) {
            const int cipherId = used.at(i);
            const ciphers::CipherValue value = inputValues.value(cipherId);

            if (value.isBig() || value.toLongLong() == 0)
                continue;   // see HistoryIndex::smallValues

            const qint64 target = value.toLongLong();
            const QVector<qint64> present = index.smallValues(cipherId);

            for (int v = 0; v < present.size(); ++v) {
                const qint64 other = present.at(v);
                const qint64 gap = qAbs(other - target);

                if (gap == 0 || gap > options.nearTolerance)
                    continue;

                const QStringList holders =
                    index.wordsWith(cipherId, ciphers::CipherValue(other));

                for (int h = 0; h < holders.size(); ++h) {
                    const QString word = holders.at(h);

                    if (word.compare(result.phrase, Qt::CaseInsensitive) == 0)
                        continue;

                    Candidate &candidate = candidates[word];
                    candidate.word = word;

                    Match match;
                    match.cipherId = cipherId;
                    match.cipherName = ciphers::name(cipherId);
                    match.inputValue = value;
                    match.candidateValue = ciphers::CipherValue(other);
                    match.exact = false;
                    match.difference = gap;
                    match.weight = 0;   // near is not agreement

                    candidate.matches.append(match);
                    candidate.nearMatches += 1;
                }
            }
        }
    }

    // ---- rank and record -------------------------------------------------

    for (QHash<QString, Candidate>::iterator it = candidates.begin(); it != candidates.end(); ++it) {
        if (it->exactMatches < options.minimumCipherMatches)
            continue;

        // Exact first, then near; within each, by cipher order, so two runs of
        // the same analysis read identically.
        std::sort(it->matches.begin(), it->matches.end(), [](const Match &a, const Match &b) {
            if (a.exact != b.exact)
                return a.exact;
            return a.cipherId < b.cipherId;
        });

        result.candidates.append(*it);
    }

    std::sort(result.candidates.begin(), result.candidates.end(), higherScore);

    if (options.maximumCandidates > 0 && result.candidates.size() > options.maximumCandidates)
        result.candidates.resize(options.maximumCandidates);

    for (int i = 0; i < result.candidates.size(); ++i) {
        const Candidate &candidate = result.candidates.at(i);
        const QString wordNode = decode::Graph::wordId(candidate.word);

        decode::Node node;
        node.id = wordNode;
        node.type = decode::NodeType::HistoryWord;
        node.label = candidate.word;
        node.detail = QString("score %1, %2 exact")
                          .arg(candidate.score).arg(candidate.exactMatches);
        result.graph.addNode(node);

        for (int m = 0; m < candidate.matches.size(); ++m) {
            const Match &match = candidate.matches.at(m);

            decode::Edge edge;
            edge.from = phraseNode;
            edge.to = wordNode;
            edge.type = match.exact ? decode::EdgeType::SharesValue
                                    : decode::EdgeType::NearValue;
            edge.cipherId = match.cipherId;
            edge.cipherName = match.cipherName;
            edge.value = match.candidateValue.key();
            edge.difference = static_cast<int>(match.difference);
            result.graph.addEdge(edge);

            if (match.exact) {
                // The shared number is a thing in its own right: it is what two
                // words have in common, and the edge through it is the answer
                // to "why are these related".
                decode::Edge toNumber;
                toNumber.from = wordNode;
                toNumber.to = decode::Graph::numberId(match.cipherId, match.candidateValue.key());
                toNumber.type = decode::EdgeType::HasValue;
                toNumber.cipherId = match.cipherId;
                toNumber.cipherName = match.cipherName;
                toNumber.value = match.candidateValue.key();
                toNumber.difference = 0;
                result.graph.addEdge(toNumber);
            }
        }
    }

    return result;
}

} // namespace analyzer
