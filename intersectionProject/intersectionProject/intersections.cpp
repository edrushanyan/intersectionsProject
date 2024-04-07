#include <iostream>
#include <vector>
#include <unordered_map>

#define MAX 2147483647

using std::vector;
using std::string;
using std::unordered_map;
using std::pair;
using std::endl;
using std::swap;

struct road {
public:

    //  declaring dist. (stores the index of the destination intersection) and len. (stores the length of the road) for road
	int destination;
	int length;

    // constructor
	road(int destination, int length) : destination(destination), length(length) {}
};

class intersectionMap {
private:

	unordered_map<string, int> nameToIndex;
	vector<string> indexToName;

public:

    intersectionMap(const vector<string>& names) {
		
        // loop through the names
		for (int i = 0; i < names.size(); ++i) {

            // map the intersection name to its index
			nameToIndex[names[i]] = i;

            // store the intersection name at its index
			indexToName.push_back(names[i]);
		}
	}

    // function for getting the index of an intersection given its name
	int getIndex(const string& name) const {

		return nameToIndex.at(name);
	}

    // function for getting the name of an intersection given its index
	string getName(int index) const {

		return indexToName[index];
	}
};

class Graph {
private:
    
    // variable that stores the adjacency list of the graph
    vector<vector<road>> adjacencyList;

public:

    // constructor
    Graph(int size) : adjacencyList(size) {}

    // function for adding roads
    void addRoad(int from, int to, int length) {
        adjacencyList[from].push_back(road(to, length));
    }

    // function for finding neighbor roads
    const vector<road>& getNeighbors(int intersection) const {
        return adjacencyList[intersection];
    }

    // function for returning adjacency list's size
    int size() const {
        return adjacencyList.size();
    }
};

class Dijkstra {
private:

    // variable that stores the graph on which Dijkstra's algorithm will be applied
    const Graph& graph;

    //  stores the distances from the starting intersection to each intersection
    vector<int> distances;

    // stores the previous intersection on the shortest path to each intersection
    vector<int> previous;

public:

    // constructor
    Dijkstra(const Graph& g) : graph(g), distances(g.size(), MAX), previous(g.size(), -1) {}

    void run(int start) {
        distances[start] = 0;

        vector<bool> visited(graph.size(), false);

        for (int i = 0; i < graph.size(); ++i) {
        
            int current = -1;
            
            for (int j = 0; j < graph.size(); ++j) {
            
                if (!visited[j] && (current == -1 || distances[j] < distances[current])) {
                    current = j;
                }
            }

            if (distances[current] == MAX) {
                break;
            }

            visited[current] = true;

            for (const road& road : graph.getNeighbors(current)) {
                
                int alt = distances[current] + road.length;
                
                if (alt < distances[road.destination]) {
                
                    distances[road.destination] = alt;
                    previous[road.destination] = current;
                }
            }
        }
    }

    int getDistance(int destination) const {
        return distances[destination];
    }

    vector<int> getPath(int destination) const {

        vector<int> path;

        for (int v = destination; v != -1; v = previous[v]) {

            path.push_back(v);
        }
        
        reverse(path.begin(), path.end());
        
        return path;
    }
};

class PermutationGenerator {
public:

    static vector<vector<int>> generatePermutations(int L) {

        vector<vector<int>> permutations;
        vector<int> current(L);
        
        for (int i = 0; i < L; ++i) {
            current[i] = i + 1;
        }
        
        permutations.push_back(current);
        
        while (nextPermutation(current)) {
            permutations.push_back(current);
        }
        
        return permutations;
    }

private:

    static bool nextPermutation(vector<int>& arr) {
        int i = arr.size() - 2;
        
        while (i >= 0 && arr[i] >= arr[i + 1]) {
            i--;
        }
        
        if (i < 0) {
            return false;
        }
        
        int j = arr.size() - 1;
        
        while (arr[j] <= arr[i]) {
            j--;
        }
        
        swap(arr[i], arr[j]);
        
        reverse(arr.begin() + i + 1, arr.end());
        
        return true;
    }
};



int main() {

    int L = 4;

    vector<vector<int>> perms = PermutationGenerator::generatePermutations(L);
    
    for (const auto& perm : perms) {
    
        for (int i : perm) {
        
            std::cout << i << " ";
        }

        std::cout << endl;
    }

    return 0;
}