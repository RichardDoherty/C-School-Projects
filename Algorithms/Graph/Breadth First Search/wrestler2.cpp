/*
Suppose there are two types of professional wrestlers: “Babyfaces” (“good guys”) and “Heels” (“bad guys”). Between any pair of professional wrestlers, there may or may not be a rivalry.
Suppose we have n wrestlers and we have a list of r pairs of rivalries. Give an efficient algorithm that determines whether it is possible to designate some of the wrestlers as Babyfaces and the remainder as Heels such that each rivalry is between a Babyface and a Heel.
*/

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
using namespace std;

const string unkown = "unkown";
const string baby = "babyface";
const string heel = "heel";

// structure to represent the vertices
struct wrestler
{
    string name;
    string role;
    bool visited;
};

// method to initialize our graph, here we are only setting up the vertices
void createVertices(vector < vector <wrestler*> >& graph, vector <string> values)
{

    for (int x = 0; x < graph.size(); x++)
    {
        // creating the new vertex
        graph[x].push_back(new wrestler);

        // we assign the name to each node/vertex
        graph[x][0]->name = values[x];

        // as a starter each wrestler will not be assigned a role
        graph[x][0]->role = unkown;

        graph[x][0]->visited = false;
    }
}


// quick hash function to get the index based on the name provided
int getIndex(vector<string> values, string source)
{
    for (int x = 0; x < values.size(); x++)
    {
        if (source == values[x])
        {
            return x;
        }
    }
    return -1;
}

/*
method to make connections within the graph
since this is an undirected graph, we also need to update the connection for the second wrestler
for the input we take the names of the wrestlers involved in the pair
we utilize a hash function to quickly extract the node location/index
we then create the pair by modifying each separate list
*/
void addPair(vector < vector <wrestler*> >& graph, string A, string B, vector <string> reference)
{
    int source = getIndex(reference, A);
    int partner = getIndex(reference, B);

    // creating the pairs
    graph[source].push_back(graph[partner][0]);
    graph[partner].push_back(graph[source][0]);
}




/*
method to check if our network of wrestlers provides non conflicting rivalries
The approach we will take is going to be modifying BFS
we will begin at the first vertex, then set it as a baby face and update it to discovered
we then check it's neighbors and update them to the opposite which will be having a heel role
we continue this process until either we encounter a conflict or when all undiscovered vertices has been analyzed
a conflict occurs if the current vertex is heel and has a connection to another heel vertex or vice versa
basically if the neighbors have the same role as the predecessor then a conflict has been found
*/
void checkPairs(vector < vector <wrestler*> >& graph, vector <string> reference)
{
    vector <string> babyFaces;
    vector <string> heels;

    queue < vector <wrestler*> > list;

    // analyze all the undiscovered vertices in our graph
    for (int i = 0; i < graph.size(); i++)
    {
        if (!graph[i][0]->visited)
        {
            // we begin at the initial vertex, we also set the starting vertex to have the role of a babyface
            graph[i][0]->role = baby;

            // we also add it to our list of baby faces
            babyFaces.push_back(graph[i][0]->name);

            // add it to our queue
            list.push(graph[i]);

            while (!list.empty())
            {
                // extract the current vertex from our queue
                vector <wrestler*> currentNode = list.front();
                currentNode[0]->visited = true;
                list.pop();

                // here we now check for the neighbors of this vertex
                for (int x = 1; x < currentNode.size(); x++)
                {
                    // condition to check if the neighbor has been previously discovered
                    if (currentNode[x]->visited == false)
                    {
                        // here is how we assign the roles
                        /*
                        basically if the current wrestler is a babyface than its neighbors must be heels
                        we first check what role the neighbors have
                        if they are unknown, than set it to the opposite role from the current vertex
                        if they have the same role as the predecessor than we end the traversal because this signals a conflict
                        */
                        if (currentNode[x]->role == unkown && currentNode[0]->role == baby)
                        {
                            currentNode[x]->role = heel; // set the neighbor to heel if the predecessor is a babyface
                            heels.push_back(currentNode[x]->name); // updating our list of heels
                        }
                        else if (currentNode[x]->role == unkown && currentNode[0]->role == heel)
                        {
                            currentNode[x]->role = baby; // set the neighbor to babyface if the predecessor is a heel
                            babyFaces.push_back(currentNode[x]->name);  // updating our list of babyfaces
                        }
                        else if (currentNode[x]->role == currentNode[0]->role)
                        {
                            // conflict found
                            cout << "Impossible" << endl;
                            return;
                        }

                        // lastly we add the undiscovered neighboring vertex to our queue
                        list.push(graph[getIndex(reference, currentNode[x]->name)]);
                    }
                }
            }
        }
    }

    // showcasing the result
    cout << "Yes \n" ;
    cout << "Babyfaces: ";
    for (int x = 0; x < babyFaces.size(); x++)
    {
        cout << babyFaces[x] << " ";
    }
    cout << endl << "Heels: ";
    for (int x = 0; x < heels.size(); x++)
    {
        cout << heels[x] << " ";
    }
    cout << endl;
    return;
}

// method to showcase the graph layout
//void viewGraph(vector < vector <wrestler*> > graph)
//{
//	for (int x = 0; x < graph.size(); x++)
//	{
//		cout << "Current Node: " << graph[x][0]->name << endl;
//
//		if (graph[x].size() > 1)
//		{
//			cout << "Connections are:" << endl;
//		}
//
//		for (int y = 1; y < graph[x].size(); y++)
//		{
//			cout << graph[x][y]->name << " " << graph[x][y]->role << " " << graph[x][y]->visited << endl;
//		}
//		cout << endl;
//	}
//}

int main(int argc, char* argv[]){
    ifstream infile;
    cout << "Please enter the input file name> " << flush;
    string filename;
    do{
        cin >> filename;
        infile.open(filename);

        if (!infile){
            cout << "Invalid file. Please enter a valid input file name> " << flush;
        }
    } while (!infile);
      //while (!readFile.eof()){
        int size;
        int edges;
        string currentName;
        vector <string> names;

        // getting the number of vertices
        infile >> size;
        // declaring our graph
        vector < vector < wrestler*> > graph(size);

        // storing the names
        for (int x = 0; x < size; x++){
            infile >> currentName;
            names.push_back(currentName);
        }

        // creating the vertices for our graph
        createVertices(graph, names);
        // getting the number of pairs/edge connections
        infile >> edges;

        // making the connections for our graph
        for (int x = 0; x < edges; x++){
            // getting the vertices involved in the pair
            string a, b;
            infile >> a;
            infile >> b;

            // making the connections for our graph
            addPair(graph, a, b, names);
        }
        checkPairs(graph, names);
    //}
    infile.close();
    return 0;
}
