#include "ciphers.h"
#include "tools.h"

#include <QChar>

namespace ciphers {

namespace {

// ---------------------------------------------------------------- Latin tables
//
// A..Z. Written out in full rather than generated, so each can be read against
// the source it came from without running anything.

// Z=36 .. A=61. Equivalent to 62 - ordinal, which is how the test checks it:
// the table is the statement, the formula is the proof.
const int reverseSatanic[26] = {
    61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49,
    48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36
};

// The first twenty-six primes.
const int primes[26] = {
      2,   3,   5,   7,  11,  13,  17,  19,  23,  29,  31,  37,  41,
     43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97, 101
};

// Chaldean numerology. No letter is ever 9, and the pattern is not a plain
// repeating 1-8: G is 3, X is 5, Z is 7. A named historical table, not an
// arithmetic rule, and not to be "tidied up" into one.
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

const int ordinal[26] = {
      1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,
     14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26
};

// Reverse Ordinal: 27 - ordinal, matching tools.cpp's reverse().
const int reverseOrdinal[26] = {
     26,  25,  24,  23,  22,  21,  20,  19,  18,  17,  16,  15,  14,
     13,  12,  11,  10,   9,   8,   7,   6,   5,   4,   3,   2,   1
};

// The Elizabethan twenty-four letter alphabet, in which I and J are one letter
// and U and V are one letter:
//
//   A B C D E F G H I K L M N O P Q R S T V W X Y Z
//
// so J shares I's value of 9 and V shares U's value of 20, and Z is 24 rather
// than 26. This is a distinct historical family, not modern ordinal values with
// two letters quietly merged - the whole alphabet after I is shifted.
const int elizabethan[26] = {
      1,   2,   3,   4,   5,   6,   7,   8,   9,   9,  10,  11,  12,
     13,  14,  15,  16,  17,  18,  19,  20,  20,  21,  22,  23,  24
};

// ---------------------------------------------------------------- other scripts

// Greek isopsephy, the historical numeral system. Includes the three letters
// kept only as numerals - stigma (6), koppa (90) and sampi (900) - and both
// forms of sigma.
const ScriptLetter greekIsopsephy[] = {
    { 0x3B1,   1 }, { 0x3B2,   2 }, { 0x3B3,   3 }, { 0x3B4,   4 }, { 0x3B5,   5 },
    { 0x3DB,   6 },                                                                  // stigma
    { 0x3B6,   7 }, { 0x3B7,   8 }, { 0x3B8,   9 }, { 0x3B9,  10 }, { 0x3BA,  20 },
    { 0x3BB,  30 }, { 0x3BC,  40 }, { 0x3BD,  50 }, { 0x3BE,  60 }, { 0x3BF,  70 },
    { 0x3C0,  80 },
    { 0x3D9,  90 }, { 0x3DF,  90 },                                                  // koppa, both forms
    { 0x3C1, 100 }, { 0x3C3, 200 }, { 0x3C2, 200 },                                  // sigma, final sigma
    { 0x3C4, 300 }, { 0x3C5, 400 }, { 0x3C6, 500 }, { 0x3C7, 600 }, { 0x3C8, 700 },
    { 0x3C9, 800 },
    { 0x3E1, 900 }                                                                   // sampi
};

// The modern twenty-four letter Greek alphabet in order.
const ScriptLetter greekOrdinal[] = {
    { 0x3B1,  1 }, { 0x3B2,  2 }, { 0x3B3,  3 }, { 0x3B4,  4 }, { 0x3B5,  5 },
    { 0x3B6,  6 }, { 0x3B7,  7 }, { 0x3B8,  8 }, { 0x3B9,  9 }, { 0x3BA, 10 },
    { 0x3BB, 11 }, { 0x3BC, 12 }, { 0x3BD, 13 }, { 0x3BE, 14 }, { 0x3BF, 15 },
    { 0x3C0, 16 }, { 0x3C1, 17 },
    { 0x3C3, 18 }, { 0x3C2, 18 },                                                    // sigma, final sigma
    { 0x3C4, 19 }, { 0x3C5, 20 }, { 0x3C6, 21 }, { 0x3C7, 22 }, { 0x3C8, 23 },
    { 0x3C9, 24 }
};

// Abjad, the eastern (Mashriqi) order: abjad hawwaz hutti kalaman sa'fas
// qarashat thakhadh dazagh.
//
// The hamza-bearing alef forms and alef maksura are given their base letter's
// value, and teh marbuta is given heh's, because that is how the text is read.
// Nothing is transliterated: these are Arabic letters being normalised to other
// Arabic letters.
const ScriptLetter abjad[] = {
    { 0x627,    1 }, { 0x623,    1 }, { 0x625,    1 }, { 0x622,    1 },   // alef and hamza forms
    { 0x628,    2 }, { 0x62C,    3 }, { 0x62F,    4 },
    { 0x647,    5 }, { 0x629,    5 },                                     // heh, teh marbuta
    { 0x648,    6 }, { 0x632,    7 }, { 0x62D,    8 }, { 0x637,    9 },
    { 0x64A,   10 }, { 0x649,   10 },                                     // yeh, alef maksura
    { 0x643,   20 }, { 0x644,   30 }, { 0x645,   40 }, { 0x646,   50 },
    { 0x633,   60 }, { 0x639,   70 }, { 0x641,   80 }, { 0x635,   90 },
    { 0x642,  100 }, { 0x631,  200 }, { 0x634,  300 }, { 0x62A,  400 },
    { 0x62B,  500 }, { 0x62E,  600 }, { 0x630,  700 }, { 0x636,  800 },
    { 0x638,  900 }, { 0x63A, 1000 }
};

// The modern twenty-eight letter Arabic alphabet in hija'i order.
const ScriptLetter arabicOrdinal[] = {
    { 0x627,  1 }, { 0x623,  1 }, { 0x625,  1 }, { 0x622,  1 },
    { 0x628,  2 }, { 0x62A,  3 }, { 0x629,  3 }, { 0x62B,  4 }, { 0x62C,  5 },
    { 0x62D,  6 }, { 0x62E,  7 }, { 0x62F,  8 }, { 0x630,  9 }, { 0x631, 10 },
    { 0x632, 11 }, { 0x633, 12 }, { 0x634, 13 }, { 0x635, 14 }, { 0x636, 15 },
    { 0x637, 16 }, { 0x638, 17 }, { 0x639, 18 }, { 0x63A, 19 }, { 0x641, 20 },
    { 0x642, 21 }, { 0x643, 22 }, { 0x644, 23 }, { 0x645, 24 }, { 0x646, 25 },
    { 0x647, 26 }, { 0x648, 27 },
    { 0x64A, 28 }, { 0x649, 28 }
};

// Cyrillic numerals, the Church Slavonic system. Note that two letters of the
// modern Russian alphabet carry no numeric value at all - б and ж were never
// numerals - and that several numerals are letters Russian no longer uses.
const ScriptLetter cyrillicNumerals[] = {
    { 0x430,   1 },                                   // а
    { 0x432,   2 }, { 0x433,   3 }, { 0x434,   4 },   // в г д
    { 0x435,   5 },                                   // е
    { 0x455,   6 },                                   // ѕ dzelo
    { 0x437,   7 }, { 0x438,   8 },                   // з и
    { 0x473,   9 },                                   // ѳ fita
    { 0x456,  10 },                                   // і
    { 0x43A,  20 }, { 0x43B,  30 }, { 0x43C,  40 }, { 0x43D,  50 },
    { 0x46F,  60 },                                   // ѯ ksi
    { 0x43E,  70 }, { 0x43F,  80 },
    { 0x447,  90 },                                   // ч
    { 0x440, 100 }, { 0x441, 200 }, { 0x442, 300 }, { 0x443, 400 },
    { 0x444, 500 }, { 0x445, 600 },
    { 0x471, 700 },                                   // ѱ psi
    { 0x461, 800 },                                   // ѡ omega
    { 0x446, 900 }                                    // ц
};

// The modern thirty-three letter Russian alphabet in order, ё in its place
// between е and ж.
const ScriptLetter russianOrdinal[] = {
    { 0x430,  1 }, { 0x431,  2 }, { 0x432,  3 }, { 0x433,  4 }, { 0x434,  5 },
    { 0x435,  6 }, { 0x451,  7 }, { 0x436,  8 }, { 0x437,  9 }, { 0x438, 10 },
    { 0x439, 11 }, { 0x43A, 12 }, { 0x43B, 13 }, { 0x43C, 14 }, { 0x43D, 15 },
    { 0x43E, 16 }, { 0x43F, 17 }, { 0x440, 18 }, { 0x441, 19 }, { 0x442, 20 },
    { 0x443, 21 }, { 0x444, 22 }, { 0x445, 23 }, { 0x446, 24 }, { 0x447, 25 },
    { 0x448, 26 }, { 0x449, 27 }, { 0x44A, 28 }, { 0x44B, 29 }, { 0x44C, 30 },
    { 0x44D, 31 }, { 0x44E, 32 }, { 0x44F, 33 }
};

template <size_t N>
int count(const ScriptLetter (&)[N]) { return static_cast<int>(N); }

// ---------------------------------------------------------------- the table

#define LATIN(id, nm, tbl, sub, agg, dup) \
    { id, nm, false, 0, 0, 0, Script::Latin, tbl, nullptr, 0, sub, agg, dup }

#define LEGACY(id, nm, red, rev, typ) \
    { id, nm, true, red, rev, typ, Script::Latin, nullptr, nullptr, 0, \
      Substitution::None, Aggregate::Sum, -1 }

const Cipher table[] = {
    LEGACY(EnglishOrdinal,        "English Ordinal",        0, 0, 0),
    LEGACY(FullReduction,         "Full Reduction",         1, 0, 0),
    LEGACY(ReverseOrdinal,        "Reverse Ordinal",        0, 1, 0),
    LEGACY(ReverseFullReduction,  "Reverse Full Reduction", 1, 1, 0),
    LEGACY(SingleReduction,       "Single Reduction",       0, 0, 1),
    LEGACY(FrancisBacon,          "Francis Bacon",          0, 0, 2),
    LEGACY(Satanic,               "Satanic",                0, 0, 3),
    LEGACY(Jewish,                "Jewish",                 0, 0, 4),
    LEGACY(Sumerian,              "Sumerian",               0, 0, 5),
    LEGACY(ReverseSumerian,       "Reverse Sumerian",       0, 1, 5),
    LEGACY(Fibonacci,             "Fibonacci",              0, 0, 6),

    LATIN(ReverseSatanic,        "Reverse Satanic",        reverseSatanic, Substitution::None,   Aggregate::Sum,         -1),
    LATIN(Primes,                "Primes",                 primes,         Substitution::None,   Aggregate::Sum,         -1),
    LATIN(Chaldean,              "Chaldean",               chaldean,       Substitution::None,   Aggregate::Sum,         -1),
    LATIN(Scrabble,              "Scrabble",               scrabble,       Substitution::None,   Aggregate::Sum,         -1),
    LATIN(Qwerty,                "QWERTY Keyboard",        qwerty,         Substitution::None,   Aggregate::Sum,         -1),
    LATIN(BuildingValue,         "Building Value",         ordinal,        Substitution::None,   Aggregate::Building,    -1),
    LATIN(ReverseBuildingValue,  "Reverse Building Value", reverseOrdinal, Substitution::None,   Aggregate::Building,    -1),
    LATIN(WordReduction,         "Word Reduction",         ordinal,        Substitution::None,   Aggregate::DigitalRoot, -1),
    LATIN(ReverseWordReduction,  "Reverse Word Reduction", reverseOrdinal, Substitution::None,   Aggregate::DigitalRoot, -1),

    LATIN(Multiplicative,        "Multiplicative",         ordinal,        Substitution::None,   Aggregate::Product,     -1),
    LATIN(WordSquare,            "Word Square",            ordinal,        Substitution::None,   Aggregate::SquareOfSum, -1),
    LATIN(ElizabethanOrdinal,    "Elizabethan Ordinal",    elizabethan,    Substitution::None,   Aggregate::Sum,         -1),
    LATIN(MirrorAtbash,          "Mirror (Atbash)",        ordinal,        Substitution::Atbash, Aggregate::Sum,         ReverseOrdinal),

    { GreekIsopsephy,   "Greek Isopsephy",    false, 0,0,0, Script::Greek,    nullptr, greekIsopsephy,   count(greekIsopsephy),   Substitution::None, Aggregate::Sum, -1 },
    { GreekOrdinal,     "Greek Ordinal",      false, 0,0,0, Script::Greek,    nullptr, greekOrdinal,     count(greekOrdinal),     Substitution::None, Aggregate::Sum, -1 },
    { Abjad,            "Abjad",              false, 0,0,0, Script::Arabic,   nullptr, abjad,            count(abjad),            Substitution::None, Aggregate::Sum, -1 },
    { ArabicOrdinal,    "Arabic Ordinal",     false, 0,0,0, Script::Arabic,   nullptr, arabicOrdinal,    count(arabicOrdinal),    Substitution::None, Aggregate::Sum, -1 },
    { CyrillicNumerals, "Cyrillic Numerals",  false, 0,0,0, Script::Cyrillic, nullptr, cyrillicNumerals, count(cyrillicNumerals), Substitution::None, Aggregate::Sum, -1 },
    { RussianOrdinal,   "Russian Ordinal",    false, 0,0,0, Script::Cyrillic, nullptr, russianOrdinal,   count(russianOrdinal),   Substitution::None, Aggregate::Sum, -1 },
};

#undef LATIN
#undef LEGACY

const std::vector<Cipher> &theTable()
{
    static const std::vector<Cipher> v(table, table + sizeof(table) / sizeof(table[0]));
    return v;
}

// A..Z as 0..25, or -1. Case is ignored, which is what every cipher here wants
// and what the original engine does by lower-casing first.
int latinIndex(QChar c)
{
    const ushort u = c.unicode();

    if (u >= 'a' && u <= 'z') return u - 'a';
    if (u >= 'A' && u <= 'Z') return u - 'A';

    return -1;
}

// The value of one character in a non-Latin cipher, or -1 when the character
// is not part of that script's table. Linear: the longest table is
// thirty-four entries, and a map would cost more to build than it saves.
int scriptValue(QChar c, const Cipher &cipher)
{
    const uint lower = c.toLower().unicode();

    for (int i = 0; i < cipher.entryCount; ++i) {
        if (cipher.entries[i].codepoint == lower)
            return cipher.entries[i].value;
    }

    return -1;
}

// Every letter value of a word, in order, skipping anything the cipher does not
// recognise. Separating this from the arithmetic is what lets the substitution
// be a step of its own rather than a special case inside each aggregate.
std::vector<int> letterValues(const QString &word, const Cipher &cipher)
{
    std::vector<int> values;

    values.reserve(word.size());

    for (int i = 0; i < word.size(); ++i) {
        if (cipher.script == Script::Latin) {
            int index = latinIndex(word.at(i));

            if (index < 0)
                continue;

            // The substitution happens here, to the letter, before anything
            // numeric is looked at.
            if (cipher.substitution == Substitution::Atbash)
                index = 25 - index;

            values.push_back(cipher.letters[index]);
        } else {
            const int v = scriptValue(word.at(i), cipher);

            if (v >= 0)
                values.push_back(v);
        }
    }

    return values;
}

CipherValue aggregate(const std::vector<int> &values, Aggregate how)
{
    if (values.empty())
        return CipherValue(0);

    switch (how) {
    case Aggregate::Product: {
        // Arbitrary precision: a twelve-letter word already passes 64 bits.
        BigUInt product(1);

        for (size_t i = 0; i < values.size(); ++i)
            product *= static_cast<quint32>(values[i] < 0 ? 0 : values[i]);

        return CipherValue::fromBig(product);
    }

    case Aggregate::Building: {
        // The running total after each letter, all added together: ABC is
        // 1 + (1+2) + (1+2+3) = 10. Spaces do not restart the run, so a phrase
        // builds across its words.
        qint64 running = 0, sum = 0;

        for (size_t i = 0; i < values.size(); ++i) {
            running += values[i];
            sum += running;
        }

        return CipherValue(sum);
    }

    default: {
        qint64 sum = 0;

        for (size_t i = 0; i < values.size(); ++i)
            sum += values[i];

        if (how == Aggregate::DigitalRoot)
            return CipherValue(digitalRoot(static_cast<int>(sum)));

        if (how == Aggregate::SquareOfSum)
            return CipherValue(sum * sum);   // the square of the total, not the sum of squares

        return CipherValue(sum);
    }
    }
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

CipherValue valueOf(const QString &word, int cipherId)
{
    const Cipher *cipher = byId(cipherId);

    if (cipher == nullptr)
        return CipherValue(0);

    // The original engine, unchanged and unduplicated. Whatever it does with
    // ampersands, digits and the letter S, it keeps doing.
    if (cipher->legacy) {
        return CipherValue(getwordnumericvalue(word.toStdString(),
                                               cipher->legacyReduced,
                                               cipher->legacyReversed,
                                               cipher->legacyType));
    }

    return aggregate(letterValues(word, *cipher), cipher->aggregate);
}

CipherValue valueOf(const std::string &word, int cipherId)
{
    return valueOf(QString::fromStdString(word), cipherId);
}

qint64 value(const QString &word, int cipherId)
{
    const CipherValue v = valueOf(word, cipherId);

    return v.isBig() ? -1 : v.toLongLong();
}

qint64 value(const std::string &word, int cipherId)
{
    return value(QString::fromStdString(word), cipherId);
}

std::vector<CipherValue> allValues(const QString &word)
{
    const std::vector<Cipher> &t = theTable();
    std::vector<CipherValue> values;

    values.reserve(t.size());

    for (size_t i = 0; i < t.size(); ++i)
        values.push_back(valueOf(word, t[i].id));

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
