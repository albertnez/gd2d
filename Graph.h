#ifndef GRAPH_H
#define GRAPH_H



struct Graph {
	int nodes, edges;
	std::vector <Vec> disp, pos;
	std::vector <int> degree, adj, ind;
};

void initGraph(Graph& g);



//set nodes randomly
void setNodesPosition(Graph& g, const int& xMargin, const int& yMargin);

/*
void insert(vector<int> &v, const int& i, const int& k);

void addEdge(Graph& g, const int& a, const int& b);
*/
#endif