#include <QtWidgets/QApplication>
//#include <QTCore/QTextCodec>
#include "hello.h"
#include "analysislog.h"

/*
 *文件名：main.cpp
 *程序入口，调用QT应用程序入口函数QApplication。
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
	QFile File(xmlFile);//xml文件

	if (!File.open(QIODevice::ReadOnly))//只读方式打开
	{
		qDebug() << "open xml file failed!";
		return pluginList;
	}

	if (!Doc.setContent(&File))//解析文件
	{
		File.close();
		qDebug() << "parse xml file failed!";
		return pluginList;
	}
	File.close();//关闭文件

	QDomElement RootElem = Doc.documentElement();//xml文件的根元素
	QDomNode Elem = RootElem.firstChild();//第一个子元素

	//循环遍历xml文件
	while(!Elem.isNull())//判空
	{
		QDomElement ElemInfo = Elem.toElement(); // try to convert the node to an element.
		if(!ElemInfo.isNull()) 
		{
			pluginList << ElemInfo.attribute("name"); // 获取属性name的值.
		}
		Elem = Elem.nextSibling();//寻找下一个元素
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

	printf("输入文件名：");
	scanf("%s",inputFile);
	printf("文件名为：%s\n",inputFile);

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
