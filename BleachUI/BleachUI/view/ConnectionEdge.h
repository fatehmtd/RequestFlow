#pragma once
#include <QGraphicsPathItem>
#include <QGraphicsTextItem>
#include <model/Slot.h>

namespace view
{
	class Slot;

	class ConnectionEdge : public QGraphicsPathItem
	{
	public:
		ConnectionEdge();

		void setOrigin(Slot* slot);
		void setDestination(const QPointF& position);

		enum Eligibility
		{
			ELIGIBLE,
			NOT_ELIGIBLE,
			NO_CANDIDATE
		};

		void setEligible();
		void setNotEligible();
		void setNoCandidateAvaible();

	protected:
		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

		QPainterPath buildPath() const;
		virtual QRectF boundingRect() const override;

	protected:
		Slot* _slotOrigin = nullptr;
		QPointF _destinationPos;
		int _thickness = 10;
		Eligibility _eligibility;
	};
}