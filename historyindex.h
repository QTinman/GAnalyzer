#ifndef HISTORYINDEX_H
#define HISTORYINDEX_H

#include "ciphervalue.h"

#include <QHash>
#include <QString>
#include <QStringList>
#include <QVector>

// The stored words, with their cipher values worked out once.
//
// searchwords() in gcalc.cpp opens history.txt and, for every line, recomputes
// eleven cipher values and compares each to one target. That is the whole file
// re-encoded on every search, and it answers only "did anything match" - the
// knowledge of which cipher matched is computed and thrown away.
//
// This keeps, for each cipher, a map from value to the words holding it:
//
//     English Ordinal:  156 -> ["word one", "word two"]
//     Reverse Ordinal:  156 -> [...]
//
// so a multi-cipher match is a handful of lookups rather than a file scan per
// cipher. Two hundred words do not need this; two hundred words also do not
// care, and the structure is what makes "which ciphers agreed" cheap to answer
// rather than something to recompute afterwards.
//
// Values are keyed by CipherValue::key(), a decimal string, so an arbitrary
// precision Multiplicative value indexes exactly like a small one.

class HistoryIndex
{
public:
    HistoryIndex();

    // Replaces the contents. Blank lines are dropped; the original spelling and
    // order of everything else is kept, because it is what the user typed and
    // what the rest of the program prints back to them.
    void setWords(const QStringList &words);

    // Reads one phrase per line, as history.txt has always been stored.
    // Returns false and leaves the index untouched if the file cannot be read -
    // an unreadable history must never look like an empty one.
    bool load(const QString &path, QString *error = nullptr);

    // Keeps the maps in step. Adding a word already present is a no-op rather
    // than a duplicate entry.
    void add(const QString &word);
    void remove(const QString &word);

    void clear();

    const QStringList &words() const { return _words; }
    int count() const { return _words.size(); }
    bool isEmpty() const { return _words.isEmpty(); }

    // Which ciphers this index was built for. Rebuilding with a different set
    // is the caller's job; the analyzer asks for what it needs.
    const QVector<int> &ciphers() const { return _ciphers; }
    void setCiphers(const QVector<int> &cipherIds);

    // The stored words whose value in this cipher is exactly this one.
    QStringList wordsWith(int cipherId, const ciphers::CipherValue &value) const;

    // One stored word's value, from the cache rather than recomputed.
    ciphers::CipherValue valueOf(const QString &word, int cipherId) const;

    // Every value present in one cipher, for near-match scanning. Sorted
    // ascending, small values only - see AnalyzerOptions::nearTolerance.
    QVector<qint64> smallValues(int cipherId) const;

private:
    void indexWord(const QString &word);
    void unindexWord(const QString &word);

    QStringList _words;
    QVector<int> _ciphers;

    // cipherId -> value key -> words holding it
    QHash<int, QHash<QString, QStringList> > _byValue;

    // word -> cipherId -> value
    QHash<QString, QHash<int, ciphers::CipherValue> > _valuesByWord;
};

#endif // HISTORYINDEX_H
