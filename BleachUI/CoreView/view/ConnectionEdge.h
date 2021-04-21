#pragma once
#include <QGraphicsPathItem>
#include <QGraphicsTextItem>
#include <model/Slot.h>
#include <QColor>

namespace view
{
	class Slot;

	class ConnectionEdge : public QGraphicsPathItem
	{
	public:
		ConnectionEdge();

		void setOrigin(Slot* slot);
		void setOriginP(const QPointF& position);
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
		virtual QPainterPath shape() const override;

	protected:
		Slot* _slotOrigin = nullptr;
		QPointF _destinationPos, _originPos;
		int _thickness = 10;
		Eligibility _eligibility;

		QColor _noCandidateColor = QColor("#FFFFA637");
		QColor _notEligibleColor = QColor("#FF0000");
		QColor _eligibleColor = QColor("#00FF00");
	};
}