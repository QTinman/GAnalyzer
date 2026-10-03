#ifndef CIPHERVALUE_H
#define CIPHERVALUE_H

#include <QString>
#include <cstdint>
#include <vector>

// What a cipher produces.
//
// Every cipher here used to return int, and for all but one it still could:
// the largest ordinary total a long phrase reaches is in the thousands.
// Multiplicative is the exception. It multiplies the letter values together, so
// a twelve-letter word already passes 64 bits and a twenty-letter phrase passes
// it by many orders of magnitude - silently, wrapping to a number that is not
// wrong so much as meaningless, and which would then be indexed and matched
// against other meaningless numbers.
//
// So a value is either a machine integer or an arbitrary-precision one, and the
// difference is invisible to everything downstream: comparison, display and the
// history index all go through this type. The index keys on key(), a canonical
// decimal string, so a big value and a small one are the same kind of key.

namespace ciphers {

// The small part of the arbitrary-precision arithmetic: enough for a running
// product and nothing more. Non-negative, because no cipher produces a negative
// letter value.
class BigUInt
{
public:
    BigUInt();
    explicit BigUInt(quint64 value);

    // The only operation a product needs.
    BigUInt &operator*=(quint32 factor);

    bool operator==(const BigUInt &other) const;
    bool operator!=(const BigUInt &other) const { return !(*this == other); }

    bool isZero() const;

    // True when the value fits in a qint64, with it written to out.
    bool fitsIn64(qint64 *out) const;

    QString toString() const;

private:
    // Base 1e9, least significant limb first. Base 1e9 rather than 2^32 so
    // printing is a matter of writing out the limbs, which is the only other
    // thing this class has to do.
    std::vector<quint32> _limbs;

    void trim();
};

class CipherValue
{
public:
    CipherValue();
    CipherValue(qint64 value);                  // implicit on purpose: most are
    static CipherValue fromBig(const BigUInt &value);

    // True when the value does not fit in a qint64 and is held as a BigUInt.
    bool isBig() const { return _big; }

    // Only meaningful when !isBig(). A big value returns 0, so callers that
    // care must ask first - and the ones that do not care should use key().
    qint64 toLongLong() const { return _small; }

    QString toString() const;

    // The canonical form the history index and any equality test use. Same
    // number, same string, whether it arrived big or small.
    QString key() const { return toString(); }

    bool operator==(const CipherValue &other) const;
    bool operator!=(const CipherValue &other) const { return !(*this == other); }
    bool operator<(const CipherValue &other) const;

private:
    bool    _big;
    qint64  _small;
    BigUInt _large;
};

} // namespace ciphers

#endif // CIPHERVALUE_H
