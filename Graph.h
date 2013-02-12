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

#endif