#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <omp.h>
#include <random>

using namespace std;

long long factorial(int n) { // Use long long for factorial
    return (n == 1 || n == 0) ? 1 : n * factorial(n - 1);
}

vector<int> findShortestRoute(const vector<vector<long long>>& distances, int numThreads) { // distances are long long
    int numCities = distances.size();
    vector<int> cities(numCities);
    for (int i = 0; i < numCities; ++i) {
        cities[i] = i;
    }

    vector<int> shortestRoute;
    long long shortestDistance = LLONG_MAX; // Use long long

    #pragma omp parallel num_threads(numThreads)
    {
        vector<int> localShortestRoute;
        long long localShortestDistance = LLONG_MAX; // Use long long

        #pragma omp for
        for (long long i = 0; i < factorial(numCities - 1); ++i) { // Use long long for loop index
            vector<int> localCities = cities;
            next_permutation(localCities.begin() + 1, localCities.end());

            long long currentDistance = 0; // Use long long
            for (int j = 0; j < numCities - 1; ++j) {
                currentDistance += distances[localCities[j]][localCities[j + 1]];
            }
            currentDistance += distances[localCities[numCities - 1]][localCities[0]]; // add return to the starting city

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

    int numThreads = 1;

    // Start measuring time
    double startTime = omp_get_wtime();

    // Perform TSP computation
    vector<int> shortestRoute = findShortestRoute(distances, numThreads);

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