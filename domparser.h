/*************************************************
 *�ļ�����domparser.h
 *�����������ļ���Ϣ��ȡ��ͷ�ļ������������ļ���Ϣ��ȡ������ݡ�
*************************************************/
#ifndef DOMPARSER_H
#define DOMPARSER_H

class QDomElement;
class QString;
class QStringList;
//class QTreeWidget;
//class QTreeWidgetItem;

class DomParser
{
public:
    DomParser();
    bool readFile(const QString &fileName);

    QStringList id_list;
    QStringList text_list;

private:
    //void parseBookindexElement(const QDomElement &element);
    void parseEntryElement(const QDomElement &element);
    void parsePageElement(const QDomElement &element);

    //int stringToInt(const QString id_str);
    //int id_size;



};

#endif // DOMPARSER_H
