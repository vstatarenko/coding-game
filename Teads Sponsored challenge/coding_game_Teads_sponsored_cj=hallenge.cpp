
/*
/////////////////////////////////////////////////////////////////////
/// Here is realization of an issue from coding_game source. Read ///
/// full description here https://www.codingame.com/ide/          ///
/// 1603793d36662b3bd14d5f3dc609ec3f79a4116                       ///  
/// I have some statements which help me to solve the issue       ///
/// Assume we have coherent, non-oriented, non-cyclic graph:      ///
///                                                               ///
/// Statement 1:                                                  ///
/// Optimal node is a part of the longest way in a graph(tree).   ///
/// Statement 2:                                                  ///
/// The longest way is spreaded between two leafs of the tree.    ///
///                                                               ///
/// I proofed both statements and here  my idea ho to realize it. ///
/// First I try to find all leafs in the tree except leafs which  ///
/// have the same parent. Leaf() procedure does it. Then I try    ///
/// to find out the longest way between elements in leafs vector. ///
/// I use BFS() function to do it. BFS() is just a breadth first  ///
/// serach but it works untill we reach exit_leaf and it also     ///
/// provided parents vector to recover the way from start_node to ///
/// exit_node. BFS() is a part on Longest_way() procedure.        ///
/// Longest_way() returns the longer way between two leafs, the   ///
/// longest will be the way having minimum "-1" values in parent  ///
/// vector ("-1" means no parent for cureent node). When we get   ///
/// the longest way in the tree, the optimal node is exactly in   ///
/// the middle of this way!!!                                     ///
/// This realization of the issue works fine, but it fails on     ///
/// tests 7-9, due to timeout. Also it fails on test 10, because  ///
/// I supposed we have k links and k+1 nodes id. But in general   ///
/// node's id could be whatever.                                  ///
///                                                               ///
/// Further steps!!!                                              ///
/// To overcome 7-9 tests Longest_way() should be optimized!      ///
/// One way is to rewrite it using some optimal algorithm.        ///
/// The another one is to reduce set of leafs and rebuild the     ///
/// tree. To reduce set of leafs we can consider parents of       ///
/// current leafs as a leafs and select only leafs(parents) which ///
/// we are interested in(e.g. leafs which have different parents) ///
///                                                               ///
/// Author Valentin Tatarenko                                     ///
/// Moscow, 2015                                                  ///
/////////////////////////////////////////////////////////////////////
*/


#include <iostream>
#include <fstream>
#include <vector>
#include <typeinfo>
#include <queue>
#include <stack>
#include <ctime>

using namespace std;
ifstream fin;
ofstream fout;

struct list
{
	int node;
	struct list* child;
};

struct list* Insert(struct list* head, int parent, int child);
int CountMinusOne(std::vector<int> parent);
void Display_Graph (std::vector<struct list*> tree);
void DFS(int start_node, std::vector<struct list*> tree);
vector<int> BFS(int start_leaf, int exit_leaf, std::vector<struct list*> tree);
vector<int> Leaf(std::vector<struct list*> tree);
vector<int> Longest_Way(std::vector<int> leafs, std::vector<struct list*> tree);

int main()
{	
	char file_name[ ] = "/home/valek/Dropbox/С++/coding-game/Teads Sponsored challenge/test4.txt";
	int buff;
	int n;                 //number of links
	ifstream fin(file_name);
	fin >> buff;
	n = buff;
	std::vector<struct list*> adjacency(n+1);

	int parent, child, k = 0;
	while(k < n)
	{	
		fin >> parent;
		fin >> child;
		adjacency[parent] = Insert(adjacency[parent], parent, child);
		adjacency[child]  = Insert(adjacency[child], child, parent); 
		k++;
		if (fin.eof())
			break;
	}

	fin.close();
	std::vector<int> leaf;
	leaf = Leaf(adjacency);
	cout << leaf.size()<<endl;
	std::vector<int> vparent;
	unsigned int start_time = clock();
	vparent = Longest_Way(leaf, adjacency);
	unsigned int end_time = clock();
	
	int _len = vparent.size();
	int _min_way;
	if (_len % 2 == 0)
		_min_way = (_len/2);
	if (_len % 2 != 0)
		_min_way = _len/2;
	cout << _min_way << endl;
	
	cout << end_time - start_time << endl;
	return 0; 
	
}

struct list* Insert(struct list* head, int parent, int child)
{	
	struct list* temp;
	temp = new struct list;
	temp -> node = child;
	temp -> child = NULL;

	if (head == NULL)
	{
		head = temp;
		return head;
	}
	struct list* _iterator = head;
	while(_iterator -> child)
	{
		_iterator = _iterator -> child;	
	} 
	_iterator -> child = temp;
	_iterator = head;
	return _iterator;
}

void Display_Graph(std::vector<struct list*> tree)
{

	struct list* temp;
	for (int i = 0; i < tree.size(); i++)
	{
		temp = tree[i];
		cout << i << " : "; 
		while(temp)
		{
			cout << temp -> node << " --> ";
			temp = temp  -> child;
		}
		cout << " NULL" << endl;
	}	
}

void DFS(int start_node, std::vector<struct list*> tree)
{
	std::stack<int> deep_nodes;
	std::vector<int> visited_nodes(tree.size(), 0);
	deep_nodes.push(start_node);
	visited_nodes[start_node] = 1;

	while(!deep_nodes.empty())
	{	
		int head;
		head = deep_nodes.top();
		cout << head << " ";
		deep_nodes.pop();
		struct list* temp = new struct list;
		temp = tree[head];
		while(temp)
		{
			if (visited_nodes[temp -> node] == 0)
			{
				visited_nodes[temp -> node] = 1;
				deep_nodes.push(temp -> node);

			}
			temp = temp -> child;
		}

	}
}

vector<int> Leaf(std::vector<struct list*> tree)
{
	vector<int> leafs;
	for(int i = 0; i < tree.size(); ++i)
	{
		struct list* temp = tree[i];
		if((temp -> child == NULL) && (leafs.size() == 0))
			{
				leafs.push_back(i);
				continue;
			}

		if(temp -> child == NULL)
		{	
			int _size = leafs.size();
			leafs.push_back(i);
			for(int j = 0; j < _size; ++j)
			{	
				if(temp -> node == tree[leafs[j]] -> node)
				{	
					leafs.pop_back();
					break;
				}
			}
			
		}
	}
	return leafs;
}

vector<int> BFS(int start_leaf, int exit_leaf, std::vector<struct list*> tree)
{
	queue <int> q;
	vector<int> parents(tree.size(), -1);
	vector<int> visited_nodes(tree.size(), 0);
	q.push(start_leaf);
	visited_nodes[start_leaf] = 1;

	while(!q.empty())
	{
		int node;
		node = q.front();
		q.pop();
		if (node == exit_leaf)
			break;
		struct list* temp = new struct list;
		temp = tree[node];
		while(temp)
		{
			if(visited_nodes[temp -> node] == 0)
			{	
				q.push(temp -> node);
				visited_nodes[temp -> node] = 1;
				parents[temp -> node] = node;
			}
			temp = temp -> child;
		}
	}
	return parents;
}

int CountMinusOne(std::vector<int> parent)
{
	int counter = 0;
	for (int i = 0; i < parent.size(); ++i)
	{
		if(parent[i] == -1)
			counter++;
	}
	return counter;
}

vector<int> Longest_Way(std::vector<int> leafs, std::vector<struct list*> tree)
{	  
	int n = 10000;   //it's just random start number of "-1" which we need in future
	std::vector<int> parents, best_parent;
	int start, finish;

	for (int i = 0; i < leafs.size() - 1; ++i)
	{
		for (int j = i+1; j < leafs.size(); ++j)
		{	
			int _minus_one_quantaty;
			parents = BFS(leafs[i], leafs[j], tree);
			_minus_one_quantaty = CountMinusOne(parents);
			
			if(n > _minus_one_quantaty)
			{
				n = _minus_one_quantaty;
				best_parent = parents;
				start  = leafs[i];
				finish = leafs[j];
			}

		}
	}
	std::vector<int> way(1, finish);
	start = finish;
	while(best_parent[start] != -1)
	{	
		way.insert(way.begin(), best_parent[start]);
		start = best_parent[start];
	}
	return way;
}
