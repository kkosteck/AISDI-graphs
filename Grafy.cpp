#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <set>
#include <bits/stdc++.h>

#define ABSENT -1

const int X = 7;
const int Y = 7;

void readArrayFromFile(int graph[X][Y]) {
    char ch;
    std::fstream fin("graf.txt", std::fstream::in);
    for (int y=0; y<Y; y++) {
        for (int x=0; x<X; x++) {
            fin >> std::noskipws >> ch;
            graph[x][y] = ch - '0';
        }
        fin >> std::noskipws >> ch;
    }
}

void printArray(int graph[X][Y]) {
   for (int y=0; y<Y; y++) {
        for (int x=0; x<X; x++) {
            std::cout << graph[x][y];
        }
        std::cout << std::endl;
    }
}

class Graph {
private:
    int vCount;
    std::list<std::pair<int, int>> *edges;  //destination, weight

    int heuristic(int adj, int v) {
        int diff = adj - v;
        switch (diff) {
            case -X: return 1;
            case 1:  return 1;
            case -1: return 0;
            case X:  return 0;
            default: return 0;
        }
        return 1;
    }
public:
    ~Graph() {
        delete[] edges;
    }

    Graph(int graphArray[X][Y]) {
        vCount = X*Y;
        edges = new std::list<std::pair<int, int>> [vCount];
        for (int y=0; y<Y; y++) {
            for (int x=0; x<X; x++) {
                int index = y*Y + x;
                if (y-1 >= 0)
                    edges[index].push_back(std::make_pair(index-Y, graphArray[x][y-1]));
                if (y+1 < Y)
                    edges[index].push_back(std::make_pair(index+Y, graphArray[x][y+1]));
                if (x-1 >= 0)
                    edges[index].push_back(std::make_pair(index-1, graphArray[x-1][y]));
                if (x+1 < X)
                    edges[index].push_back(std::make_pair(index+1, graphArray[x+1][y]));
            }
        }
    }

    void print() {
        for (int i=0; i<vCount; i++) {
            std::cout << i << ": ";
            for (const auto &e : edges[i]) {
                std::cout << e.first << "," << e.second << " ";
            }
            std::cout << std::endl;
        }
    }

    std::list<int> findShortestPath(int src, int dest) {
        std::vector<int> dist(vCount, INT_MAX);
        std::vector<int> stp(vCount, ABSENT);      //Contains index of the previous v in the shortest path tree
        std::set<std::pair<int, int>> considered;   //Contains vertices considered for the next v in stp; weight, index; sorted by weight
        dist[src] = 0;
        stp[src] = src;
        considered.insert(std::make_pair(0, src));

        while (!considered.empty()) {
            int v = considered.begin()->second;
            considered.erase(considered.begin());

            for (auto it : edges[v]) {
                int adj = it.first;
                int weight = (it.second) + heuristic(adj, v);
                if (dist[adj] > dist[v] + weight) { //found better path to adj (or found it at all)
                    if (dist[adj] != INT_MAX)   //the new v was already found before, but the new path is better; will be pushed in anyway
                        considered.erase(considered.find(std::make_pair(dist[adj], adj)));
                    dist[adj] = dist[v] + weight;
                    stp[adj] = v;
                    considered.insert(std::make_pair(dist[adj], adj));
                }
            }
        }

        //calculate the path
        std::list<int> path;
        if (stp[dest] != ABSENT) {
            int index = dest;
            path.push_front(dest);

            while (index != src) {
                index = stp[index];
                path.push_front(index);
            }
        }
        return path;
    }
};

void printPath(std::list<int> path) {
    char chArr[X*Y];
    std::memset(chArr, ' ', X*Y);
    for(auto a : path) {
        chArr[a] = '1';
    }
    chArr[*path.begin()] = '0';
    chArr[*(--path.end())] = '0';

    std::string out;
    for (int y=0; y<Y; ++y) {
        for (int x=0; x<X; ++x) {
            out.push_back(chArr[y*X+x]);
        }
        out.push_back('\n');
    }
    std::cout << out;
}

int main() {
    int graphArray[X][Y];
    readArrayFromFile(graphArray);
    printArray(graphArray);

    Graph graph(graphArray);
    graph.print();

    std::list<int> path = graph.findShortestPath(X-1, X*(Y-1));

    printPath(path);
}
