#include "JsonFormatingClass.h"

JsonFormatingClass::JsonFormatingClass(QObject* parent)
	: QObject(parent)
{
}

JsonFormatingClass::~JsonFormatingClass()
{
}

void JsonFormatingClass::reFormat(QTreeWidget* val)
{
	QJsonDocument doc;

	QJsonArray messageArray;
	QJsonObject taskObject;
	QJsonArray arrayOfTask;

	for (int count = 0; count < val->topLevelItemCount(); count++)
	{
		QTreeWidgetItem* parentTop = val->topLevelItem(count);

		for (int countChild = 0; countChild < parentTop->childCount(); countChild++)
		{
			QTreeWidgetItem* child = parentTop->child(countChild);

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
		  
		   { "messegeArray", messageArray },
			{ "numberTask", parentTop->text(0) }
		};

		arrayOfTask.push_back(taskObject);
	}

	doc.setArray(arrayOfTask);
	QByteArray bytes = doc.toJson(QJsonDocument::Compact); // или Indented

	emit sendJsonToServer(bytes);

	//doc.setObject(taskObject);

	//qDebug() << doc.toJson();
}