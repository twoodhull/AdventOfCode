#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>

using namespace std;

struct Node {
    Node* parent = nullptr;
    vector<Node*> children;
    int unresolved = 0;
    int directOrbits = 0;
    int indirectOrbits = 0;
};

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
        orbiter.unresolved++;
        orbiter.directOrbits++;
    }

    queue<Node*> resolved;
    for (auto it : nodes) {
        Node& cur = nodes[it.first];
        if (cur.unresolved == 0) {
            resolved.emplace(&cur);
        }
    }

    int directOrbits = 0;
    int indirectOrbits = 0;
    while (!resolved.empty()) {
        Node* cur = resolved.front();
        resolved.pop();

        if (cur->parent != nullptr) {
            cur->indirectOrbits = cur->parent->directOrbits + cur->parent->indirectOrbits;
        }

        for (Node* child : cur->children) {
            child->unresolved--;

            if (child->unresolved == 0) {
                resolved.push(child);
            }
        }

        directOrbits += cur->directOrbits;
        indirectOrbits += cur->indirectOrbits;
    }

    cout << "Direct: " << directOrbits << ", Indirect: " << indirectOrbits << ", Total: " << (directOrbits + indirectOrbits) << endl; 

    return 0;
}