//
// Created by cirkul on 28.11.2020.
//

#ifndef LAB4_GRAPH_H
#define LAB4_GRAPH_H

#include <fstream>

template<typename NodeType, typename ArcType>
class Node;

template<typename NodeType, typename ArcType>
class IncidentArcIterator;

template<typename NodeType, typename ArcType>
class IncidentArcRevIterator;

template<typename NodeType, typename ArcType>
class AdjacentIterator;

template<typename NodeType, typename ArcType>
class AdjacentRevIterator;

class FileReader {
private:
    explicit FileReader(const char* path) {
        this->input.exceptions(std::ifstream::badbit|std::ifstream::failbit);
        this->input.open(path);
    }
    ~FileReader() {
        this->input.close();
    }
    std::ifstream input;

    template<typename NodeType, typename ArcType>
    friend class Graph;
};

template<typename NodeType, typename ArcType>
class Arc {
public:
    Arc() = default;

    Arc(Node<NodeType, ArcType> *bgn, Node<NodeType, ArcType> *end) :
            begin(bgn),
            end(end) {};

    Arc(Node<NodeType, ArcType> bgn, Node<NodeType, ArcType> end, ArcType data) : Arc(bgn, end) {
        this->data = data;
    };

    ArcType* data;

private:
    Node<NodeType, ArcType> *begin;
    Node<NodeType, ArcType> *end;


    template<typename NodeType1, typename ArcType1>
    friend class Graph;
    template<typename NodeType1, typename ArcType1>
    friend class IncidentArcIterator;
    template<typename NodeType1, typename ArcType1>
    friend class IncidentArcRevIterator;
    template<typename NodeType1, typename ArcType1>
    friend class AdjacentIterator;
    template<typename NodeType1, typename ArcType1>
    friend class AdjacentRevIterator;
    template<typename NodeType1, typename ArcType1>
    friend class NodeIterator;
    template<typename NodeType1, typename ArcType1>
    friend class NodeRevIterator;
    template<typename NodeType1, typename ArcType1>
    friend class ArcIterator;
    template<typename NodeType1, typename ArcType1>
    friend class AdjacentRevIterator;
};

template<typename NodeType, typename ArcType>
class Node {
public:

    typedef IncidentArcIterator<NodeType, ArcType> incidentIterator;
    typedef IncidentArcRevIterator<NodeType, ArcType> reversedIncidentIterator;
    typedef AdjacentIterator<NodeType, ArcType> adjacentIterator;
    typedef AdjacentRevIterator<NodeType, ArcType> reversedAdjacentIterator;

    Node() {
        this->inCount = 0;
        this->inCapacity = 3;
        this->incoming = new Arc<NodeType, ArcType>*[this->inCapacity];

        this->outCount = 0;
        this->outCapacity = 3;
        this->outgoing = new Arc<NodeType, ArcType>*[this->outCapacity];

        this->allCapacity = this->inCapacity + this->outCapacity;
        this->allArcs = new Arc<NodeType, ArcType>*[this->allCapacity];

        this->data = nullptr;
    }

    Node(NodeType* data) : Node() {
        this->data = data;
    };

    incidentIterator beginIncidentalArcs() { return IncidentArcIterator<NodeType, ArcType>(this->allArcs); }
    incidentIterator cbeginIncidentalArcs() const { return IncidentArcIterator<NodeType, ArcType>(this->allArcs); }

    incidentIterator endIncidentalArcs() {
        return ++IncidentArcIterator<NodeType, ArcType>(&this->allArcs[this->inCount +
                                                                       this->outCount - 1]);
    }

    incidentIterator cendIncidentalArcs() const {
        return ++IncidentArcIterator<NodeType, ArcType>(&this->allArcs[this->inCount +
                                                                       this->outCount - 1]);
    }

    reversedIncidentIterator rbeginIncidentalArcs() {
        return IncidentArcRevIterator<NodeType, ArcType>(&this->allArcs[this->inCount +
                                                                          this->outCount - 1]);
    }

    reversedIncidentIterator rendIncidentalArcs() {
        return ++IncidentArcRevIterator<NodeType, ArcType>(this->allArcs);
    }

    adjacentIterator beginAdjacentNode() {
        return AdjacentIterator<NodeType, ArcType>(this->outgoing);
    }

    adjacentIterator cbeginAdjacentNode() const {
        return AdjacentIterator<NodeType, ArcType>(this->outgoing);
    }

    adjacentIterator endAdjacentNode() {
        return ++AdjacentIterator<NodeType, ArcType>(&this->outgoing[this->outCount - 1]);
    }

    adjacentIterator cendAdjacentNode() const {
        return ++AdjacentIterator<NodeType, ArcType>(&this->outgoing[this->outCount - 1]);
    }

    reversedAdjacentIterator rbeginAdjacentNode()  {
        return AdjacentRevIterator<NodeType, ArcType>(&this->outgoing[this->outCount - 1]);
    }

    reversedAdjacentIterator rendAdjacentNode()  {
        return ++AdjacentRevIterator<NodeType, ArcType>(this->outgoing);
    }

    void addInArc(Arc<NodeType, ArcType> *newArc);

    void addOutArc(Arc<NodeType, ArcType> *newArc);

    void incomingExtension();

    void outgoingExtension();

    void allArcsExtension();

    void removeArc(Arc<NodeType, ArcType> *toDel);

    NodeType* data;

private:

    Arc<NodeType, ArcType> **incoming;
    Arc<NodeType, ArcType> **outgoing;
    Arc<NodeType, ArcType> **allArcs;
    int inCount;
    int inCapacity;
    int outCount;
    int outCapacity;
    int allCapacity;

    template<typename NodeType1, typename ArcType1>
    friend class Graph;
    template<typename NodeType1, typename ArcType1>
    friend class IncidentArcIterator;
    template<typename NodeType1, typename ArcType1>
    friend class IncidentArcRevIterator;
    template<typename NodeType1, typename ArcType1>
    friend class AdjacentIterator;
    template<typename NodeType1, typename ArcType1>
    friend class AdjacentRevIterator;
    template<typename NodeType1, typename ArcType1>
    friend class NodeIterator;
    template<typename NodeType1, typename ArcType1>
    friend class NodeRevIterator;
    template<typename NodeType1, typename ArcType1>
    friend class ArcIterator;
    template<typename NodeType1, typename ArcType1>
    friend class AdjacentRevIterator;
};

template<typename NodeType, typename ArcType>
void Node<NodeType, ArcType>::incomingExtension() {
    if (this->inCount < this->inCapacity)
        return;
    this->inCapacity *= 2;
    Arc<NodeType, ArcType> **tmp = new Arc<NodeType, ArcType>*[this->inCapacity];
    std::copy(this->incoming, this->incoming + this->inCount, tmp);
    delete[] this->incoming;
    this->incoming = tmp;
}

template<typename NodeType, typename ArcType>
void Node<NodeType, ArcType>::outgoingExtension() {
    if (this->outCount < this->outCapacity)
        return;
    this->outCapacity *= 2;
    Arc<NodeType, ArcType> **tmp = new Arc<NodeType, ArcType>*[this->outCapacity];
    std::copy(this->outgoing, this->outgoing + this->outCount, tmp);
    delete[] this->outgoing;
    this->outgoing = tmp;
}

template<typename NodeType, typename ArcType>
void Node<NodeType, ArcType>::allArcsExtension() {
    if (this->inCount + this->outCount < this->allCapacity)
        return;
    this->allCapacity *= 2;
    Arc<NodeType, ArcType> **tmp = new Arc<NodeType, ArcType>*[this->allCapacity];
    std::copy(this->allArcs, this->allArcs + this->inCount + this->outCount, tmp);
    delete[] this->allArcs;
    this->allArcs = tmp;
}

template<typename NodeType, typename ArcType>
void Node<NodeType, ArcType>::addInArc(Arc<NodeType, ArcType> *newArc) {
    this->incomingExtension();
    this->allArcsExtension();
    this->incoming[this->inCount] = newArc;
    for (auto it = this->beginIncidentalArcs(); it != this->endIncidentalArcs(); ++it)
        if (&*it == newArc) {
            ++this->inCount;
            return;
        }
    this->allArcs[this->inCount + this->outCount] = newArc;
    this->inCount++;
}

template<typename NodeType, typename ArcType>
void Node<NodeType, ArcType>::addOutArc(Arc<NodeType, ArcType>* newArc) {
    this->outgoingExtension();
    this->allArcsExtension();
    this->outgoing[this->outCount] = newArc;
    for (auto it = this->beginIncidentalArcs(); it != this->endIncidentalArcs(); ++it)
        if (&*it == newArc) {
            ++this->outCount;
            return;
        }
    this->allArcs[this->inCount + this->outCount] = newArc;
    this->outCount++;
}

template<typename NodeType, typename ArcType>
void Node<NodeType, ArcType>::removeArc(Arc<NodeType, ArcType> *toDel) {

    int allCount = this->inCount + this->outCount;
    int flag = 0;
    for (int i = 0; i < this->inCount + this->outCount; ++i) {
        if (i < this->inCount)
            if (this->incoming[i] == toDel) {
                std::copy(this->incoming + i + 1, this->incoming + this->inCount--, this->incoming + i);
                flag++;
            }
        if (i < this->outCount)
            if (this->outgoing[i] == toDel){
                std::copy(this->outgoing + i + 1, this->outgoing + this->outCount--, this->outgoing + i);
                flag++;
            }
        if (this->allArcs[i] == toDel){
            std::copy(this->allArcs + i + 1, this->allArcs + allCount--, this->allArcs + i);
            flag++;
        }
        if (flag == 3)
            break;
    }
}


template<typename NodeType, typename ArcType>
class NodeIterator {
public:
    typedef NodeType valueType;
    typedef NodeType *pointer;
    typedef NodeType &reference;
    typedef const NodeType &const_ref;

    NodeIterator(Node<NodeType, ArcType> **node) : node(node) {}

    NodeIterator &operator=(const NodeIterator &other) {
        if (this != &other)
            *this = other;
        return *this;
    }

    NodeIterator &operator++() {
        ++this->node;
        return *this;
    }

    NodeIterator operator++(int) {
        NodeIterator prev(this->node);
        ++this->node;
        return prev;
    }

    NodeIterator &operator--() {
        --this->node;
        return *this;
    }

    NodeIterator &operator--(int) {
        NodeIterator prev(this->node);
        --this->node;
        return prev;
    }

    const Node<NodeType, ArcType> &operator*() { return **node; }

    const Node<NodeType, ArcType> *operator->() { return *node; }

    bool operator==(const NodeIterator &toCmp) { return node == toCmp.node; }

    bool operator!=(const NodeIterator &toCmp) { return this->node != toCmp.node; }

//private:
    Node<NodeType, ArcType> **node;

    template<typename NodeType1, typename ArcType1>
    friend class Graph;
};

template<typename NodeType, typename ArcType>
class NodeRevIterator {
public:
    typedef NodeType valueType;
    typedef NodeType *pointer;
    typedef NodeType &reference;
    typedef const NodeType &const_ref;

    NodeRevIterator(Node<NodeType, ArcType> **node) : node(node) {}

    NodeRevIterator &operator=(const NodeRevIterator &other) {
        if (this != &other)
            node = other.node;
        return *this;
    }

    NodeRevIterator &operator++() {
        --this->node;
        return *this;
    }

    NodeRevIterator operator++(int) {
        NodeRevIterator prev(this->node);
        --this->node;
        return prev;
    }

    NodeRevIterator &operator--() {
        ++this->node;
        return *this;
    }

    NodeRevIterator &operator--(int) {
        NodeRevIterator prev(this->node);
        ++this->node;
        return prev;
    }

    const Node<NodeType, ArcType> &operator*() { return **node; }

    const Node<NodeType, ArcType> *operator->() { return *node; }

    bool operator==(const NodeRevIterator &toCmp) { return node == toCmp.node; }

    bool operator!=(const NodeRevIterator &toCmp) { return node != toCmp.node; }

//private:
    Node<NodeType, ArcType> **node;

    template<typename NodeType1, typename ArcType1>
    friend class Graph;
};

template<typename NodeType, typename ArcType>
class ArcIterator {
public:
    typedef ArcType valueType;
    typedef ArcType *pointer;
    typedef ArcType &reference;
    typedef const ArcType &const_ref;

    ArcIterator(Arc<NodeType, ArcType> **arc) : arc(arc) {}

    ArcIterator &operator=(const ArcIterator &other) {
        if (this != &other)
            arc = other.arc;
        return *this;
    }

    ArcIterator &operator++() {
        ++this->arc;
        return *this;
    }

    ArcIterator operator++(int) {
        ArcIterator prev(this->arc);
        ++this->arc;
        return prev;
    }

    ArcIterator &operator--() {
        --this->arc;
        return *this;
    }

    ArcIterator &operator--(int) {
        ArcIterator prev(this->arc);
        --this->arc;
        return prev;
    }

    const Arc<NodeType, ArcType> &operator*() { return **arc; }

    const Arc<NodeType, ArcType> *operator->() { return *arc; }

    bool operator==(const ArcIterator &toCmp) { return arc == toCmp.arc; }

    bool operator!=(const ArcIterator &toCmp) { return arc != toCmp.arc; }

private:
    Arc<NodeType, ArcType> **arc;

    template<typename NodeType1, typename ArcType1>
    friend class Graph;
};

template<typename NodeType, typename ArcType>
class ArcRevIterator {
public:
    typedef ArcType valueType;
    typedef ArcType *pointer;
    typedef ArcType &reference;
    typedef const ArcType &const_ref;

    ArcRevIterator(Arc<NodeType, ArcType> **arc) : arc(arc) {}

    ArcRevIterator &operator=(const ArcRevIterator &other) {
        if (this != &other)
            arc = other.arc;
        return *this;
    }

    ArcRevIterator &operator++() {
        --this->arc;
        return *this;
    }

    ArcRevIterator operator++(int) {
        ArcRevIterator prev(this->arc);
        --this->arc;
        return prev;
    }

    ArcRevIterator &operator--() {
        ++this->arc;
        return *this;
    }

    ArcRevIterator &operator--(int) {
        ArcRevIterator prev(this->arc);
        ++this->arc;
        return prev;
    }

    const Arc<NodeType, ArcType> &operator*() { return **arc; }

    const Arc<NodeType, ArcType> *operator->() { return *arc; }

    bool operator==(const ArcRevIterator &toCmp) { return arc == toCmp.arc; }

    bool operator!=(const ArcRevIterator &toCmp) { return arc != toCmp.arc; }

private:
    Arc<NodeType, ArcType> **arc;

    template<typename NodeType1, typename ArcType1>
    friend class Graph;
};

template<typename NodeType, typename ArcType>
class IncidentArcIterator {
public:

    IncidentArcIterator(Arc<NodeType, ArcType> **arc) : arc(arc) {}

    IncidentArcIterator &operator=(const IncidentArcIterator &other) {
        if (this != &other)
            arc = other.arc;
        return *this;
    }

    IncidentArcIterator &operator++() {
        ++this->arc;
        return *this;
    }

    IncidentArcIterator operator++(int) {
        IncidentArcIterator prev(this->arc);
        ++this->arc;
        return prev;
    }

    IncidentArcIterator &operator--() {
        --this->arc;
        return *this;
    }

    IncidentArcIterator &operator--(int) {
        IncidentArcIterator prev(this->arc);
        --this->arc;
        return prev;
    }

    const Arc<NodeType, ArcType> &operator*() { return **arc; }

    const Arc<NodeType, ArcType> *operator->() { return *arc; }

    bool operator==(const IncidentArcIterator& toCmp) const { return arc == toCmp.arc; }

    bool operator!=(const IncidentArcIterator& toCmp) { return arc != toCmp.arc; }

private:
    Arc<NodeType, ArcType> **arc;

    template<typename NodeType1, typename ArcType1>
    friend class Graph;
};

template<typename NodeType, typename ArcType>
class IncidentArcRevIterator {
public:

    IncidentArcRevIterator(Arc<NodeType, ArcType> **arc) : arc(arc) {}

    IncidentArcRevIterator &operator=(const IncidentArcRevIterator &other) {
        if (this != &other)
            arc = other.arc;
        return *this;
    }

    IncidentArcRevIterator &operator++() {
        --this->arc;
        return *this;
    }

    IncidentArcRevIterator operator++(int) {
        IncidentArcRevIterator prev(this->arc);
        --this->arc;
        return prev;
    }

    IncidentArcRevIterator &operator--() {
        ++this->arc;
        return *this;
    }

    IncidentArcRevIterator &operator--(int) {
        IncidentArcRevIterator prev(this->arc);
        ++this->arc;
        return prev;
    }

    const Arc<NodeType, ArcType> &operator*() { return **arc; }

    const Arc<NodeType, ArcType> *operator->() { return *arc; }

    bool operator==(const IncidentArcRevIterator &toCmp) { return arc == toCmp.arc; }

    bool operator!=(const IncidentArcRevIterator &toCmp) { return arc != toCmp.arc; }

private:
    Arc<NodeType, ArcType> **arc;

    template<typename NodeType1, typename ArcType1>
    friend class Graph;
};

template<typename NodeType, typename ArcType>
class AdjacentIterator {
public:

    AdjacentIterator(Arc<NodeType, ArcType> **arc) : arc(arc) {}

    AdjacentIterator &operator=(const AdjacentIterator &other) {
        if (this != &other)
            arc = other.arc;
        return *this;
    }

    AdjacentIterator &operator++() {
        ++this->arc;
        return *this;
    }

    AdjacentIterator operator++(int) {
        AdjacentIterator prev(this->arc);
        ++this->arc;
        return prev;
    }

    AdjacentIterator &operator--() {
        --this->arc;
        return *this;
    }

    AdjacentIterator &operator--(int) {
        AdjacentIterator prev(this->arc);
        --this->arc;
        return prev;
    }

    const Node<NodeType, ArcType> &operator*() {
        auto preRes = *arc;
        auto res = preRes->end;
        return *res; }

    const Node<NodeType, ArcType> *operator->() {
        auto preRes = *arc;
        auto res = preRes->end;
        return res; }

    bool operator==(const AdjacentIterator &toCmp) { return this->arc == toCmp.arc; }

    bool operator!=(const AdjacentIterator &toCmp) { return this->arc != toCmp.arc; }

private:
    Arc<NodeType, ArcType> **arc;

    template<typename NodeType1, typename ArcType1>
    friend class Graph;
};

template<typename NodeType, typename ArcType>
class AdjacentRevIterator {
public:

    AdjacentRevIterator(Arc<NodeType, ArcType> **arc) : arc(arc) {}

    AdjacentRevIterator &operator=(const AdjacentRevIterator &other) {
        if (this != &other)
            arc = other.arc;
        return *this;
    }

    AdjacentRevIterator &operator++() {
        --this->arc;
        return *this;
    }

    AdjacentRevIterator operator++(int) {
        AdjacentRevIterator prev(this->arc);
        --this->arc;
        return prev;
    }

    AdjacentRevIterator &operator--() {
        ++this->arc;
        return *this;
    }

    AdjacentRevIterator &operator--(int) {
        AdjacentRevIterator prev(this->arc);
        ++this->arc;
        return prev;
    }

    const Node<NodeType, ArcType> &operator*() {
        auto preRes = *arc;
        auto res = preRes->end;
        return *res; }

    const Node<NodeType, ArcType> *operator->() {
        auto preRes = *arc;
        auto res = preRes->end;
        return res; }

    bool operator==(const AdjacentRevIterator &toCmp) { return this->arc == toCmp.arc; }

    bool operator!=(const AdjacentRevIterator &toCmp) { return this->arc != toCmp.arc; }

private:
    Arc<NodeType, ArcType> **arc;

    template<typename NodeType1, typename ArcType1>
    friend class Graph;
};


template<typename NodeType, typename ArcType>
class Graph {
public:

    typedef NodeIterator<NodeType, ArcType> iterator;
    typedef NodeRevIterator<NodeType, ArcType> revIterator;
    typedef ArcIterator<NodeType, ArcType> aiter;
    typedef ArcRevIterator<NodeType, ArcType> revAiter;
    typedef IncidentArcIterator<NodeType, ArcType> incidentIterator;
    typedef AdjacentIterator<NodeType, ArcType> adjacentIterator;
    typedef Node<NodeType, ArcType> valueType;
    typedef NodeType *pointer;
    typedef NodeType &reference;
    typedef const NodeType &constReference;

    Graph() {

        this->arcCount = 0;
        this->arcCapacity = 5;
        this->arcs = new Arc<NodeType, ArcType>*[this->arcCapacity];

        this->nodeCount = 0;
        this->nodeCapacity = 5;
        this->nodes = new Node<NodeType, ArcType>*[this->nodeCapacity];
    }

    Graph(const Graph<NodeType, ArcType> &graph) {
        this->nodeCount = 0;
        this->nodeCapacity = graph.nodeCapacity;
        this->arcCount = 0;
        this->arcCapacity = graph.arcCapacity;
        this->arcs = new Arc<NodeType, ArcType>* [this->arcCapacity];
        this->nodes = new Node<NodeType, ArcType>* [this->nodeCapacity];

        std::vector<std::vector<int>> matrix;
        auto iBegin = graph.cbegin();
        for (int i = 0; i < graph.nodeCount; ++i) {
            matrix.push_back(std::vector<int>());
            auto jBegin = graph.cbegin();
            for (int j = 0; j < graph.nodeCount; ++j) {
                graph.linked(iBegin, jBegin) ?
                matrix[i].push_back(1):
                matrix[i].push_back(0);
                ++jBegin;
            }
            ++iBegin;
        }
        for (auto it = graph.cbegin(); it != graph.cend(); ++it)
            this->addNode(*it.node[0]->data);


        int i = 0, j = 0;
        for (auto it1 = this->begin(); it1 != this->end(); ++it1) {
            for (auto it2 = this->begin(); it2 != this->end(); ++it2) {
                if (!matrix[i][j]) {
                    ++j;
                    continue;
                }
                this->addArc(it1, it2);
                ++j;
            }
            ++i;
            j = 0;
        }

        auto grIt = graph.cbeginArc();
        for (int i = 0; i < this->arcCount; ++i) {
            this->arcs[i]->data = new ArcType(*grIt.arc[0]->data);
            ++grIt;
        }
    }

    ~Graph() { this->clear(); }

    void createTheGraph(const char* path);
    void fillTheGraph(const char* path);

    void nodeExtension();

    void arcExtension();

    void eraseNode(iterator position);

    void eraseArc(aiter position);

    aiter findArc(Arc<NodeType, ArcType>* needed);

    void delNode(int index);

    iterator find(NodeType node) const;

    bool linked(iterator& begin, iterator& end) const;

    bool empty() const {return !this->nodeCount; };

    void clear();

    int getNodeCount() const { return this->nodeCount; }

    int getArcCount() const { return this->arcCount; }

    int nodePower(iterator node) const { return node.node[0]->inCount + node.node[0]->outCount; }

    int arcPower(aiter arc) const { return arc.arc->begin == arc.arc.end ? 1 : 2; }

    void addNode(NodeType node);

    void addArc(iterator &begin, iterator &end); //добавление дуги из begin в end
    iterator begin() { return NodeIterator<NodeType, ArcType>(this->nodes); };

    iterator cbegin() const { return NodeIterator<NodeType, ArcType>(this->nodes); }

    iterator end() {
        return ++NodeIterator<NodeType, ArcType>(&this->nodes[this->nodeCount - 1]);
    }
    iterator cend() const {
        return ++NodeIterator<NodeType, ArcType>(&this->nodes[this->nodeCount - 1]);
    }

    revIterator rbegin() {
        return NodeRevIterator<NodeType, ArcType>(&this->nodes[this->nodeCount - 1]);
    }

    revIterator rend() { return ++NodeRevIterator<NodeType, ArcType>(this->nodes); }

    aiter beginArc() { return ArcIterator<NodeType, ArcType>(this->arcs); }

    aiter cbeginArc() const { return ArcIterator<NodeType, ArcType>(this->arcs); }

    aiter endArc() { return ++ArcIterator<NodeType, ArcType>(&this->arcs[this->arcCount - 1]); }

    aiter cendArc() const { return ++ArcIterator<NodeType, ArcType>(&this->arcs[this->arcCount - 1]); }

    revAiter rbeginArc() { return ArcRevIterator<NodeType, ArcType>(&this->arcs[this->arcCount - 1]); }

    revAiter rendArc() { return ++ArcRevIterator<NodeType, ArcType>(this->arcs); }

    valueType operator[](int index) {
        auto it = this->begin();
        for (int i = 0; i < index; ++i) ++it;
        return *it;
    }
    Node<NodeType, ArcType> **nodes;
    Arc<NodeType, ArcType> **arcs;
    int nodeCount;
    int arcCount;
    int nodeCapacity;
    int arcCapacity;

    template<typename NodeType1, typename ArcType1>
    friend class IncidentArcIterator;
    template<typename NodeType1, typename ArcType1>
    friend class IncidentArcRevIterator;
    template<typename NodeType1, typename ArcType1>
    friend class AdjacentIterator;
    template<typename NodeType1, typename ArcType1>
    friend class AdjacentRevIterator;
    template<typename NodeType1, typename ArcType1>
    friend class NodeIterator;
    template<typename NodeType1, typename ArcType1>
    friend class NodeRevIterator;
    template<typename NodeType1, typename ArcType1>
    friend class ArcIterator;
    template<typename NodeType1, typename ArcType1>
    friend class AdjacentRevIterator;
    template<typename NodeType1, typename ArcType1>
    friend std::ostream& operator<< (std::ostream &out, Graph<NodeType1, ArcType1> &graph);
};

template<typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::addNode(NodeType node) {
    this->nodeExtension();
    this->nodes[this->nodeCount] = new Node<NodeType, ArcType>;
    this->nodes[this->nodeCount]->data = new NodeType(node);
    ++this->nodeCount;
}

template<typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::nodeExtension() {
    if (this->nodeCount < this->nodeCapacity)
        return;
    this->nodeCapacity *= 2;
    Node<NodeType, ArcType> **tmp = new Node<NodeType, ArcType>*[this->nodeCapacity]();
    std::copy(this->nodes, this->nodes + this->nodeCount, tmp);
    delete[] this->nodes;
    this->nodes = tmp;
}

template<typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::arcExtension() {
    if (this->arcCount < this->arcCapacity)
        return;
    this->arcCapacity *= 2;
    Arc<NodeType, ArcType> **tmp = new Arc<NodeType, ArcType>*[this->arcCapacity];
    std::copy(this->arcs, this->arcs + this->arcCount, tmp);
    delete[] this->arcs;
    this->arcs = tmp;
}

template<typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::addArc(Graph::iterator &begin, Graph::iterator &end) {
    if (this->linked(begin, end))
        return;
    this->arcExtension();
    auto newArc = new Arc<NodeType, ArcType>(*begin.node, *end.node);
    begin.node[0]->addOutArc(newArc);
    end.node[0]->addInArc(newArc);
    this->arcs[this->arcCount++] = newArc;
}

template<typename NodeType, typename ArcType>
bool Graph<NodeType, ArcType>::linked(Graph::iterator& begin, Graph::iterator& end) const {
    for (int i = 0; i < begin.node[0]->outCount; ++i)
        if (begin.node[0]->outgoing[i]->end == &*end)
            return true;
    return false;
}

template<typename NodeType, typename ArcType>
typename Graph<NodeType, ArcType>::iterator Graph<NodeType, ArcType>::find(NodeType node) const {
    for (auto it = this->cbegin(); it != this->cend(); ++it) {
        if (*(it->data) == node)
            return it;
    }
    return this->cend();
}

template<typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::createTheGraph(const char *path) {
    FileReader file(path);
    int nodeCount;
    file.input >> nodeCount;

    for (int i = 0; i < nodeCount; ++i) {
        this->addNode(NodeType());
    }

    for (auto it1 = this->begin(); it1 != this->end(); ++it1) {
        for (auto it2 = this->begin(); it2 != this->end(); ++it2) {
            bool exists;
            file.input >> exists;
            if (!exists)
                continue;
            this->addArc(it1, it2);
        }
    }

}

template<typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::fillTheGraph(const char* path) {
    FileReader file(path);
    int nodeCount;
    int arcCount;
    file.input >> nodeCount;
    for (int i = 0; i < nodeCount; ++i) {
        NodeType data;
        file.input >> data;
        this->nodes[i]->data = new NodeType(data);
    }
    file.input >> arcCount;
    for (int i = 0; i < arcCount; ++i) {
        ArcType data;
        file.input >> data;
        this->arcs[i]->data = new ArcType(data);
    }
}

template<typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::eraseNode(Graph::iterator position) {
    for (auto it = position.node[0]->beginIncidentalArcs(); it != position.node[0]->endIncidentalArcs();)
        if (it.arc) eraseArc(findArc(it.arc[0]));

    auto tmp = position.node[0];
    std::copy(position.node + 1, this->nodes + this->nodeCount--, position.node);
    delete tmp;
    tmp = nullptr;
}



template<typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::eraseArc(aiter position) {
    position.arc[0]->begin->removeArc(position.arc[0]);
    position.arc[0]->end->removeArc(position.arc[0]);
    int i = 0;
    for (; i < this->arcCount;) if (arcs[i++] == position.arc[0]) break;
    delete position.arc[0];
    position.arc[0] = nullptr;
    std::copy(this->arcs + i, this->arcs + this->arcCount--, this->arcs + i - 1);
}

template<typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::delNode(int index) {
    auto toDel = this->begin();
    for (int i = 0; i < index; ++i) ++toDel;
    this->eraseNode(toDel);
}

template <typename NodeType1, typename ArcType1>
std::ostream& operator<< (std::ostream &out, Graph<NodeType1, ArcType1> &graph)
{
    auto iBegin = graph.begin();

    for (int i = 0; i < graph.nodeCount; ++i) {
        auto jBegin = graph.begin();
        for (int j = 0; j < graph.nodeCount; ++j) {
            graph.linked(iBegin, jBegin) ?
                out << 1 << ' ' :
                out << 0 << ' ';
            ++jBegin;
        }
        out << '\n';
        ++iBegin;
    }

    return out;
}

template<typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::clear() {
    int count = this->nodeCount;
    for (int i = 0; i < count; ++i)
        delNode(0);
    this->nodes = nullptr;
    this->arcs = nullptr;
}

template<typename NodeType, typename ArcType>
typename Graph<NodeType, ArcType>::aiter Graph<NodeType, ArcType>::findArc(Arc<NodeType, ArcType>* needed) {
    for (auto it = this->beginArc(); it != this->endArc(); ++it) {
        if (it.arc[0] == needed)
            return it;
    }
    return this->endArc();
}


#endif //LAB4_GRAPH_H
