#ifndef ANALYZER_H
#define ANALYZER_H

#include "ciphervalue.h"
#include "decodegraph.h"
#include "historyindex.h"

#include <QString>
#include <QVector>

// Finds the stored words that share a number with an input phrase, and says
// exactly why it thinks so.
//
// What this replaces: searchhistory() picked one cipher by a switch, computed
// one value, and asked searchwords() whether any stored word matched it in any
// of eleven ciphers. The answer was a bool per word. Which cipher agreed, how
// many agreed, and what the shared number was - all computed, none kept.
//
// The scoring here is deliberately the simplest thing that can be explained in
// one sentence, and the sentence is carried in the result so it can be printed
// next to the numbers. Nothing is weighted without the user having said so.

namespace analyzer {

enum class Source {
    History,
    Dictionary,     // not wired: this program has no dictionary yet
    Both
};

struct Options
{
    Options();

    // Which ciphers to compare. Empty means every cipher that is not declared
    // redundant with another - Mirror (Atbash) always equals Reverse Ordinal,
    // and counting both would turn one agreement into two.
    QVector<int> ciphers;

    Source source;

    // Exact agreement is the default. Near matches are found separately, never
    // added to the exact score, and reported as their own count - a number that
    // is close is not a number that agrees.
    bool includeNearMatches;
    int  nearTolerance;          // how far apart "near" may be

    // A candidate must agree in at least this many ciphers to be reported.
    int  minimumCipherMatches;

    // Ciphers the user has marked as mattering more. Absent means weight 1.
    // The weights are shown in the explanation, so a ranking can always be
    // traced back to a choice somebody made.
    QHash<int, int> cipherWeights;

    int  maximumCandidates;      // 0 = no limit
};

// One cipher agreeing (or nearly agreeing) between the phrase and a candidate.
struct Match
{
    int                  cipherId;
    QString              cipherName;
    ciphers::CipherValue inputValue;
    ciphers::CipherValue candidateValue;
    bool                 exact;
    qint64               difference;   // 0 when exact
    int                  weight;       // what it contributed to the score
};

struct Candidate
{
    QString        word;
    int            score;
    int            exactMatches;
    int            nearMatches;
    QVector<Match> matches;     // exact first, then near, each cipher once
};

struct Result
{
    QString phrase;

    // The phrase's own value in every cipher that was compared.
    QVector<QPair<int, ciphers::CipherValue> > values;

    QVector<Candidate> candidates;   // highest score first

    // The scoring rule in words, built from the options actually used. Shown in
    // the UI and included in anything sent to a model, so the formula is never
    // something the reader has to take on trust.
    QString scoringExplanation;

    int wordsSearched;

    // The same analysis as nodes and edges. The UI reads candidates; the AI
    // layer reads this and nothing else.
    decode::Graph graph;
};

// Runs an analysis. The index supplies the candidates and their values; it is
// not modified.
Result analyze(const QString &phrase, const HistoryIndex &index, const Options &options);

// The ciphers an analysis would compare, given the options: either what was
// asked for, or every non-redundant cipher.
QVector<int> effectiveCiphers(const Options &options);

} // namespace analyzer

#endif // ANALYZER_H
