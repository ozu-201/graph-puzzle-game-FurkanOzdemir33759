#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

bool oneCharDifferent(string first, string second, int length) {
    if (first.size() != length || second.size() != length) {
        return false;
    }
    bool detectedOneDifference = false;
    for (int i = 0; i < first.size(); i++) {
        if (detectedOneDifference && first[i] != second[i]) {
            return false;
        }
        if (first[i] != second[i]) {
            detectedOneDifference = true;
        }
    }
    return detectedOneDifference;
}

struct Vertex {
    int id;
    string word;

    Vertex(string word, int id) : word(std::move(word)), id(id) {}
};

struct Path {
    vector<Vertex> path;
    Path(vector<Vertex> path) : path(path) {}
};

struct Node {
    Vertex& vertex;
    Path& path;
    Node* next;
    Node* prev;

    Node(Vertex& vertex, Path& path) : vertex(vertex), path(path), next(nullptr), prev(nullptr) {}
};

struct Queue {
    Node* front;
    Node* rear;

    Queue() : front(nullptr), rear(nullptr) {}

    void enqueue(Node* node) {
        if (front == nullptr) {
            front = node;
        } else if (rear == nullptr) {
            rear = node;
            front->prev = node;
            node->next = front;
        } else {
            rear->prev = node;
            node->next = rear;
            rear = node;
        }
    }

    Node* dequeue() {
        if (rear != nullptr) {
            Node* temp = rear;
            rear = rear->next;
            rear->prev = nullptr;
            return temp;
        } else if (front != nullptr) {
            Node* temp = front;
            front = nullptr;
            return temp;
        }
        return nullptr;
    }

    bool isEmpty() {
        return front == nullptr;
    }
};

struct Graph {
    int wordLength;
    int size;
    vector<Vertex> vertices;
    vector<vector<int>> adjMat;

    Graph(int wordLength, string filePath) : wordLength(wordLength), vertices(vector<Vertex>()) {

        ifstream file(filePath);

        if (file.is_open()) {
            int id = 0;
            string word;
            while (getline(file, word)) {
                if (word.length() == wordLength) {
                    vertices.emplace_back(Vertex(word, id));
                    id++;
                }
            }
        } else {
            cerr << "File is not available!" << endl;
        }

        file.close();

        size = vertices.size();

        adjMat = vector<vector<int>>(size,vector<int>(size, 0));

        for (int i = 0; i < size; i++) {
            for (int j = i + 1; j < size; j++) {
                if (oneCharDifferent(vertices[i].word, vertices[j].word, wordLength)) {
                    adjMat[i][j] = 1;
                    adjMat[j][i] = 1;
                }
            }
        }
    }

    void printEdges() {
        for (int i = 0; i < size; i++) {
            for (int j = i+1; j < size; j++) {
                if (adjMat[i][j] == 1) {
                    cout << vertices[i].word << " <---> " << vertices[j].word << endl;
                }
            }
        }
    }

    Path findShortestPath(string start, string end) {
        bool visited[size];
        for (int i = 0; i < size; i++) {
            visited[i] = false;
        }
        int v_s = -1;
        int v_e = -1;
        for (int i = 0; i < size; i++) {
            if (vertices[i].word.compare(start) == 0) {
                v_s = i;
            }
            if (vertices[i].word.compare(end) == 0) {
                v_e = i;
            }
        }
        if (v_s == -1) {
            vector<Vertex> start_not_found{Vertex("START NOT FOUND", -1)};
            return Path(start_not_found);
        }
        if (v_e == -1) {
            vector<Vertex> end_not_found{Vertex("END NOT FOUND", -1)};
            return Path(end_not_found);
        }
        if (v_s == v_e) {
            vector<Vertex> end_and_start_same{Vertex("END AND START ARE EQUAL", -1)};
            return Path(end_and_start_same);
        }
        Queue queue = Queue();
        Path path = Path(vector<Vertex>{vertices[v_s]});
        Node node = Node(vertices[v_s], path);
        queue.enqueue(&node);
        while (!queue.isEmpty()) {
            Node* from = queue.dequeue();
            visited[from->vertex.id] = true;
            for (int i = 0; i < size; i++) {
                if (adjMat[from->vertex.id][i] == 1 && !visited[i]) {
                    vector<Vertex> p = from->path.path;
                    p.push_back(vertices[i]);
                    Path path1 = Path(p);
                    if (i == v_e) {
                        return path1;
                    }
                    Node node = Node(vertices[i], path1);
                    queue.enqueue(&node);
                }
            }
        }
        vector<Vertex> end_not_reachable{Vertex("END NOT REACHABLE", -1)};
        return Path(end_not_reachable);
    }
};


int main() {

    Graph g = Graph(5, "turkish-dictionary.txt");
    Path p = g.findShortestPath("tamam", "devam");

    for (auto& item : p.path) {
        cout << item.word << " --->";
    }

    return 0;
}
