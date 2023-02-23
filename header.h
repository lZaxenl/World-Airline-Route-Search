#include <iostream>
#include <vector>
#include <ctime> 
#include <cstdlib>
#include <string>
#include <fstream>
#include <map>

#define INFINITY 999
using namespace std;

class Graph{
    public:
        Graph(int d);
        ~Graph(){};

        void print(string cities[]);
        void addEdge(int node1, int node2){ adj[node1][node2] = 1; }; // designates a 1 for the cities that node1 can travel to
        bool Dijkstra(int src, int distance[], bool visited[], int parent[]); // Algorithm to find the shortest path between 2 nodes
        bool Dijkstra2(int src, int distance[], bool visited[], int parent[], int last);
        int minimumDist(int distance[], bool visited[]); // assist Kijkstra() function
        void printPath(int parent[], int i, int j, string cities[]); // used in Question 1
        void savePath(int parent[], int i, int j, string cities[], int *pos, string **arr2D, int f);// used in Question 2
        void printSolution(int distance[], int parent[], int s); // prints the shortest distance from 1 node to all other nodes
        bool possibleRoutes(int src); // test if a city can fly to at least 1 city
        int adjCities(string cities[], string *temp, int city_A_num); // used to save all adj cities to an array
    private:
        int n;
        int adj[140][140]; // You may want to use adjacent list implementation for sparse graph instead
};

Graph::Graph(int d){
    for (int i=0; i < d; i++){
        for (int j=0; j < d; j++){
            adj[i][j] = 0;
        }
    }
    n = d;
};

void Graph::print(string cities[]){
    cout << endl << "The graph generated can be represented by the following adjacent list: " << endl;
    cout <<          "-------------------------------------------------------------------------" << endl;
    for (int i=0; i < n; i++){
        cout << cities[i] << endl;
        cout <<"   " << i << " -> ";
        for (int j=0; j < n; j++){
            if (adj[i][j]){
                cout << j << " ";
            }
        }
        cout << endl;
    }
};


struct strCmp {
    bool operator()( const string s1, const string s2 ) const {
        return s1 <s2;
    }
};
//****************************************************************************************
// gets the next city in an imaginary queue that hasen't 
// been visited and has a distance value less than 999
int Graph::minimumDist(int distance[], bool visited[]){
	int min = INFINITY;
    int index = 0;        
	for(int i = 0; i < n; ++i) 
	{
		if(!visited[i] && distance[i] < min && distance[i] > -1)      
		{
			min = distance[i];
			index = i;
		}
	}
	return index;
}
//****************************************************************************************
// prints the shortest route from 1 node to another
// used in Question 1
void Graph::printPath(int parent[], int i, int j, string cities[]){
    if (parent[i] == j){
        cout << "\n -> " << cities[i];
        return;
    }
    printPath(parent, parent[i], j, cities);
    cout << "\n -> " << cities[i];
}
// save cities[i] to arr2D
// used in Question 2
void Graph::savePath(int parent[], int i, int j, string cities[], int *pos, string **arr2D, int f){
    if (parent[i] == j){
        arr2D[f][*pos] = cities[i];
        //cout << "\n -> " << arr2D[f][*pos];
        (*pos)++;
        return;
    }
    savePath(parent, parent[i], j, cities, pos, arr2D, f);
    arr2D[f][*pos] = cities[i];
    //cout << "\n -> " << arr2D[f][*pos];
    (*pos)++;
}
// prints the shortest distance from 1 node to all other nodes
void Graph::printSolution(int distance[], int parent[], int src){
    cout << "\ncity\tDistance    Path\n";
    cout << "-------------------------------------------------------------------------\n";
    parent[src] = src;
    for(int i = 0; i < n && distance[i] != INFINITY; ++i){
        cout << i << "\t" << distance[i] << "\t   " << " " << i << " ";
        int parnode = parent[i];

        while(parnode != src ){
            cout << " <- " << parnode << " ";
            parnode = parent[parnode];
        }
        if(distance[i] != INFINITY && i != src)
            cout << " <- " << src;
        cout << endl;
    }
}
//****************************************************************************************
// Dijkstra algorithm
bool Graph::Dijkstra(int src, int distance[], bool visited[], int parent[]){
	for(int i = 0; i < n; i++)
		distance[i] = INFINITY; // set every cities distance to an initial value of 999
	distance[src] = 0;          // set city-A distance from itself to 0; this allows minimuDist() to find it 1st
	
    // the 1st 'nearest' value will always be city-A and it then creates an imaginary queue 
    // by using minimumDist() to grab the next city in the queue and then calculate 
    // the distance from city-A to the current cities children
    for(int i = 0; i < n; ++i) {
        
        int nearest = minimumDist(distance, visited);
        visited[nearest] = true;
        //cout << "\tdistance[j]:\t" << distance[nearest] << endl;
        for(int j = 0; j < n; ++j){
            
            if(!visited[j] && adj[nearest][j] && distance[j] > distance[nearest] + adj[nearest][j]) {
                distance[j] = distance[nearest] + adj[nearest][j];
                //cout << j << "\tdistance[j]:\t" << distance[j] << endl;
                parent[j] = nearest;
            }
        }
    }
    return possibleRoutes(src);
}
// used with Question 2 that has must-visit-cities
// this version of Dijkstra keeps the must-visit-cities from traveling through city[B]
bool Graph::Dijkstra2(int src, int distance[], bool visited[], int parent[], int last){
	for(int i = 0; i < n; i++)
		distance[i] = INFINITY; // set every cities distance to an initial value of 999
	distance[src] = 0;          // set city-A distance from itself to 0; this allows minimuDist() to find it 1st
	distance[last] = -1;
    // the 1st 'nearest' value will always be city-A and it then creates an imaginary queue 
    // by using minimumDist() to grab the next city in the queue and then calculate 
    // the distance from city-A to the current cities children
    for(int i = 0; i < n; ++i) {
        
        int nearest = minimumDist(distance, visited);
        visited[nearest] = true;

        for(int j = 0; j < n; ++j){
            if(!visited[j] && adj[nearest][j] && distance[j] > distance[nearest] + adj[nearest][j]) {
                distance[j] = distance[nearest] + adj[nearest][j];
                parent[j] = nearest;
            }
        }
    }
    return possibleRoutes(src);
}
// returns true if the city passed to Dijkstra has at least 1 city to fly to
bool Graph::possibleRoutes(int src) {
    bool min = false;
    for (int j=0; j < n; j++){
        if (adj[src][j] == 1){
            min = true;
        }
    }
    if(min == 0)
        return min;
    else 
        return min;
}
// used with Question 3
int Graph::adjCities(string cities[], string temp[], int city_A_num) {
    int num = 0;
    for(int i = 0; i < n; ++i) {
        if (adj[city_A_num][i] == 1){
            temp[num] = cities[i];
            ++num;
        }
    }
    return num;
}