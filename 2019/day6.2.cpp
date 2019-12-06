#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>

using namespace std;

struct Node {
    Node* parent = nullptr;
    vector<Node*> children;
    int depth = -1;
};

void resolveDepths(Node* node, int depth) {
    if (node == nullptr) {
        return;
    }

    node->depth = depth;

    for (Node* child : node->children) {
        resolveDepths(child, depth + 1);
    }
}

Node* lca(Node* a, Node* b) {
    while (a != b) {
        if (a == nullptr || b == nullptr) {
            return nullptr;
        }

        if (a->depth >= b->depth) {
            a = a->parent;
        } else {
            b = b->parent;
        }
    }

    return a;
}

int main() {
    string input;

    unordered_map<string, Node> nodes;

    while (getline(cin, input)) {
        size_t parenIndex = input.find(')');

        string orbiteeName = input.substr(0, parenIndex);
        string orbiterName = input.substr(parenIndex + 1);

        Node& orbitee = nodes[orbiteeName];
        Node& orbiter = nodes[orbiterName];

        orbitee.children.push_back(&orbiter);
        orbiter.parent = &orbitee;
    }

    Node* you = nullptr;
    Node* san = nullptr;
    for (auto it : nodes) {
        Node& cur = nodes[it.first];

        if (it.first.compare("YOU") == 0) {
            you = &cur;
        }
        if (it.first.compare("SAN") == 0) {
            san = &cur;
        }

        if (cur.parent == nullptr) {
            resolveDepths(&cur, 0);
        }
    }

    Node* ancestor = lca(you, san);

    cout << "LCA: " << ancestor->depth << ", YOU: " << you->depth << ", SAN: " << san->depth << endl;
    cout << "Total Dist: " << (you->parent->depth + san->parent->depth - ancestor->depth * 2) << endl; 

    return 0;
}