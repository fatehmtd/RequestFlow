#pragma once
#include "../view/Node.h"
#include <QTextEdit>
#include "ui_ViewerNodeUi.h"

namespace logic
{
    class ViewerNode : public view::Node
    {
    public:
        Q_INVOKABLE ViewerNode(model::Node* modelNode);
        virtual QJSValue toJSValue(QJSEngine& engine) const;

        virtual void clearUI() override;
    private:
        void setupUi();

    private:
        Ui::ViewerWidgetUiForm _ui;
    };
}