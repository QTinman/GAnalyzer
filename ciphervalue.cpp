#include "ciphervalue.h"

namespace ciphers {

namespace {
const quint32 Base = 1000000000u;   // 1e9: one limb prints as nine digits
}

// ---------------------------------------------------------------- BigUInt

BigUInt::BigUInt()
{
}

BigUInt::BigUInt(quint64 value)
{
    while (value > 0) {
        _limbs.push_back(static_cast<quint32>(value % Base));
        value /= Base;
    }
}

BigUInt &BigUInt::operator*=(quint32 factor)
{
    if (factor == 0) {
        _limbs.clear();
        return *this;
    }

    quint64 carry = 0;

    for (size_t i = 0; i < _limbs.size(); ++i) {
        const quint64 product = static_cast<quint64>(_limbs[i]) * factor + carry;

        _limbs[i] = static_cast<quint32>(product % Base);
        carry = product / Base;
    }

    while (carry > 0) {
        _limbs.push_back(static_cast<quint32>(carry % Base));
        carry /= Base;
    }

    trim();
    return *this;
}

bool BigUInt::operator==(const BigUInt &other) const
{
    return _limbs == other._limbs;
}

bool BigUInt::isZero() const
{
    return _limbs.empty();
}

bool BigUInt::fitsIn64(qint64 *out) const
{
    // Three limbs is at most 1e27, well past the range, so anything longer is
    // decided without arithmetic.
    if (_limbs.size() > 3)
        return false;

    quint64 value = 0;

    for (size_t i = _limbs.size(); i > 0; --i) {
        // Would multiplying by the base overflow before the add?
        if (value > (0x7FFFFFFFFFFFFFFFull - _limbs[i - 1]) / Base)
            return false;

        value = value * Base + _limbs[i - 1];
    }

    if (out != nullptr)
        *out = static_cast<qint64>(value);

    return true;
}

QString BigUInt::toString() const
{
    if (_limbs.empty())
        return QStringLiteral("0");

    QString text = QString::number(_limbs.back());

    for (size_t i = _limbs.size() - 1; i > 0; --i) {
        // Every limb but the most significant is exactly nine digits, zeros
        // included - forgetting the padding is how 1,000,000,002 prints as 12.
        text += QString("%1").arg(_limbs[i - 1], 9, 10, QChar('0'));
    }

    return text;
}

void BigUInt::trim()
{
    while (!_limbs.empty() && _limbs.back() == 0)
        _limbs.pop_back();
}

// ---------------------------------------------------------------- CipherValue

CipherValue::CipherValue()
    : _big(false), _small(0)
{
}

CipherValue::CipherValue(qint64 value)
    : _big(false), _small(value)
{
}

CipherValue CipherValue::fromBig(const BigUInt &value)
{
    CipherValue result;
    qint64 small = 0;

    // Stay small whenever the number allows it, so that a short word's
    // Multiplicative value compares equal to the same number arrived at any
    // other way.
    if (value.fitsIn64(&small)) {
        result._small = small;
        result._big = false;
    } else {
        result._large = value;
        result._big = true;
    }

    return result;
}

QString CipherValue::toString() const
{
    return _big ? _large.toString() : QString::number(_small);
}

bool CipherValue::operator==(const CipherValue &other) const
{
    if (_big != other._big)
        return false;   // one is beyond 64 bits and the other is not

    return _big ? _large == other._large : _small == other._small;
}

bool CipherValue::operator<(const CipherValue &other) const
{
    if (!_big && !other._big)
        return _small < other._small;

    if (_big != other._big)
        return other._big;   // anything small is below anything big

    // Both big: longer decimal string wins, otherwise lexicographic.
    const QString a = toString();
    const QString b = other.toString();

    return a.size() != b.size() ? a.size() < b.size() : a < b;
}

} // namespace ciphers
