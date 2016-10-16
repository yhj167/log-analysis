/*************************************************
 *�ļ�����hello.cpp
 *�������������࣬���������棬�������������ࡣ
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
        QStringList() << QStringLiteral("��־����") << QStringLiteral("�����Ĵ���")
                << QStringLiteral("������ʱ��") << QStringLiteral("��ǰ����ʱ��") << QStringLiteral("��ǰCCUʱ��"));

    createAction();
    createToolBars();
    createStatusBar();

    connect(ui.browseLogFile,SIGNAL(clicked()),this,SLOT(openLogfile()));
    connect(ui.analysisLogFile, SIGNAL(clicked()),this,SLOT(analysis()));
    connect(ui.browseCfgFile,SIGNAL(clicked()),this,SLOT(openCfgfile()));/****/
    connect(ui.parseCfgFile,SIGNAL(clicked()),this,SLOT(parseCfgfile()));/****/

    setWindowTitle(QStringLiteral("��־��������"));
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
    QMessageBox::warning(this, QStringLiteral("��Ϣ��ʾ"), QStringLiteral("����ά����...\n"), QMessageBox::Ok);
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
            ui.cfgFilePath->setText(QString("%1").arg(cfgFileName));/*��·����ֵ��logFilePath��ʾ��Ϊʲô��%1��*/
    }
    return 0;
}


/***************************
*����xml�ļ�
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
        QMessageBox::warning(this, QStringLiteral("�����ɹ�"), QStringLiteral("Parse Cfgfile Success.\n"), QMessageBox::Ok);
    else
        QMessageBox::warning(this, QStringLiteral("����ʧ��"), QStringLiteral("Parse Cfgfile Failed.\n"), QMessageBox::Ok);
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
			ui.logFilePath->setText(QString("%1").arg(logFileName));/*��·����ֵ��logFilePath��ʾ��Ϊʲô��%1��*/
	}
	return 0;
}

/***************************
*��־����
***************************/
int hello::analysis()
{	
	int current_num=0;
	int read_completed = 0;
    ClearTabel();/*���table������*/
    /*����ûʲôЧ��-_-!!!�������������ˣ���Ϊ��ʾ����ui.analysisResultTableWidget����loginfos�޹�*/
    loginfos.erase(loginfos.begin(),loginfos.end());/*���loginfos������*/
    //ui.analysisResultTableWidget->setRowCount(999);

	/*for (QList<LogInfo>::iterator iter = loginfos.begin(); iter!=loginfos.end(); iter++)
	{
		iter = loginfos.erase(iter);
	}*/

    if(parseOK == false){
        QMessageBox::warning(this, QStringLiteral("��Ϣ��ʾ"), QStringLiteral("���Ƚ��������ļ�.\n"), QMessageBox::Ok);
        return 0;
    }

    QByteArray filename = logFileName.toLocal8Bit();//Qstringתchar*
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
	/*��Ϣ��ʾ*/
    QMessageBox::warning(this, QStringLiteral("��Ϣ��ʾ"), QStringLiteral("The document has been analysised.\n"), QMessageBox::Ok);
    analysis_completed = true;


#if 0	
	//��������
	if (ui.analysisResultTableWidget->rowCount() < current_num)
		ui.analysisResultTableWidget->setRowCount(current_num);
	//����
	QList<LogInfo> loginfos;
	loginfos << LogInfo("LOG_ISRHDLC_WRITE_ERR", 50, QStringLiteral("0Сʱ59��27.000��"), QStringLiteral("2014�� 4��30�� 0Сʱ22��32.000��"), QStringLiteral("2000�� 0�� 0��  0Сʱ 0��0.000��"));
	AnalysisLog l(&loginfos);
#endif
	return 0;
}

/***************************
*��־����detail
***************************/
int hello::readLogFile(char* filename, int currentreadnum)
{
	FILE *fpRead = NULL;
	unsigned int file_len=0;
	unsigned int rd_len=0;
	//FILE *fpWrite = NULL;
	//unsigned int wr_len=0;

    //char logidx[30] = {0};//����
    QString logidx;
	//unsigned char proccesstime[15] = {0};//��������ʱ�䣨���룩
	char avTimeString[50/*35*/] = {0};//����ʱ��
	char rtcTimeString[50] = {0};//����ʱ��

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
			//printf("�ļ����ȣ�%d\n",file_len);

			//printf("����ļ�����");
			//scanf("%s",outputFile);

			ui.analysisProgressBar->setRange(0, file_len/24-1);//ÿһ��24Byte
		}

		fseek(fpRead, currentreadnum*24, SEEK_SET);
		memset(file_test,0, 25*sizeof(unsigned char));
		rd_len = fread(file_test,1,24,fpRead);

		if(rd_len == 0){	
			//��������
			if (ui.analysisResultTableWidget->rowCount() < currentreadnum)
				ui.analysisResultTableWidget->setRowCount(currentreadnum);
			//����
			AnalysisLog l(&loginfos);
			return 1;
		}
		//printf("�ļ����ݣ�%s\n",file_test);

	}else{
		//printf("can not open file %s!\n",filename);
		return 1;
	}
	fclose(fpRead);
    fflush(fpRead);

    /**��ȡIDֵ�Ͷ�Ӧ����Ϣ**/
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

	//����
	logmsg.property.propertyvalue2 = byteToShort(file_test[2], file_test[3]);

	//������
	logmsg.property.propertytime = byteToInt(file_test[4], file_test[5], file_test[6], file_test[7]);

	//����ʱ��
	logmsg.avtime.year = byteToShort(file_test[8], file_test[9]);
	logmsg.avtime.month = file_test[10];
	logmsg.avtime.day = file_test[11];
	logmsg.avtime.us40_l = byteToShort(file_test[12], file_test[13]);
	logmsg.avtime.us40_h = byteToShort(file_test[14], file_test[15]);
	avTime = (int)((logmsg.avtime.us40_h<<16)|logmsg.avtime.us40_l);
	sprintf( avTimeString, "%4d��%2d��%2d��%16s", 2000+logmsg.avtime.year, 
		logmsg.avtime.month, logmsg.avtime.day, intToString(avTime/1000));

	//����ʱ��
	logmsg.rtctime.year = byteToShort(file_test[16], file_test[17]);
	logmsg.rtctime.month = file_test[18];
	logmsg.rtctime.day = file_test[19];
	logmsg.rtctime.us40_l = byteToShort(file_test[20], file_test[21]);
	logmsg.rtctime.us40_h = byteToShort(file_test[22], file_test[23]);
	rtcTime = (int)((logmsg.rtctime.us40_h<<16)|logmsg.rtctime.us40_l);
	sprintf( rtcTimeString, "%4d��%2d��%2d��%16s", 2000+logmsg.rtctime.year, 
		logmsg.rtctime.month, logmsg.rtctime.day, intToString(rtcTime/1000));

	//д��
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


/*=======================================================�������=============================================================*/
/*save to txt slot*/
bool hello::saveToTxt()
{
    if(false == parseOK){
        QMessageBox::warning(this, QStringLiteral("��Ϣ��ʾ"), QStringLiteral("���Ƚ��������ļ�.\n"), QMessageBox::Ok);
        return 0;
    }
    if(false == analysis_completed){
        QMessageBox::warning(this, QStringLiteral("��Ϣ��ʾ"), QStringLiteral("���ȷ�����־�ļ�.\n"), QMessageBox::Ok);
        return 0;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
        QStringLiteral("�����ļ�"),"D:\LogAnalysisResult.txt",
        QStringLiteral("Txt �ļ�(*.txt)"));

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

    QByteArray outputLogFileName = fileName.toLocal8Bit();//Qstringתchar*
    QByteArray inputLogFileName = logFileName.toLocal8Bit();//Qstringתchar*

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
    /*��Ϣ��ʾ*/
    //QMessageBox::warning(this, QStringLiteral("��Ϣ��ʾ"), QStringLiteral("The document has been saved.\n"), QMessageBox::Ok);
    if(QMessageBox::question(NULL,QStringLiteral("���"),QStringLiteral("�ļ��Ѿ�����,�Ƿ����ڴ�?"),QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
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

    //char logidx[30] = {0};//����
    QString logidx;
    QByteArray logidxArray;//QString to char*
    //unsigned char proccesstime[15] = {0};//��������ʱ�䣨���룩
    char avTimeString[35] = {0};//����ʱ��
    char rtcTimeString[35] = {0};//����ʱ��
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
            //printf("�ļ����ȣ�%d\n",file_len);

            //printf("����ļ�����");
            //scanf("%s",outputFile);

            ui.analysisProgressBar->setRange(0, file_len/24-1);//ÿһ��24Byte
        }

        fseek(fpRead, currentreadnum*24, SEEK_SET);
        memset(file_test,0, 25*sizeof(unsigned char));
        rd_len = fread(file_test,1,24,fpRead);

        if(rd_len == 0){
            return 1;
        }
        //printf("�ļ����ݣ�%s\n",file_test);

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
            fprintf(fpWrite, "|          ��־����               |  ����   |      ������ʱ��     |          ��ǰ����ʱ��             |           ��ǰCCUʱ��             |\n");
            fprintf(fpWrite, "| - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
        }

        /*����parser�б���Ҷ�Ӧ�ı��*/
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

        logmsg.property.propertyvalue2 = byteToShort(file_test[2], file_test[3]);//����
        logmsg.property.propertytime = byteToInt(file_test[4], file_test[5], file_test[6], file_test[7]);//������

        //����ʱ��
        logmsg.avtime.year = byteToShort(file_test[8], file_test[9]);
        logmsg.avtime.month = file_test[10];
        logmsg.avtime.day = file_test[11];
        logmsg.avtime.us40_l = byteToShort(file_test[12], file_test[13]);
        logmsg.avtime.us40_h = byteToShort(file_test[14], file_test[15]);

        avTime = (int)((logmsg.avtime.us40_h<<16)|logmsg.avtime.us40_l);
        sprintf( avTimeString, "%4d��%2d��%2d��%18s", 2000+logmsg.avtime.year,
            logmsg.avtime.month, logmsg.avtime.day, intToString(avTime/1000));

        //����ʱ��
        logmsg.rtctime.year = byteToShort(file_test[16], file_test[17]);
        logmsg.rtctime.month = file_test[18];
        logmsg.rtctime.day = file_test[19];
        logmsg.rtctime.us40_l = byteToShort(file_test[20], file_test[21]);
        logmsg.rtctime.us40_h = byteToShort(file_test[22], file_test[23]);

        rtcTime = (int)((logmsg.rtctime.us40_h<<16)|logmsg.rtctime.us40_l);
        sprintf( rtcTimeString, "%4d��%2d��%2d��%18s", 2000+logmsg.rtctime.year,
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
**���ܣ�
**���浽excel SLOT
***************************/
int hello::saveAs()
{
    if(parseOK == false){
        QMessageBox::warning(this, QStringLiteral("��Ϣ��ʾ"), QStringLiteral("���Ƚ��������ļ�.\n"), QMessageBox::Ok);
        return 0;
    }
    if(false == analysis_completed){
        QMessageBox::warning(this, QStringLiteral("��Ϣ��ʾ"), QStringLiteral("���ȷ�����־�ļ�.\n"), QMessageBox::Ok);
        return 0;
    }

    QTableWidget* table = ui.analysisResultTableWidget;/*yhj*/
    QString fileName = QFileDialog::getSaveFileName(table, QStringLiteral("�����ļ�"),
        "D:\LogAnalysisResult.xlsx", QStringLiteral("Excel �ļ�(*.xls *.xlsx)"));

    if (!fileName.isEmpty())
    {
        QAxObject* excel = new QAxObject;
        if(excel->setControl("Excel.Application"))/**************����Excel�ؼ�**************/
        {
            excel->dynamicCall("SetVisible(bool Visible)",false);/*����ʾ����*/
            excel->setProperty("DisplayAlers",false);/*����ʾ����*/

            QAxObject* workbooks = excel->querySubObject("WorkBooks");/*��ȡ����������*/
            workbooks->dynamicCall("Add");/*�½�һ��������*/

            QAxObject* workbook = excel->querySubObject("ActiveWorkBook");/*��ȡ��ǰ������*/

            QAxObject* worksheet = workbook->querySubObject("Worksheets(int)",1);


            int i,j;
            int colcount=table->columnCount();

            QAxObject *cell,*col;
            /*������*/
            cell=worksheet->querySubObject("Cells(int,int)",1,1);
            cell->dynamicCall("SetValue(const QString&)",QStringLiteral("��־����"));
            cell->querySubObject("Font")->setProperty("Size",18);/*�����и�*/
            worksheet->querySubObject("Range(const QString&)","1:1")->setProperty("RowHeight",30);/*�ϲ�������*/
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
            /*�б���*/
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
                cell->querySubObject("Font")->setProperty("Bold",true);/*����*/
                cell->querySubObject("Interior")->setProperty("Color",QColor(191,191,191));
                cell->setProperty("HorizontalAlignment",-4108);//xlCenter
                cell->setProperty("VerticalAlignment",-4108);//xlCenter
            }


            ui.analysisProgressBar->setRange(0, table->rowCount()-1);/*output bar...test*/

            /*������*/
            for(i=0; i<table->rowCount(); i++)
            {
                for(j=0; j<table->columnCount(); j++)
                {
                    worksheet->querySubObject("Cells(int,int)",i+3,j+1)->dynamicCall("SetValue(const QString&)",table->item(i,j)?table->item(i,j)->text():"");
                }

                ui.analysisProgressBar->setValue(i);/*output bar...test*/
            }
#if 1
            /*������*/
            QString lrange;
            lrange.append("A2:");
            lrange.append(colcount-1+'A');
            lrange.append(QString::number(table->rowCount()+2));
            range = worksheet->querySubObject("Range(const QString&)",lrange);
            range->querySubObject("Borders")->setProperty("LineStyle",QString::number(1));
            range->querySubObject("Borders")->setProperty("Color",QColor(0,0,0));

            /*�����������и�*/
            QString rowsName;
            rowsName.append("2:");
            rowsName.append(QString::number(table->rowCount()+2));
            range = worksheet->querySubObject("Range(const QString&)",rowsName);
            range->setProperty("RowHeight",20);
#endif
            workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(fileName));/*������fileName*/
            workbook->dynamicCall("Close()");

            /*�ͷ�Excel*/
            if(excel != NULL){
                excel->dynamicCall("Quit()");
                delete excel;
                excel = NULL;
            }

            if(QMessageBox::question(NULL,QStringLiteral("���"),QStringLiteral("�ļ��Ѿ�����,�Ƿ����ڴ�?"),QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
            {
                //QMessageBox::warning(this, QStringLiteral("��Ϣ��ʾ"), QString("%1").arg(table->rowCount()), QMessageBox::Ok);/*yhj 4test*/
                QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(fileName)));
            }
        }
        else{
            QMessageBox::warning(NULL,"����","δ�ܴ���Excel����,�밲װMicrosoft Excel.",QMessageBox::Apply);
        }
    }

    return 1;
}


void hello::ClearTabel()
{
        for (int row = loginfos.count(); row >= 0; row--){
            ui.analysisResultTableWidget->removeRow(row);/*һ��*/
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
	sprintf(timeString,"%2dʱ%2d��%2.3f��",hours,minutes,seconds);
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
        QMessageBox::warning(this, QStringLiteral("��Ϣ��ʾ"), QStringLiteral("���ȷ�����־�ļ�.\n"), QMessageBox::Ok);
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
        QMessageBox::warning(this, QStringLiteral("��Ϣ��ʾ"), QStringLiteral("���ȷ�����־�ļ�.\n"), QMessageBox::Ok);
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
