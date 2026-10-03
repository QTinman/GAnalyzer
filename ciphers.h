#ifndef CIPHERS_H
#define CIPHERS_H

#include "ciphervalue.h"

#include <QString>
#include <string>
#include <vector>

// A table of ciphers, beside the original engine rather than in place of it.
//
// Every cipher the program had before this file existed is still calculated by
// getwordnumericvalue() in tools.cpp, which is left exactly as it was. The
// entries marked legacy call straight into it, so their numbers cannot drift
// from what the program has always produced - not because the arithmetic was
// copied carefully, but because it is not copied at all. Saved history,
// settings and printed output keep meaning the same thing.
//
// Everything else is described in three independent parts:
//
//   an alphabet   which letters exist and what each one is worth
//   a substitution   an optional letter-for-letter swap applied first
//   an aggregate     how the letter values become one number
//
// Keeping the substitution separate from the evaluation is what makes Atbash
// honest: see the note on Mirror below.

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

    ReverseSatanic        = 11,
    Primes                = 12,
    Chaldean              = 13,
    Scrabble              = 14,
    Qwerty                = 15,
    BuildingValue         = 16,
    ReverseBuildingValue  = 17,
    WordReduction         = 18,
    ReverseWordReduction  = 19,

    Multiplicative        = 20,
    WordSquare            = 21,
    ElizabethanOrdinal    = 22,
    MirrorAtbash          = 23,
    GreekIsopsephy        = 24,
    GreekOrdinal          = 25,
    Abjad                 = 26,
    ArabicOrdinal         = 27,
    CyrillicNumerals      = 28,
    RussianOrdinal        = 29,

    CipherCount           = 30
};

// Which characters a cipher reads. A cipher scores only its own script and
// ignores everything else, so Greek text has no English Ordinal value and
// Latin text has no Abjad value. Transliteration is deliberately not done:
// there is more than one accepted scheme for each of these, and picking one
// silently would turn an arbitrary choice into a number people reason about.
enum class Script {
    Latin,
    Greek,
    Arabic,
    Cyrillic
};

// Applied to each letter before its value is looked up.
enum class Substitution {
    None,
    Atbash      // A<->Z, B<->Y, ... within the Latin alphabet
};

// How the letter values are combined.
enum class Aggregate {
    Sum,         // add them
    Building,    // add the running totals: A, A+B, A+B+C ...
    DigitalRoot, // sum, then fold to a single digit
    Product,     // multiply them - arbitrary precision, see CipherValue
    SquareOfSum  // sum, then square. NOT the sum of each letter squared.
};

// One codepoint and what it is worth, for the non-Latin scripts.
struct ScriptLetter {
    uint   codepoint;   // lower case where the script has case
    int    value;
};

struct Cipher {
    int                 id;
    const char         *name;       // exactly the spelling the program shows
    bool                legacy;     // true: delegate to getwordnumericvalue()
    int                 legacyReduced;
    int                 legacyReversed;
    int                 legacyType;
    Script              script;
    const int          *letters;    // Latin: 26 values A..Z, else null
    const ScriptLetter *entries;    // non-Latin table, else null
    int                 entryCount;
    Substitution        substitution;
    Aggregate           aggregate;

    // The id of a cipher this one always equals numerically, or -1.
    //
    // Mirror (Atbash) is the reason this field exists. Atbash maps a letter at
    // position n to position 27-n, which is exactly what Reverse Ordinal
    // computes - so the two always agree, for every input, by arithmetic rather
    // than by coincidence. Both are offered, because the substitution is a real
    // and separately useful step, but the analyzer must not count them as two
    // pieces of evidence. One agreement is one agreement.
    int                 redundantWith;
};

// Every cipher, in id order.
const std::vector<Cipher> &all();

// One cipher by id, or null if there is no such id.
const Cipher *byId(int id);

// The value of a word or phrase in one cipher.
//
// Characters outside the cipher's own script are skipped, as are spaces and
// punctuation. One compatibility exception: the legacy ciphers add the face
// value of the digits 1-9, because they always have. The newer ones do not - a
// Scrabble set has no tile for "7".
CipherValue valueOf(const QString &word, int cipherId);
CipherValue valueOf(const std::string &word, int cipherId);

// Convenience for the ciphers that cannot overflow, which is all of them except
// Multiplicative. Returns -1 when the value does not fit in a qint64, a result
// no cipher can otherwise produce, so it cannot be mistaken for an answer.
qint64 value(const QString &word, int cipherId);
qint64 value(const std::string &word, int cipherId);

// Every cipher's value for one word, indexed by CipherId.
std::vector<CipherValue> allValues(const QString &word);

// The display name, or an empty string for an unknown id.
QString name(int cipherId);

// Folds a number to a single digit by repeatedly adding its digits.
//
// Deliberately not tools.cpp's reduce(), which subtracts 9 or 18 and is only
// correct for a single letter's value. Applied to a whole word's total -
// "Washington" is 130 in English Ordinal - reduce() returns 112, which is not a
// reduction of anything. This returns 4.
int digitalRoot(int n);

// TODO: logarithmic cipher variants.
//
// Asked for, and deliberately absent. No definition could be found that two
// sources agree on - whether the logarithm is taken of each letter value or of
// the word total, in which base, and how the result is rounded back to an
// integer all vary. Inventing one would produce numbers that look like evidence
// and are not. Pending a definition to implement against.

} // namespace ciphers

#endif // CIPHERS_H
