#include "cipherselection.h"
#include "ciphers.h"

#include <QSet>
#include <QSettings>
#include <QString>

// The seven that the rest of the program reads directly, and the settings group
// everything else uses.
extern bool single_r_on, francis_on, satanic_on, jewish_on, sumerian_on,
            rev_sumerian_on, fibonacci_on;
extern QString appgroup;

namespace cipherselection {

namespace {

QSet<int> &selection()
{
    static QSet<int> ids;
    return ids;
}

bool &loaded()
{
    static bool done = false;
    return done;
}

// Where the four that are never optional are written down. Printed by printword
// before any selection is consulted, so offering to turn them off would be an
// offer the program cannot keep.
const int alwaysOn[] = {
    ciphers::EnglishOrdinal,
    ciphers::FullReduction,
    ciphers::ReverseOrdinal,
    ciphers::ReverseFullReduction
};

// The seven optional ciphers that predate this file, each with the global the
// existing code reads. Keeping the globals in step is what lets printword,
// findword and searchwords stay exactly as they are.
struct LegacyFlag {
    int    cipherId;
    bool  *flag;
};

const LegacyFlag legacyFlags[] = {
    { ciphers::SingleReduction,  &single_r_on },
    { ciphers::FrancisBacon,     &francis_on },
    { ciphers::Satanic,          &satanic_on },
    { ciphers::Jewish,           &jewish_on },
    { ciphers::Sumerian,         &sumerian_on },
    { ciphers::ReverseSumerian,  &rev_sumerian_on },
    { ciphers::Fibonacci,        &fibonacci_on }
};

// Exactly where the rest of the program keeps its settings. MainWindow uses
// QSettings("QTinman", appgroup) and then beginGroup(appgroup); the default
// QSettings constructor would have written somewhere else entirely, and a
// setting would appear to vanish between sessions.
//
// QSettings cannot be returned by value - it is a QObject - so this hands back
// the name and each caller opens its own.
QString applicationName()
{
    return appgroup.isEmpty() ? QStringLiteral("GAnalyzer") : appgroup;
}

QString keyFor(int cipherId)
{
    // By id, not by name. A cipher renamed in the UI must not lose a user's
    // setting, and two ciphers must never share a key.
    return QString("cipher%1").arg(cipherId);
}

void syncLegacyFlags()
{
    for (size_t i = 0; i < sizeof(legacyFlags) / sizeof(legacyFlags[0]); ++i)
        *legacyFlags[i].flag = selection().contains(legacyFlags[i].cipherId);
}

} // namespace

bool isAlwaysOn(int cipherId)
{
    for (size_t i = 0; i < sizeof(alwaysOn) / sizeof(alwaysOn[0]); ++i) {
        if (alwaysOn[i] == cipherId)
            return true;
    }

    return false;
}

void load()
{
    QSettings store(QStringLiteral("QTinman"), applicationName());

    store.beginGroup(applicationName());
    store.beginGroup(QStringLiteral("ciphers"));

    selection().clear();

    const std::vector<ciphers::Cipher> &all = ciphers::all();

    for (size_t i = 0; i < all.size(); ++i) {
        const int id = all[i].id;

        if (isAlwaysOn(id)) {
            selection().insert(id);
            continue;
        }

        // Default off. A user who has never opened the dialog must see the
        // same columns after this upgrade as before it.
        if (store.value(keyFor(id), false).toBool())
            selection().insert(id);
    }

    store.endGroup();
    store.endGroup();

    loaded() = true;
    syncLegacyFlags();
}

QVector<int> enabled()
{
    if (!loaded())
        load();

    QVector<int> result;
    const std::vector<ciphers::Cipher> &all = ciphers::all();

    // Table order, so the output columns never reshuffle between runs.
    for (size_t i = 0; i < all.size(); ++i) {
        if (selection().contains(all[i].id))
            result.append(all[i].id);
    }

    return result;
}

bool isEnabled(int cipherId)
{
    if (!loaded())
        load();

    return selection().contains(cipherId);
}

void setEnabled(const QVector<int> &cipherIds)
{
    selection().clear();

    for (int i = 0; i < cipherIds.size(); ++i) {
        if (ciphers::byId(cipherIds.at(i)) != nullptr)
            selection().insert(cipherIds.at(i));
    }

    for (size_t i = 0; i < sizeof(alwaysOn) / sizeof(alwaysOn[0]); ++i)
        selection().insert(alwaysOn[i]);

    QSettings store(QStringLiteral("QTinman"), applicationName());

    store.beginGroup(applicationName());
    store.beginGroup(QStringLiteral("ciphers"));

    const std::vector<ciphers::Cipher> &all = ciphers::all();

    for (size_t i = 0; i < all.size(); ++i) {
        if (!isAlwaysOn(all[i].id))
            store.setValue(keyFor(all[i].id), selection().contains(all[i].id));
    }

    store.endGroup();
    store.endGroup();

    loaded() = true;
    syncLegacyFlags();
}

} // namespace cipherselection
