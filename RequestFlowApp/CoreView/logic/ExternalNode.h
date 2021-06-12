#ifndef EXTERNALNODE_H
#define EXTERNALNODE_H

#include "../view/Node.h"
#include <model/ExternalNode.h>
#include "ui_ExternalNodeUi.h"


namespace logic
{
    class COREVIEW_EXPORT ExternalNode : public view::Node
    {
        Q_OBJECT
    public:
        ExternalNode(model::ExternalNode* modelNode);
        virtual void clearUI() override;
    private:
        void setupUi();
        model::ExternalNode *getExternalNode() const;
        void setSelectedNode(model::Node* node);
    private slots:
        void onDeltaDelayChanged(int value);
        void onBrowse();
    private:
        Ui::ExternalNodeUi _ui;
    };
}

#endif // EXTERNALNODE_H
