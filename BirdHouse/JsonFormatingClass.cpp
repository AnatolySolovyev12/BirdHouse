#include "JsonFormatingClass.h"

JsonFormatingClass::JsonFormatingClass(QObject* parent)
	: QObject(parent)
{
}

JsonFormatingClass::~JsonFormatingClass()
{
}


void JsonFormatingClass::reFormat(QTreeWidgetItem* any)
{
	QJsonDocument doc;
	QJsonArray messageArray;

	for (int countChild = 0; countChild < any->childCount(); countChild++)
	{
		QTreeWidgetItem* child = any->child(countChild);

		QJsonObject messegeObject
		{
		   { "numberMessege", child->text(0) },
		   { "mail", child->text(1) },
		   { "phoneNumber", child->text(2) },
		   { "sendMax", child->checkState(3) },
		   { "sendTelegram", child->checkState(4) },
		   { "sendMail", child->checkState(5) },
		   { "sendSms", child->checkState(6) },
		   { "date", child->text(7) },
		   { "time", child->text(8) },
		   { "text", child->text(9) }
		};

		messageArray.push_back(messegeObject);
	}

	QJsonObject taskObject
	{
	   {"numberTask", any->text(0)},
	   { "messegeArray", messageArray }
	};

	qDebug() << taskObject;





	//doc.setObject(taskObject);

	//qDebug() << doc.toJson();
}