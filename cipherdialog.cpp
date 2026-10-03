#include "cipherdialog.h"
#include "cipherselection.h"
#include "ciphers.h"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

cipherDialog::cipherDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Ciphers"));
    resize(420, 560);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *note = new QLabel(
        tr("Which ciphers to show. The first four are always shown."), this);
    note->setWordWrap(true);
    note->setStyleSheet("color: gray;");
    layout->addWidget(note);

    _list = new QListWidget(this);
    _list->setSelectionMode(QAbstractItemView::NoSelection);
    layout->addWidget(_list, 1);

    const std::vector<ciphers::Cipher> &all = ciphers::all();

    for (size_t i = 0; i < all.size(); ++i) {
        const int id = all[i].id;

        QListWidgetItem *item = new QListWidgetItem(ciphers::name(id), _list);

        item->setData(Qt::UserRole, id);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(cipherselection::isEnabled(id) ? Qt::Checked : Qt::Unchecked);

        if (cipherselection::isAlwaysOn(id)) {
            // Shown ticked and disabled rather than hidden: a checkbox you
            // cannot untick explains itself, while a column that appears from
            // nowhere does not.
            item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
            item->setToolTip(tr("Always shown."));
        } else if (all[i].redundantWith >= 0) {
            item->setToolTip(tr("Always equal to %1.").arg(ciphers::name(all[i].redundantWith)));
        }
    }

    QHBoxLayout *buttons = new QHBoxLayout;

    QPushButton *selectAll = new QPushButton(tr("All"), this);
    QPushButton *selectNone = new QPushButton(tr("None"), this);

    buttons->addWidget(selectAll);
    buttons->addWidget(selectNone);
    buttons->addStretch(1);
    layout->addLayout(buttons);

    QDialogButtonBox *box = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    layout->addWidget(box);

    connect(selectAll, &QPushButton::clicked, this, &cipherDialog::onSelectAll);
    connect(selectNone, &QPushButton::clicked, this, &cipherDialog::onSelectNone);
    connect(box, &QDialogButtonBox::accepted, this, &cipherDialog::onAccepted);
    connect(box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(box, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

cipherDialog::~cipherDialog() = default;

void cipherDialog::onSelectAll()
{
    for (int i = 0; i < _list->count(); ++i) {
        QListWidgetItem *item = _list->item(i);

        if (item->flags() & Qt::ItemIsEnabled)
            item->setCheckState(Qt::Checked);
    }
}

void cipherDialog::onSelectNone()
{
    for (int i = 0; i < _list->count(); ++i) {
        QListWidgetItem *item = _list->item(i);

        if (item->flags() & Qt::ItemIsEnabled)
            item->setCheckState(Qt::Unchecked);
    }
}

void cipherDialog::onAccepted()
{
    QVector<int> chosen;

    for (int i = 0; i < _list->count(); ++i) {
        const QListWidgetItem *item = _list->item(i);

        if (item->checkState() == Qt::Checked)
            chosen.append(item->data(Qt::UserRole).toInt());
    }

    // Writes the settings and updates the seven legacy globals the rest of the
    // program reads, so printword and findword need to know nothing about this.
    cipherselection::setEnabled(chosen);
}
