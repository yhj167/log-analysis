/*************************************************
 *文件名：analysislog.h
 *描述：日志分析结果头文件，定义日志分析结果类的内容。
*************************************************/
#ifndef ANALYSISLOG_H
#define ANALYSISLOG_H

#include <QDialog>
#include <QList>

class QTableWidget;

class LogInfo{
public:
	LogInfo(const QString &logIdx = "", unsigned int propertyvalue2 = 0, const QString &timeString = "", const QString &avTimeString = "", const QString &rtcTimeString = "");

	QString logIdx;
	int propertyvalue2;
	QString timeString;
	QString avTimeString;
	QString rtcTimeString;
};

class AnalysisLog : public QDialog
{
	Q_OBJECT

public:
	AnalysisLog(QList<LogInfo>*loginfos, QWidget *parent=0);
	
	void done(int result);
    //void Table2Excel(QTableWidget* table, QString title);


/*public slots:
    void findNext(const QString &str, Qt::CaseSensitivity cs);
    void findPrevious(const QString &str, Qt::CaseSensitivity cs);
    */

private:
	//QTableWidget *tableWidget;
    QList<LogInfo> *loginfos;
};


#endif // ANALYSISLOG_H
