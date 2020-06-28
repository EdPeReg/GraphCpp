#include <iostream>
#include <iterator>

#include "Graph.h"

using namespace std;

int main() {
	bool weighted = false, directed = false;
	Graph graph;
	string origen, destino, option;

	directed = true;
	weighted = true;

	graph.setDirected(directed);
	graph.setWeighted(weighted);

	graph.insertEdge("h", "c", 5);
	graph.insertEdge("h", "a", 3);
	graph.insertEdge("h", "b", 2);
	graph.insertEdge("a", "d", 3);
	graph.insertEdge("d", "b", 1);
	graph.insertEdge("d", "f", 4);
	graph.insertEdge("b", "e", 6);
	graph.insertEdge("c", "e", 2);
	graph.insertEdge("e", "f", 1);
	graph.insertEdge("e", "school", 4);
	graph.insertEdge("f", "school", 2);

	cout << "\nGrafo: \n";
	graph.toString();
	cout << "\n";

	if(weighted) {
		cout << "Inserte origen: ";
		getline(cin, origen);
		cout << "Inserte destino: ";
		getline(cin, destino);

		vector<string> path = graph.dijkstra(origen, destino);

		for(vector<string>::reverse_iterator it = path.rbegin(); it != path.rend(); it++) {
			cout << *it;

			if(it + 1 != path.rend()) {
				cout << " -> ";
			}
		}
		cout << "(" << graph.getTotalPathWeight() << ")" << "\n"; 
	}

	return 0;
}
