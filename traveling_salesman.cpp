#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <random>

using namespace std;

vector<int> findShortestRoute(const vector<vector<int>>& distances) {
    int numCities = distances.size();
    vector<int> cities(numCities);
    for (int i = 0; i < numCities; ++i) {
        cities[i] = i;
    }

    vector<int> shortestRoute;
    int shortestDistance = INT_MAX;

    do {
        int currentDistance = 0;
        for (int i = 0; i < numCities - 1; ++i) {
            currentDistance += distances[ cities[i] ][ cities[i+1] ];
        }
        currentDistance += distances[ cities[ numCities - 1] ][ cities[0] ]; // add return to the starting city

        if (currentDistance < shortestDistance) {
            shortestDistance = currentDistance;
            shortestRoute = cities;
        }
    } while (next_permutation(cities.begin() + 1, cities.end())); // generate nex permutation of cities

    return shortestRoute;
}

int main() {
    int numCities = 10;
    vector<vector<int>> distances(numCities, vector<int>(numCities));
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1, 100);

    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            if (i == j) {
                distances[i][j] = 0;
            } else {
                distances[i][j] = distrib(gen);
                distances[j][i] = distances[i][j];
            }
        }
    }

    vector<int> shortestRoute = findShortestRoute(distances);

    cout << "Shortest route: ";
    for (int city : shortestRoute) {
        cout << city << " ";
    }
    cout << shortestRoute[0] << endl;

    return 0;
}