/*************************************************
 *文件名：domparser.cpp
 *描述：配置文件信息获取类，解析配置文件config.xml的内容。
*************************************************/
#include <QtGui>
#include <QtXml>
#include <iostream>
//#include <QtWidgets/QTreeWidget>
//#include <QtWidgets/QTreeWidgetItem>
#include <QDomElement>

#include "domparser.h"

DomParser::DomParser()
{
}

bool DomParser::readFile(const QString &fileName)
{
    QFile file(fileName);/**/
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        return false;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;/**/
    if (!doc.setContent(&file, false, &errorStr, &errorLine,
                        &errorColumn)) {
        std::cerr << "Error: Parse error at line " << errorLine << ", "
                  << "column " << errorColumn << ": "
                  << qPrintable(errorStr) << std::endl;
        return false;
    }

    QDomElement root = doc.documentElement();/****root element****/
    if (root.tagName() != "csc") {
        std::cerr << "Error: Not a csc xml file" << std::endl;
        return false;
    }

    QDomNode child = root.firstChild();
    while (!child.isNull()) {
        if (child.toElement().tagName() == "entry")
        {
            qDebug() << child.toElement().attribute("name");
            parseEntryElement(child.toElement());
        }
        child = child.nextSibling();
    }
    return true;
}

void DomParser::parseEntryElement(const QDomElement &element)
{
    /*QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, element.attribute("name"));
*******/

    QDomNode child = element.firstChild();
    while (!child.isNull()) {
        if (child.toElement().tagName() == "log") {
            parsePageElement(child.toElement());
        }
        child = child.nextSibling();
    }
}


void DomParser::parsePageElement(const QDomElement &element)
{
    id_list << element.attribute("id");
    text_list << element.text();

    /*QString allPages = parent->text(1);
    if (!allPages.isEmpty())
        allPages += ", ";
    allPages += page;
    parent->setText(1, allPages);*/
}

