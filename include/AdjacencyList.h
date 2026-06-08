#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H

#include <iostream>
#include "GraphFileReader.h"

struct ListNode {
    int to;
    int weight;
    ListNode* next;

    ListNode(int t, int w) : to(t), weight(w), next(nullptr) {}
};

class AdjacencyList {
private:
    ListNode** heads;
    int vertices;

public:
    explicit AdjacencyList(int v) : vertices(v) {
        heads = new ListNode*[vertices];
        for (int i = 0; i < vertices; i++) {
            heads[i] = nullptr;
        }
    }

    ~AdjacencyList() {
        for (int i = 0; i < vertices; i++) {
            ListNode* curr = heads[i];
            while (curr != nullptr) {
                ListNode* next = curr->next;
                delete curr;
                curr = next;
            }
        }
        delete[] heads;
    }

    void addEdge(int from, int to, int weight) {
        ListNode* node = new ListNode(to, weight);
        node->next = heads[from];
        heads[from] = node;
    }

    void loadFromData(const GraphData& data, bool directed) {
        for (int i = 0; i < data.edgeCount; i++) {
            int from = data.edges[i].from;
            int to = data.edges[i].to;
            int w = data.edges[i].weight;

            addEdge(from, to, w);
            if (!directed) {
                addEdge(to, from, w);
            }
        }
    }

    ListNode* getHead(int vertex) const {
        return heads[vertex];
    }

    int getVertices() const { return vertices; }

    void print() const {
        std::cout << "Adjacency list:\n";
        for (int i = 0; i < vertices; i++) {
            std::cout << "v" << i << ": ";
            ListNode* curr = heads[i];
            while (curr != nullptr) {
                std::cout << "-> (v" << curr->to << ", w=" << curr->weight << ") ";
                curr = curr->next;
            }
            std::cout << "\n";
        }
    }
};

#endif