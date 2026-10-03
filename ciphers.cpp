#include "ciphers.h"
#include "tools.h"

#include <cctype>

namespace ciphers {

namespace {

// ---------------------------------------------------------------- letter tables
//
// A..Z. Each table is written out in full rather than generated, so that it can
// be read against the source it came from without running anything.

// Z=36 .. A=61, as specified. Equivalent to 62 - ordinal, which is how it is
// checked in the tests: the table is the statement, the formula is the proof.
const int reverseSatanic[26] = {
    61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49,
    48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36
};

// The first twenty-six primes.
const int primes[26] = {
      2,   3,   5,   7,  11,  13,  17,  19,  23,  29,  31,  37,  41,
     43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97, 101
};

// Chaldean numerology. Note that no letter is ever 9, and that the pattern is
// not a plain repeating 1-8: G is 3, X is 5, Z is 7. It is a named historical
// table, not an arithmetic rule, and must not be "tidied up" into one.
const int chaldean[26] = {
      1,   2,   3,   4,   5,   8,   3,   5,   1,   1,   2,   3,   4,
      5,   7,   8,   1,   2,   3,   4,   6,   6,   6,   5,   1,   7
};

// English Scrabble tile values.
const int scrabble[26] = {
      1,   3,   3,   2,   1,   4,   2,   4,   1,   8,   5,   1,   3,
      1,   1,   3,  10,   1,   1,   1,   1,   4,   4,   8,   4,  10
};

// Position on a QWERTY keyboard, reading the three letter rows in order:
// QWERTYUIOP = 1..10, ASDFGHJKL = 11..19, ZXCVBNM = 20..26.
const int qwerty[26] = {
     11,  24,  22,  13,   3,  14,  15,  16,   8,  17,  18,  19,  26,
     25,   9,  10,   1,   4,  12,   5,   7,  23,   2,  21,   6,  20
};

// English Ordinal, for the building variants.
const int ordinal[26] = {
      1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,
     14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26
};

// Reverse Ordinal: 27 - ordinal, matching tools.cpp's reverse().
const int reverseOrdinal[26] = {
     26,  25,  24,  23,  22,  21,  20,  19,  18,  17,  16,  15,  14,
     13,  12,  11,  10,   9,   8,   7,   6,   5,   4,   3,   2,   1
};

// ---------------------------------------------------------------- the table

const Cipher table[] = {
    // id                      name                       legacy  red rev type letters          transform
    { EnglishOrdinal,         "English Ordinal",          true,   0,  0,  0,   nullptr,         Transform::Sum },
    { FullReduction,          "Full Reduction",           true,   1,  0,  0,   nullptr,         Transform::Sum },
    { ReverseOrdinal,         "Reverse Ordinal",          true,   0,  1,  0,   nullptr,         Transform::Sum },
    { ReverseFullReduction,   "Reverse Full Reduction",   true,   1,  1,  0,   nullptr,         Transform::Sum },
    { SingleReduction,        "Single Reduction",         true,   0,  0,  1,   nullptr,         Transform::Sum },
    { FrancisBacon,           "Francis Bacon",            true,   0,  0,  2,   nullptr,         Transform::Sum },
    { Satanic,                "Satanic",                  true,   0,  0,  3,   nullptr,         Transform::Sum },
    { Jewish,                 "Jewish",                   true,   0,  0,  4,   nullptr,         Transform::Sum },
    { Sumerian,               "Sumerian",                 true,   0,  0,  5,   nullptr,         Transform::Sum },
    { ReverseSumerian,        "Reverse Sumerian",         true,   0,  1,  5,   nullptr,         Transform::Sum },
    { Fibonacci,              "Fibonacci",                true,   0,  0,  6,   nullptr,         Transform::Sum },

    { ReverseSatanic,         "Reverse Satanic",          false,  0,  0,  0,   reverseSatanic,  Transform::Sum },
    { Primes,                 "Primes",                   false,  0,  0,  0,   primes,          Transform::Sum },
    { Chaldean,               "Chaldean",                 false,  0,  0,  0,   chaldean,        Transform::Sum },
    { Scrabble,               "Scrabble",                 false,  0,  0,  0,   scrabble,        Transform::Sum },
    { Qwerty,                 "QWERTY Keyboard",          false,  0,  0,  0,   qwerty,          Transform::Sum },
    { BuildingValue,          "Building Value",           false,  0,  0,  0,   ordinal,         Transform::Building },
    { ReverseBuildingValue,   "Reverse Building Value",   false,  0,  0,  0,   reverseOrdinal,  Transform::Building },
    { WordReduction,          "Word Reduction",           false,  0,  0,  0,   ordinal,         Transform::DigitalRoot },
    { ReverseWordReduction,   "Reverse Word Reduction",   false,  0,  0,  0,   reverseOrdinal,  Transform::DigitalRoot },
};

const std::vector<Cipher> &theTable()
{
    static const std::vector<Cipher> v(table, table + sizeof(table) / sizeof(table[0]));
    return v;
}

// A..Z as 0..25, or -1 for anything else. Case is ignored, which is what every
// cipher here wants and what the original engine does by lower-casing first.
int letterIndex(char c)
{
    const unsigned char u = static_cast<unsigned char>(c);

    if (u >= 'a' && u <= 'z') return u - 'a';
    if (u >= 'A' && u <= 'Z') return u - 'A';

    return -1;
}

int tableValue(const std::string &word, const Cipher &cipher)
{
    const int *letters = cipher.letters;
    int sum = 0;
    int running = 0;

    for (size_t i = 0; i < word.size(); ++i) {
        const int index = letterIndex(word[i]);

        if (index < 0)
            continue;   // spaces, punctuation and digits carry no value here

        const int v = letters[index];

        if (cipher.transform == Transform::Building) {
            // The running total after each letter, all added together: ABC is
            // 1 + (1+2) + (1+2+3) = 10. Spaces do not restart the run, so a
            // phrase builds across its words.
            running += v;
            sum += running;
        } else {
            sum += v;
        }
    }

    if (cipher.transform == Transform::DigitalRoot)
        return digitalRoot(sum);

    return sum;
}

} // namespace

// ---------------------------------------------------------------- public

const std::vector<Cipher> &all()
{
    return theTable();
}

const Cipher *byId(int id)
{
    const std::vector<Cipher> &t = theTable();

    for (size_t i = 0; i < t.size(); ++i) {
        if (t[i].id == id)
            return &t[i];
    }

    return nullptr;
}

int value(const std::string &word, int cipherId)
{
    const Cipher *cipher = byId(cipherId);

    if (cipher == nullptr)
        return 0;

    // The original engine, unchanged and unduplicated. Whatever it does with
    // ampersands, digits and the letter S, it keeps doing.
    if (cipher->legacy)
        return getwordnumericvalue(word, cipher->legacyReduced, cipher->legacyReversed, cipher->legacyType);

    return tableValue(word, *cipher);
}

int value(const QString &word, int cipherId)
{
    return value(word.toStdString(), cipherId);
}

std::vector<int> allValues(const std::string &word)
{
    const std::vector<Cipher> &t = theTable();
    std::vector<int> values;

    values.reserve(t.size());

    for (size_t i = 0; i < t.size(); ++i)
        values.push_back(value(word, t[i].id));

    return values;
}

QString name(int cipherId)
{
    const Cipher *cipher = byId(cipherId);

    return cipher != nullptr ? QString::fromLatin1(cipher->name) : QString();
}

int digitalRoot(int n)
{
    if (n < 0)
        n = -n;

    while (n > 9) {
        int sum = 0;

        while (n > 0) {
            sum += n % 10;
            n /= 10;
        }

        n = sum;
    }

    return n;
}

} // namespace ciphers
