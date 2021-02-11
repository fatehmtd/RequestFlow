#include "BleachUI.h"
#include <QtWidgets/QApplication>

#include <model/Graph.h>
#include <model/Node.h>
#include <model/Slot.h>
#include <model/Edge.h>

void testModel()
{
    auto graph = new model::Graph();

    auto nodeA = new model::Node(graph);
    auto nodeB = new model::Node(graph);
    auto nodeC = new model::Node(graph);
    nodeA->setObjectName("nodeA");
    nodeB->setObjectName("nodeB");
    nodeC->setObjectName("nodeC");

    auto slotA = nodeA->addOutputSlot("out", model::Slot::DataType::CUSTOM + 1);

    auto slotB0 = nodeB->addInputSlot("in0", model::Slot::DataType::CUSTOM + 1);
    auto slotB1 = nodeB->addInputSlot("in1", model::Slot::DataType::CUSTOM + 1);

    auto slotC0 = nodeC->addInputSlot("in", model::Slot::DataType::CUSTOM + 1);
    auto slotC1 = nodeC->addOutputSlot("out", model::Slot::DataType::CUSTOM + 1);

    auto edgeAB0 = new model::Edge(graph);
    edgeAB0->setDestination(slotB0);
    edgeAB0->setOrigin(slotA);

    auto edgeAC = new model::Edge(graph);
    edgeAC->setOrigin(slotA);
    edgeAC->setDestination(slotC0);

    auto edgeCB = new model::Edge(graph);
    edgeCB->setOrigin(slotC1);
    edgeCB->setDestination(slotB1);

    slotA->setData(QVariant("fatehssss"));

    graph->start();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    testModel();
    //BleachUI w;
    //w.show();

    return a.exec();
}

