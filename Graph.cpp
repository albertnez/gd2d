#include <iostream>
#include <vector>
#include <cstdlib>
#include "Vec2d.h"
#include "Graph.h"

using namespace std;

void initGraph(Graph& g) {
	int n;

	//nodes
	cin >> n;

	g.nodes = n;
	g.pos = vector<Vec> (n);
	g.disp = vector<Vec> (n);
	g.degree = vector<int> (n);
	g.ind = vector<int> (n+1);
	//edges
	cin >> n;

	g.edges = n;
	g.adj = vector<int>(2*n+1);

	//set degree
	for (int i = 0; i < g.nodes; ++i) {
		cin >> n;
		g.degree[i] = n;
	}
	//set adjacent list
	for (int i = 0; i < g.edges*2 +1; ++i) {
		cin >> n;
		g.adj[i] = n;
	}
	//set index
	for (int i = 0; i < g.nodes+1; ++i) {
		cin >> n;
		g.ind[i] = n;
	}
}

void setNodesPosition(Graph& g, const int& xMargin, const int& yMargin) {
	for (int i = 0; i < g.nodes; ++i) {
		g.pos[i] = Vec(rand()%xMargin, rand()%yMargin);
	}
}
/**

void insert(vector<int> &v, const int& i, const int& k) {
	v.resize(v.size() + 1);
	for(int j = v.size(); j > i; --j){
    	v[j] = v[j - 1];
	}
	v[i] = k;
}

void addEdge(Graph& g, const int& a, const int& b) {
	++g.edges;
	++g.degree[a];
	++g.degree[b];
	cout << "ind a: " << g.ind[a] << endl << "ind b: " << g.ind[b] << endl;
	insert(g.adj, g.ind[b], a);
	insert(g.adj, g.ind[a], b);

	for (int i = a+1; i <= g.nodes; ++i) {
		++g.ind[i];
	}
	for (int i = b+1; i <= g.nodes; ++i) {
		++g.ind[i];
		}
}
*/