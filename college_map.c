#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define V 12
#define MAX_ROUTES 10000
#define MAX_RECENT 5
#define INF 1000000000

char locations[V][60] = {
    "Main Gate",
    "Main Building",
    "Central Library",
    "Canteen",
    "Computer Science Department",
    "Admin Block",
    "CEG Main Building",
    "Mechanical Department",
    "Auditorium",
    "Hostel",
    "Sports Ground",
    "Playground"
};

int graph[V][V];

typedef struct {
    int path[V];
    int length;
    int distance;
} Route;

typedef struct {
    char source[60];
    char destination[60];
    int distance;
} SearchHistory;

Route routes[MAX_ROUTES];
int routeCount = 0;
int currentPath[V];
int visited[V];
int recentCount = 0;
int recentNext = 0;
SearchHistory recent[MAX_RECENT];

/* Compare names without considering uppercase/lowercase */
int sameName(const char a[], const char b[]) {
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0') {
        if (tolower((unsigned char)a[i]) !=
            tolower((unsigned char)b[i])) {
            return 0;
        }
        i++;
    }

    return a[i] == '\0' && b[i] == '\0';
}

/* Find location using its name */
int findLocation(const char name[]) {
    int i;

    for (i = 0; i < V; i++) {
        if (sameName(name, locations[i])) {
            return i;
        }
    }

    return -1;
}

/* Read a complete line */
void readLine(char text[], int size) {
    if (fgets(text, size, stdin) != NULL) {
        text[strcspn(text, "\n")] = '\0';
    }
}

/* Add a two-way road */
void addRoad(int a, int b, int distance) {
    graph[a][b] = distance;
    graph[b][a] = distance;
}

/* Build the sample campus graph */
void buildGraph(void) {
    addRoad(0, 1, 100);
    addRoad(0, 5, 150);
    addRoad(1, 2, 100);
    addRoad(1, 4, 150);
    addRoad(1, 6, 200);
    addRoad(2, 3, 100);
    addRoad(2, 4, 120);
    addRoad(3, 6, 180);
    addRoad(3, 8, 120);
    addRoad(4, 6, 100);
    addRoad(4, 7, 180);
    addRoad(5, 6, 200);
    addRoad(5, 9, 250);
    addRoad(6, 7, 150);
    addRoad(6, 8, 100);
    addRoad(7, 10, 200);
    addRoad(8, 11, 150);
    addRoad(9, 10, 180);
    addRoad(10, 11, 100);
}

/* Display all locations */
void displayLocations(void) {
    int i;

    printf("\n========== COLLEGE LOCATIONS ==========\n");

    for (i = 0; i < V; i++) {
        printf("%2d. %s\n", i + 1, locations[i]);
    }
}

/* Display a route */
void printRoute(const int path[], int length, int distance) {
    int i;

    for (i = 0; i < length; i++) {
        printf("%s", locations[path[i]]);

        if (i < length - 1) {
            printf(" -> ");
        }
    }

    printf("\nDistance: %d metres\n", distance);
}

/* Save a search in recent history */
void saveHistory(int source, int destination, int distance) {
    strcpy(recent[recentNext].source, locations[source]);
    strcpy(recent[recentNext].destination, locations[destination]);
    recent[recentNext].distance = distance;

    recentNext = (recentNext + 1) % MAX_RECENT;

    if (recentCount < MAX_RECENT) {
        recentCount++;
    }
}

/* DFS with backtracking to enumerate simple routes */
void findRoutesDFS(int current, int destination,
                   int length, int distance) {
    int i;

    currentPath[length++] = current;
    visited[current] = 1;

    if (current == destination) {
        if (routeCount < MAX_ROUTES) {
            memcpy(routes[routeCount].path, currentPath,
                   length * sizeof(int));

            routes[routeCount].length = length;
            routes[routeCount].distance = distance;
            routeCount++;
        }

        visited[current] = 0;
        return;
    }

    for (i = 0; i < V; i++) {
        if (graph[current][i] > 0 && !visited[i]
            && routeCount < MAX_ROUTES) {
            findRoutesDFS(i, destination, length,
                          distance + graph[current][i]);
        }
    }

    visited[current] = 0;
}

/* Display every possible route and its distance */
void displayAllRoutes(int source, int destination) {
    int i;
    int shortestIndex = 0;

    routeCount = 0;
    memset(visited, 0, sizeof(visited));

    findRoutesDFS(source, destination, 0, 0);

    if (routeCount == 0) {
        printf("\nNo route found.\n");
        return;
    }

    printf("\n========== ALL POSSIBLE ROUTES ==========\n");

    for (i = 0; i < routeCount; i++) {
        printf("\nRoute %d:\n", i + 1);

        printRoute(routes[i].path, routes[i].length,
                   routes[i].distance);

        if (routes[i].distance < routes[shortestIndex].distance) {
            shortestIndex = i;
        }
    }

    printf("\nTotal routes found: %d\n", routeCount);

    if (routeCount == MAX_ROUTES) {
        printf("Warning: route limit reached; more routes may exist.\n");
    }

    printf("\n========== SHORTEST ROUTE ==========\n");
    printRoute(routes[shortestIndex].path,
               routes[shortestIndex].length,
               routes[shortestIndex].distance);
}

/* Dijkstra's algorithm */
int dijkstra(int source, int destination,
             int shortestPath[], int *pathLength) {
    int distance[V];
    int parent[V];
    int used[V] = {0};
    int i, j, current;

    for (i = 0; i < V; i++) {
        distance[i] = INF;
        parent[i] = -1;
    }

    distance[source] = 0;

    for (i = 0; i < V; i++) {
        int minimum = INF;
        current = -1;

        for (j = 0; j < V; j++) {
            if (!used[j] && distance[j] < minimum) {
                minimum = distance[j];
                current = j;
            }
        }

        if (current == -1) {
            break;
        }

        used[current] = 1;

        for (j = 0; j < V; j++) {
            if (graph[current][j] > 0 &&
                distance[current] + graph[current][j] < distance[j]) {

                distance[j] = distance[current] + graph[current][j];
                parent[j] = current;
            }
        }
    }

    if (distance[destination] == INF) {
        return -1;
    }

    int reversePath[V];
    int count = 0;
    current = destination;

    while (current != -1) {
        reversePath[count++] = current;
        current = parent[current];
    }

    *pathLength = count;

    for (i = 0; i < count; i++) {
        shortestPath[i] = reversePath[count - i - 1];
    }

    return distance[destination];
}

/* Show the three shortest routes */
void displayTopThree(void) {
    int chosen[3];
    int i, j, best;

    if (routeCount == 0) {
        printf("\nFind routes first.\n");
        return;
    }

    for (i = 0; i < 3; i++) {
        chosen[i] = -1;
    }

    for (i = 0; i < 3; i++) {
        best = -1;

        for (j = 0; j < routeCount; j++) {
            int alreadyChosen = 0;
            int k;

            for (k = 0; k < i; k++) {
                if (chosen[k] == j) {
                    alreadyChosen = 1;
                    break;
                }
            }

            if (!alreadyChosen &&
                (best == -1 ||
                 routes[j].distance < routes[best].distance)) {
                best = j;
            }
        }

        if (best == -1) {
            break;
        }

        chosen[i] = best;
    }

    printf("\n========== TOP 3 SHORTEST ROUTES ==========\n");

    for (i = 0; i < 3; i++) {
        if (chosen[i] == -1) {
            break;
        }

        printf("\nRank %d:\n", i + 1);

        printRoute(routes[chosen[i]].path,
                   routes[chosen[i]].length,
                   routes[chosen[i]].distance);
    }
}

/* Estimate walking time at 5 km/h */
void estimateWalkingTime(int distance) {
    double minutes = distance / (5000.0 / 60.0);

    printf("\nEstimated walking time: %.1f minutes\n", minutes);
    printf("Assumed walking speed: 5 km/h\n");
}

/* Display recent searches */
void displayHistory(void) {
    int i, index;

    printf("\n========== RECENT SEARCHES ==========\n");

    if (recentCount == 0) {
        printf("No searches yet.\n");
        return;
    }

    for (i = 0; i < recentCount; i++) {
        index = (recentNext - recentCount + i + MAX_RECENT)
                % MAX_RECENT;

        printf("%d. %s -> %s | %d metres\n",
               i + 1,
               recent[index].source,
               recent[index].destination,
               recent[index].distance);
    }
}

/* Search for a location */
void searchLocation(void) {
    char name[100];
    int location;

    printf("\nEnter location name: ");
    readLine(name, sizeof(name));

    location = findLocation(name);

    if (location == -1) {
        printf("Location not found. Check the spelling.\n");
    } else {
        printf("Location found: %s (ID: %d)\n",
               locations[location], location + 1);
    }
}

/* Select source and destination */
void routeMenu(void) {
    char sourceName[100];
    char destinationName[100];
    int source, destination;
    int shortestPath[V];
    int pathLength = 0;
    int shortestDistance;

    printf("\nEnter source location: ");
    readLine(sourceName, sizeof(sourceName));

    printf("Enter destination location: ");
    readLine(destinationName, sizeof(destinationName));

    source = findLocation(sourceName);
    destination = findLocation(destinationName);

    if (source == -1 || destination == -1) {
        printf("\nInvalid location name.\n");
        printf("Please use the names shown in the location list.\n");
        return;
    }

    if (source == destination) {
        printf("\nSource and destination are the same.\n");
        return;
    }

    shortestDistance = dijkstra(source, destination,
                                shortestPath, &pathLength);

    if (shortestDistance == -1) {
        printf("\nNo connected route exists.\n");
        return;
    }

    saveHistory(source, destination, shortestDistance);

    displayAllRoutes(source, destination);

    printf("\n========== DIJKSTRA RESULT ==========\n");
    printRoute(shortestPath, pathLength, shortestDistance);

    estimateWalkingTime(shortestDistance);
}

int main(void) {
    char input[100];
    int choice;

    buildGraph();

    do {
        printf("\n========================================\n");
        printf("   COLLEGE MAP NAVIGATION SYSTEM\n");
        printf("========================================\n");
        printf("1. Display All College Locations\n");
        printf("2. Find All Routes and Shortest Route\n");
        printf("3. Display Top 3 Shortest Routes\n");
        printf("4. Search College Location\n");
        printf("5. View Recent Searches\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        readLine(input, sizeof(input));

        if (sscanf(input, "%d", &choice) != 1) {
            printf("Please enter a valid menu number.\n");
            continue;
        }

        switch (choice) {
            case 1:
                displayLocations();
                break;

            case 2:
                routeMenu();
                break;

            case 3:
                displayTopThree();
                break;

            case 4:
                searchLocation();
                break;

            case 5:
                displayHistory();
                break;

            case 6:
                printf("\nThank you for using College Map Navigation!\n");
                break;

            default:
                printf("Invalid choice. Select 1 to 6.\n");
        }

    } while (choice != 6);

    return 0;
}
