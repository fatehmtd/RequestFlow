#pragma once

#include "NotifiableEntity.h"
#include <QVariant>
#include "Message.h"

namespace model
{
	class Node;
	class Edge;

	class COREMODEL_EXPORT Slot : public NotifiableEntity
	{
		Q_OBJECT
		Q_PROPERTY(int dataType MEMBER _dataType READ getDataType)
		Q_PROPERTY(int direction MEMBER _direction READ getDirection)
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

		void clear() override;

		int getDirection() const;

		int getDataType() const;

		Message getData() const;
		void setData(const Message& data);

		Node* getNode() const;

	protected:
		int _dataType;		
		int _direction;
		Message _data;
	};

	class COREMODEL_EXPORT InputSlot : public Slot
	{
		Q_OBJECT
	public:
		InputSlot(Node* parent, QString name, int dataType);
	public slots:
		virtual void onDataReceived();
		void onFailed();
	signals:
		void dataReceived();
		void failed();
	};

	class COREMODEL_EXPORT OutputSlot : public Slot
	{
		Q_OBJECT
	public:
		OutputSlot(Node* parent, QString name, int dataType);

		void sendData();
	public slots:
		void onNodeFail();
	signals:
		void dataSent();
		void failed();
	};
}