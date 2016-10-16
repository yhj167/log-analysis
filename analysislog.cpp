/*************************************************
 *文件名：analysislog.cpp
 *描述：日志分析结果文件，存储日志分析结果并插入到表格中。
*************************************************/
#include <QtGui>
#include <QTableWidget>
#include "ui_hello.h"
#include "hello.h"
#include "analysislog.h"

extern Ui::helloClass ui;

LogInfo::LogInfo(const QString &logIdx /* = "" */, unsigned int propertyvalue2 /* = 0 */, const QString &timeString /* = "" */, const QString &avTimeString /* = "" */, const QString &rtcTimeString /* = "" */)
{
	this->logIdx = logIdx;
	this->propertyvalue2 = propertyvalue2;
	this->timeString = timeString;
	this->avTimeString = avTimeString;
	this->rtcTimeString = rtcTimeString;
}

AnalysisLog::AnalysisLog(QList<LogInfo>*loginfos, QWidget *parent)
	: QDialog(parent)
{
	this->loginfos = loginfos;

	for (int row = 0; row < loginfos->count(); ++row)
	{
		LogInfo loginfo = loginfos->at(row);

		QTableWidgetItem *item0 = new QTableWidgetItem(loginfo.logIdx);
		//item0->setTextAlignment(Qt::AlignLeft);/*左对齐*/
		ui.analysisResultTableWidget->setItem(row, 0, item0);

		QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(loginfo.propertyvalue2));
		item1->setTextAlignment(Qt::AlignCenter);/**/
		ui.analysisResultTableWidget->setItem(row, 1, item1);

		QTableWidgetItem *item2 = new QTableWidgetItem(loginfo.timeString);
		//item2->setTextAlignment(Qt::AlignLeft);/*左对齐*/
		ui.analysisResultTableWidget->setItem(row, 2, item2);

		QTableWidgetItem *item3 = new QTableWidgetItem(loginfo.avTimeString);
		//item3->setTextAlignment(Qt::AlignLeft);/*左对齐*/
		ui.analysisResultTableWidget->setItem(row, 3, item3);

		QTableWidgetItem *item4 = new QTableWidgetItem(loginfo.rtcTimeString);
		//item4->setTextAlignment(Qt::AlignLeft);/*左对齐*/
		ui.analysisResultTableWidget->setItem(row, 4, item4);
	}

	ui.analysisResultTableWidget->resizeColumnToContents(0);/*表格适应内容*/
	ui.analysisResultTableWidget->resizeColumnToContents(2);/*表格适应内容*/
	ui.analysisResultTableWidget->resizeColumnToContents(3);/*表格适应内容*/
	ui.analysisResultTableWidget->resizeColumnToContents(4);/*表格适应内容*/
}

void AnalysisLog::done(int result)
{
	if(result == QDialog::Accepted){
		loginfos->clear();
		for (int row = 0; row < ui.analysisResultTableWidget->rowCount(); ++row){
			QString logIdx = ui.analysisResultTableWidget->item(row, 0)->text();
			int propertyvalue2 = ui.analysisResultTableWidget->item(row, 1)->text().toInt();
			QString timeString = ui.analysisResultTableWidget->item(row, 2)->text();
			QString avTimeString = ui.analysisResultTableWidget->item(row, 3)->text();
			QString rtcTimeString = ui.analysisResultTableWidget->item(row, 4)->text();
			loginfos->append( LogInfo(logIdx, propertyvalue2, timeString, avTimeString, rtcTimeString) );
		}
	}
	QDialog::done(result);
}


