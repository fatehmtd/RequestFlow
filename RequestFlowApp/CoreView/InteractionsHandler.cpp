#include "InteractionsHandler.h"
#include "view/SceneGraph.h"
#include "view/Node.h"
#include "view/Slot.h"
#include "view/Colors.h"
#include "view/Edge.h"
#include "view/ConnectionEdge.h"

#include "logic/PayloadNode.h"
#include "logic/EndpointNode.h"
#include "logic/ViewerNode.h"
#include "logic/DelayNode.h"
#include "logic/ScriptNode.h"
#include "logic/AssertionNode.h"
#include "logic/ExternalNode.h"

view::InteractionsHandler::InteractionsHandler(SceneGraph* scene) : QObject(scene), _sceneGraph(scene)
{
	registerCommonActions();
}

view::InteractionsHandler::~InteractionsHandler()
{
}

void view::InteractionsHandler::registerGenericAction(const QString& name,
	std::function<bool(QGraphicsItem*)> filter,
    ExecFunc func,
	QIcon icon, int order)
{
	ItemAction action = { order, name, func, filter, icon };
	_itemActionsList << action;

    std::sort(_itemActionsList.begin(), _itemActionsList.end(), [](const ItemAction & a, const ItemAction & b)
              {
                  return a.order > b.order;
              });
    //qSort(_itemActionsList);
}

void view::InteractionsHandler::registerNodeTypeAction(const QString& name, const QString& nodeType, ExecFunc func, QIcon icon, int order)
{
	registerGenericAction(name, [nodeType](QGraphicsItem* item)
		{
			auto node = dynamic_cast<view::Node*>(item);
			if (node == nullptr) return false;
			return nodeType.toLower() == node->getNodeType().toLower();
		}, func, icon, order);
}

void view::InteractionsHandler::registerNodeAction(const QString& name, ExecFunc func, QIcon icon, int order)
{
	registerGenericAction(name, [](QGraphicsItem* item)
		{
			auto node = dynamic_cast<view::Node*>(item);
			return node != nullptr;
		}, func, icon, order);
}

void view::InteractionsHandler::registerEdgeAction(const QString& name, ExecFunc func, QIcon icon, int order)
{
	registerGenericAction(name, [](QGraphicsItem* item)
		{
			auto edge = dynamic_cast<view::Edge*>(item);
			return edge != nullptr;
		}, func, icon, order);
}

void view::InteractionsHandler::registerEmptySpaceAction(const QString& name, ExecFunc func, QIcon icon, int order)
{
	registerGenericAction(name, [](QGraphicsItem* item)
		{
			return item == nullptr;
		}, func, icon, order);
}

#include <model/EndpointEntry.h> 

view::Node* view::InteractionsHandler::createEndpointNode(const model::EndpointEntry* entry)
{
	auto nodeModel = new model::EndpointNode(_sceneGraph->getModelGraph());
	nodeModel->createModel();

	if (entry != nullptr)
	{
		nodeModel->setHttpMethod(entry->getHttpMethod());
		nodeModel->setUrl(entry->getUrl());
	}

	auto grNodeC = new logic::EndpointNode(nodeModel);
	_sceneGraph->addItem(grNodeC);
	return grNodeC;
}

view::Node* view::InteractionsHandler::createPayloadNode()
{
	auto modelNode = new model::PayloadNode(_sceneGraph->getModelGraph());
	modelNode->createModel();
	auto grNodeA = new logic::PayloadNode(modelNode);
	_sceneGraph->addItem(grNodeA);
	return grNodeA;
}

view::Node* view::InteractionsHandler::createViewerNode()
{
	auto modelNode = new model::ViewerNode(_sceneGraph->getModelGraph());
	modelNode->createModel();

	auto grNodeB = new logic::ViewerNode(modelNode);
	_sceneGraph->addItem(grNodeB);
	return grNodeB;
}

view::Node* view::InteractionsHandler::createDelayNode()
{
	auto modelNode = new model::DelayNode(_sceneGraph->getModelGraph());
	modelNode->createModel();

	auto graphicNode = new logic::DelayNode(modelNode);
	_sceneGraph->addItem(graphicNode);
	return graphicNode;
}

view::Node* view::InteractionsHandler::createScriptNode()
{
	auto modelNode = new model::ScriptNode(_sceneGraph->getModelGraph());
	modelNode->createModel();

	auto graphicNode = new logic::ScriptNode(modelNode);
	_sceneGraph->addItem(graphicNode);
	return graphicNode;
}

view::Node* view::InteractionsHandler::createAssertionNode()
{
	auto modelNode = new model::AssertionNode(_sceneGraph->getModelGraph());
	modelNode->createModel();

	auto graphicNode = new logic::AssertionNode(modelNode);
	_sceneGraph->addItem(graphicNode);
    return graphicNode;
}

view::Node *view::InteractionsHandler::createExternalNode()
{
    auto modelNode = new model::ExternalNode(_sceneGraph->getModelGraph());
    modelNode->createModel();

    auto graphicNode = new logic::ExternalNode(modelNode);
    _sceneGraph->addItem(graphicNode);
    return graphicNode;
}

void view::InteractionsHandler::deleteNode(Node* node)
{
	auto edges = _sceneGraph->getModelGraph()->findEdges(node->getModelNode());
	for (auto edge : edges)
	{
		auto grEdge = _sceneGraph->findbyModel(edge);
		delete grEdge;
		delete edge;
	}

	delete node->getModelNode();
	delete node;
}

void view::InteractionsHandler::deleteEdge(Edge* edge)
{
	delete edge->getModelEdge();
	delete edge;
}

void view::InteractionsHandler::deleteInputSlot(Slot* slot)
{
	// find and delete the edges
	auto modelSlot = slot->getModelSlot();
	for (auto edgeModel : _sceneGraph->getModelGraph()->findEdges(modelSlot))
	{
		deleteEdge(_sceneGraph->findbyModel(edgeModel));
	}
	// delete the graphicsitem and the model slot

	auto node = slot->getNode();
	delete slot;
	delete modelSlot;
	node->update();
}

#include <QMenu>
#include <model/Project.h>

QMenu* view::InteractionsHandler::createContextMenu(const QPointF& p)
{
    auto selectedItems = _sceneGraph->selectedItems();

	auto item = _sceneGraph->itemAt(p, QTransform());
	QMenu* menu = new QMenu();

	QList<ItemAction> availableActions;
	for (const auto& action : _itemActionsList)
	{
		if (action.filter(item))
			availableActions << action;
    }

    // Environments menu
    if(selectedItems.isEmpty())
    {
        auto envMenu = menu->addMenu(QIcon(":/ui/environment"), "Active Environment");
        auto environments = _sceneGraph->getModelGraph()->getProject()->getEnvironments();

        if(_sceneGraph->getModelGraph()->getActiveEnvironment() == nullptr)
        {
            _sceneGraph->getModelGraph()->setActiveEnvironment(environments.at(0));
        }

        QHash<model::Environment*, QAction*> envActionMap;
        std::for_each(environments.begin(), environments.end(), [=, &envActionMap](model::Environment* environment)
                      {
                          auto action = envMenu->addAction(environment->getName(), this, [this, environment]()
                                                                         {
                                                                             _sceneGraph->getModelGraph()->setActiveEnvironment(environment);
                                                                         });
                          action->setCheckable(true);
                          action->setChecked(_sceneGraph->getModelGraph()->getActiveEnvironment() == environment);
                          envActionMap[environment] = action;
                      });

        envMenu->setEnabled(!_sceneGraph->getModelGraph()->isRunning());
        menu->addSeparator();
    }

	if (!availableActions.isEmpty())
    {
        //qSort(availableActions);
        std::sort(availableActions.begin(), availableActions.end(), [](const ItemAction & a, const ItemAction & b)
                  {
                      return a.order > b.order;
                  });

		int prevOrder = -1;
		if (availableActions.size() > 0)
			prevOrder = availableActions[0].order;

        std::for_each(availableActions.begin(), availableActions.end(), [=, &prevOrder](const ItemAction &action)
                      {
                          if (prevOrder != action.order)
                              menu->addSeparator();

                          auto qaction = menu->addAction(action.icon, action.name);

                          if (_sceneGraph->getModelGraph()->isRunning())
                          {
                              qaction->setEnabled(action.name == "Stop");
                          }
                          else
                          {
                              qaction->setEnabled(action.name != "Stop");
                          }

                          connect(qaction, &QAction::triggered, this, [=]()
                                  {
                                      action.func(p, qaction);
                                  });
                          prevOrder = action.order;
                      });
	}

	return menu;
}

#include <model/PersistenceHandler.h>
#include <QInputDialog>
#include <QGraphicsView>
#include <QPixmap>
#include <QColor>
#include <view/Colors.h>

#include <QSvgRenderer>
#include <QPainter>

QIcon createColoredRect(QColor color, const QString & icon = "")
{
    if(!icon.isEmpty())
    {
        int s = 128;
        int padding = 8;
        QImage outputImg(s, s, QImage::Format::Format_RGBA8888);
        outputImg.fill(color);

        QImage iconImage(s-2*padding, s-2*padding, QImage::Format::Format_RGBA8888);
        iconImage.fill(color);
        QPainter painter(&iconImage);

        QSvgRenderer svgRenderer(icon);
        svgRenderer.render(&painter);


        const float inv255 = 1.0f/255.0f;
        auto colorRgba = color.rgba();
        //auto iconImage = QPixmap(icon).scaled(s-2*padding, s-2*padding, Qt::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation).toImage();
        //auto iconImage = srcIcon;
        for(int j=0;j<iconImage.height();j++)
        {
            for(int i=0;i<iconImage.width();i++)
            {
                auto src = iconImage.pixel(i, j);

                const float af = (float)qAlpha(src) * inv255;
                //if(af <= 0.3f) continue;
                const float r = (qRed(src) * af + (1.0f-af)*qRed(colorRgba));
                const float g = (qGreen(src) * af + (1.0f-af)*qGreen(colorRgba));
                const float b = (qBlue(src) * af + (1.0f-af)*qBlue(colorRgba));

                outputImg.setPixel(padding + i, padding + j, qRgba((int)r, (int)g, (int)b, qAlpha(src)));
            }
        }
        return QIcon(QPixmap::fromImage(outputImg));
    }
    else
    {
        QPixmap pixmap(16, 16);
        pixmap.fill(color);
        return QIcon(pixmap);
    }
}

void view::InteractionsHandler::registerCommonActions()
{
    registerEmptySpaceAction("Stop", [=](const QPointF& p, QAction* action)
		{
			_sceneGraph->getModelGraph()->cancel();
		}, QIcon(":/BleachUI/stop"));

    registerEmptySpaceAction("Execute", [=](const QPointF& p, QAction* action)
        {
            _sceneGraph->getModelGraph()->start();
        }, QIcon(":/BleachUI/play"));

	registerGenericAction("Delete",
		[](QGraphicsItem* item)
		{
			if (dynamic_cast<Node*>(item)) return true;
			if (dynamic_cast<Edge*>(item)) return true;
			return false;
		},
        [=](const QPointF& p, QAction* action)
		{
			auto item = _sceneGraph->itemAt(p, QTransform());
			auto edge = dynamic_cast<Edge*>(item);
			if (edge != nullptr)
			{
				deleteEdge(edge);
			}
			else
			{
				auto node = dynamic_cast<Node*>(item);
				deleteNode(node);
			}
        }, QIcon(":/ui/delete"));

    registerEmptySpaceAction("New Payload", [=](const QPointF& p, QAction* action)
		{
			auto node = createPayloadNode();
			node->setPos(p);
        }, createColoredRect(view::colors::nodes::payload, ":/nodes/payload"), 1);

    registerEmptySpaceAction("New Endpoint", [=](const QPointF& p, QAction* action)
		{
			auto node = createEndpointNode();
			node->setPos(p);
        }, createColoredRect(view::colors::nodes::endpoint, ":/nodes/endpoint"), 1);

    registerEmptySpaceAction("New Viewer", [=](const QPointF& p, QAction* action)
		{
			auto node = createViewerNode();
			node->setPos(p);
        }, createColoredRect(view::colors::nodes::viewer, ":/nodes/viewer"), 1);

    registerEmptySpaceAction("New Delay", [=](const QPointF& p, QAction* action)
		{
			auto node = createDelayNode();
			node->setPos(p);
        }, createColoredRect(view::colors::nodes::delay, ":/nodes/delay"), 1);

    registerEmptySpaceAction("New Script", [=](const QPointF& p, QAction* action)
		{
			auto node = createScriptNode();
			node->setPos(p);
        }, createColoredRect(view::colors::nodes::script, ":/nodes/script"), 1);

    registerEmptySpaceAction("New Assertion", [=](const QPointF& p, QAction* action)
		{
			auto node = createAssertionNode();
			node->setPos(p);
        }, createColoredRect(view::colors::nodes::assertion, ":/nodes/assertion"), 1);

    registerEmptySpaceAction("New External", [=](const QPointF& p, QAction* action)
        {
            auto node = createExternalNode();
            node->setPos(p);
        }, createColoredRect(view::colors::nodes::external, ":/nodes/external"), 1);

    registerNodeAction("Duplicate", [=](const QPointF& p, QAction* action)
		{
			auto item = _sceneGraph->itemAt(p, QTransform());
			auto originalNode = dynamic_cast<view::Node*>(item);

			auto node = _sceneGraph->cloneNode(originalNode);
			if (node != nullptr)
			{
				node->setPos(p);
			}
        }, QIcon(":/ui/duplicate"), 1);

    registerNodeAction("Rename", [=](const QPointF& p, QAction* action)
		{
			auto item = _sceneGraph->itemAt(p, QTransform());
			auto originalNode = dynamic_cast<view::Node*>(item);

            bool okSelected = false;
            auto newName = QInputDialog::getText(_sceneGraph->views()[0]->parentWidget(), "Rename Node", "New name :", QLineEdit::Normal, originalNode->getModelNode()->getName(), &okSelected);
            //if(!newName.isEmpty())
            if(okSelected)
            {
                originalNode->setTitle(newName);
                originalNode->update();
            }

		}, QIcon(":/ui/pen"), 1);

    registerNodeTypeAction("Add input slot", "Script", [=](const QPointF& p, QAction* action)
		{
			auto item = _sceneGraph->itemAt(p, QTransform());
			auto node = dynamic_cast<logic::ScriptNode*>(item);
			auto modelNode = node->getModelNode();

			auto modelSlot = modelNode->addInputSlot("Input", model::Slot::CUSTOM);
			auto slot = new view::Slot(node, modelSlot);
			renameInputSlots(slot->getNode());
			//_sceneGraph->addItem(slot);
			node->setSize(node->width(), node->height());
		}, QIcon(":/BleachUI/copy"), 1);

	registerGenericAction("Delete",
		[](QGraphicsItem* item)
		{
			auto slot = dynamic_cast<Slot*>(item);
			if (slot != nullptr)
			{
                if (slot->getModelSlot()->getDirection() != model::Slot::Direction::INPUT) return false;
				return dynamic_cast<model::ScriptNode*>(slot->getNode()->getModelNode()) != nullptr;
			}
			return false;
		},
        [=](const QPointF& p, QAction* action)
		{
			auto item = _sceneGraph->itemAt(p, QTransform());
			auto slot = dynamic_cast<Slot*>(item);
			auto node = slot->getNode();
			deleteInputSlot(slot);
			renameInputSlots(node);
			node->setSize(node->width(), node->height());
		}, QIcon(":/BleachUI/delete"));
}

void view::InteractionsHandler::renameInputSlots(view::Node* node) const
{
	auto modelSlots = node->getModelNode()->getInputSlots();

	int number = modelSlots.size();
	if (number > 1)
	{
		int index = 0;
		for (auto modelSlot : modelSlots)
		{
			auto slot = _sceneGraph->findbyModel(modelSlot);
			auto name = QString("Input %1").arg(index++);
			slot->setName(name);
			slot->update();
		}
	}
	else if (number == 1)
	{
		auto slot = _sceneGraph->findbyModel(modelSlots[0]);
		slot->setName("Input");
	}

	node->update();
}
