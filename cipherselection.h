#ifndef CIPHERSELECTION_H
#define CIPHERSELECTION_H

#include <QVector>

// Which ciphers the program shows, in one place.
//
// It used to be seven global bools - single_r_on, francis_on, satanic_on and
// the rest - declared in mainwindow.h, read in four files, and written by a
// dialog with seven hand-placed checkboxes. Adding a cipher meant a global, a
// checkbox, a line in the dialog's constructor, a line in its accept handler,
// and a branch everywhere the value was printed. Nineteen more would have meant
// ninety-five edits and one forgotten.
//
// Those seven globals still exist and still mean what they meant: the existing
// code reads them and must keep working unchanged. This owns them. Setting a
// selection here updates them, so nothing downstream has to know this file is
// what moved.

namespace cipherselection {

// The ciphers that are always shown, whatever anyone selects. They are the four
// the program has printed since before any of this was configurable, and they
// are offered in the dialog as ticked and disabled rather than hidden - a
// checkbox you cannot untick is clearer than a column that appears from
// nowhere.
bool isAlwaysOn(int cipherId);

// The selected ciphers, in table order, including the always-on four.
QVector<int> enabled();

bool isEnabled(int cipherId);

// Replaces the selection, writes it to QSettings and updates the legacy
// globals. Always-on ciphers are included whether or not they were passed.
void setEnabled(const QVector<int> &cipherIds);

// Reads the stored selection and updates the globals. Call once at startup.
//
// Anything with no stored setting stays off, which is what keeps existing
// output identical after an upgrade: a user who has never opened the dialog
// sees exactly the columns they saw yesterday.
void load();

} // namespace cipherselection

#endif // CIPHERSELECTION_H
