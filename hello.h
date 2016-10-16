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
#if 1
#define POWER_ON					0x0001		   	/*VCM上电*/
#define RESET						0x0002         	/*VCM复位*/
#define IDR_INSERT					0x0003     	/*DTC从IDR弹出再次接入IDR*/
#define BUFFER_OVERFLOW				0x0004   	/*缓冲区溢出*/
#define DATA_WRITE_START			0x0005   	/*数据开始写入*/
#define DATA_WRITE_STOP				0x0006		/*数据停止写入*/
#define IEEE1394_BUSRESET			0x0007		/*1394通道复位*/
#define DATA_WRITE_FAULT			0X0008		/*数据写入DTC失败*/
#define LOG_DSP1_SEMTAKE_ERROR			0X0009	/*vTransBS[0]任务等待semIntrId[0]信号量超时*/
#define LOG_DSP2_SEMTAKE_ERROR			0X000A	/*vTransBS[1]任务等待semIntrId[1]信号量超时*/
#define LOG_DSP3_SEMTAKE_ERROR			0X000B	/*vTransBS[2]任务等待semIntrId[2]信号量超时*/
#define LOG_DSP4_SEMTAKE_ERROR			0X000C	/*vTransBS[3]任务等待semIntrId[3]信号量超时*/
#define LOG_DSP5_SEMTAKE_ERROR			0X000D	/*vTransBS[4]任务等待semIntrId[4]信号量超时*/
#define LOG_TASK_SYSCTRL_SUSPEND		0X000E  /*vcm_idr_comm任务挂起*/
#define LOG_TASK_BMFETCH_SUSPEND		0X000F  /*vcm_mbi_comm任务挂起*/
#define LOG_TASK_DATAFETCH_SUSPEND	0X0010	/*vcm_mcu_comm任务挂起*/
#define LOG_TASK_ISRSEND_SUSPEND		0X0011  /*不用*/
#define LOG_TASK_IDRSEND_SUSPEND		0X0012	/*vcm_dataTx任务挂起*/
#define LOG_TASK_TRANSBS0_SUSPEND		0X0013	/*vTransBS[0]任务挂起*/
#define LOG_TASK_TRANSBS1_SUSPEND		0X0014  /*vTransBS[1]任务挂起*/
#define LOG_TASK_TRANSBS2_SUSPEND		0X0015  /*vTransBS[2]任务挂起*/
#define LOG_TASK_TRANSBS3_SUSPEND		0X0016  /*vTransBS[3]任务挂起*/
#define LOG_TASK_TRANSBS4_SUSPEND		0X0017  /*vTransBS[4]任务挂起*/
#define LOG_CMD_START_VIDEO_RECORD	0X0018	/*收到启动视频记录命令*/
#define LOG_CMD_STOP_VIDEO_RECORD		0X0019  /*收到停止视频记录命令*/
#define LOG_EXECUTE_CMD_START_RECORD	0X001A	/*执行启动视频记录命令*/
#define LOG_EXECUTE_CMD_STOP_RECORD		0X001B	/*执行停止视频记录命令*/
#define LOG_TASK_DSPBIT_SUSPEND			0X001C	/*vcm_dspbit任务挂起*/
#define LOG_TASK_MBI2BIT_SUSPEND			0X001D	/*vcm_mbi2bit任务挂起*/
#define LOG_POWER_EXCHANGE				0x001E				/*不用*/
#define LOG_LMFD_BUFFER_OVERFLOW			0x001f		/*存放LMFD视频数据缓冲区空间不足*/
#define LOG_CMFD_BUFFER_OVERFLOW			0x0020		/*存放CMFD视频数据缓冲区空间不足*/
#define LOG_RMFD_BUFFER_OVERFLOW			0x0021		/*存放RMFD视频数据缓冲区空间不足*/
#define LOG_HMD_BUFFER_OVERFLOW				0x0022		/*存放HMD视频数据缓冲区空间不足*/
#define LOG_UFCP_BUFFER_OVERFLOW			0x0023		/*存放UFCP视频数据缓冲区空间不足*/
#define LOG_MMP_BUFFER_OVERFLOW			0x0024			/*存放MMP视频数据缓冲区空间不足*/
#define LOG_AUDIO_BUFFER_OVERFLOW		0x0025			/*存放音频数据缓冲区空间不足*/
#define LOG_ABM_BUFFER_OVERFLOW			0x0026 			/*存放第0路GJB289A总线监控数据缓冲区空间不足*/
#define LOG_BBM_BUFFER_OVERFLOW			0x0027 			/*存放第1路GJB289A总线监控数据缓冲区空间不足*/
#define LOG_CBM_BUFFER_OVERFLOW			0x0028 			/*存放第2路GJB289A总线监控数据缓冲区空间不足*/
#define LOG_FLY_BUFFER_OVERFLOW			0x0029			/*存放飞参数据缓冲区空间不足*/
#define LOG_BUS_BUFFER_OVERFLOW			0x002A			/*存放武器链卸载数据缓冲区空间不足*/
#define LOG_EW_BUFFER_OVERFLOW			0x002B			/*存放电子战卸载数据缓冲区空间不足*/
#define LOG_MFL_BUFFER_OVERFLOW			0x002C 			/*存放武器系统数据缓冲区空间不足*/
#define LOG_ISRMMP_BUFFER_OVERFLOW		0x002D		/*存放向CCU发送的平显视频数据缓冲区空间不足*/
#define LOG_FLY_DUALPORT_RAM_NODATA		0x002E		/*超时5次没有收到飞参数据*/
#define LOG_ISRHDLC_WRITE_ERR				0x002F			/*与CCU通讯故障*/
#define LOG_ISRHDLC_ERR_RECOVER			0x0030			/*与CCU通讯故障恢复正常*/
#define LOG_HDLC_SNIFFER_ONLINE			0x0031			/*不用*/
#define LOG_HDLC_SNIFFER_OFFLINE			0x0032 		/*不用*/
#define LOG_ABM_DATA_ERR					0x0033				/*不用*/
#define LOG_BBM_DATA_ERR					0x0034				/*不用*/
#define LOG_CBM_DATA_ERR					0x0035				/*不用*/
#define LOG_ABM_NODATA					0x0036		/*第0路GJB289A总线监控数据超过3分钟无数据*/
#define LOG_BBM_NODATA					0x0037		/*第1路GJB289A总线监控数据超过3分钟无数据*/
#define LOG_CBM_NODATA					0x0038		/*第2路GJB289A总线监控数据超过3分钟无数据*/
#define LOG_DSP1_ERR						0x0039		/*第一颗DSP故障，根据PCI识别顺序*/
#define LOG_DSP2_ERR						0x003A		/*第一颗DSP故障，根据PCI识别顺序*/
#define LOG_DSP3_ERR						0x003B		/*第一颗DSP故障，根据PCI识别顺序*/
#define LOG_DSP4_ERR						0x003C		/*第一颗DSP故障，根据PCI识别顺序*/
#define LOG_DSP5_ERR						0x003D		/*第一颗DSP故障，根据PCI识别顺序*/
#define LOG_TASK_DSPBIT1_SUSPEND		0x003E
#define LOG_TASK_DSPBIT2_SUSPEND		0x003F
#define LOG_MCU_BIN_VALIDITY     0x0040		/*不用*/
#define LOG_VCM_BIN_VALIDITY     0x0041		/*不用*/
#define LOG_MCU_PORT_VALUE				0x0042	/*不用*/
#define LOG_VCM_PORT_VALUE				0x0043	/*不用*/
#define LOG_FOLDER_TIME_FROM_CCU				0x0044	/*文件夹从CCU获取时间,Add by yhj 20140704*/
#define LOG_FOLDER_TIME_FROM_MBI				0x0045	/*文件夹从MBI获取时间*/
#define LOG_FILE_TIME_FROM_CCU				0x0046	/*文件从CCU获取时间*/
#define LOG_FILE_TIME_FROM_MBI				0x0047	/*文件从MBI获取时间*/
#endif

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
