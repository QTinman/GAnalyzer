#ifndef CIPHERS_H
#define CIPHERS_H

#include <QString>
#include <string>
#include <vector>

// A table of ciphers, beside the original engine rather than in place of it.
//
// Every cipher the program had before this file existed is still calculated by
// getwordnumericvalue() in tools.cpp, which is left exactly as it was. The
// entries below marked Legacy call straight into it, so their numbers cannot
// drift from what the program has always produced - not because the arithmetic
// was copied carefully, but because it is not copied at all. Saved history,
// settings and printed output keep meaning the same thing.
//
// New ciphers are described by a table of twenty-six letter values plus an
// optional transform. That is enough for every system added here, and it means
// the next one is a row of numbers rather than another branch inside a
// forty-line if-chain.

namespace ciphers {

// Stable identifiers. The numbers are written down because they end up in
// settings and in saved analyses: inserting a cipher in the middle of this list
// must never renumber the others. Append only.
enum CipherId {
    EnglishOrdinal        = 0,
    FullReduction         = 1,
    ReverseOrdinal        = 2,
    ReverseFullReduction  = 3,
    SingleReduction       = 4,
    FrancisBacon          = 5,
    Satanic               = 6,
    Jewish                = 7,
    Sumerian              = 8,
    ReverseSumerian       = 9,
    Fibonacci             = 10,

    // Added on feature/extended-ciphers-analyzer-ai.
    ReverseSatanic        = 11,
    Primes                = 12,
    Chaldean              = 13,
    Scrabble              = 14,
    Qwerty                = 15,
    BuildingValue         = 16,
    ReverseBuildingValue  = 17,
    WordReduction         = 18,
    ReverseWordReduction  = 19,

    CipherCount           = 20
};

// What happens to the letter values once they are looked up.
enum class Transform {
    Sum,          // add them together - what almost every cipher does
    Building,     // add the running totals: A, A+B, A+B+C ... then sum those
    DigitalRoot   // sum, then fold to a single digit
};

struct Cipher {
    int         id;
    const char *name;        // exactly the spelling the program already shows
    bool        legacy;      // true: delegate to getwordnumericvalue()
    int         legacyReduced;
    int         legacyReversed;
    int         legacyType;
    const int  *letters;     // 26 values, A..Z; null when legacy
    Transform   transform;
};

// Every cipher, in id order.
const std::vector<Cipher> &all();

// One cipher by id, or null if there is no such id.
const Cipher *byId(int id);

// The value of a word or phrase in one cipher.
//
// Non-letters are skipped, as the original engine does, with one deliberate
// exception kept for compatibility: in the legacy ciphers the digits 1-9 add
// their own face value, because they always have. New ciphers ignore digits -
// a Scrabble board has no tile for "7".
int value(const std::string &word, int cipherId);

int value(const QString &word, int cipherId);

// Every cipher's value for one word, indexed by CipherId.
std::vector<int> allValues(const std::string &word);

// The display name, or an empty string for an unknown id.
QString name(int cipherId);

// Folds a number to a single digit by repeatedly adding its digits.
//
// Deliberately not tools.cpp's reduce(), which subtracts 9 or 18 and is only
// correct for a single letter's value. Applied to a whole word's total -
// "Washington" is 125 in English Ordinal - reduce() returns 107, which is not a
// reduction of anything. This returns 8.
int digitalRoot(int n);

} // namespace ciphers

#endif // CIPHERS_H
