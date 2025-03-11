#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <omp.h> // OpenMP header
#include <random>

using namespace std;

int factorial(int n) {
    return (n == 1 || n == 0) ? 1 : n * factorial(n - 1);
}

vector<int> findShortestRoute(const vector<vector<int>>& distances, int numThreads) {
    int numCities = distances.size();
    vector<int> cities(numCities);
    for (int i = 0; i < numCities; ++i) {
        cities[i] = i;
    }

    vector<int> shortestRoute;
    int shortestDistance = INT_MAX;

    #pragma omp parallel num_threads(numThreads)
    {
        vector<int> localShortestRoute;
        int localShortestDistance = INT_MAX;

        #pragma omp for
        for (int i = 0; i < factorial(numCities - 1); ++i) {
            vector<int> localCities = cities;
            next_permutation(localCities.begin() + 1, localCities.end());

            int currentDistance = 0;
            for (int j = 0; j < numCities - 1; ++j) {
                currentDistance += distances[ localCities[j] ][ localCities[j+1] ];
            }
            currentDistance += distances[ localCities[numCities - 1] ][ localCities[0] ]; // add return to the starting city

            if (currentDistance < localShortestDistance) {
                localShortestDistance = currentDistance;
                localShortestRoute = localCities;
            }
        }

        #pragma omp critical
        {
            if (localShortestDistance < shortestDistance) {
                shortestDistance = localShortestDistance;
                shortestRoute = localShortestRoute;
            }
        }
    }

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

    int numThreads = 64;
    vector<int> shortestRoute = findShortestRoute(distances, numThreads);

    cout << "Shortest route: ";
    for (int city : shortestRoute) {
        cout << city << " ";
    }
    cout << shortestRoute[0] << endl;

    return 0;
}
