#pragma once

#include "Node.h"

namespace view {
class COREVIEW_EXPORT GroupNode : public Node {
public:
  GroupNode(model::Node *modelNode);
};
} // namespace view
