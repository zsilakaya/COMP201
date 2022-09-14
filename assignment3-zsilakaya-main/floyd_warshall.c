/*This this code, Floyd-Warshall algorithm is implemented
 * to find the shortest paths between any two cities
 * in a given graph of cities.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

const int INF = 1e7;

/* Array of vertices. 
 * Each element of the str array contains a city name and 
 * its index in str becomes the city's numeric id.
 */
typedef struct vertices {
	// This attribute shows the size of dynamically allocated memory
	int size;
	// This attribute shows the number of cities in the array
	int city_count;
	// This attribute is the array of city names
	char (*str)[30];
} vertices;

/* Array of edges. 
 * Each element of the edge array contains the ids of two cities 
 * connected directy by a road and the length of the road.
 */
typedef struct edges {
	// This attribute shows the size of dynamically allocated memory
	int size;
	// This attribute shows the number of roads in the array
	int edge_count;
	// This attribute is the array of road information
	int (*edge)[3];
} edges;

vertices cities;

edges roads;

/* A two-dimensional array that shows 
 * the length of the shortest path connecting 
 * any two cities.
 */
int **city_graph;

/* A two-dimensional array that shows the direction 
 * to the shortest path between any two cities.
 */
int **shortest_paths;

/* A function that implements Floyd-Warshall algorithm. 
 * This algorithm finds the shortest path between 
 * any two vertices in a given graph.
 */
void floydWarshall() {
	for (int k = 0; k < cities.city_count; k++) {
		for (int i = 0; i < cities.city_count; i++) {
			for (int j = 0; j < cities.city_count; j++) {
				// We cannot cross a road that doesn't exist  
				if ((city_graph[i][k] == INF) || (city_graph[k][j] == INF)) {
					continue;
				}
				if (city_graph[i][j] > (city_graph[i][k] + city_graph[k][j])) {
					city_graph[i][j] = city_graph[i][k] + city_graph[k][j];
					shortest_paths[i][j] = shortest_paths[i][k];
				}
			}
		}
	}
}

/* A function that initializes the values in 
 * city_graph and shortest_paths arrays.
 */
void initialise() {
	for (int i = 0; i < cities.city_count; i++) {
		for (int j = 0; j < cities.city_count; j++) {

			// No edge between node i and j  
			if (city_graph[i][j] == INF) {
				shortest_paths[i][j] = -1;
			} else {
				shortest_paths[i][j] = j;
			}
		}
	}
}
/* A function that looks up if a city name
 * is already in the cities array.
 */
int lookup_string(char str[30]) {
	for (int i = 0; i < cities.city_count; i++) {
		if (strncmp(str, cities.str[i], 30) == 0) {
			return i;
		}
	}
	printf("city %s is not found\n", str);
	return -1;
}

/* A function that inserts the name of a new city 
 * to the cities array.
 */
int insert_to_cities(char str[30]) {
	// Write your code here
	// Check if the city name already exists in the array 
	// If it does, return the array index of the city 

	// If the city name doesn't exist, it is inserted to the vertex array, and
	// return the array index of the city

	// If the dynamically allocated size of vertex_array is not large enough, 
	// vertex_array is enlarged and the city name is inserted 
	// to the newly added region
	int lastEl = cities.city_count;
	int retVal = lookup_string(str);
	if (retVal != -1) {
		return retVal;
	} else {
		if (lastEl < 30) {
			strcpy(cities.str[lastEl], str);//Adds the cities into cities.str array.
			cities.city_count++;
			return lastEl;
		} else {
			cities.size = cities.city_count * sizeof(char[30]);//İf size is not enough resize the cities.str array.
			cities.str = realloc(cities.str, cities.size);
			if (cities.str == NULL) {
				printf("memory allocation error");
				exit(1);
			}
			strcpy(cities.str[lastEl], str);
			cities.city_count++;
			return lastEl;
		}
	}
	// Return the array index of the city.
}

void printPath(int u, int v) {
	// Write your code here  
	int city = shortest_paths[u][v];
	if (city == INF) {
		printf("NO PATH\n");
	} else if (city == v) {
		printf("%s %s", cities.str[u], cities.str[v]);
	} else {
		printf("%s ", cities.str[u]);
		while (city != v) {
			
			printf("%s ", cities.str[city]);
			city = shortest_paths[city][v];
		}
		printf("%s \n", cities.str[city]);
	}
}

int main(int argc, char *argv[]) {
	// Write your code here
	cities.city_count = 0;
	cities.size = 30 * sizeof(char[30]);
	cities.str = malloc(cities.size);// Allocate memory regions dynamically to vertex array
	if (cities.str == NULL) {// If memory is not allocated exit.
		printf("Memory allocation error\n");
		exit(1);
	}
	roads.edge_count = 0;// Allocate memory regions dynamically to edge_array.
	roads.size = 30 * sizeof(int[3]);
	roads.edge = malloc(roads.size);
	if (roads.edge == NULL) {// if memory is not allocated exit.
		printf("Memory allocation error\n");
		exit(1);
	}
	FILE *inputFile;
	inputFile = fopen("input.txt", "r");
	char line[100];
	while (fgets(line, sizeof(line), inputFile) != NULL) { // Reads the input file.
		char *words = strtok(line, " ");
		char *wordsArray[3];
		int wordCounter = 0;
		int cityIds = 0;
		while (words != NULL) {
			wordsArray[wordCounter] = words;
			wordCounter++;
			words = strtok(NULL, " ");
		}
		for (int i = 0; i < wordCounter; i++) {
			if (i < wordCounter - 1) {
				insert_to_cities(wordsArray[i]); //Insert the city names and ids to vertex_array.
				cityIds = (int) lookup_string(wordsArray[i]);
				roads.edge[roads.edge_count][i] = cityIds;
			} else {
				int length = atoi(wordsArray[i]);
				roads.edge[roads.edge_count][i] = length;//Inserts the roads to roads.edge array.
				roads.edge_count++;
			}
		}

	}
	fclose(inputFile);
	// Allocate memory regions dynamically to city_graph,
	// distance, and shortest_paths arrays.
	city_graph = (int**) calloc(cities.city_count, sizeof(int*));
	for (int i = 0; i < cities.city_count; i++) {
		city_graph[i] = (int*) calloc(cities.city_count, sizeof(int));
	}
	shortest_paths = (int**) calloc(cities.city_count, sizeof(int*));
	for (int i = 0; i < cities.city_count; i++) {
		shortest_paths[i] = (int*) calloc(cities.city_count, sizeof(int));
	}
	// Initialize the values in city_graph array with road lengths,
	// such that the value in city_graph[i][j] is the road length
	// between cities i and j if these cities are directly connected
	// by a road
	// For cities m and n that are not connected directly by a road,
	// the value in city_graph[m][n] will be INF,
	// which is a large value like 10M,
	// that is assumed to be infinite.

	for (int i = 0; i < (roads.edge_count - 1); i++) {//Initializes the city_graph array using roads.edge array
		int index1 = roads.edge[i][0];
		int index2 = roads.edge[i][1];
		int value = roads.edge[i][2];
		memcpy(&city_graph[index1][index2], &value, sizeof(int));
		memcpy(&city_graph[index2][index1], &value, sizeof(int));
	}

	for (size_t i = 0; i < cities.city_count; i++) {
		for (size_t j = 0; j < cities.city_count; j++) {
			if (city_graph[i][j] == 0) {
				city_graph[i][j] = INF;
			}
			if (i == j) {//If cities are the same distance is zero..
				city_graph[i][j] = 0;
			}
		}
	}

	initialise();
	floydWarshall();
	int a = 1;
	while (a) {
		// prompt user to enter two city names
		// print the shortest path between the two cities
		// print the length of the path

		char city1[20];
		char city2[20];
		printf("Enter the cities:\n");
		scanf("%s %s", &city1, &city2);
		int len1 = strlen(city1);
		int len2 = strlen(city2);
		city1[len1 + 1] = '\0';
		city2[len2 + 1] = '\0';
		size_t cityID1 = lookup_string(city1);
		size_t cityID2 = lookup_string(city2);
		printPath(cityID1, cityID2);
		printf("\n%d", city_graph[cityID1][cityID2]);
		//a = 0;
	}
	free(city_graph);
	free(shortest_paths);
	free(cities.str);
	free(roads.edge);
	return 0;
}

