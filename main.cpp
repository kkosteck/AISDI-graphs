#include <iostream>
#include <fstream>
#include <vector>
#include <set>
using namespace std;

#define X 7
#define Y 7
#define INFINITY 999999999

void readArray(int graph[X][Y]){
	fstream graphFile;
	graphFile.open("graf.txt", ios::in);
	for(int i = 0; i< Y; ++i){
		string temporary;
		graphFile >> temporary;
		for(int j = 0; j < X; ++j)
			graph[j][i] = temporary[j] - '0';
	}
}

void printArray(int graph[X][Y]){
	for(int i = 0; i< Y; ++i){
		for(int j = 0; j < X; ++j){
			cout<< graph[j][i];
		}
		cout<<endl;
	}
}

class Graph{
private:
	vector<pair<int, int>> nodeEdges[X*Y];
	
	int hValue(int cellIndex, int destinationIndex){
		return abs(cellIndex % X - destinationIndex % X) + abs(cellIndex / Y - destinationIndex / Y);
	}
public:
	Graph(int array[X][Y]){
		for(int i = 0; i < Y; ++i){
			for(int j = 0; j < X; ++j){
				int index = Y*i + j;
				if(j + 1 < X)
					nodeEdges[index].push_back(make_pair(index + 1, array[j + 1][i]));
				if(j - 1 >= 0)
					nodeEdges[index].push_back(make_pair(index - 1, array[j - 1][i]));
				if(i + 1 < Y)
					nodeEdges[index].push_back(make_pair(index + Y, array[j][i + 1]));
				if(i - 1 >= 0)
					nodeEdges[index].push_back(make_pair(index - Y, array[j][i - 1]));
			}
		}
	}
	
	vector<int> findPath(int startIndex, int endIndex){
		vector<int> distance(X*Y, INFINITY);
		set<pair<int, int>> openSet;
		vector<int> theBestPath(X*Y, -1);
		
		distance[startIndex] = 0;
		theBestPath[startIndex] = startIndex;
		openSet.insert(make_pair(0, startIndex));
		
		while(!openSet.empty()){
			int current = openSet.begin()->second;
			openSet.erase(openSet.begin());
			
			for(auto it : nodeEdges[current]){
				int weight = it.second + hValue(it.first, endIndex);
				if(distance[it.first] > distance[current] + weight){
					if(distance[it.first] != INFINITY)
						openSet.erase(openSet.find(make_pair(distance[it.first], it.first)));
					distance[it.first] = distance[current] + weight;
					theBestPath[it.first] = current;
					openSet.insert(make_pair(distance[it.first], it.first));
				}
			}
		}
		
        //calculate the path
        vector<int> path;
        if (theBestPath[endIndex] != -1) {
            int index = endIndex;
            path.push_back(endIndex);

            while (index != startIndex) {
                index = theBestPath[index];
                path.push_back(index);
            }
        }
        return path;
    }
	
};

void printPath(vector<int> path){
    vector<char> chArr(X*Y, ' ');
    for(auto a : path) {
        chArr[a] = '*';
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

int main(){
	int graph[X][Y];
	readArray(graph);
	printArray(graph);
	Graph map(graph);
	
	vector<int> path = map.findPath(X-1, (Y-1)*X);
	printPath(path);
	
}
