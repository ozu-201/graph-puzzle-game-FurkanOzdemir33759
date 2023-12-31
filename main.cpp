#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

bool oneCharDiff(string& s1, string& s2) {
    bool diffDetected = false;
    for (int i = 0; i < s1.size(); i++) {
        if (diffDetected && s1[i] != s2[i]) {
            return false;
        }
        if (s1[i] != s2[i]) {
            diffDetected = true;
        }
    }
    return diffDetected;
}

struct Graph {
    vector<string> vertices;
    vector<vector<int>> edges;

    Graph(int wordSize, string filePath) {
        vertices = vector<string>{};

        fstream file(filePath);

        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                if (line.size() == wordSize) {
                    vertices.push_back(line);
                }
            }
        } else {
            cerr << "File not readable!" << endl;
        }

        edges = vector<vector<int>>(vertices.size(), vector<int>(vertices.size(), 0));

        for (int i = 0; i < vertices.size(); i++) {
            for (int j = i + 1; j < vertices.size(); j++) {
                if (oneCharDiff(vertices[i],vertices[j])) {
                    edges[i][j] = 1;
                    edges[j][i] = 1;
                }
            }
        }
    }

    vector<string> BFS(string start, string end) {
        int v_s = -1;
        int v_e = -1;
        vector<bool> visited = vector<bool>(vertices.size(), false);
        vector<int> prev = vector<int>(vertices.size(), -1);
        for (int i = 0; i < vertices.size(); i++) {
            if (vertices[i] == start) {v_e = i;}
            if (vertices[i] == end) {v_s = i;}
        }
        if (v_e == -1) {return vector<string>{"START NOT FOUND!"};}
        if (v_s == -1) {return vector<string>{"END NOT FOUND!"};}
        if (v_e == v_s) {return vector<string>{"SAME START AND END!"};}
        vector<int> queue{};
        queue.push_back(v_s);
        visited[v_s] = true;
        while (!queue.empty()) {
            int current = *queue.erase(queue.begin());
            if (current == v_e) {
                vector<string> path{};
                while (current != v_s) {
                    path.push_back(vertices[current]);
                    current = prev[current];
                }
                path.push_back(vertices[v_s]);
                return path;
            }
            for (int i = 0; i < vertices.size(); i++) {
                if (!visited[i] && edges[current][i] == 1) {
                    queue.push_back(i);
                    visited[i] = true;
                    prev[i] = current;
                }
            }
        }
        return vector<string>{"END IS NOT REACHABLE FROM THIS START!"};
    }

    // BUGGY
    /*
    vector<string> Dijkstra(string start, string end) {
        int v_s = -1;
        int v_e = -1;
        vector<int> distance = vector<int>(vertices.size(), INT32_MAX);
        vector<int> prev = vector<int>(vertices.size(), -1);
        vector<int> visitedSet = vector<int>(vertices.size(), -1);
        for (int i = 0; i < vertices.size(); i++) {
            if (vertices[i] == start) {v_e = i;}
            if (vertices[i] == end) {v_s = i;}
        }
        if (v_e == -1) {return vector<string>{"START NOT FOUND!"};}
        if (v_s == -1) {return vector<string>{"END NOT FOUND!"};}
        if (v_e == v_s) {return vector<string>{"SAME START AND END!"};}
        distance[v_s] = 0;
        bool fullyExplored = false;
        bool firstPass = true;
        while (!fullyExplored && visitedSet[v_e] != 1) {
            int min = INT32_MAX;
            int current = 0;
            for (int i = 0; i < vertices.size(); i++) {
                if (min > distance[i] && visitedSet[i] != 1) {
                    current = i;
                    min = distance[i];
                }
            }
            if (min == INT32_MAX) {
                fullyExplored = true;
            }
            visitedSet[current] = 1;
            if (firstPass) {
                prev[current] = v_s;
                firstPass = false;
            }
            for (int i = 0; i < vertices.size(); i++) {
                if (edges[current][i] == 1) {
                    if (distance[current] + 1 < distance[i]) {
                        distance[i] = distance[current] + 1;
                        prev[i] = current;
                    }
                }
            }
        }
        vector<string> path{};
        int trace = v_e;
        while (trace != -1) {
            path.push_back(vertices[prev[trace]]);
            trace = prev[trace];
        }
        return path;
    }
     */

    friend ostream& operator<<(ostream& os, const vector<string>& container);
};

// Overloaded (<<) operator to print BFS and Dijkstra results!
ostream& operator<<(ostream& os, const vector<string>& container) {
    for (auto iter = container.begin(); iter != container.end(); iter++) {
        os << *iter;
        if (iter + 1 != container.end()) {
            os << " --> ";
        }
    }
    os << "\n";
    return os;
}

int main() {
    // IF FILE IS NOT READABLE MAKE SURE TO CONFIGURE WORKING DIRECTORY PROPERLY OR USE ABSOLUTE PATH!
    Graph g{5, "turkish-dictionary.txt"};
    cout << g.BFS("selim", "devam");
    return -1;
}



