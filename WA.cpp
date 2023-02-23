/***********************************************************
Group#  : 7 
Member  : Jared Singleton & Yousif Al hamadani
Synopsis: read from city.name and flight.txt
        store the graph in a adjacency matrix
***********************************************************/

#include "header.h"

void createCityFile(int n);
Graph createFlightFile(string filename, Graph graph);
void swap(string arr[], int a, int b);
void permutation(string **arr2D, string arr[], int n, int l);
void reset(int distance[], bool visited[], int parent[], int n);
string pickFile(int &n);
void printCities(string cities[], int n);

//****************************************************************************************
// to map a city name with an unique integer
map<const string, int> city;
string cities[140];
int count;  // global variable to help track which row to save the city name
bool has_route;

int main(int argc, char *argv[]){
    int i,j,n;
    string filename = pickFile(n);

    Graph graph(n);             
    int distance[n] = {0};      // used to save the minimum distance from city-n to city-A                          
	bool visited[n] = {false};  // track which cities have been visted/unvisted
    int parent[n] = {-1};       // holds the parent-city location for every city
    
 
    //****************************************************************************************
    // Input n-number of cities into the map with a specific value
    createCityFile(n);

    // Runs through the flight.txt file and adds the cities to the graph, along 
    // with the cities that you can travel to from initial city
    graph = createFlightFile(filename, graph);

    // print the graph in adjacent list form : USE FOR PRESENTATION PURPOSES
    graph.print(cities);   

    //****************************************************************************************
    // output all possible routes for city[A]
        cout << "-------------------------------------------------------------------------\n";
        cout << "Shanghai, People's Republic of China";
        has_route = graph.Dijkstra(city["Shanghai, People's Republic of China"], distance, visited, parent);
        cout << "src = " << city["Shanghai, People's Republic of China"] << endl;
        graph.printSolution(distance, parent, city["Shanghai, People's Republic of China"]);
        cout << "-------------------------------------------------------------------------\n";

    
    int choice;
    do {
        cout << "\n-------------------------------------------------------------------------\n";
        cout << "Select which question you want to do" << endl;
        cout << " 1) Question 1\n 2) Question 2\n 3) Question 3\n 4) Question 4\n 5) Quit" << endl;
        cout << "Input: ";
        cin >> choice;

// QUESTION 1
        if(choice == 1) {
            cout << "-------------------------------------------------------------------------\n";
            int connectingFlights = 0; // maximum number of connecting flights from A to B
            
            do{ // Pick your starting city
                cout << "Pick your starting city" << endl;
                printCities(cities, n);
                cout << "Input: "; cin >> choice;
                if(choice < 1 || choice > n)
                    cout << "\nInvalid choice" << endl;
            } while(choice < 1 || choice > n);
            string A = cities[choice-1];
            do{ // pick your ending city
                cout << "\nPick your ending city" << endl;
                printCities(cities, n);
                cout << "Input: "; cin >> choice;
                if(choice < 1 || choice > n)
                    cout << "\nInvalid choice" << endl;
            } while(choice < 1 || choice > n);
            string B = cities[choice-1];

            cout << "\nInput the maximum amount of connecting flights: "; cin >> connectingFlights;
            cout << "-------------------------------------------------------------------------\n";
            reset(distance, visited, parent, n);
            if(has_route = graph.Dijkstra(city[A], distance, visited, parent)) {
                if(distance[city[B]] < connectingFlights){
                    cout << "\nShortest route from \"" << A << "\" to \"" << B << "\".";
                    //printf("\nShortest route from \"%s\" to \"%s\"\n", &A, &B);
                    printf("\nIn under %d connections\n\n", connectingFlights);
                    
                    cout << A;
                    graph.printPath(parent, city[B], city[A], cities);
                    printf("\n\nTotal connections: %d\n", distance[city[B]]);
                }
                else {
                    cout << "\nShortest route from \"" << A << "\" to \"" << B << "\".";
                    //printf("\nShortest route from \"%s\" to \"%s\"\n", &A, &B);
                    printf("\nIn under %d connections\n", connectingFlights);
                    printf("*** There is no such route ***\n\n");
                }
            }
            else {
                cout << "\nShortest route from \"" << A << "\" to \"" << B << "\".";
                //printf("\nShortest route from \"%s\" to \"%s\"\n", &A, &B);
                printf("\nIn under %d connections\n", connectingFlights);
                printf("*** There is no such route ***\n\n");
            }
            cout << "-------------------------------------------------------------------------\n";
            cout << "\nInput paused, press 'Enter'";
            cin.get();
            cin.get();
        }
// QUESTION 2
        else if(choice == 2) {
            cout << "-------------------------------------------------------------------------\n";
            int choice;
            string A, B;

            do{ // Pick your starting city
                cout << "Pick your starting city" << endl;
                printCities(cities, n);
                cout << "Input: "; cin >> choice;
                if(choice < 1 || choice > n)
                    cout << "\nInvalid choice" << endl;
            } while(choice < 1 || choice > n);
            A = cities[choice-1];
            do{ // pick your ending city
                cout << "\nPick your ending city" << endl;
                printCities(cities, n);
                cout << "Input: "; cin >> choice;
                if(choice < 1 || choice > n)
                    cout << "\nInvalid choice" << endl;
            } while(choice < 1 || choice > n);
            B = cities[choice-1];

            int numMVC; // number of Must-Visit-Cities
            do { // get Must-Visit-Cities from user
                cout << "\nHow many Must-Visit-Cities do you want (1, 2, or 3): "; cin >> numMVC;
                if(numMVC < 0 || numMVC > 3)
                    cout << "\nInvalid choice" << endl;
            } while(numMVC < 0 || numMVC > 3);

            string temp[numMVC]; // save Must-Visit-Cities to this array
            for(int i = 1; i <= numMVC; ++i) {
                string test;
                do{ 
                    cout << "\nPick your Must-Visit-City #" << i << endl;
                    printCities(cities, n);
                    cout << "Input: "; cin >> choice;
                    test = cities[choice-1];
                    if(choice < 1 || choice > n || test == A || test == B)
                        cout << "\nInvalid choice";
                } while(choice < 1 || choice > n || test == A || test == B);
                
                temp[i-1] = cities[choice-1];
            }
        
            // calculate the total number of permutations from Must-Visit-Cities
            int factorial = 1;
            for(int i = 2; i <= numMVC; ++i) {
                factorial = factorial * i;
            }
            // store all permutations of Must-Visit-Cities in temp2D array
            string **temp2D;
            string **arr2D;
            temp2D = new string*[factorial];
            arr2D = new string*[factorial];
            for(int i = 0; i < factorial; ++i) {
                temp2D[i] = new string[numMVC];
                arr2D[i] = new string[100];
            }
            
            count = 0; // global variable to help track which row to save the city name
            permutation(temp2D, temp, numMVC, 0); // create all possible permutations of Must-Visit-Cities
            
            // run through all permutations and save the total distance and route traveled
            int tot_dist[factorial]; 
            int k;
            for(int i = 0; i < factorial; ++i) {
                int pos = 1; // lets the savePath() function know which position to save each city in arr2D

                reset(distance, visited, parent, n); // reset distance, visited, and parent arrays to starting values
                arr2D[i][0] = A; // set first position to the starter city A
                if(has_route = graph.Dijkstra2(city[A], distance, visited, parent, city[B])) {
                    graph.savePath(parent, city[temp2D[i][0]], city[A], cities, &pos, arr2D, i);
                }
                else {
                    cout << "\nShortest route from \"" << A << "\" to \"" << B << "\".";
                    cout << "\n-------------------------------------------------------------------------\n";
                    printf("*** There is no such route ***\n\n");
                    exit(1);
                }

                for(k = 0; k < numMVC-1; ++k) {
                    reset(distance, visited, parent, n);
                    if(has_route = graph.Dijkstra2(city[temp2D[i][k]], distance, visited, parent, city[B])) {
                        graph.savePath(parent, city[temp2D[i][k+1]], city[temp2D[i][k]], cities, &pos, arr2D, i);
                    }
                    else {
                        cout << "\nShortest route from \"" << A << "\" to \"" << B << "\".";
                        cout << "\n-------------------------------------------------------------------------\n";
                        printf("*** There is no such route ***\n\n");
                        exit(1);
                    }
                }

                reset(distance, visited, parent, n);
                if(has_route = graph.Dijkstra(city[temp2D[i][k]], distance, visited, parent)) {
                    graph.savePath(parent, city[B], city[temp2D[i][k]], cities, &pos, arr2D, i);
                }
                else {
                    cout << "\nShortest route from \"" << A << "\" to \"" << B << "\".";
                    cout << "\n-------------------------------------------------------------------------\n";
                    printf("*** There is no such route ***\n\n");
                    exit(1);
                    }

                tot_dist[i] = pos - 1;
            }
            
            // find the shortest route
            int min = tot_dist[0];
            int shortest = 0;
            for(int i = 0; i < factorial; ++i) {
                min > tot_dist[i] ? shortest=i : shortest;
            }
            
            // output the shortest route from city[A] to city[B]
            cout << "-------------------------------------------------------------------------\n";
            cout << "\nShortest route from \"" << A << "\" to \"" << B << "\".";
            printf("\nMust visit cities:\n");
            for(int i = 0; i < numMVC; ++i) {
                cout << " -> " << temp[i] << endl;
            }
            cout << "-------------------------------------------------------------------------\n";
            cout << arr2D[shortest][0];
            for(int j = 1; j <= tot_dist[shortest]; ++j) {
                cout << "\n -> " << arr2D[shortest][j];
            }
            cout << "\n\nSmallest number of connection: " << tot_dist[shortest];
            cout << "\n-------------------------------------------------------------------------\n";

            // delete temp2D & arr2D dynamic arrays
            for(int i = 0; i < factorial; ++i){
                delete[] temp2D[i];
                delete[] arr2D[i];
            }
            delete[] temp2D;
            delete[] arr2D;

            cout << "\nInput paused, press 'Enter'";
            cin.get();
            cin.get();
        }
// QUESTION 3
        else if(choice == 3) {
            cout << "-------------------------------------------------------------------------\n";
            int choice;
            string A;

            do{ // Pick your starting city
                cout << "Pick your starting city" << endl;
                printCities(cities, n);
                cout << "Input: "; cin >> choice;
                if(choice < 1 || choice > n)
                    cout << "\nInvalid choice" << endl;
            } while(choice < 1 || choice > n);
            A = cities[choice-1];

            // Check if city A has any routes
            reset(distance, visited, parent, n);
            if(has_route = graph.Dijkstra(city[A], distance, visited, parent)) {
                /**** Find all adjacent cities to city A ****/
                string temp[n]; // holds all adjacent cities from city A
                int numOfCities = graph.adjCities(cities, temp, city[A]);
                cout << "\n-------------------------------------------------------------------------\n";
                cout << "\nAdjacent cities to \"" << A << "\":" << endl;
                //cout << "numOfCities = " << numOfCities << endl;
                for(int i = 0; i < numOfCities; ++i) {
                    cout << " -> " << temp[i] << endl;
                }

                /**** calculate the total number of permutations ****/
                int factorial = 1;
                for(int i = 2; i <= numOfCities; ++i) {
                    factorial = factorial * i;
                }
                //cout << "factorial = " << factorial << endl;
                // store all permutations of must-visit-cities in temp2D array
                string **temp2D = new string*[factorial];
                string **arr2D = new string*[factorial];
                for(int i = 0; i < factorial; ++i) {
                    temp2D[i] = new string[numOfCities];
                    arr2D[i] = new string[500]; // A city can have at most 10 cities to fly to. 
                }
                // create all possible permutations of must-visit-cities
                count = 0;
                permutation(temp2D, temp, numOfCities, 0);

                /**** run through all permutations and save the total distance and route traveled ****/
                int tot_dist[factorial]; 
                int k;
                for(int i = 0; i < factorial; ++i) {
                    int pos = 1; // lets the savePath() function know which position to save each city in arr2D
                    reset(distance, visited, parent, n); // reset distance, visited, and parent arrays to starting values
                    arr2D[i][0] = A; // set first position to the city started in -> A
                    //cout << endl << arr2D[i][0];

                    if(has_route = graph.Dijkstra(city[A], distance, visited, parent)) {
                        graph.savePath(parent, city[temp2D[i][0]], city[A], cities, &pos, arr2D, i);
                    }
                    else {
                        cout << "\nShortest route from \"" << A << "\" to all adjacent cities and back:" << endl;
                        cout << "-------------------------------------------------------------------------\n";
                        printf("*** There is no such route ***\n\n");
                        exit(1);
                    }

                    for(k = 0; k < numOfCities-1; ++k) {
                        reset(distance, visited, parent, n);
                        if(has_route = graph.Dijkstra2(city[temp2D[i][k]], distance, visited, parent, city[A])) {
                            graph.savePath(parent, city[temp2D[i][k+1]], city[temp2D[i][k]], cities, &pos, arr2D, i);
                        }
                        else {
                            cout << "\nShortest route from \"" << A << "\" to all adjacent cities and back:" << endl;
                            cout << "-------------------------------------------------------------------------\n";
                            printf("*** There is no such route ***\n\n");
                            exit(1);
                        }
                    }
                    
                    reset(distance, visited, parent, n);
                    if(has_route = graph.Dijkstra(city[temp2D[i][k]], distance, visited, parent)) {
                        graph.savePath(parent, city[A], city[temp2D[i][k]], cities, &pos, arr2D, i);
                    }
                    else {
                        cout << "\nShortest route from \"" << A << "\" to all adjacent cities and back:" << endl;
                        cout << "-------------------------------------------------------------------------\n";
                        printf("*** There is no such route ***\n\n");
                        exit(1);
                        }
                    
                    tot_dist[i] = pos - 1;
                    //cout << "\ni = " << i << ", tot_dist[i] = " << tot_dist[i] << endl;
                }

                /**** find the shortest route ****/
                int min = tot_dist[0];
                int shortest = 0;
                for(int i = 0; i < factorial; ++i) {
                    min > tot_dist[i] ? shortest=i : shortest;
                }
                
                /**** output the shortest route from city[A] to all adjacent cities and back ****/
                cout << "-------------------------------------------------------------------------\n";
                cout << "Shortest route from \"" << A << "\" to all adjacent cities and back:\n\n";
                
                cout << arr2D[shortest][0];
                for(int j = 1; j <= tot_dist[shortest]; ++j) {
                    cout << "\n -> " << arr2D[shortest][j];
                }
                cout << "\n\nSmallest number of connection: " << tot_dist[shortest];
                cout << "\n-------------------------------------------------------------------------\n";

                /**** delete temp2D & arr2D dynamic arrays ****/
                for(int i = 0; i < factorial; ++i){
                    delete[] temp2D[i];
                    delete[] arr2D[i];
                }
                delete[] temp2D;
                delete[] arr2D; 
            } 
            else {
                cout << "City \"" << A << "\" has no routes\n";
                cout << "-------------------------------------------------------------------------\n";
                exit(1);
            }

            cout << "\nInput paused, press 'Enter'";
            cin.get();
            cin.get();
        }
// QUESTION 4
        else if(choice == 4) {
            cout << "-------------------------------------------------------------------------\n";
            int choice;
            string temp;
            /* Pick the cities that each friend lives in */
            do{ // Pick city for Friend A
                cout << "Pick city for Friend A" << endl;
                printCities(cities, n);
                cout << "Input: "; cin >> choice;
                temp = cities[choice-1];
                if(choice < 1 || choice > n)
                    cout << "\nInvalid choice" << endl;
            } while(choice < 1 || choice > n);
            string A = cities[choice-1];

            do{ // Pick city for Friend B
                cout << "\nPick city for Friend B" << endl;
                printCities(cities, n);
                cout << "Input: "; cin >> choice;
                temp = cities[choice-1];
                if(choice < 1 || choice > n || temp == A)
                    cout << "\nInvalid choice" << endl;
            } while(choice < 1 || choice > n || temp == A);
            string B = cities[choice-1];

            do{ // Pick city for Friend C
                cout << "\nPick city for Friend C" << endl;
                printCities(cities, n);
                cout << "Input: "; cin >> choice;
                temp = cities[choice-1];
                if(choice < 1 || choice > n || temp == A || temp == B)
                    cout << "\nInvalid choice" << endl;
            } while(choice < 1 || choice > n || temp == A || temp == B);
            string C = cities[choice-1];

            int tot_dist[n] = {0}; // holds the combined total distance for the friends to every city
            int shortest = tot_dist[0]; // used to find the city with the least connnecting flights
            int min; // holds the integer for the city the friends should meet at

            // friend A
            reset(distance, visited, parent, n);
            if(has_route = graph.Dijkstra(city[A], distance, visited, parent)) {
                for(int i = 0; i < n; ++i) {
                    tot_dist[i] += distance[i];
                }
            }
            else {
                printf("*** There is no city that all 3 friends can fly to ***\n\n");
                exit(1);
            }
            // friend B
            reset(distance, visited, parent, n);
            if(has_route = graph.Dijkstra(city[B], distance, visited, parent)) {
                for(int i = 0; i < n; ++i) {
                    tot_dist[i] += distance[i];
                }
            }
            else {
                printf("*** There is no city that all 3 friends can fly to ***\n\n");
                exit(1);
            }
            // friend C
            reset(distance, visited, parent, n);
            if(has_route = graph.Dijkstra(city[C], distance, visited, parent)) {
                for(int i = 0; i < n; ++i) {
                    tot_dist[i] += distance[i];
                }
            }
            else {
                printf("*** There is no city that all 3 friends can fly to ***\n\n");
                exit(1);
            }
            // find the city with the shortest distance between all 3 friends
            tot_dist[city[A]] = tot_dist[city[B]] = tot_dist[city[C]] = INFINITY;

            for(int i = 0; i < n; ++i) {
                if(shortest > tot_dist[i]){
                    shortest = tot_dist[i];
                    min = i;
                }
            }
            
            // print the following:
            //   1. city the friends should meet at
            //   2. individual Routes they take
            //   3. total number of connections between the friends
            cout << "\n-------------------------------------------------------------------------\n";
            cout << "You three should meet at \"" << cities[min] << "\"" << endl;

            reset(distance, visited, parent, n);
            graph.Dijkstra(city[A], distance, visited, parent);
            cout << "\nRoute for person A\n------------------\n" << A;
            graph.printPath(parent, min, city[A], cities);

            reset(distance, visited, parent, n);
            graph.Dijkstra(city[B], distance, visited, parent);
            cout << "\n\nRoute for person B\n------------------\n" << B;
            graph.printPath(parent, min, city[B], cities);

            reset(distance, visited, parent, n);
            graph.Dijkstra(city[C], distance, visited, parent);
            cout << "\n\nRoute for person C\n------------------\n" << C;
            graph.printPath(parent, min, city[C], cities);

            cout << "\n\nTotal  number of connection: " << tot_dist[min] << endl;
            cout << "-------------------------------------------------------------------------\n";

            cout << "\nInput paused, press 'Enter'";
            cin.get();
            cin.get();
        }

    } while(choice != 5);

   return 0;
}

void createCityFile(int n){
    ifstream cityFile;
    string line;
    cityFile.open("city.name", ios::in);
    for (int i = 0; i < n; i++){
        getline(cityFile, line);
        line.erase(std::prev(line.end()));
        city[line] = i;
        cities[i] = line;
        //cout << line << " is numbered as city " << city[line]<< endl;
    }
    // loop does not input correct number value for the last city "Asuncion, Paraguay"
    // so if n = 140, then the if statement inputs the correct value for the city
    if(n == 140)
        city["Asuncion, Paraguay"] = 139;

    cityFile.close();
}

Graph createFlightFile(string filename, Graph graph){
    int node1, node2;
    char lineChar[256];
    string line;
    ifstream flightFile;                    // Runs through the flight.txt file and adds the cities to the graph, along 
    flightFile.open(filename, ios::in); // with the cities that you can travel to from initial city
    while (flightFile.getline(lineChar, 256)){
        /* if line constains From: */
        line = lineChar; 
        if (line.find("From:", 0) == 0 ){
            line.erase(0,7);                // removes "From:  " and leaves just he city name
            node1 = city[line];             // node1 is the city you are currently in
        } else {
            line.erase(0,7);                // removes the 7-spaces infront of the cities that the node1 city can travel to
            node2 = city[line];             // node2 are the cities that you can travel to from node1
            graph.addEdge(node1,node2);
        }
    }
    flightFile.close();
    return graph;
}
void swap(string arr[], int a, int b){
    string temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}
void permutation(string **temp2D, string arr[], int n, int l){
    // starting value of l = 0
    if(l == n-1) {
        for(int i = 0; i < n; i++) {
            temp2D[count][i] = arr[i];
        }
        // global variable to help track 
        // which row to save the city name
        ++count; 
    }
    for(int i = l; i < n; i++) {
        swap(arr, i, l);
        permutation (temp2D, arr, n, l+1);
        swap(arr, i, l);
    }
}
// reset distance, visited, and parent arrays to starting values
void reset(int distance[], bool visited[], int parent[], int n) {
    for(int i = 0; i < n; ++i) {
        distance[i] = 0;                         
	    visited[i] = false;
        parent[i] = -1;
    }
}
string pickFile(int &n) {
    bool cont = true;
    int choice;
    string filename;
    do {
        cout << "Select how many cities you want in the graph" << endl;
        cout << " 1) 20  cities\n 2) 50  cities\n 3) 100 cities\n 4) 140 cities" << endl;
        cout << "input: "; cin >> choice;
        switch (choice) {
            case 1:
                filename = "flight_20.txt";
                n = 20; cont = false;
                break;
            case 2:
                filename = "flight_50.txt";
                n = 50; cont = false;
                break;
            case 3:
                filename = "flight_100.txt";
                n = 100; cont = false;
                break;
            case 4:
                filename = "flight.txt";
                n = 140; cont = false;
                break;
            default:
                cout << "Invalid choice" << endl;
        }
    } while(cont);

    return filename;
}
void printCities(string cities[], int n) {
    for(int i = 1; i <= n; ++i) 
        cout << "  " << i << ") " << cities[i-1] << endl;
}