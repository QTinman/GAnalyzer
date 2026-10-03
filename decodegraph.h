#ifndef DECODEGRAPH_H
#define DECODEGRAPH_H

#include <QJsonObject>
#include <QString>
#include <QVector>

// What the analyzer produces, and the only thing the AI layer is ever shown.
//
// The program already relates four different kinds of thing to each other -
// phrases, numbers, calendar dates and astronomical events - and until now it
// expressed those relationships as sentences of HTML. That is fine to read and
// useless to reason over: there is no way to ask "which numbers connect this
// phrase to that date" except by parsing prose the program just finished
// writing.
//
// So an analysis is a small graph. Nodes are the things; edges are the reasons
// two things are related, each carrying the cipher and the value that produced
// it. The ranked candidate list the UI shows is a view of this graph, and the
// JSON sent to a model is this graph serialised - so the model is given typed
// facts with their provenance rather than a paragraph it has to believe.
//
// It is deliberately not a general graph database. It is built per analysis,
// holds the subgraph around one input phrase, and is thrown away afterwards.

namespace decode {

enum class NodeType {
    Phrase,       // what the user typed
    HistoryWord,  // a stored word
    Number,       // a cipher value that two or more things share
    Date,
    Eclipse,
    LunarPhase
};

enum class EdgeType {
    HasValue,     // phrase or word -> number, in a named cipher
    SharesValue,  // phrase <-> word, because some cipher agrees
    NearValue,    // the same, but off by a little: never counted as agreement
    OccursOn,     // event -> date
    DerivedFrom   // number -> number (prime, triangular, reduction ...)
};

struct Node {
    QString  id;       // stable within one graph: "phrase:eclipse", "word:murder"
    NodeType type;
    QString  label;
    QString  detail;   // free text for the UI; never parsed

    QJsonObject toJson() const;
};

struct Edge {
    QString  from;
    QString  to;
    EdgeType type;

    int      cipherId;   // -1 when the edge is not about a cipher
    QString  cipherName;
    QString  value;      // decimal string: big values index and print the same
    int      difference; // NearValue only: how far apart, 0 for exact

    QJsonObject toJson() const;
};

class Graph
{
public:
    // Adding the same id twice keeps the first node, so callers can add freely
    // without checking - a word reached through three ciphers is one node.
    void addNode(const Node &node);
    void addEdge(const Edge &edge);

    const QVector<Node> &nodes() const { return _nodes; }
    const QVector<Edge> &edges() const { return _edges; }

    bool hasNode(const QString &id) const;

    int nodeCount() const { return _nodes.size(); }
    int edgeCount() const { return _edges.size(); }

    void clear();

    // The whole graph, for a model to read. Stable key order so two identical
    // analyses produce byte-identical payloads - which is what makes a cached
    // or logged request comparable with a later one.
    QJsonObject toJson() const;

    static QString typeName(NodeType type);
    static QString typeName(EdgeType type);

    // Node id helpers, so the same thing is never given two ids by accident.
    static QString phraseId(const QString &phrase);
    static QString wordId(const QString &word);
    static QString numberId(int cipherId, const QString &value);

private:
    QVector<Node> _nodes;
    QVector<Edge> _edges;
};

} // namespace decode

#endif // DECODEGRAPH_H
