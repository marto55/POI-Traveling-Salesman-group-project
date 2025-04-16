#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <iomanip>
#include <algorithm>
#include <random>
#include <omp.h> // Include OpenMP

using namespace std;

// Function to reconstruct the shortest path
vector<int> reconstructPath(const vector<vector<int>>& dpPrev, int numCities, int lastCity) {
    vector<int> path;
    int currentCity = lastCity; // Start from the last city
    int mask = (1 << numCities) - 1; // All cities visited

    while (path.size() < numCities) {
        path.push_back(currentCity);
        int nextCity = dpPrev[mask][currentCity];
        mask ^= (1 << currentCity); // Remove the current city from the visited set
        currentCity = nextCity;
    }
    reverse(path.begin(), path.end()); // Reverse the path to get the correct order
    return path;
}

pair<long long, vector<int>> tspDynamicProgramming(const vector<vector<int>>& distances) {
    int numCities = distances.size();
    int numSubsets = 1 << numCities;
    long long INF = 1e18;
    vector<vector<long long>> dp(numSubsets, vector<long long>(numCities, INF));
    vector<vector<int>> dpPrev(numSubsets, vector<int>(numCities, -1));

    dp[1][0] = 0;

    for (int mask = 1; mask < numSubsets; ++mask) {
        for (int u = 0; u < numCities; ++u) {
            if (mask & (1 << u)) {
                for (int v = 0; v < numCities; ++v) {
                    if (!(mask & (1 << v))) {
                        int nextMask = mask | (1 << v);
                        long long newDistance = dp[mask][u] + distances[u][v];

                        if (newDistance < dp[nextMask][v]) {
                            dp[nextMask][v] = newDistance;
                            dpPrev[nextMask][v] = u;
                        }
                    }
                }
            }
        }
    }

    long long shortestDistance = INF;
    int lastCity = -1;
    int fullMask = numSubsets - 1;

    for (int u = 1; u < numCities; ++u) {
        long long candidateDistance = dp[fullMask][u] + distances[u][0];
        if (candidateDistance < shortestDistance) {
            shortestDistance = candidateDistance;
            lastCity = u;
        }
    }

    vector<int> shortestPath = reconstructPath(dpPrev, numCities, lastCity); // Pass lastCity
    shortestPath.push_back(0);

    return {shortestDistance, shortestPath};
}

int main() {

    int numCities = 22;
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

    // vector<vector<int>> distances = {
    //     {0, 10, 15, 20},
    //     {10, 0, 35, 25},
    //     {15, 35, 0, 30},
    //     {20, 25, 30, 0}
    // };

    // Start measuring time (using OpenMP)
    double startTime = omp_get_wtime();

    pair<long long, vector<int>> result = tspDynamicProgramming(distances);

    // End measuring time (using OpenMP)
    double endTime = omp_get_wtime();

    // Calculate elapsed time
    double elapsedTime = endTime - startTime;

    // cout << "Shortest distance: " << result.first << endl;
    // cout << "Shortest path: ";
    // for (int city : result.second) {
    //     cout << city << " ";
    // }
    // cout << endl;

    cout << elapsedTime  << endl;

    return 0;
}