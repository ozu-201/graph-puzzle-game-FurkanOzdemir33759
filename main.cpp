//
// Created by fo033759 on 12/21/2023.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

bool oneCharDifferent(string first, string second) {
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

    Vertex(string word) : word(move(word)) {}
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
                    vertices.push_back(Vertex(word));
                }
            }
        } else {
            cerr << "File is not available!" << endl;
        }

        file.close();

        size = vertices.size();

        adjMat = vector<vector<int>>();
        for (int i = 0; i < size; i++) {
            adjMat[i] = vector<int>(0);
        }


        for (int i = 0; i < size; i++) {
            for (int j = i + 1; j < size; j++) {
                if (oneCharDifferent(vertices[i].word, vertices[j].word)) {
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
};


int main() {

    Graph g = Graph(5, "turkish-dictionary.txt");
    g.printEdges();


    return 0;
}
