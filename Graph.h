#ifndef GRAPH_H
#define GRAPH_H

/* The presented graph can be directed-undirected, with weight/without weight. 
 * Dijstra algorithm implemented only working for graphs with weight. */

#include <algorithm>
#include <limits>
#include <set>
#include <vector>
#include <string>
#include <map>
#include <iostream>

class Node {
	private:
		std::string name;
	public:
		Node(const std::string &n);

		std::string getName() const;
};

class Edge {
	private:
		int weight;
		Node *target;
	public:
		Edge(Node *target, int weight = 0);

		Node* getTarget() const;
		int getWeight() const;
};

template<class T>
class Graph {
	private:
		bool weighted, directed;
		int totalPathWeight;
		std::map<T, std::vector<Edge *>> adjacencyList;
	public:
		Graph();

		/* Fill find the node with the min distance. */
		struct distanceFinder {
			std::map<std::string, int> distanceTable;
			distanceFinder(std::map<std::string, int> d) : distanceTable(d) {  }

			bool operator()(const std::string &str1, const std::string &str2) {
				return distanceTable[str1] < distanceTable[str2];
			}
		};

		void setWeighted(bool w);
		void setDirected(bool d);
		void setTotalPathWeight(int totalPathWeight);
		int getTotalPathWeight() const;
		bool getWeighted() const;
		bool getDirected() const;

		void insertEdge(const T &source, const T &destination, int weight = 0);
		std::vector<std::string> dijkstra(const T &source, const T &destination);
		void toString() const;

		virtual ~Graph();
};

/************ Node implementation. ************/
Node::Node(const std::string &n) : name(n) { } 

std::string Node::getName() const { return name; }

/************ Edge implementation. ************/
Edge::Edge(Node *t, int w) {
	target = t;
	weight = w;
}

Node* Edge::getTarget() const { return target; }
int Edge::getWeight() const { return weight; }

/************ Graph implementation. ************/
template<class T> Graph<T>::Graph() {  }

template<class T> void Graph<T>::setWeighted(bool w) { weighted = w; }
template<class T> void Graph<T>::setDirected(bool d) { directed = d; }
template<class T> void Graph<T>::setTotalPathWeight(int tpw) { totalPathWeight = tpw; }
template<class T> bool Graph<T>::getWeighted() const { return weighted;  }
template<class T> bool Graph<T>::getDirected() const { return directed;  }
template<class T> int Graph<T>::getTotalPathWeight() const { return totalPathWeight; }

template<class T> void Graph<T>::insertEdge(const T &source, const T &target, int weight) {
	Node* t = new Node(target);
	Edge *edgeTarget = new Edge(t, weight);

	if(getDirected() == true) {
		adjacencyList[source].push_back(edgeTarget);

		// The node with no connection to another nodes will appear un our adjaceny list.
		adjacencyList[target]; 
	} else if(getDirected() == false) {
		Node* s = new Node(source);
		Edge *edgeSource = new Edge(s, weight);
		adjacencyList[source].push_back(edgeTarget);

		// Because also our target node has a relation with out source node.
		adjacencyList[target].push_back(edgeSource);
	}
}

// BASED ON WIKIPEDIA PSEUDOCODE.
template<class T> std::vector<std::string> Graph<T>::dijkstra(const T &source, const T &destination) {
	std::map<T, int> dist;
	std::map<T, std::string> prev;
	std::set<T> nodes;

	std::map<std::string, std::vector<Edge *>>::iterator git;

	// For each vertex in our graph.
	for(git = adjacencyList.begin(); git != adjacencyList.end(); git++) {
		// Add our vertex with an "infinite" value to our dist map.
		dist[git->first] = std::numeric_limits<int>::max();
		
		// Initialize with an undefined value.
		prev[git->first] = "-1";
	
		// Set all nodes in the graph.
		nodes.insert(git->first);
	}

	// Distance from source to source.
	dist[source] = 0;

	// While our set of nodes aren't empty.
	while(!nodes.empty()) {
		std::string minDisVertex = *std::min_element(nodes.begin(), nodes.end(), distanceFinder(dist));
		
		// All remaining vertices are inaccessible from source.
		if(dist[minDisVertex] == std::numeric_limits<int>::max()) {
			break;
		}

		if(minDisVertex == destination) {
			break;
		}

		nodes.erase(minDisVertex);

		// Get all the edges that are adjacent to our node.
		std::vector<Edge *> edges = adjacencyList.find(minDisVertex)->second;
		for(std::vector<Edge *>::iterator it = edges.begin(); it != edges.end(); it++) {
			Edge *edge = *it;

			// if our vertex is in our nodes map.
			if(nodes.find(edge->getTarget()->getName()) == nodes.end()) {
				continue;
			}

			int totalDistance = dist[minDisVertex] + edge->getWeight(); 

			if(totalDistance < dist[edge->getTarget()->getName()]) {
				dist[edge->getTarget()->getName()] = totalDistance;
				prev[edge->getTarget()->getName()] = minDisVertex;
			}
		}
	}

	std::vector<std::string> path;
	if(getWeighted()) {
		for(std::string n = destination; n != source;) {
			path.push_back(n);
			n = prev[n];
		}
		path.push_back(source);
		setTotalPathWeight(dist[destination]);
	}

	return path;
}

template<class T> void Graph<T>::toString() const {
	std::map< std::string, std::vector<Edge *> >::const_iterator it;
	std::vector<Edge *>::const_iterator eit;
	for(it = adjacencyList.begin(); it != adjacencyList.end(); it++) {

		// it->first is our node source. 
		std::cout << "[" << it->first << "] --> ";
		for(eit = it->second.begin(); eit != it->second.end(); eit++) {
			Edge *edge = *eit;

			if(getWeighted() == true) {
				std::cout << edge->getTarget()->getName() << " (" 
						  << std::to_string(edge->getWeight()) << "), ";
			} else if(getWeighted() == false){
				std::cout << edge->getTarget()->getName() << ", ";
			}
		}
		std::cout << "\n";
	}
}

template<class T> Graph<T>::~Graph() {
	std::map< std::string, std::vector<Edge *> >::iterator it;
	std::vector<Edge *>::iterator eit;
	for(it = adjacencyList.begin(); it != adjacencyList.end(); it++) {
		for(eit = it->second.begin(); eit != it->second.end(); eit++) {
			Edge *edge = *eit;
			delete edge->getTarget();
			delete edge;
		}
	}
}

#endif
