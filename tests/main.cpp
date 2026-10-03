#include <QtTest>
#include <QString>

// Two suites, one binary. QTEST_APPLESS_MAIN would give each its own main, so
// each exposes an entry point and this runs them.
//
// The first argument may name one suite - "ciphers" or "analyzer" - and the
// rest are passed to QTest as usual. Running one at a time matters more than it
// looks: QTest writes its report to whatever -o names, so two suites in one run
// overwrite each other's file and only the second one's totals survive.
//
//     ganatests                 both suites
//     ganatests ciphers         just the cipher tests
//     ganatests analyzer -o r.txt,txt
//
// The exit code is the number of failures across whatever ran.

int runCipherTests(int argc, char **argv);
int runAnalyzerTests(int argc, char **argv);
int runAiTests(int argc, char **argv);
int runSelectionTests(int argc, char **argv);

int main(int argc, char **argv)
{
    const QString first = argc > 1 ? QString::fromLocal8Bit(argv[1]).toLower() : QString();
    const bool named = (first == "ciphers" || first == "analyzer" || first == "ai" || first == "selection");

    // Hide the suite name from QTest, which would take it for a test to run.
    int testArgc = argc;
    char **testArgv = argv;
    QVector<char *> forwarded;

    if (named) {
        forwarded.append(argv[0]);

        for (int i = 2; i < argc; ++i)
            forwarded.append(argv[i]);

        testArgc = forwarded.size();
        testArgv = forwarded.data();
    }

    int failures = 0;

    if (!named || first == "ciphers")
        failures += runCipherTests(testArgc, testArgv);

    if (!named || first == "analyzer")
        failures += runAnalyzerTests(testArgc, testArgv);

    if (!named || first == "ai")
        failures += runAiTests(testArgc, testArgv);

    if (!named || first == "selection")
        failures += runSelectionTests(testArgc, testArgv);

    return failures;
}
