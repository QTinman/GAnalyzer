#ifndef ANALYZERDIALOG_H
#define ANALYZERDIALOG_H

#include "aiprovider.h"
#include "analyzer.h"
#include "historyindex.h"

#include <QDialog>

class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;
class QRadioButton;
class QSpinBox;
class QTableWidget;
class QTextBrowser;

// The Analyze-against-history window.
//
// Built in code rather than as a .ui file on purpose. This repository commits
// the generated ui_*.h headers, so every change to a .ui has to be followed by
// regenerating its header by hand - which has already been its own commit once,
// and which silently compiles against stale widget names when it is forgotten.
// A dialog assembled here cannot fall out of step with itself.

class AnalyzerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AnalyzerDialog(QWidget *parent = nullptr);
    ~AnalyzerDialog() override;

    // Lets a test or a caller supply the words instead of reading the file.
    void setHistory(const QStringList &words);

private slots:
    void onAnalyze();
    void onSelectAllCiphers();
    void onSelectNoCiphers();
    void onSelectionChanged();
    void onAnalyzeWithAi();
    void onAiSettingsChanged();

private:
    void buildUi();
    void loadCiphers();
    void loadHistory();
    void loadAiSettings();
    void showResult();
    void showCandidateDetail(int row);

    analyzer::Options optionsFromUi() const;
    ai::Settings aiSettingsFromUi() const;

    HistoryIndex     _index;
    analyzer::Result _result;
    bool             _hasResult;

    QLineEdit     *_phrase;
    QPushButton   *_analyze;
    QLabel        *_historyLabel;

    QListWidget   *_ciphers;
    QPushButton   *_selectAll;
    QPushButton   *_selectNone;

    QRadioButton  *_sourceHistory;
    QRadioButton  *_sourceDictionary;
    QRadioButton  *_sourceBoth;

    QCheckBox     *_includeNear;
    QSpinBox      *_nearTolerance;
    QSpinBox      *_minimumMatches;

    QTableWidget  *_results;
    QLabel        *_scoring;
    QTextBrowser  *_detail;

    QGroupBox      *_aiBox;
    QCheckBox      *_aiEnabled;

    // The settings rows, folded away unless AI is switched on.
    QWidget        *_aiSettings;

    QComboBox      *_aiProvider;
    QLineEdit      *_aiModel;
    QLineEdit      *_aiEndpoint;
    QLineEdit      *_aiKeyVariable;
    QDoubleSpinBox *_aiTemperature;
    QSpinBox       *_aiMaxCandidates;
    QPushButton    *_aiAnalyze;
    QLabel         *_aiStatus;
    QTextBrowser   *_aiOutput;
};

#endif // ANALYZERDIALOG_H
