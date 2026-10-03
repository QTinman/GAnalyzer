#include "analyzerdialog.h"
#include "ciphers.h"

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QSplitter>
#include <QTableWidget>
#include <QTextBrowser>
#include <QVBoxLayout>

// gcalc.cpp owns the settings helpers the rest of the program uses.
QVariant loadsettings(QString settings);

namespace {

// Column order of the results table.
enum Column {
    ColumnCandidate = 0,
    ColumnScore     = 1,
    ColumnCiphers   = 2,
    ColumnValues    = 3,
    ColumnCount     = 4
};

QString escape(const QString &text)
{
    return text.toHtmlEscaped();
}

} // namespace

AnalyzerDialog::AnalyzerDialog(QWidget *parent)
    : QDialog(parent), _hasResult(false)
{
    setWindowTitle(tr("Analyze against history"));
    resize(1000, 700);

    buildUi();
    loadCiphers();
    loadHistory();
    loadAiSettings();
}

AnalyzerDialog::~AnalyzerDialog() = default;

void AnalyzerDialog::buildUi()
{
    QVBoxLayout *outer = new QVBoxLayout(this);

    // ---- phrase ---------------------------------------------------------

    QHBoxLayout *top = new QHBoxLayout;

    top->addWidget(new QLabel(tr("Phrase:"), this));

    _phrase = new QLineEdit(this);
    _phrase->setPlaceholderText(tr("a word or phrase to look for"));
    top->addWidget(_phrase, 1);

    _analyze = new QPushButton(tr("Analyze"), this);
    _analyze->setDefault(true);
    top->addWidget(_analyze);

    outer->addLayout(top);

    _historyLabel = new QLabel(this);
    _historyLabel->setStyleSheet("color: gray;");
    outer->addWidget(_historyLabel);

    // ---- ciphers, source, options ---------------------------------------

    QHBoxLayout *middle = new QHBoxLayout;

    QGroupBox *cipherBox = new QGroupBox(tr("Ciphers"), this);
    QVBoxLayout *cipherLayout = new QVBoxLayout(cipherBox);

    _ciphers = new QListWidget(cipherBox);
    _ciphers->setSelectionMode(QAbstractItemView::NoSelection);
    cipherLayout->addWidget(_ciphers);

    QHBoxLayout *cipherButtons = new QHBoxLayout;
    _selectAll = new QPushButton(tr("All"), cipherBox);
    _selectNone = new QPushButton(tr("None"), cipherBox);
    cipherButtons->addWidget(_selectAll);
    cipherButtons->addWidget(_selectNone);
    cipherButtons->addStretch(1);
    cipherLayout->addLayout(cipherButtons);

    middle->addWidget(cipherBox, 1);

    QVBoxLayout *right = new QVBoxLayout;

    QGroupBox *sourceBox = new QGroupBox(tr("Search"), this);
    QVBoxLayout *sourceLayout = new QVBoxLayout(sourceBox);

    _sourceHistory = new QRadioButton(tr("History"), sourceBox);
    _sourceDictionary = new QRadioButton(tr("Dictionary"), sourceBox);
    _sourceBoth = new QRadioButton(tr("History and dictionary"), sourceBox);
    _sourceHistory->setChecked(true);

    // There is no dictionary in this program yet. The options are shown so the
    // shape of the thing is honest, and disabled so nobody waits for an answer
    // that cannot come.
    _sourceDictionary->setEnabled(false);
    _sourceBoth->setEnabled(false);
    _sourceDictionary->setToolTip(tr("No dictionary is installed."));
    _sourceBoth->setToolTip(tr("No dictionary is installed."));

    sourceLayout->addWidget(_sourceHistory);
    sourceLayout->addWidget(_sourceDictionary);
    sourceLayout->addWidget(_sourceBoth);
    right->addWidget(sourceBox);

    QGroupBox *optionBox = new QGroupBox(tr("Matching"), this);
    QFormLayout *optionLayout = new QFormLayout(optionBox);

    _minimumMatches = new QSpinBox(optionBox);
    _minimumMatches->setRange(1, 40);
    _minimumMatches->setValue(1);
    optionLayout->addRow(tr("At least this many ciphers must agree:"), _minimumMatches);

    _includeNear = new QCheckBox(tr("Also show near matches"), optionBox);
    optionLayout->addRow(_includeNear);

    _nearTolerance = new QSpinBox(optionBox);
    _nearTolerance->setRange(1, 100);
    _nearTolerance->setValue(1);
    _nearTolerance->setEnabled(false);
    optionLayout->addRow(tr("Near means within:"), _nearTolerance);

    QLabel *nearNote = new QLabel(
        tr("A near match is listed but scores nothing. A number that is close\n"
           "is not a number that agrees."), optionBox);
    nearNote->setStyleSheet("color: gray;");
    optionLayout->addRow(nearNote);

    right->addWidget(optionBox);
    right->addStretch(1);

    middle->addLayout(right, 1);
    outer->addLayout(middle);

    // ---- results ---------------------------------------------------------

    _results = new QTableWidget(0, ColumnCount, this);
    _results->setHorizontalHeaderLabels(QStringList()
                                        << tr("Candidate") << tr("Score")
                                        << tr("Matching ciphers") << tr("Values"));
    _results->setSelectionBehavior(QAbstractItemView::SelectRows);
    _results->setSelectionMode(QAbstractItemView::SingleSelection);
    _results->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _results->setSortingEnabled(true);
    _results->horizontalHeader()->setStretchLastSection(true);
    _results->verticalHeader()->setVisible(false);

    _scoring = new QLabel(this);
    _scoring->setWordWrap(true);
    _scoring->setStyleSheet("color: gray;");

    _detail = new QTextBrowser(this);
    _detail->setMinimumHeight(120);

    QSplitter *split = new QSplitter(Qt::Vertical, this);
    split->addWidget(_results);
    split->addWidget(_detail);
    split->setStretchFactor(0, 3);
    split->setStretchFactor(1, 2);

    outer->addWidget(split, 1);
    outer->addWidget(_scoring);

    // ---- AI ---------------------------------------------------------------

    _aiBox = new QGroupBox(tr("AI interpretation (optional)"), this);
    QVBoxLayout *aiLayout = new QVBoxLayout(_aiBox);

    QFormLayout *aiForm = new QFormLayout;

    _aiEnabled = new QCheckBox(tr("Enabled"), _aiBox);
    aiForm->addRow(_aiEnabled);

    _aiProvider = new QComboBox(_aiBox);
    _aiProvider->addItem(ai::Settings::providerName(ai::Provider::Anthropic));
    _aiProvider->addItem(ai::Settings::providerName(ai::Provider::OpenAI));
    _aiProvider->addItem(ai::Settings::providerName(ai::Provider::OpenAICompatible));
    _aiProvider->addItem(ai::Settings::providerName(ai::Provider::Ollama));
    aiForm->addRow(tr("Provider:"), _aiProvider);

    _aiModel = new QLineEdit(_aiBox);
    aiForm->addRow(tr("Model:"), _aiModel);

    _aiEndpoint = new QLineEdit(_aiBox);
    _aiEndpoint->setPlaceholderText(tr("blank for the provider's default"));
    aiForm->addRow(tr("Endpoint:"), _aiEndpoint);

    _aiKeyVariable = new QLineEdit(_aiBox);
    _aiKeyVariable->setPlaceholderText(tr("e.g. ANTHROPIC_API_KEY"));
    _aiKeyVariable->setToolTip(tr("The NAME of an environment variable holding the key.\n"
                                  "The key itself is never stored by this program."));
    aiForm->addRow(tr("API key environment variable:"), _aiKeyVariable);

    _aiTemperature = new QDoubleSpinBox(_aiBox);
    _aiTemperature->setRange(0.0, 2.0);
    _aiTemperature->setSingleStep(0.1);
    _aiTemperature->setValue(0.2);
    aiForm->addRow(tr("Temperature:"), _aiTemperature);

    _aiMaxCandidates = new QSpinBox(_aiBox);
    _aiMaxCandidates->setRange(1, 500);
    _aiMaxCandidates->setValue(25);
    _aiMaxCandidates->setToolTip(tr("How many candidates may be described to the model.\n"
                                    "Your history file is never sent."));
    aiForm->addRow(tr("Candidates sent at most:"), _aiMaxCandidates);

    aiLayout->addLayout(aiForm);

    QHBoxLayout *aiButtons = new QHBoxLayout;
    _aiAnalyze = new QPushButton(tr("Analyze with AI"), _aiBox);
    _aiAnalyze->setEnabled(false);
    aiButtons->addWidget(_aiAnalyze);

    _aiStatus = new QLabel(_aiBox);
    _aiStatus->setStyleSheet("color: gray;");
    aiButtons->addWidget(_aiStatus, 1);

    aiLayout->addLayout(aiButtons);

    _aiOutput = new QTextBrowser(_aiBox);
    _aiOutput->setMinimumHeight(100);
    aiLayout->addWidget(_aiOutput);

    outer->addWidget(_aiBox);

    // ---- wiring -----------------------------------------------------------

    connect(_analyze, &QPushButton::clicked, this, &AnalyzerDialog::onAnalyze);
    connect(_phrase, &QLineEdit::returnPressed, this, &AnalyzerDialog::onAnalyze);
    connect(_selectAll, &QPushButton::clicked, this, &AnalyzerDialog::onSelectAllCiphers);
    connect(_selectNone, &QPushButton::clicked, this, &AnalyzerDialog::onSelectNoCiphers);
    connect(_results, &QTableWidget::itemSelectionChanged, this, &AnalyzerDialog::onSelectionChanged);
    connect(_includeNear, &QCheckBox::toggled, _nearTolerance, &QSpinBox::setEnabled);
    connect(_aiAnalyze, &QPushButton::clicked, this, &AnalyzerDialog::onAnalyzeWithAi);

    connect(_aiEnabled, &QCheckBox::toggled, this, &AnalyzerDialog::onAiSettingsChanged);
    connect(_aiProvider, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AnalyzerDialog::onAiSettingsChanged);
    connect(_aiModel, &QLineEdit::editingFinished, this, &AnalyzerDialog::onAiSettingsChanged);
    connect(_aiEndpoint, &QLineEdit::editingFinished, this, &AnalyzerDialog::onAiSettingsChanged);
    connect(_aiKeyVariable, &QLineEdit::editingFinished, this, &AnalyzerDialog::onAiSettingsChanged);
}

void AnalyzerDialog::loadCiphers()
{
    const std::vector<ciphers::Cipher> &all = ciphers::all();

    for (size_t i = 0; i < all.size(); ++i) {
        QListWidgetItem *item = new QListWidgetItem(ciphers::name(all[i].id), _ciphers);

        item->setData(Qt::UserRole, all[i].id);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);

        // On by default, except the ones that always equal another cipher:
        // ticking Mirror as well as Reverse Ordinal cannot add evidence, and
        // the analyzer would drop it anyway.
        const bool redundant = all[i].redundantWith >= 0;

        item->setCheckState(redundant ? Qt::Unchecked : Qt::Checked);

        if (redundant) {
            item->setToolTip(tr("Always equal to %1, so it is counted once.")
                                 .arg(ciphers::name(all[i].redundantWith)));
        }
    }
}

void AnalyzerDialog::setHistory(const QStringList &words)
{
    _index.setWords(words);
    _historyLabel->setText(tr("%1 stored word(s).").arg(_index.count()));
}

void AnalyzerDialog::loadHistory()
{
    const QString path = loadsettings("historyfile").toString();
    QString error;

    if (path.isEmpty() || !_index.load(path, &error)) {
        // An unreadable history is not an empty one, and the difference has to
        // be visible or every search silently answers "nothing found".
        _historyLabel->setText(error.isEmpty()
                                   ? tr("No history file is configured.")
                                   : error);
        _historyLabel->setStyleSheet("color: #aa0000;");
        return;
    }

    _historyLabel->setStyleSheet("color: gray;");
    _historyLabel->setText(tr("%1 stored word(s) from %2.").arg(_index.count()).arg(path));
}

void AnalyzerDialog::loadAiSettings()
{
    const ai::Settings s = ai::Settings::fromSettings();

    _aiEnabled->setChecked(s.enabled);
    _aiProvider->setCurrentText(ai::Settings::providerName(s.provider));
    _aiModel->setText(s.model);
    _aiEndpoint->setText(s.endpoint);
    _aiKeyVariable->setText(s.apiKeyVariable);
    _aiTemperature->setValue(s.temperature);
    _aiMaxCandidates->setValue(s.maximumCandidates);

    onAiSettingsChanged();
}

analyzer::Options AnalyzerDialog::optionsFromUi() const
{
    analyzer::Options options;

    for (int i = 0; i < _ciphers->count(); ++i) {
        const QListWidgetItem *item = _ciphers->item(i);

        if (item->checkState() == Qt::Checked)
            options.ciphers.append(item->data(Qt::UserRole).toInt());
    }

    options.source = _sourceDictionary->isChecked() ? analyzer::Source::Dictionary
                   : _sourceBoth->isChecked()       ? analyzer::Source::Both
                                                    : analyzer::Source::History;

    options.includeNearMatches = _includeNear->isChecked();
    options.nearTolerance = _nearTolerance->value();
    options.minimumCipherMatches = _minimumMatches->value();

    return options;
}

ai::Settings AnalyzerDialog::aiSettingsFromUi() const
{
    ai::Settings s;

    s.enabled = _aiEnabled->isChecked();
    s.provider = ai::Settings::providerFromName(_aiProvider->currentText());
    s.model = _aiModel->text().trimmed();
    s.endpoint = _aiEndpoint->text().trimmed();
    s.apiKeyVariable = _aiKeyVariable->text().trimmed();
    s.temperature = _aiTemperature->value();
    s.maximumCandidates = _aiMaxCandidates->value();

    return s;
}

void AnalyzerDialog::onSelectAllCiphers()
{
    for (int i = 0; i < _ciphers->count(); ++i)
        _ciphers->item(i)->setCheckState(Qt::Checked);
}

void AnalyzerDialog::onSelectNoCiphers()
{
    for (int i = 0; i < _ciphers->count(); ++i)
        _ciphers->item(i)->setCheckState(Qt::Unchecked);
}

void AnalyzerDialog::onAnalyze()
{
    const QString phrase = _phrase->text().trimmed();

    if (phrase.isEmpty()) {
        _scoring->setText(tr("Enter a phrase to analyze."));
        return;
    }

    const analyzer::Options options = optionsFromUi();

    if (options.ciphers.isEmpty()) {
        _scoring->setText(tr("Select at least one cipher."));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    _result = analyzer::analyze(phrase, _index, options);
    _hasResult = true;
    QApplication::restoreOverrideCursor();

    showResult();
    onAiSettingsChanged();
}

void AnalyzerDialog::showResult()
{
    _results->setSortingEnabled(false);
    _results->setRowCount(0);
    _aiOutput->clear();

    for (int i = 0; i < _result.candidates.size(); ++i) {
        const analyzer::Candidate &candidate = _result.candidates.at(i);

        QStringList cipherNames;
        QStringList values;

        for (int m = 0; m < candidate.matches.size(); ++m) {
            const analyzer::Match &match = candidate.matches.at(m);

            if (!match.exact)
                continue;

            cipherNames.append(match.cipherName);
            values.append(QString("%1 = %2").arg(match.cipherName, match.inputValue.toString()));
        }

        const int row = _results->rowCount();
        _results->insertRow(row);

        QTableWidgetItem *word = new QTableWidgetItem(candidate.word);
        word->setData(Qt::UserRole, i);          // index into _result.candidates
        _results->setItem(row, ColumnCandidate, word);

        // setData rather than a string, so the column sorts by number and not
        // by the spelling of a number.
        QTableWidgetItem *score = new QTableWidgetItem;
        score->setData(Qt::DisplayRole, candidate.score);
        _results->setItem(row, ColumnScore, score);

        QTableWidgetItem *count = new QTableWidgetItem;
        count->setData(Qt::DisplayRole, candidate.exactMatches);
        count->setToolTip(cipherNames.join(", "));
        _results->setItem(row, ColumnCiphers, count);

        _results->setItem(row, ColumnValues, new QTableWidgetItem(values.join("   ")));
    }

    _results->setSortingEnabled(true);
    _results->sortByColumn(ColumnScore, Qt::DescendingOrder);
    _results->resizeColumnsToContents();

    QString summary;

    if (_result.candidates.isEmpty()) {
        summary = tr("No stored word shares a value with \"%1\" in the selected ciphers. "
                     "%2 word(s) searched.")
                      .arg(_result.phrase).arg(_result.wordsSearched);
    } else {
        summary = tr("%1 candidate(s) from %2 stored word(s).")
                      .arg(_result.candidates.size()).arg(_result.wordsSearched);
    }

    _scoring->setText(summary + "  " + _result.scoringExplanation);

    // The phrase's own values, so the table above can be checked against them.
    QString html = QString("<b>%1</b><br><table cellpadding='3'>").arg(escape(_result.phrase));

    for (int i = 0; i < _result.values.size(); ++i) {
        html += QString("<tr><td>%1</td><td align='right'><tt>%2</tt></td></tr>")
                    .arg(escape(ciphers::name(_result.values.at(i).first)),
                         escape(_result.values.at(i).second.toString()));
    }

    html += "</table>";
    _detail->setHtml(html);
}

void AnalyzerDialog::onSelectionChanged()
{
    const QList<QTableWidgetItem *> selected = _results->selectedItems();

    if (selected.isEmpty())
        return;

    const QTableWidgetItem *first = _results->item(selected.first()->row(), ColumnCandidate);

    if (first != nullptr)
        showCandidateDetail(first->data(Qt::UserRole).toInt());
}

void AnalyzerDialog::showCandidateDetail(int index)
{
    if (index < 0 || index >= _result.candidates.size())
        return;

    const analyzer::Candidate &candidate = _result.candidates.at(index);

    QString html = QString("<b>%1</b> &nbsp; vs &nbsp; <b>%2</b><br>"
                           "score %3, %4 cipher(s) agree")
                       .arg(escape(_result.phrase), escape(candidate.word))
                       .arg(candidate.score).arg(candidate.exactMatches);

    if (candidate.nearMatches > 0)
        html += tr(", %1 near").arg(candidate.nearMatches);

    html += "<br><br><table cellpadding='4'>"
            "<tr><th align='left'>Cipher</th>"
            "<th align='right'>" + escape(_result.phrase) + "</th>"
            "<th align='right'>" + escape(candidate.word) + "</th>"
            "<th></th></tr>";

    for (int m = 0; m < candidate.matches.size(); ++m) {
        const analyzer::Match &match = candidate.matches.at(m);

        // Both values, always, so "why are these related" is answered on the
        // screen rather than taken on trust.
        html += QString("<tr><td>%1</td><td align='right'><tt>%2</tt></td>"
                        "<td align='right'><tt>%3</tt></td><td>%4</td></tr>")
                    .arg(escape(match.cipherName),
                         escape(match.inputValue.toString()),
                         escape(match.candidateValue.toString()),
                         match.exact ? tr("match")
                                     : tr("near, off by %1 - scores nothing")
                                           .arg(match.difference));
    }

    html += "</table>";
    _detail->setHtml(html);
}

void AnalyzerDialog::onAiSettingsChanged()
{
    const ai::Settings settings = aiSettingsFromUi();

    settings.save();

    QString reason;
    const ai::Status usable = ai::usability(settings, &reason);

    _aiAnalyze->setEnabled(_hasResult && usable == ai::Status::Ok
                           && !_result.candidates.isEmpty());

    if (usable != ai::Status::Ok) {
        _aiStatus->setText(reason);
        _aiStatus->setStyleSheet("color: gray;");
        return;
    }

    if (!_hasResult || _result.candidates.isEmpty()) {
        _aiStatus->setText(tr("Run an analysis first."));
        _aiStatus->setStyleSheet("color: gray;");
        return;
    }

    _aiStatus->setStyleSheet(settings.isLocal() ? "color: #117711;" : "color: #aa6600;");
    _aiStatus->setText(settings.isLocal()
                           ? tr("Ready. This provider runs on this computer.")
                           : tr("Ready. This will send data over the internet."));
}

void AnalyzerDialog::onAnalyzeWithAi()
{
    if (!_hasResult || _result.candidates.isEmpty())
        return;

    const ai::Settings settings = aiSettingsFromUi();

    // Said plainly, with the detail, before anything goes anywhere - and the
    // default button is No.
    const QString notice = ai::privacyNotice(_result, settings);
    const QMessageBox::StandardButton answer =
        QMessageBox::question(this, tr("Send this analysis?"), notice,
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (answer != QMessageBox::Yes)
        return;

    _aiAnalyze->setEnabled(false);
    _aiStatus->setText(tr("Waiting for the model..."));
    QApplication::setOverrideCursor(Qt::WaitCursor);

    const ai::Client client(settings);
    const ai::Analysis analysis = client.interpret(_result);

    QApplication::restoreOverrideCursor();
    _aiAnalyze->setEnabled(true);

    if (!analysis.isOk()) {
        _aiStatus->setStyleSheet("color: #aa0000;");
        _aiStatus->setText(analysis.error);

        // The calculated result is untouched and stays on screen. A failure
        // here is a failure to add commentary, not a failed analysis.
        _aiOutput->setHtml(QString("<i>%1</i>").arg(escape(analysis.error)));
        return;
    }

    _aiStatus->setStyleSheet("color: #117711;");
    _aiStatus->setText(tr("Done."));

    _aiOutput->setHtml(QString("<p style='color:#666;'><i>%1</i></p><hr>%2")
                           .arg(escape(analysis.disclaimer()),
                                escape(analysis.text).replace("\n", "<br>")));
}
