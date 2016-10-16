/*************************************************
 *�ļ�����hello.h
 *��������������ͷ�ļ������忪���������ࡣ
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
#define POWER_ON					0x0001		   	/*VCM�ϵ�*/
#define RESET						0x0002         	/*VCM��λ*/
#define IDR_INSERT					0x0003     	/*DTC��IDR�����ٴν���IDR*/
#define BUFFER_OVERFLOW				0x0004   	/*���������*/
#define DATA_WRITE_START			0x0005   	/*���ݿ�ʼд��*/
#define DATA_WRITE_STOP				0x0006		/*����ֹͣд��*/
#define IEEE1394_BUSRESET			0x0007		/*1394ͨ����λ*/
#define DATA_WRITE_FAULT			0X0008		/*����д��DTCʧ��*/
#define LOG_DSP1_SEMTAKE_ERROR			0X0009	/*vTransBS[0]����ȴ�semIntrId[0]�ź�����ʱ*/
#define LOG_DSP2_SEMTAKE_ERROR			0X000A	/*vTransBS[1]����ȴ�semIntrId[1]�ź�����ʱ*/
#define LOG_DSP3_SEMTAKE_ERROR			0X000B	/*vTransBS[2]����ȴ�semIntrId[2]�ź�����ʱ*/
#define LOG_DSP4_SEMTAKE_ERROR			0X000C	/*vTransBS[3]����ȴ�semIntrId[3]�ź�����ʱ*/
#define LOG_DSP5_SEMTAKE_ERROR			0X000D	/*vTransBS[4]����ȴ�semIntrId[4]�ź�����ʱ*/
#define LOG_TASK_SYSCTRL_SUSPEND		0X000E  /*vcm_idr_comm�������*/
#define LOG_TASK_BMFETCH_SUSPEND		0X000F  /*vcm_mbi_comm�������*/
#define LOG_TASK_DATAFETCH_SUSPEND	0X0010	/*vcm_mcu_comm�������*/
#define LOG_TASK_ISRSEND_SUSPEND		0X0011  /*����*/
#define LOG_TASK_IDRSEND_SUSPEND		0X0012	/*vcm_dataTx�������*/
#define LOG_TASK_TRANSBS0_SUSPEND		0X0013	/*vTransBS[0]�������*/
#define LOG_TASK_TRANSBS1_SUSPEND		0X0014  /*vTransBS[1]�������*/
#define LOG_TASK_TRANSBS2_SUSPEND		0X0015  /*vTransBS[2]�������*/
#define LOG_TASK_TRANSBS3_SUSPEND		0X0016  /*vTransBS[3]�������*/
#define LOG_TASK_TRANSBS4_SUSPEND		0X0017  /*vTransBS[4]�������*/
#define LOG_CMD_START_VIDEO_RECORD	0X0018	/*�յ�������Ƶ��¼����*/
#define LOG_CMD_STOP_VIDEO_RECORD		0X0019  /*�յ�ֹͣ��Ƶ��¼����*/
#define LOG_EXECUTE_CMD_START_RECORD	0X001A	/*ִ��������Ƶ��¼����*/
#define LOG_EXECUTE_CMD_STOP_RECORD		0X001B	/*ִ��ֹͣ��Ƶ��¼����*/
#define LOG_TASK_DSPBIT_SUSPEND			0X001C	/*vcm_dspbit�������*/
#define LOG_TASK_MBI2BIT_SUSPEND			0X001D	/*vcm_mbi2bit�������*/
#define LOG_POWER_EXCHANGE				0x001E				/*����*/
#define LOG_LMFD_BUFFER_OVERFLOW			0x001f		/*���LMFD��Ƶ���ݻ������ռ䲻��*/
#define LOG_CMFD_BUFFER_OVERFLOW			0x0020		/*���CMFD��Ƶ���ݻ������ռ䲻��*/
#define LOG_RMFD_BUFFER_OVERFLOW			0x0021		/*���RMFD��Ƶ���ݻ������ռ䲻��*/
#define LOG_HMD_BUFFER_OVERFLOW				0x0022		/*���HMD��Ƶ���ݻ������ռ䲻��*/
#define LOG_UFCP_BUFFER_OVERFLOW			0x0023		/*���UFCP��Ƶ���ݻ������ռ䲻��*/
#define LOG_MMP_BUFFER_OVERFLOW			0x0024			/*���MMP��Ƶ���ݻ������ռ䲻��*/
#define LOG_AUDIO_BUFFER_OVERFLOW		0x0025			/*�����Ƶ���ݻ������ռ䲻��*/
#define LOG_ABM_BUFFER_OVERFLOW			0x0026 			/*��ŵ�0·GJB289A���߼�����ݻ������ռ䲻��*/
#define LOG_BBM_BUFFER_OVERFLOW			0x0027 			/*��ŵ�1·GJB289A���߼�����ݻ������ռ䲻��*/
#define LOG_CBM_BUFFER_OVERFLOW			0x0028 			/*��ŵ�2·GJB289A���߼�����ݻ������ռ䲻��*/
#define LOG_FLY_BUFFER_OVERFLOW			0x0029			/*��ŷɲ����ݻ������ռ䲻��*/
#define LOG_BUS_BUFFER_OVERFLOW			0x002A			/*���������ж�����ݻ������ռ䲻��*/
#define LOG_EW_BUFFER_OVERFLOW			0x002B			/*��ŵ���սж�����ݻ������ռ䲻��*/
#define LOG_MFL_BUFFER_OVERFLOW			0x002C 			/*�������ϵͳ���ݻ������ռ䲻��*/
#define LOG_ISRMMP_BUFFER_OVERFLOW		0x002D		/*�����CCU���͵�ƽ����Ƶ���ݻ������ռ䲻��*/
#define LOG_FLY_DUALPORT_RAM_NODATA		0x002E		/*��ʱ5��û���յ��ɲ�����*/
#define LOG_ISRHDLC_WRITE_ERR				0x002F			/*��CCUͨѶ����*/
#define LOG_ISRHDLC_ERR_RECOVER			0x0030			/*��CCUͨѶ���ϻָ�����*/
#define LOG_HDLC_SNIFFER_ONLINE			0x0031			/*����*/
#define LOG_HDLC_SNIFFER_OFFLINE			0x0032 		/*����*/
#define LOG_ABM_DATA_ERR					0x0033				/*����*/
#define LOG_BBM_DATA_ERR					0x0034				/*����*/
#define LOG_CBM_DATA_ERR					0x0035				/*����*/
#define LOG_ABM_NODATA					0x0036		/*��0·GJB289A���߼�����ݳ���3����������*/
#define LOG_BBM_NODATA					0x0037		/*��1·GJB289A���߼�����ݳ���3����������*/
#define LOG_CBM_NODATA					0x0038		/*��2·GJB289A���߼�����ݳ���3����������*/
#define LOG_DSP1_ERR						0x0039		/*��һ��DSP���ϣ�����PCIʶ��˳��*/
#define LOG_DSP2_ERR						0x003A		/*��һ��DSP���ϣ�����PCIʶ��˳��*/
#define LOG_DSP3_ERR						0x003B		/*��һ��DSP���ϣ�����PCIʶ��˳��*/
#define LOG_DSP4_ERR						0x003C		/*��һ��DSP���ϣ�����PCIʶ��˳��*/
#define LOG_DSP5_ERR						0x003D		/*��һ��DSP���ϣ�����PCIʶ��˳��*/
#define LOG_TASK_DSPBIT1_SUSPEND		0x003E
#define LOG_TASK_DSPBIT2_SUSPEND		0x003F
#define LOG_MCU_BIN_VALIDITY     0x0040		/*����*/
#define LOG_VCM_BIN_VALIDITY     0x0041		/*����*/
#define LOG_MCU_PORT_VALUE				0x0042	/*����*/
#define LOG_VCM_PORT_VALUE				0x0043	/*����*/
#define LOG_FOLDER_TIME_FROM_CCU				0x0044	/*�ļ��д�CCU��ȡʱ��,Add by yhj 20140704*/
#define LOG_FOLDER_TIME_FROM_MBI				0x0045	/*�ļ��д�MBI��ȡʱ��*/
#define LOG_FILE_TIME_FROM_CCU				0x0046	/*�ļ���CCU��ȡʱ��*/
#define LOG_FILE_TIME_FROM_MBI				0x0047	/*�ļ���MBI��ȡʱ��*/
#endif

class FindDialog;

/***************************/
typedef struct {
	unsigned short propertyvalue1;				/*����ֵ1����ʶ��������*/
	unsigned short propertyvalue2;				/*����ֵ2����ʶ���Է����Ĵ���*/
	unsigned int propertytime;				/*���Է���ʱ��ʱ��*/
}st_logproperty;

typedef struct {
	unsigned short year;					/*��*/
	unsigned short month	:8;				/*��*/	
	unsigned short day	:8;				/*��*/
	unsigned short us40_l;					/*40us�ĵ���(ʱ����)*/
	unsigned short us40_h;				/*40us�ĸ���(ʱ����)*/
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
	QList<LogInfo> loginfos;//�б���Ϣ
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
