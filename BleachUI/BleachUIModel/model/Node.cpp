#include "Node.h"
#include "Graph.h"
#include "Slot.h"
#include <QChildEvent>
#include <QDebug>

model::Node::Node(Graph* parent, const QString& name) : BaseEntity(parent)
{
	setObjectName(name);
	connect(parent, &Graph::started, this, [this]() 
		{
			onGraphStart();
		});

	connect(parent, &Graph::stopped, this, [this]()
		{
			onGraphStop();
		});

	//connect(this, &Node::readyForEvaluation, this, &Node::evaluate);
	connect(this, &Node::evaluated, parent, &Graph::onNodeEvaluated);
}

model::InputSlot* model::Node::getDestination(const QString& name) const
{
	return findChild<InputSlot*>(name, Qt::FindChildOption::FindDirectChildrenOnly);
}

model::OutputSlot* model::Node::getOrigin(const QString& name) const
{
	return findChild<OutputSlot*>(name, Qt::FindChildOption::FindDirectChildrenOnly);
}

model::InputSlot* model::Node::addInputSlot(QString name, int dataType)
{
	auto slot = new InputSlot(this, name, dataType);
	connect(slot, &InputSlot::dataReceived, this, &Node::slotDataReceived);
	return slot;
}

model::OutputSlot* model::Node::addOutputSlot(QString name, int dataType)
{
	auto slot = new OutputSlot(this, name, dataType);
	connect(this, &Node::evaluated, slot, [slot]() { slot->sendData(); });
	connect(slot, &OutputSlot::dataSent, this, &Node::slotDataSent);
	return slot;
}

QMap<QString, model::InputSlot*> model::Node::getInputSlots() const
{
	QMap<QString, InputSlot*> output;
	auto listOfChildren = children();
	for (auto child : listOfChildren)
	{
		auto slot = dynamic_cast<InputSlot*>(child);
		if (slot != nullptr)
		{
			output[slot->objectName()] = slot;
		}
	}
	return std::move(output);
}

QMap<QString, model::OutputSlot*> model::Node::getOutputSlots() const
{
	QMap<QString, OutputSlot*> output;
	auto listOfChildren = children();
	for (auto child : listOfChildren)
	{
		auto slot = dynamic_cast<OutputSlot*>(child);
		if (slot != nullptr)
		{
			output[slot->objectName()] = slot;
		}
	}
	return std::move(output);
}

model::Graph* model::Node::getGraph() const
{
	return dynamic_cast<Graph*>(parent());
}

void model::Node::clear()
{
	_listOfReadySlots.clear();
}

void model::Node::evaluate()
{
	emit evaluated();

	for (auto slot : getOutputSlots().values())
	{
		slot->sendData();
	}	
}

void model::Node::onGraphStart()
{
	clear();

	// If the node has no input slots, proceed to evaluation when the graph execution starts
	if (getInputSlots().size() == 0)
	{
		emit readyForEvaluation();
	}
}

void model::Node::onGraphStop()
{
	clear();
}

void model::Node::dataReceived(InputSlot* slot)
{

}

void model::Node::slotDataReceived() 
{
	auto inputSlot = dynamic_cast<InputSlot*>(sender());
	if (inputSlot != nullptr)
	{
		if (!_listOfReadySlots.contains(inputSlot))
		{
			dataReceived(inputSlot);

			_listOfReadySlots.push_back(inputSlot);
			
			// When all the slots receive data, the node becomes ready for evaluation
			if (_listOfReadySlots.size() == getInputSlots().size())
			{
				emit readyForEvaluation();
			}
		}
	}
}

void model::Node::slotDataSent() 
{

}