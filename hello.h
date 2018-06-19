/*************************************************
 *文件名：hello.h
 *描述：主界面类头文件，定义开启主界面类。
*************************************************/
#ifndef HELLO_H
#define HELLO_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QFileDialog>
#include <QToolBar>
#include <QLabel>
#include <QStatusBar>
#include "analysislog.h"
#include "domparser.h"

class FindDialog;

/***************************/
typedef struct {
	unsigned short propertyvalue1;				/*属性值1，标识属性类型*/
	unsigned short propertyvalue2;				/*属性值2，标识属性发生的次数*/
	unsigned int propertytime;				/*属性发生时的时间*/
}st_logproperty;

typedef struct {
	unsigned short year;					/*年*/
	unsigned short month	:8;				/*月*/	
	unsigned short day	:8;				/*日*/
	unsigned short us40_l;					/*40us的低字(时分秒)*/
	unsigned short us40_h;				/*40us的高字(时分秒)*/
}st_logtime;

typedef struct {
	st_logproperty property;
	st_logtime avtime;
	st_logtime rtctime;
}LOGMSG;//24


class hello : public QMainWindow
{
	Q_OBJECT
public:
	hello(QWidget *parent = 0);
	~hello();

private slots:	
    int openLogfile();
	int analysis();
    int openCfgfile();
    bool parseCfgfile();

	bool saveFile(const QString &fileName);
    int saveAs();/*save to excel slot*/
    bool saveToTxt();/*save to txt slot*/
	void about();
    void reserved();
    void find();
    void findNext(const QString &str, Qt::CaseSensitivity cs);
    void findPrevious(const QString &str, Qt::CaseSensitivity cs);

signals:
    void modified();

private:
	bool okToContinue();

	int readLogFile(char *filename, int currentreadnum);
	unsigned short byteToShort(const unsigned char a, const unsigned char b);
	int byteToInt(const unsigned char a, const unsigned char b, const unsigned char c, const unsigned char d);
	char* intToString(int propertytime);
    int saveFileToTxt(char *inputfilename, char *outputfilename, int currentreadnum);/*save to txt really*/
    void ClearTabel();
    QString logFileName;
    QString cfgFileName;

    /**/
	QList<LogInfo> loginfos;//列表信息
	unsigned char file_test[25];
    LOGMSG logmsg;

    /**/
    DomParser *parser;
    bool parseOK;
    bool analysis_completed;

    FindDialog *findDialog;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    void createAction();
    void createToolBars();

    QLabel *locationLabel;
    QLabel *formulaLabel;
    void createStatusBar();
    void updateStatusBar();
    void spreadsheetModified();
};

#endif // HELLO_H
