#pragma once
#include "../view/Node.h"
#include <QTextEdit>
#include "ui_PayloadNodeUi.h"
#include <model/PayloadNode.h>
#include "../JSONHighlighter.h"

namespace logic
{
    class COREVIEW_EXPORT PayloadNode : public view::Node
    {
        Q_OBJECT
    public:
        Q_INVOKABLE PayloadNode(model::PayloadNode* modelNode);

        virtual void clearUI() override;
    private:
        void setupUi();
        void prepareAndSend();
		model::Message composeMessage() const;
        void fillFromMessage(const model::Message& message);

        QMap<QString, QVariant> fillFromTable(QTableWidget* tableWidget) const;

    private:
        Ui::PayloadNodeUi _ui;
        view::JSONHighlighter* _highlighter;
    };
}

