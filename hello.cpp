/*************************************************
 *文件名：hello.cpp
 *描述：主界面类，开启主界面，调用其他功能类。
*************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QTextCodec>
#include <QMessageBox>
#include <QList>
#include <QAxObject>
#include <QDesktopServices>
//#include <QCloseEvent>
#include <QVBoxLayout>
#include <QUrl>
#include <iostream>
#include <QtGui>
#include <QtWidgets\QFileDialog>
#include <QtWidgets\QApplication>
#include <QtWidgets\QTableWidgetItem>

#include "hello.h"
#include "ui_hello.h"
#include "analysislog.h"
#include "domparser.h"
#include "finddialog.h"

#include <QTableWidget>
#include <QWidget>

//unsigned char file_test[25]={0};
//char outputFile[30] = {0};
//QString logFileName=NULL;
Ui::helloClass ui;

hello::hello(QWidget *parent)
	: QMainWindow(parent)
{
    parser = new DomParser;
    parseOK = false;
    findDialog = 0;
    analysis_completed = false;

    /*yhj
    QVBoxLayout* verticalLayout;
    verticalLayout->addWidget(ui.analysisResultTableWidget);*/
    ui.setupUi(this);
    //ui.centralWidget->setStyleSheet("background-image:url(:/images/2.JPG)");
    //ui.centralWidget->setStyleSheet(QStringLiteral("background-color: rgb(0, 176, 240);"));

    ui.analysisProgressBar->setValue(0);
	ui.analysisResultTableWidget->setHorizontalHeaderLabels(
        QStringList() << QStringLiteral("日志类型") << QStringLiteral("发生的次数")
                << QStringLiteral("发生的时间") << QStringLiteral("当前总线时间") << QStringLiteral("当前CCU时间"));

    createAction();
    createToolBars();
    createStatusBar();

    connect(ui.browseLogFile,SIGNAL(clicked()),this,SLOT(openLogfile()));
    connect(ui.analysisLogFile, SIGNAL(clicked()),this,SLOT(analysis()));
    connect(ui.browseCfgFile,SIGNAL(clicked()),this,SLOT(openCfgfile()));/****/
    connect(ui.parseCfgFile,SIGNAL(clicked()),this,SLOT(parseCfgfile()));/****/

    setWindowTitle(QStringLiteral("日志分析工具"));
	//QTextCodec *codec = QTextCodec::codecForName("GB18030");
	//QTextCodec::setCodecForTr(codec);
	//QTextCodec::setCodecForCStrings(codec);
	//QTextCodec::setCodecForLocale(codec);

    cfgFileName = "config.xml";
    ui.cfgFilePath->setText(QString("%1").arg(cfgFileName));
}

hello::~hello()
{

}

void hello::reserved()
{
    QMessageBox::warning(this, QStringLiteral("消息提示"), QStringLiteral("功能维护中...\n"), QMessageBox::Ok);
}


void hello::find()
{
        ui.analysisResultTableWidget->setCurrentCell(0,0);/****/
        if (!findDialog) {
            findDialog = new FindDialog(this);
            connect(findDialog, SIGNAL(findNext(const QString &,Qt::CaseSensitivity)),this, SLOT(findNext(const QString &,Qt::CaseSensitivity)));
            connect(findDialog, SIGNAL(findPrevious(const QString &,Qt::CaseSensitivity)),this, SLOT(findPrevious(const QString &,Qt::CaseSensitivity)));
        }

        findDialog->show();
        findDialog->raise();
        findDialog->activateWindow();
}

bool hello::okToContinue()
{
	if(isWindowModified()){
		int r = QMessageBox::warning(this,tr("Spreadsheet"),
			tr("The document has been modified.\n"
			"Do you want to save your changes?"),
			QMessageBox::Yes | QMessageBox::No |QMessageBox::Cancel);
		if (r == QMessageBox::Yes)
        {
		} 
		else if(r == QMessageBox::Cancel)
		{
			return false;
		}
	}
	return true;
}

#if 0
void hello::closeEvent(QCloseEvent *event)
{
	event->accept();
}
#endif


void hello::about()
{
    QMessageBox::about(this,tr("About IDMS-LAT"),
        tr("<h2>Log Analysis Tool of Airborne Integrated Data Management System</h2>"
		"<p>Copyright &copy; 2014 Software Inc."
        "<p>Version1.0 2014.10"));
}


int hello::openCfgfile()
{
    //cfgFileName = "";
    if(okToContinue()){
        cfgFileName = QFileDialog::getOpenFileName(this,
            tr("please open cfgfile!"),".",
            tr("Spreadsheet files(*.*)"));
        if(!cfgFileName.isEmpty())
            ui.cfgFilePath->setText(QString("%1").arg(cfgFileName));/*将路径赋值给logFilePath显示，为什么是%1？*/
    }
    return 0;
}


/***************************
*解析xml文件
***************************/
bool hello::parseCfgfile()
{
    //bool ok;
    parseOK = parser->readFile(cfgFileName);
#if 0
    int id_size = parser->id_list.size();
    unsigned int id_array[100]={0};
    for(int i; i<id_size; ++i)
    {
        id_array[i] = parser->id_list[i].toInt(&ok, 16);
        if(ok == false){
            std::cerr << "Error: QString Cannot convert toInt " << qPrintable(cfgFileName)
                      << std::endl;
            return false;
        }
    }
#endif
    if(parseOK == true)
        QMessageBox::warning(this, QStringLiteral("解析成功"), QStringLiteral("Parse Cfgfile Success.\n"), QMessageBox::Ok);
    else
        QMessageBox::warning(this, QStringLiteral("解析失败"), QStringLiteral("Parse Cfgfile Failed.\n"), QMessageBox::Ok);
    return true;
}


int hello::openLogfile()
{
	logFileName = "";
	if(okToContinue()){
		logFileName = QFileDialog::getOpenFileName(this,
			tr("please open analysis file!"),".",
			tr("Spreadsheet files(*.*)"));
		if(!logFileName.isEmpty())
			ui.logFilePath->setText(QString("%1").arg(logFileName));/*将路径赋值给logFilePath显示，为什么是%1？*/
	}
	return 0;
}

/***************************
*日志分析
***************************/
int hello::analysis()
{	
	int current_num=0;
	int read_completed = 0;
    ClearTabel();/*清空table的数据*/
    /*好像没什么效果-_-!!!，我终于明白了，因为显示的是ui.analysisResultTableWidget，和loginfos无关*/
    loginfos.erase(loginfos.begin(),loginfos.end());/*清空loginfos的数据*/
    //ui.analysisResultTableWidget->setRowCount(999);

	/*for (QList<LogInfo>::iterator iter = loginfos.begin(); iter!=loginfos.end(); iter++)
	{
		iter = loginfos.erase(iter);
	}*/

    if(parseOK == false){
        QMessageBox::warning(this, QStringLiteral("消息提示"), QStringLiteral("请先解析配置文件.\n"), QMessageBox::Ok);
        return 0;
    }

    QByteArray filename = logFileName.toLocal8Bit();//Qstring转char*
	for(;;)
	{
		if((read_completed = readLogFile(filename.data(),current_num)) != 1)
		{
            ui.analysisProgressBar->setValue(current_num);
			current_num++;
		}else{
			break;
		}
	}
	/*消息提示*/
    QMessageBox::warning(this, QStringLiteral("消息提示"), QStringLiteral("The document has been analysised.\n"), QMessageBox::Ok);
    analysis_completed = true;


#if 0	
	//设置行数
	if (ui.analysisResultTableWidget->rowCount() < current_num)
		ui.analysisResultTableWidget->setRowCount(current_num);
	//载入
	QList<LogInfo> loginfos;
	loginfos << LogInfo("LOG_ISRHDLC_WRITE_ERR", 50, QStringLiteral("0小时59分27.000秒"), QStringLiteral("2014年 4月30日 0小时22分32.000秒"), QStringLiteral("2000年 0月 0日  0小时 0分0.000秒"));
	AnalysisLog l(&loginfos);
#endif
	return 0;
}

/***************************
*日志分析detail
***************************/
int hello::readLogFile(char* filename, int currentreadnum)
{
	FILE *fpRead = NULL;
	unsigned int file_len=0;
	unsigned int rd_len=0;
	//FILE *fpWrite = NULL;
	//unsigned int wr_len=0;

    //char logidx[30] = {0};//类型
    QString logidx;
	//unsigned char proccesstime[15] = {0};//经过多少时间（毫秒）
	char avTimeString[50/*35*/] = {0};//总线时间
	char rtcTimeString[50] = {0};//本地时间

	int avTime =0;
	int rtcTime =0;
    bool ok;

	if((fpRead = fopen(filename,"rb")) > 0)
	{

		if(currentreadnum == 0)
		{
			//printf("file %s open!\n",filename);
			fseek(fpRead,0,SEEK_END);
			file_len = ftell(fpRead);
			//printf("文件长度：%d\n",file_len);

			//printf("输出文件名：");
			//scanf("%s",outputFile);

			ui.analysisProgressBar->setRange(0, file_len/24-1);//每一行24Byte
		}

		fseek(fpRead, currentreadnum*24, SEEK_SET);
		memset(file_test,0, 25*sizeof(unsigned char));
		rd_len = fread(file_test,1,24,fpRead);

		if(rd_len == 0){	
			//设置行数
			if (ui.analysisResultTableWidget->rowCount() < currentreadnum)
				ui.analysisResultTableWidget->setRowCount(currentreadnum);
			//载入
			AnalysisLog l(&loginfos);
			return 1;
		}
		//printf("文件内容：%s\n",file_test);

	}else{
		//printf("can not open file %s!\n",filename);
		return 1;
	}
	fclose(fpRead);
    fflush(fpRead);

    /**获取ID值和对应的消息**/
    for(int i=0;i < parser->id_list.size();i++)
    {
        if( file_test[1] == parser->id_list[i].toInt(&ok,16) )
        {
            //std::cout << "id: " << parser->id_list[i].toInt(&ok,16) << std::endl;
            //strcpy(logidx,parser->text_list[i]);
            logidx = parser->text_list[i];
            break;
        }
    }

	//次数
	logmsg.property.propertyvalue2 = byteToShort(file_test[2], file_test[3]);

	//毫秒数
	logmsg.property.propertytime = byteToInt(file_test[4], file_test[5], file_test[6], file_test[7]);

	//总线时间
	logmsg.avtime.year = byteToShort(file_test[8], file_test[9]);
	logmsg.avtime.month = file_test[10];
	logmsg.avtime.day = file_test[11];
	logmsg.avtime.us40_l = byteToShort(file_test[12], file_test[13]);
	logmsg.avtime.us40_h = byteToShort(file_test[14], file_test[15]);
	avTime = (int)((logmsg.avtime.us40_h<<16)|logmsg.avtime.us40_l);
	sprintf( avTimeString, "%4d年%2d月%2d日%16s", 2000+logmsg.avtime.year, 
		logmsg.avtime.month, logmsg.avtime.day, intToString(avTime/1000));

	//本地时间
	logmsg.rtctime.year = byteToShort(file_test[16], file_test[17]);
	logmsg.rtctime.month = file_test[18];
	logmsg.rtctime.day = file_test[19];
	logmsg.rtctime.us40_l = byteToShort(file_test[20], file_test[21]);
	logmsg.rtctime.us40_h = byteToShort(file_test[22], file_test[23]);
	rtcTime = (int)((logmsg.rtctime.us40_h<<16)|logmsg.rtctime.us40_l);
	sprintf( rtcTimeString, "%4d年%2d月%2d日%16s", 2000+logmsg.rtctime.year, 
		logmsg.rtctime.month, logmsg.rtctime.day, intToString(rtcTime/1000));

	//写表
	loginfos << LogInfo(  
        /*QString(QLatin1String(logidx))*/logidx,
		logmsg.property.propertyvalue2, 
		QString::fromLocal8Bit(intToString(logmsg.property.propertytime)),//QString(QLatin1String(intToString(logmsg.property.propertytime))), 
		QString::fromLocal8Bit(avTimeString),//QString(QLatin1String(avTimeString)), 
		QString::fromLocal8Bit(rtcTimeString)//QString(QLatin1String(rtcTimeString)) 
		);
	//QStringLiteral();

	return 0;
}


/*=======================================================结果保存=============================================================*/
/*save to txt slot*/
bool hello::saveToTxt()
{
    if(false == parseOK){
        QMessageBox::warning(this, QStringLiteral("消息提示"), QStringLiteral("请先解析配置文件.\n"), QMessageBox::Ok);
        return 0;
    }
    if(false == analysis_completed){
        QMessageBox::warning(this, QStringLiteral("消息提示"), QStringLiteral("请先分析日志文件.\n"), QMessageBox::Ok);
        return 0;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
        QStringLiteral("保存文件"),"D:\LogAnalysisResult.txt",
        QStringLiteral("Txt 文件(*.txt)"));

    if (fileName.isEmpty())
    {
        return false;
    }
    else{
        saveFile(fileName);/*save to txt*/
        return true;
    }
}

/*save to txt*/
bool hello::saveFile(const QString &fileName)
{
    int current_num=0;
    int read_completed;

    QByteArray outputLogFileName = fileName.toLocal8Bit();//Qstring转char*
    QByteArray inputLogFileName = logFileName.toLocal8Bit();//Qstring转char*

    for(;;)
    {
        /*save to txt detail*/
        if((read_completed = saveFileToTxt(inputLogFileName.data(), outputLogFileName.data(), current_num)) != 1)
        {
            ui.analysisProgressBar->setValue(current_num);
            current_num++;
        }else{
            break;
        }
    }
    /*消息提示*/
    //QMessageBox::warning(this, QStringLiteral("消息提示"), QStringLiteral("The document has been saved.\n"), QMessageBox::Ok);
    if(QMessageBox::question(NULL,QStringLiteral("完成"),QStringLiteral("文件已经导出,是否现在打开?"),QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(fileName)));
    }
    return true;
}

/*save to txt detail*/
int hello::saveFileToTxt(char *inputfilename, char *outputfilename, int currentreadnum)
{
    FILE *fpRead = NULL;
    FILE *fpWrite = NULL;
    unsigned int file_len=0;
    unsigned int rd_len=0;
    //unsigned int wr_len=0;

    //char logidx[30] = {0};//类型
    QString logidx;
    QByteArray logidxArray;//QString to char*
    //unsigned char proccesstime[15] = {0};//经过多少时间（毫秒）
    char avTimeString[35] = {0};//总线时间
    char rtcTimeString[35] = {0};//本地时间
    int avTime =0;
    int rtcTime =0;
    bool ok;

    if((fpRead = fopen(inputfilename,"rb")) > 0)
    {
        if(currentreadnum == 0)
        {
            //printf("file %s open!\n",inputfilename);
            fseek(fpRead,0,SEEK_END);
            file_len = ftell(fpRead);
            //printf("文件长度：%d\n",file_len);

            //printf("输出文件名：");
            //scanf("%s",outputFile);

            ui.analysisProgressBar->setRange(0, file_len/24-1);//每一行24Byte
        }

        fseek(fpRead, currentreadnum*24, SEEK_SET);
        memset(file_test,0, 25*sizeof(unsigned char));
        rd_len = fread(file_test,1,24,fpRead);

        if(rd_len == 0){
            return 1;
        }
        //printf("文件内容：%s\n",file_test);

    }else{
        //printf("can not open file %s!\n",inputfilename);
        return 1;
    }
    fclose(fpRead);
    fflush(fpRead);

    //sprintf(outputFile,"F:\2.txt");
    if( (fpWrite = fopen(outputfilename,"ab")) > 0)
    {
        //printf("file open %6d time...\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b", currentreadnum+1);

        if (currentreadnum==0)
        {
            fprintf(fpWrite, "|          日志类型               |  次数   |      发生的时间     |          当前总线时间             |           当前CCU时间             |\n");
            fprintf(fpWrite, "| - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
        }

        /*遍历parser列表查找对应的编号*/
        for(int i=0;i<parser->id_list.size();i++)
        {
            if( file_test[1] == parser->id_list[i].toInt(&ok,16) )
            {
                //std::cout << "id: " << parser->id_list[i].toInt(&ok,16) << std::endl;
                //std::cout << "text: " << parser->text_list[i] << std::endl;
                logidx = parser->text_list[i];
                logidxArray = logidx.toLatin1();
                break;
            }
        }

        logmsg.property.propertyvalue2 = byteToShort(file_test[2], file_test[3]);//次数
        logmsg.property.propertytime = byteToInt(file_test[4], file_test[5], file_test[6], file_test[7]);//毫秒数

        //总线时间
        logmsg.avtime.year = byteToShort(file_test[8], file_test[9]);
        logmsg.avtime.month = file_test[10];
        logmsg.avtime.day = file_test[11];
        logmsg.avtime.us40_l = byteToShort(file_test[12], file_test[13]);
        logmsg.avtime.us40_h = byteToShort(file_test[14], file_test[15]);

        avTime = (int)((logmsg.avtime.us40_h<<16)|logmsg.avtime.us40_l);
        sprintf( avTimeString, "%4d年%2d月%2d日%18s", 2000+logmsg.avtime.year,
            logmsg.avtime.month, logmsg.avtime.day, intToString(avTime/1000));

        //本地时间
        logmsg.rtctime.year = byteToShort(file_test[16], file_test[17]);
        logmsg.rtctime.month = file_test[18];
        logmsg.rtctime.day = file_test[19];
        logmsg.rtctime.us40_l = byteToShort(file_test[20], file_test[21]);
        logmsg.rtctime.us40_h = byteToShort(file_test[22], file_test[23]);

        rtcTime = (int)((logmsg.rtctime.us40_h<<16)|logmsg.rtctime.us40_l);
        sprintf( rtcTimeString, "%4d年%2d月%2d日%18s", 2000+logmsg.rtctime.year,
            logmsg.rtctime.month, logmsg.rtctime.day, intToString(rtcTime/1000));


        fprintf(
            fpWrite,"|\t%30s\t|\t%6d\t|\t%18s\t|\t%32s\t|\t%32s\t|\n",
            logidxArray.data(),
            logmsg.property.propertyvalue2,
            intToString(logmsg.property.propertytime),
            avTimeString,
            rtcTimeString
            );
    }else{
        //printf("can not open file\n");
        return 1;
    }
    fclose(fpWrite);//
    fflush(fpWrite);
    return 0;
}

/***************************
**功能：
**保存到excel SLOT
***************************/
int hello::saveAs()
{
    if(parseOK == false){
        QMessageBox::warning(this, QStringLiteral("消息提示"), QStringLiteral("请先解析配置文件.\n"), QMessageBox::Ok);
        return 0;
    }
    if(false == analysis_completed){
        QMessageBox::warning(this, QStringLiteral("消息提示"), QStringLiteral("请先分析日志文件.\n"), QMessageBox::Ok);
        return 0;
    }

    QTableWidget* table = ui.analysisResultTableWidget;/*yhj*/
    QString fileName = QFileDialog::getSaveFileName(table, QStringLiteral("保存文件"),
        "D:\LogAnalysisResult.xlsx", QStringLiteral("Excel 文件(*.xls *.xlsx)"));

    if (!fileName.isEmpty())
    {
        QAxObject* excel = new QAxObject;
        if(excel->setControl("Excel.Application"))/**************连接Excel控件**************/
        {
            excel->dynamicCall("SetVisible(bool Visible)",false);/*不显示窗体*/
            excel->setProperty("DisplayAlers",false);/*不显示警告*/

            QAxObject* workbooks = excel->querySubObject("WorkBooks");/*获取工作簿集合*/
            workbooks->dynamicCall("Add");/*新建一个工作簿*/

            QAxObject* workbook = excel->querySubObject("ActiveWorkBook");/*获取当前工作簿*/

            QAxObject* worksheet = workbook->querySubObject("Worksheets(int)",1);


            int i,j;
            int colcount=table->columnCount();

            QAxObject *cell,*col;
            /*标题行*/
            cell=worksheet->querySubObject("Cells(int,int)",1,1);
            cell->dynamicCall("SetValue(const QString&)",QStringLiteral("日志分析"));
            cell->querySubObject("Font")->setProperty("Size",18);/*调整行高*/
            worksheet->querySubObject("Range(const QString&)","1:1")->setProperty("RowHeight",30);/*合并标题行*/
#if 1
            QString cellTitle;
            cellTitle.append("A1:");
            cellTitle.append(QChar(colcount-1+'A'));
            cellTitle.append(QString::number(1));

            QAxObject* range = worksheet->querySubObject("Range(const QString&)",cellTitle);
            range->setProperty("WrapText",true);
            range->setProperty("MergeCells",true);
            range->setProperty("HorizontalAlignment",-4108);//xlCenter
            range->setProperty("VerticalAlignment",-4108);//xlCenter
#endif
            /*列标题*/
            for(i=0;i<colcount;i++)
            {
                QString columnName;
                columnName.append(QChar(i+'A'));
                columnName.append(":");
                columnName.append(QChar(i+'A'));
                col = worksheet->querySubObject("Columns(const QString&)",columnName);
                col->setProperty("ColumnWidth",table->columnWidth(i)/6);
                cell=worksheet->querySubObject("Cells(int,int)",2,i+1);
                columnName= table->horizontalHeaderItem(i)->text();
                cell->dynamicCall("SetValue(const QString&)",columnName);
                cell->querySubObject("Font")->setProperty("Bold",true);/*调整*/
                cell->querySubObject("Interior")->setProperty("Color",QColor(191,191,191));
                cell->setProperty("HorizontalAlignment",-4108);//xlCenter
                cell->setProperty("VerticalAlignment",-4108);//xlCenter
            }


            ui.analysisProgressBar->setRange(0, table->rowCount()-1);/*output bar...test*/

            /*数据区*/
            for(i=0; i<table->rowCount(); i++)
            {
                for(j=0; j<table->columnCount(); j++)
                {
                    worksheet->querySubObject("Cells(int,int)",i+3,j+1)->dynamicCall("SetValue(const QString&)",table->item(i,j)?table->item(i,j)->text():"");
                }

                ui.analysisProgressBar->setValue(i);/*output bar...test*/
            }
#if 1
            /*画框线*/
            QString lrange;
            lrange.append("A2:");
            lrange.append(colcount-1+'A');
            lrange.append(QString::number(table->rowCount()+2));
            range = worksheet->querySubObject("Range(const QString&)",lrange);
            range->querySubObject("Borders")->setProperty("LineStyle",QString::number(1));
            range->querySubObject("Borders")->setProperty("Color",QColor(0,0,0));

            /*调整数据区行高*/
            QString rowsName;
            rowsName.append("2:");
            rowsName.append(QString::number(table->rowCount()+2));
            range = worksheet->querySubObject("Range(const QString&)",rowsName);
            range->setProperty("RowHeight",20);
#endif
            workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(fileName));/*保存至fileName*/
            workbook->dynamicCall("Close()");

            /*释放Excel*/
            if(excel != NULL){
                excel->dynamicCall("Quit()");
                delete excel;
                excel = NULL;
            }

            if(QMessageBox::question(NULL,QStringLiteral("完成"),QStringLiteral("文件已经导出,是否现在打开?"),QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
            {
                //QMessageBox::warning(this, QStringLiteral("消息提示"), QString("%1").arg(table->rowCount()), QMessageBox::Ok);/*yhj 4test*/
                QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(fileName)));
            }
        }
        else{
            QMessageBox::warning(NULL,"错误","未能创建Excel对象,请安装Microsoft Excel.",QMessageBox::Apply);
        }
    }

    return 1;
}


void hello::ClearTabel()
{
        for (int row = loginfos.count(); row >= 0; row--){
            ui.analysisResultTableWidget->removeRow(row);/*一行*/
        }
}

unsigned short hello::byteToShort(const unsigned char a, const unsigned char b)
{
	unsigned short c;
	c = (short)(a<<8)|(b);
	//printf("byteToShort:%x,%x,%x\n",a,b,c);
	return c;
}

int hello::byteToInt(const unsigned char a, const unsigned char b, const unsigned char c, const unsigned char d)
{
	int e;
	e = (int)(a<<24)|(b<<16)|(c<<8)|(d);
	return e;
}

char* hello::intToString(int propertytime)
{
	char *timeString = NULL;
	int hours = 0;
	int minutes = 0;
	float seconds = 0;
	timeString = (char *)malloc(20*sizeof(char));
	hours = (propertytime/1000)/3600;
	minutes = (propertytime/1000)%3600/60;
	seconds = (float)(propertytime -hours*3600000 -minutes*60000)/1000;
	sprintf(timeString,"%2d时%2d分%2.3f秒",hours,minutes,seconds);
	return timeString;
}	

void hello::findNext(const QString &str, Qt::CaseSensitivity cs)
{
    if(true == analysis_completed)
    {
        int row = ui.analysisResultTableWidget->currentRow();
        int column = ui.analysisResultTableWidget->currentColumn() + 1;

        while (row < ui.analysisResultTableWidget->rowCount()) {
            while (column < 5) {
                if (ui.analysisResultTableWidget->item(row, column)->text().contains(str, cs)) {
                    ui.analysisResultTableWidget->clearSelection();
                    ui.analysisResultTableWidget->setCurrentCell(row, column);
                    activateWindow();
                    return;
                }
                ++column;
            }
            column = 0;
            ++row;
        }
        QApplication::beep();
    }else{
        QMessageBox::warning(this, QStringLiteral("消息提示"), QStringLiteral("请先分析日志文件.\n"), QMessageBox::Ok);
    }
}

void hello::findPrevious(const QString &str, Qt::CaseSensitivity cs)
{
    if(true == analysis_completed)
    {
        int row = ui.analysisResultTableWidget->currentRow();
        int column = ui.analysisResultTableWidget->currentColumn() - 1;

        while (row >= 0) {
            while (column >= 0) {
                if (ui.analysisResultTableWidget->item(row, column)->text().contains(str, cs)) {
                    ui.analysisResultTableWidget->clearSelection();
                    ui.analysisResultTableWidget->setCurrentCell(row, column);
                    activateWindow();
                    return;
                }
                --column;
            }
            column = 5 - 1;
            --row;
        }
        QApplication::beep();
    }else{
        QMessageBox::warning(this, QStringLiteral("消息提示"), QStringLiteral("请先分析日志文件.\n"), QMessageBox::Ok);
    }
}

void hello::createAction()
{
    /*****/
    ui.actionExportToTxt->setIcon(QIcon(":/images/save.png"));
    ui.actionExportToTxt->setShortcut(tr("Ctrl+S"));
    ui.actionExportToTxt->setStatusTip(tr("Save to TXT file"));
    connect(ui.actionExportToTxt, SIGNAL(triggered()),this,SLOT(saveToTxt()));

    ui.actionExportToExcel->setIcon(QIcon(":/images/icon.png"));
    ui.actionExportToExcel->setShortcut(tr("Ctrl+D"));
    ui.actionExportToExcel->setStatusTip(tr("Save to EXCEL file"));
    connect(ui.actionExportToExcel, SIGNAL(triggered()),this,SLOT(saveAs()));

    ui.actionQuit->setIcon(QIcon(":/images/gotocell.png"));
    ui.actionQuit->setShortcut(tr("Ctrl+Q"));
    ui.actionQuit->setStatusTip(tr("Quit App"));
    connect(ui.actionQuit, SIGNAL(triggered()),this,SLOT(close()));

    /*****/
    ui.actionFind->setIcon(QIcon(":/images/find.png"));
    ui.actionFind->setShortcut(QKeySequence::Find);
    ui.actionFind->setStatusTip(tr("Find a matching cell"));/*not functional*/
    connect(ui.actionFind, SIGNAL(triggered()),this,SLOT(find()));/****/

    ui.actionHelp->setIcon(QIcon(":/images/q.png"));
    ui.actionHelp->setShortcut(tr("Ctrl+H"));
    ui.actionHelp->setStatusTip(tr("Help"));
    connect(ui.actionHelp, SIGNAL(triggered()),this,SLOT(about()));

    //connect(ui.actionImportExcel, SIGNAL(triggered()),this,SLOT(reserved()));/****/

}

void hello::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    //fileToolBar->addAction(ui.actionQuit);
    fileToolBar->addAction(ui.actionExportToTxt);
    fileToolBar->addAction(ui.actionExportToExcel);

    editToolBar = addToolBar(tr("&Edit"));
    editToolBar->addAction(ui.actionFind);
    editToolBar->addSeparator();
    editToolBar->addAction(ui.actionHelp);
}

void hello::createStatusBar()
{
    locationLabel = new QLabel(" W999 ");
    locationLabel->setAlignment(Qt::AlignHCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());

    formulaLabel = new QLabel;
    formulaLabel->setIndent(3);

    statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(formulaLabel, 1);

    connect(this, SIGNAL(currentCellChanged(int, int, int, int)),
            this, SLOT(updateStatusBar()));
    connect(this, SIGNAL(modified()),
            this, SLOT(spreadsheetModified()));

    updateStatusBar();
}


void hello::updateStatusBar()
{
    locationLabel->setText(QString::fromLocal8Bit(""));
    formulaLabel->setText(QString::fromLocal8Bit(""));
}

void hello::spreadsheetModified()
{
    setWindowModified(true);
    updateStatusBar();
}
