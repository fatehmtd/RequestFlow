#include "Graph.h"
#include "Node.h"
#include "Edge.h"

model::Graph::Graph()
{

}

model::Graph::~Graph()
{

}

QList<model::Node*> model::Graph::getNodes() const
{
	return std::move(findChildren<Node*>());
}

QList<model::Edge*> model::Graph::getEdges() const
{
	return std::move(findChildren<Edge*>());
}

void model::Graph::start()
{
	emit started();
}

void model::Graph::stop()
{
	emit stopped();
}
