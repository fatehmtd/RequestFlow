#pragma once

#include "../view/Node.h"
#include <QTextEdit>

#include <model/ViewerNode.h>
#include "LargePayloadInfoWidget.h"

namespace logic
{
    class ViewerNodeWidget;

	class ViewerNode : public view::Node
	{
		Q_OBJECT
	public:
		ViewerNode(model::ViewerNode* modelNode);
		virtual void clearUI() override;
	private:
        void setupUi();
        void showDialog();

        ViewerNodeWidget* _viewWidget = nullptr;
        LargePayloadInfoWidget* _largePayloadInfoWidget = nullptr;
	};
}
