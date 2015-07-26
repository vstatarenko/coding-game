#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

vector<int> BFS(int Exit_node, int Current_pos, int* adjacency_matrix, int N);          // breadth first search procedure and also returned vector of parents
void Shortest_way(vector<int> parents, int Start_node, vector<int>& way);               // return shortest way to exit node

int main()
{
    int N;                                  // the total number of nodes in the level, including the gateways
    int L;                                  // the number of links
    int E;                                  // the number of exit gateways
    cin >> N >> L >> E; cin.ignore();
    int adjacency_matrix[N][N];             // the adjacency matrix of nodes
    int* head = &adjacency_matrix[0][0];
    vector<int> EIs;                        // stored all exit gateways' id
    for (int i = 0; i < N; ++i)
        for(int j = 0; j < N; ++j)
            adjacency_matrix[i][j] = 0;
    
    for (int i = 0; i < L; i++) 
    {
        int N1;                         // N1 and N2 defines a link between these nodes
        int N2;
        cin >> N1 >> N2; cin.ignore();
        adjacency_matrix[N1][N2] = adjacency_matrix[N2][N1] = 1;
    }
       
    for (int i = 0; i < E; i++)
    {
        int EI;                         // the index of a gateway node
        cin >> EI; cin.ignore();
        EIs.push_back(EI);
    }
   
  
    while (1) {
        int SI;                         // The index of the node on which the Skynet agent is positioned this turn
        cin >> SI; cin.ignore();
        vector<int> min_distance;
        int step = 0;
        /* 
        below I chosn the nearest exit_node( in case severals gateways).
        Note create the function fot this procedure!!!
        */
        int EI;
        vector<int> temp1;
        for (vector<int>::iterator it = EIs.begin(); it != EIs.end(); it++)
        {   
            Shortest_way(BFS(*it, SI, head, N), SI, min_distance);
            if(min_distance.empty())
                continue;
            if (step > 0 && (min_distance.size() < temp1.size() ))
            {   
                //cout<< *it << endl;
                EI = *it;
                temp1 = min_distance;
                min_distance.clear();
                
            }
            if (step == 0)
            {
                temp1 = min_distance;
                EI = *it;
            }
            step++;
            min_distance.clear();
        }
        temp1.clear();
        // end chose
        
        Shortest_way(BFS (EI, SI, head, N), SI, min_distance);
        min_distance.push_back(SI);
        int N1 = min_distance[0];
        int N2 = min_distance[1];
        cout << std::to_string(N1)<<" " <<std::to_string(N2)<< endl; // Example: 0 1 are the indices of the nodes you wish to sever the link between
        adjacency_matrix[N1][N2] = adjacency_matrix[N2][N1] = 0;
        min_distance.clear();
    }
}

vector<int> BFS(int Exit_node, int Current_pos, int* adjacency_matrix, int N)
{
    queue<int> q;
    vector<int> parents (N, -1);        // on the i position situated parent of i element. parent[1] = 4 means that node 4 is a parent for node 1
    vector<int> nodes_id (N, 0);
    nodes_id[Exit_node] = 1;
    parents[Exit_node] = -1;            // -1 means no parent for node
    q.push(Exit_node);
    while (!q.empty())
    {
        int node = q.front();
        q.pop();
        if (node == Current_pos)
            break;
        for (int i = 0; i < N; ++i)
        {
            if (*(adjacency_matrix + node*N + i) && (!nodes_id[i]))
            {
                nodes_id[i] = 1;
                q.push(i);
                parents[i] = node;
                
            }
        }
    }
    return parents;
}

void Shortest_way(vector<int> parents, int Start_node, vector<int>& way)
{
    if (parents[Start_node] == -1)
        return;
    Shortest_way(parents, parents[Start_node], way);
    way.push_back(parents[Start_node]);
}