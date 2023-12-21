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
    string word;

    Vertex(string word) : word(std::move(word)) {}
};

struct Path {
    vector<Vertex> path;
    Path(vector<Vertex> path) : path(path) {}
};

struct Graph {
    int wordLength;
    int size;
    vector<Vertex> vertices;
    vector<vector<int>> adjMat;

    Graph(int wordLength, string filePath) : wordLength(wordLength), vertices(vector<Vertex>()) {

        ifstream file(filePath);

        if (file.is_open()) {
            string word;
            while (getline(file, word)) {
                if (word.length() == wordLength) {
                    vertices.emplace_back(Vertex(word));
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
            vector<Vertex> start_not_found{Vertex("START NOT FOUND")};
            return Path(start_not_found);
        }
        if (v_e == -1) {
            vector<Vertex> end_not_found{Vertex("END NOT FOUND")};
            return Path(end_not_found);
        }
        if (v_s == v_e) {
            vector<Vertex> end_and_start_same{Vertex("END AND START ARE EQUAL")};
            return Path(end_and_start_same);
        }

    }
};


int main() {

    Graph g = Graph(5, "turkish-dictionary.txt");
    g.printEdges();

    return 0;
}
