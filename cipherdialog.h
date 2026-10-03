#ifndef CIPHERDIALOG_H
#define CIPHERDIALOG_H

#include <QDialog>

class QListWidget;

// The cipher chooser.
//
// It used to be seven checkboxes placed by hand in cipherdialog.ui, each read
// and written by name in the constructor and the accept handler. The list is
// now built from the cipher table, so a cipher added there appears here with no
// further work - which is the point, the table having gone from eleven entries
// to thirty.
//
// No .ui file any more. This repository commits the generated ui_*.h headers,
// so a form change needs a matching regeneration by hand and compiles against
// stale widget names when that is forgotten; it has already cost one commit.
// A dialog assembled in code cannot fall out of step with itself.

class cipherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit cipherDialog(QWidget *parent = nullptr);
    ~cipherDialog() override;

private slots:
    void onAccepted();
    void onSelectAll();
    void onSelectNone();

private:
    QListWidget *_list;
};

#endif // CIPHERDIALOG_H
