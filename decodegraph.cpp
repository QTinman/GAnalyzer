#include "decodegraph.h"

#include <QJsonArray>

namespace decode {

QJsonObject Node::toJson() const
{
    QJsonObject o;

    o.insert("id", id);
    o.insert("type", Graph::typeName(type));
    o.insert("label", label);

    if (!detail.isEmpty())
        o.insert("detail", detail);

    return o;
}

QJsonObject Edge::toJson() const
{
    QJsonObject o;

    o.insert("from", from);
    o.insert("to", to);
    o.insert("type", Graph::typeName(type));

    if (cipherId >= 0) {
        o.insert("cipher", cipherName);
        o.insert("cipherId", cipherId);
    }

    if (!value.isEmpty()) {
        // A string, not a number: a Multiplicative value can be twenty-nine
        // digits, and JSON numbers are doubles in most readers - which would
        // round it and hand a model a value the program never calculated.
        o.insert("value", value);
    }

    if (type == EdgeType::NearValue)
        o.insert("difference", difference);

    return o;
}

void Graph::addNode(const Node &node)
{
    if (hasNode(node.id))
        return;

    _nodes.append(node);
}

void Graph::addEdge(const Edge &edge)
{
    _edges.append(edge);
}

bool Graph::hasNode(const QString &id) const
{
    for (int i = 0; i < _nodes.size(); ++i) {
        if (_nodes.at(i).id == id)
            return true;
    }

    return false;
}

void Graph::clear()
{
    _nodes.clear();
    _edges.clear();
}

QJsonObject Graph::toJson() const
{
    QJsonArray nodes;
    QJsonArray edges;

    for (int i = 0; i < _nodes.size(); ++i)
        nodes.append(_nodes.at(i).toJson());

    for (int i = 0; i < _edges.size(); ++i)
        edges.append(_edges.at(i).toJson());

    QJsonObject o;

    o.insert("nodes", nodes);
    o.insert("edges", edges);

    return o;
}

QString Graph::typeName(NodeType type)
{
    switch (type) {
    case NodeType::Phrase:      return QStringLiteral("phrase");
    case NodeType::HistoryWord: return QStringLiteral("historyWord");
    case NodeType::Number:      return QStringLiteral("number");
    case NodeType::Date:        return QStringLiteral("date");
    case NodeType::Eclipse:     return QStringLiteral("eclipse");
    case NodeType::LunarPhase:  return QStringLiteral("lunarPhase");
    }

    return QStringLiteral("unknown");
}

QString Graph::typeName(EdgeType type)
{
    switch (type) {
    case EdgeType::HasValue:    return QStringLiteral("hasValue");
    case EdgeType::SharesValue: return QStringLiteral("sharesValue");
    case EdgeType::NearValue:   return QStringLiteral("nearValue");
    case EdgeType::OccursOn:    return QStringLiteral("occursOn");
    case EdgeType::DerivedFrom: return QStringLiteral("derivedFrom");
    }

    return QStringLiteral("unknown");
}

QString Graph::phraseId(const QString &phrase)
{
    return QStringLiteral("phrase:") + phrase.trimmed().toLower();
}

QString Graph::wordId(const QString &word)
{
    return QStringLiteral("word:") + word.trimmed().toLower();
}

QString Graph::numberId(int cipherId, const QString &value)
{
    // A number node is per cipher. 156 in English Ordinal and 156 in Chaldean
    // are not the same fact, and merging them would invent a relationship
    // between every pair of words that happen to hit the same integer in two
    // unrelated systems.
    return QStringLiteral("number:%1:%2").arg(cipherId).arg(value);
}

} // namespace decode
