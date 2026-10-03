#include "historyindex.h"
#include "ciphers.h"

#include <QFile>
#include <QTextStream>
#include <algorithm>

HistoryIndex::HistoryIndex()
{
    // Everything the table knows, minus the ciphers that duplicate another
    // numerically. Mirror (Atbash) always equals Reverse Ordinal, so indexing
    // both would double every agreement they produce.
    const std::vector<ciphers::Cipher> &all = ciphers::all();

    for (size_t i = 0; i < all.size(); ++i) {
        if (all[i].redundantWith < 0)
            _ciphers.append(all[i].id);
    }
}

void HistoryIndex::setCiphers(const QVector<int> &cipherIds)
{
    _ciphers = cipherIds;

    // The maps describe the old cipher set, so they are rebuilt rather than
    // patched. Leaving stale entries would make a search answer from ciphers
    // nobody asked about.
    const QStringList existing = _words;

    setWords(existing);
}

void HistoryIndex::setWords(const QStringList &words)
{
    _words.clear();
    _byValue.clear();
    _valuesByWord.clear();

    for (int i = 0; i < words.size(); ++i) {
        const QString word = words.at(i).trimmed();

        if (word.isEmpty() || _valuesByWord.contains(word))
            continue;

        _words.append(word);
        indexWord(word);
    }
}

bool HistoryIndex::load(const QString &path, QString *error)
{
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Deliberately not clearing: a history that cannot be read is not an
        // empty history, and a search that silently returns nothing is worse
        // than one that says it could not look.
        if (error != nullptr)
            *error = QString("Cannot read %1: %2").arg(path, file.errorString());

        return false;
    }

    QStringList lines;
    QTextStream in(&file);

    while (!in.atEnd())
        lines.append(in.readLine());

    file.close();
    setWords(lines);

    return true;
}

void HistoryIndex::add(const QString &word)
{
    const QString trimmed = word.trimmed();

    if (trimmed.isEmpty() || _valuesByWord.contains(trimmed))
        return;

    _words.append(trimmed);
    indexWord(trimmed);
}

void HistoryIndex::remove(const QString &word)
{
    const QString trimmed = word.trimmed();

    if (!_valuesByWord.contains(trimmed))
        return;

    unindexWord(trimmed);
    _words.removeAll(trimmed);
}

void HistoryIndex::clear()
{
    _words.clear();
    _byValue.clear();
    _valuesByWord.clear();
}

void HistoryIndex::indexWord(const QString &word)
{
    QHash<int, ciphers::CipherValue> values;

    for (int i = 0; i < _ciphers.size(); ++i) {
        const int cipherId = _ciphers.at(i);
        const ciphers::CipherValue value = ciphers::valueOf(word, cipherId);

        values.insert(cipherId, value);
        _byValue[cipherId][value.key()].append(word);
    }

    _valuesByWord.insert(word, values);
}

void HistoryIndex::unindexWord(const QString &word)
{
    const QHash<int, ciphers::CipherValue> values = _valuesByWord.value(word);

    for (QHash<int, ciphers::CipherValue>::const_iterator it = values.constBegin();
         it != values.constEnd(); ++it) {
        QHash<QString, QStringList> &bucket = _byValue[it.key()];
        const QString key = it.value().key();

        QStringList &holders = bucket[key];
        holders.removeAll(word);

        // An empty bucket left behind would still be reported by smallValues()
        // as a value the history contains.
        if (holders.isEmpty())
            bucket.remove(key);
    }

    _valuesByWord.remove(word);
}

QStringList HistoryIndex::wordsWith(int cipherId, const ciphers::CipherValue &value) const
{
    return _byValue.value(cipherId).value(value.key());
}

ciphers::CipherValue HistoryIndex::valueOf(const QString &word, int cipherId) const
{
    return _valuesByWord.value(word.trimmed()).value(cipherId);
}

QVector<qint64> HistoryIndex::smallValues(int cipherId) const
{
    const QHash<QString, QStringList> bucket = _byValue.value(cipherId);
    QVector<qint64> values;

    values.reserve(bucket.size());

    for (QHash<QString, QStringList>::const_iterator it = bucket.constBegin();
         it != bucket.constEnd(); ++it) {
        bool ok = false;
        const qint64 v = it.key().toLongLong(&ok);

        // Values past 64 bits are skipped on purpose: near-matching a
        // Multiplicative product by a tolerance of one or two is meaningless
        // when the numbers are twenty-nine digits long.
        if (ok)
            values.append(v);
    }

    std::sort(values.begin(), values.end());

    return values;
}
