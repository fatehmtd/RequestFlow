#pragma once

#include "BaseEntity.h"
#include <QVariant>

namespace model
{
	class Node;
	class Edge;

	class BLEACHUIMODEL_EXPORT Slot : public BaseEntity
	{
	public:
		enum Direction
		{
			INPUT,
			OUTPUT
		};

		enum DataType
		{
			BYTES,
			STRING,
			INT,
			FLOAT,
			CHAR,
			BOOL,
			///////////////////////////////////////
			CUSTOM = 99 // customizable by the user
		};

		Slot(Node* parent, QString name, Direction direction, int dataType);
		~Slot();

		Direction getDirection() const;

		int getDataType() const;

		QVariant getData() const;
		void setData(const QVariant& data);

		Node* getNode() const;
	protected:
		int _dataType;
		QVariant _data;
	private:
		Direction _direction;
	};

	class BLEACHUIMODEL_EXPORT InputSlot : public Slot
	{
		Q_OBJECT
	public:
		InputSlot(Node* parent, QString name, int dataType);
	public slots:
		virtual void onDataReceived();
	signals:
		void dataReceived();
	};

	class BLEACHUIMODEL_EXPORT OutputSlot : public Slot
	{
		Q_OBJECT
	public:
		OutputSlot(Node* parent, QString name, int dataType);

		void sendData();
	signals:
		void dataSent();
	};
}