#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <climits>
#include <utility>

using std::string;
using std::vector;
using std::pair;
using std::priority_queue;
using std::endl;
using std::greater;
using std::unordered_map;

// Class to represent an intersection
class Intersection {
public:
    string name;
    int index;

    Intersection(string name, int index) : name(name), index(index) {}
};

// Class to represent the graph
class Graph {
public:

    int N;

    vector<vector<pair<int, int>>> adjList;
    vector<Intersection> intersections;

    Graph(int N) : N(N), adjList(N) {}

    void addDirectedRoad(int A, int B, int length) {
        adjList[A].push_back({ B, length });
    }

    int findIntersectionIndex(string name) {

        for (int i = 0; i < intersections.size(); ++i) {

            const auto& intersection = intersections[i];

            if (intersection.name == name) {

                return intersection.index;
            }
        }

        // Intersection not found
        return -1;
    }
};

// Class to perform Dijkstra's algorithm
class Dijkstra {
public:

    static vector<int> findShortestPaths(const Graph& graph, int start) {

        vector<int> distances(graph.N, INT_MAX);

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        distances[start] = 0;
        pq.push({ 0, start });

        while (!pq.empty()) {

            int u = pq.top().second;
            pq.pop();

            for (int i = 0; i < graph.adjList[u].size(); ++i) {

                const auto& neighbor = graph.adjList[u][i];
                int v = neighbor.first;
                int weight = neighbor.second;

                if (distances[u] != INT_MAX && weight != INT_MAX && weight >= 0 && distances[u] + weight < distances[v]) {

                    distances[v] = distances[u] + weight;
                    pq.push({ distances[v], v });
                }
            }
        }

        return distances;
    }
};

// Class to generate permutations
class PermutationGenerator {
public:

    static void generatePermutations(vector<int>& waypoints, vector<vector<int>>& permutations) {

        sort(waypoints.begin(), waypoints.end());

        do {

            permutations.push_back(waypoints);
        } while (next_permutation(waypoints.begin(), waypoints.end()));
    }
};

int main() {

    int N, M;

    std::cin >> N >> M;

    Graph graph(N);
    unordered_map<string, int> intersectionMap;

    // Read intersection names and store them in the graph
    for (int i = 0; i < N; ++i) {

        string name;
        std::cin >> name;

        graph.intersections.emplace_back(name, i);
        intersectionMap[name] = i;
    }

    // Read road descriptions and add them to the graph
    for (int i = 0; i < M; ++i) {

        string A, B;
        int length;

        std::cin >> A >> B >> length;

        int indexA = intersectionMap[A];
        int indexB = intersectionMap[B];
        graph.addDirectedRoad(indexA, indexB, length); // Use the directed addRoad function
    }

    int s, L;
    std::cin >> s >> L;

    // Check if L is greater than 6
    if (L > 6) {

        std::cerr << "Error: L must be less than or equal to 6." << endl;
        return 1;
    }

    vector<int> waypoints(L);

    // Read waypoint names and convert them to indices
    for (int i = 0; i < L; ++i) {

        string waypointName;
        std::cin >> waypointName;
        waypoints[i] = intersectionMap[waypointName];
    }

    // Generate all permutations of the waypoints
    vector<vector<int>> permutations;
    PermutationGenerator::generatePermutations(waypoints, permutations);

    int minTourLength = INT_MAX;
    vector<int> minTourOrder;

    // Find the shortest tour by checking all permutations
    for (int i = 0; i < permutations.size(); ++i) {

        const auto& permutation = permutations[i];
        int tourLength = 0;
        int current = s;

        // Calculate the length of the tour for the current permutation
        for (int i = 0; i < permutation.size(); ++i) {

            int waypoint = permutation[i];
            vector<int> distances = Dijkstra::findShortestPaths(graph, current);

            if (distances[waypoint] == INT_MAX) {

                // If there is no path from current to waypoint, skip this permutation
                tourLength = INT_MAX;
                break;
            }

            tourLength += distances[waypoint];
            current = waypoint;
        }

        // After visiting all waypoints, calculate the distance back to the starting intersection
        vector<int> finalDistances = Dijkstra::findShortestPaths(graph, current);

        if (finalDistances[s] == INT_MAX) {
            // If there is no path from the last waypoint back to the starting intersection, skip this permutation
            tourLength = INT_MAX;
        }

        else {
            tourLength += finalDistances[s];
        }

        // Update the minimum tour length and order if a shorter tour is found
        if (tourLength < minTourLength) {
            minTourLength = tourLength;
            minTourOrder = permutation;
        }
    }

    // Output the results
    if (minTourLength == INT_MAX) {
        std::cout << "No valid tour found." << endl;
    }

    else {

        std::cout << "Shortest tour length: " << minTourLength << endl;
        std::cout << "Tour order: ";

        for (int i = 0; i < minTourOrder.size(); ++i) {

            int index = minTourOrder[i];
            std::cout << graph.intersections[index].name << " ";
        }

        std::cout << graph.intersections[s].name << endl; // Return to the starting intersection
    }

    return 0;
}