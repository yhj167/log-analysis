#include <QtWidgets/QApplication>
//#include <QTCore/QTextCodec>
#include "hello.h"
#include "analysislog.h"

/*
 *�ļ�����main.cpp
 *������ڣ�����QTӦ�ó�����ں���QApplication��
*/

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	hello w;
	w.show();

	return a.exec();
}
#if 0
QStringList loadPluginConfig(QString xml_file)
{
	QStringList pluginList;
	QDomDocument Doc("mydocument");
	QFile File(xmlFile);//xml�ļ�

	if (!File.open(QIODevice::ReadOnly))//ֻ����ʽ��
	{
		qDebug() << "open xml file failed!";
		return pluginList;
	}

	if (!Doc.setContent(&File))//�����ļ�
	{
		File.close();
		qDebug() << "parse xml file failed!";
		return pluginList;
	}
	File.close();//�ر��ļ�

	QDomElement RootElem = Doc.documentElement();//xml�ļ��ĸ�Ԫ��
	QDomNode Elem = RootElem.firstChild();//��һ����Ԫ��

	//ѭ������xml�ļ�
	while(!Elem.isNull())//�п�
	{
		QDomElement ElemInfo = Elem.toElement(); // try to convert the node to an element.
		if(!ElemInfo.isNull()) 
		{
			pluginList << ElemInfo.attribute("name"); // ��ȡ����name��ֵ.
		}
		Elem = Elem.nextSibling();//Ѱ����һ��Ԫ��
	}

	return pluginList;


    DomParser parser;
    parser.readFile("F:/Projects/chap16/domparser/in1.xml");
    int id_size = parser.id_list.size();
    int id_array[id_size] = {0};
    for(int i; i<id_size; ++i)
    {
        id_array[i] = stringToInt(id_list[i]);
    }
}
#endif
/*=========================================================================
int main()
{
	char inputFile[80] = {0};
	int current_num=0;
	int read_completed;

#if 0
	int a,b;
	float c;
	a =130;
	b =100;
	c = ((float)a/b);
	printf("%.3f\n",c);
#endif

	printf("�����ļ�����");
	scanf("%s",inputFile);
	printf("�ļ���Ϊ��%s\n",inputFile);

	for(;;)
	{
		if((read_completed = readFile1(inputFile,current_num)) != 1)
		{
			current_num++;
		}else{
			break;
		}
	}
	printf("\nlogAnalysis end!\n");

	return 0;
}
=========================================================================*/
