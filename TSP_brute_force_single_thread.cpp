#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <random>
#include <omp.h>

using namespace std;

vector<int> findShortestRoute(const vector<vector<long long>>& distances) { // distances are long long
    int numCities = distances.size();
    vector<int> cities(numCities);
    for (int i = 0; i < numCities; ++i) {
        cities[i] = i;
    }

    vector<int> shortestRoute;
    long long shortestDistance = LLONG_MAX; // Use long long

    do {
        long long currentDistance = 0; // Use long long
        for (int i = 0; i < numCities - 1; ++i) {
            currentDistance += distances[cities[i]][cities[i + 1]];
        }
        currentDistance += distances[cities[numCities - 1]][cities[0]]; // add return to the starting city

        if (currentDistance < shortestDistance) {
            shortestDistance = currentDistance;
            shortestRoute = cities;
        }
    } while (next_permutation(cities.begin() + 1, cities.end())); // generate nex permutation of cities

    return shortestRoute;
}

int main() {
    int numCities = 12;
    vector<vector<long long>> distances(numCities, vector<long long>(numCities)); // distances are long long
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<long long> distrib(1, 100); // Use long long distribution

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

    // Start measuring time
    double startTime = omp_get_wtime();

    // Perform TSP computation
    vector<int> shortestRoute = findShortestRoute(distances);

    // End measuring time
    double endTime = omp_get_wtime();

    // Calculate elapsed time
    double elapsedTime = endTime - startTime;

    // Display the results
    cout << "Shortest route: ";
    for (int city : shortestRoute) {
        cout << city << " ";
    }
    cout << shortestRoute[0] << endl;

    cout << "Execution time: " << elapsedTime << " seconds" << endl;

    return 0;
}