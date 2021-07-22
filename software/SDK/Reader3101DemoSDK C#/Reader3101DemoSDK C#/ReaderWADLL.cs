using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Threading;
using System.Data;



namespace ReaderDemo
{

    public class Reader
    {

        /// <summary>
        /// Xác định macro.
        /// Xác định tên tệp của thư viện liên kết động
        /// </summary>
        public const string PUBLIC_COM_PATH = @"ReaderDynamicLib.dll";


        //==============================Định nghĩa không đổi==============================
        public const int ID_MAX_SIZE_64BIT	=   8;		//Số ID thẻ điện tử là 64 bit
        public const int ID_ATA_MAX_SIZE	=   20;		//Số ID thẻ điện tử ATA là 20Bytete
        public const int ID_MAX_SIZE_96BIT	=   65;		//Số ID thẻ điện tử
        public const int MAX_LABELS			=   255;        // Một hoạt động đọc / ghi là 100 nhãn

        //==============================API Chức năng trả về mã==============================
        public const int _OK				=   0x00;	// Hoạt động thành công

        //通信方面出错信息
        public const int _init_rs232_err	=   0x81;	//  Khởi tạo cổng liên lạc không thành công
        public const int _no_scanner		=   0x82;	//  Không thể tìm thấy người đọc
        public const int _comm_error        =   0x83;	// Lỗi truyền dữ liệu truyền thông
        public const int _baudrate_error    =   0x84;	//  Đặt lỗi tốc độ baud
        public const int _init_net_error	=   0x85;	//  Khởi tạo cổng ròng không thành công
        public const int _net_error         =   0x86;	//  Gửi cổng NET và nhận lỗi dữ liệu liên lạc

        // 读写器返回出错信息
        public const int _no_antenna		=   0x01;   //Kết nối ăng-ten thất bại
        public const int _no_label			=   0x02;   //Không có nhãn được phát hiện
        public const int _invalid_label		=   0x03;   //Nhãn bất hợp pháp
        public const int _less_power		=   0x04;   //Đọc và viết là không đủ
        public const int _write_prot_error	=   0x05;   //Khu vực này viết sự bảo vệ
        public const int _check_sum_error	=   0x06;   //Kiểm tra và LRI.
        public const int _parameter_error	=   0x07;   //Thông số Lỗi.
        public const int _memory_error		=   0x08;   //Khu vực dữ liệu không tồn tại
        public const int _password_error	=	0x09;   //mật khẩu không đúng
        public const int _killpassword_error=	0x0a;   //Mật khẩu hủy nhãn G2 là 0
        public const int _nonlicet_command	=   0x0b;   //Trật tự bất hợp pháp
        public const int _nonlicet_user		=   0x0c;   //Những người dùng bất hợp pháp không phù hợp với mật khẩu
        public const int _invalid_command	=   0x1e;   //Cho biết một hướng dẫn không hợp lệ, chẳng hạn như tham số không chính xác
        public const int _other_error       =   0x1f;   //Lệnh không xác định

        //函数输入错误
        public const int _no_cardID_input   =    0x20;   //Lỗi khác



        /// <summary>
        /// 
        /// </summary>
        public Reader()
        {

        }


        [StructLayout(LayoutKind.Sequential)]
        public struct ReaderDate
        {
            public byte Year;			//年
#if ENABLE_ARM9_MODEL
            public byte Yeam;			//年
#endif
            public byte Month;			//月
            public byte Day;			//日
            public byte Hour;			//时
            public byte Minute;			//分
            public byte Second;			//秒
        }

        public struct ReaderBasicParam
        {
            public byte BaudRate;			//Tốc độ giao tiếp của cổng nối tiếp, giá trị: 00h ~ 08h, thể hiện tốc độ của lệnh "cài đặt tốc độ baud".
            public byte Power;				//Truyền giá trị năng lượng, giá trị: 20 ~ 30dbm.
            public byte Min_Frequence;		//Điểm bắt đầu của tần số tín hiệu vi sóng được truyền, giá trị: 0 ~ 59.
            public byte Max_Frequence;		//Điểm chấm dứt, giá trị của tần số tín hiệu vi sóng truyềnz： 0~59。
            public byte Reserve5;			//dự trữ
            public byte WorkMode;			//Chế độ làm việc đầu đọc: 0 - Phương thức hoạt động, Chế độ 1-lệnh
            public byte ReaderAddress;		//Địa chỉ RS485: 1--254
            public byte NumofCard;			//Số thẻ đọc tối đa.
            public byte TagType;			//Các loại: 01H-ISO18000-6B, 02H-EPCC1, 04H-EPCC1G2, 08H-EM4442.
            public byte ReadDuration;		//(10)Đọc thẻ thời lượng：射频发射持续时间，只针对EM标签有效；0－10ms，1－20ms，2－30ms，3－40ms。
            public byte ReadTimes;			//Đọc số thẻ M: Nhận lệnh Trình đọc máy tính máy chủ, đọc Trình viết thực thi lệnh này。
            public byte EnableBuzzer;		//1:Cho phép Buzzer 0: Không kích hoạt Buzzers
            public byte IP1;			    //Địa chỉ IP Reader
            public byte IP2;			    //
            public byte IP3;			    //
            public byte IP4;			    //
            public byte Port1;				//读写器端口高位
            public byte Port2;				//
            public byte Mask1;				//Không có thẻ!
            public byte Mask2;				//Không có thẻ!
            public byte Mask3;				//Không có thẻ!
            public byte Mask4;				//Không có thẻ!
            public byte Gateway1;			//Không có thẻ!
            public byte Gateway2;			//
            public byte Gateway3;			//
            public byte Gateway4;			//
            public byte MAC1;			    //Không có thẻ!
            public byte MAC2;			    //
            public byte MAC3;			    //
            public byte MAC4;			    //
            public byte MAC5;			    //
            public byte MAC6;			    //
            #if ENABLE_ARM9_MODEL
	        public byte AutoParam[32];
	        #endif

        }

        //Đầu đọc tham số làm việc tích cực
        public struct ReaderAutoParam
        {
            public byte AutoMode;			//读标签模式：0-定时方式，1-触发方式。
            public byte TimeH;				//标签保留时间：单位：秒s。缺省值为1。
            public byte TimeL;				//
            public byte Interval;			//0-10ms，1-20ms，2-30ms，3-50ms，4-100ms。缺省值为2。每隔设定时间主动读取一次标签。
            public byte NumH;				//标签保留数目：缺省值为1。已读取的标签ID在读写器内存中保留的数目。
            public byte NumL;				//
            public byte OutputManner;	    //数据输出格式：0-简化格式，1-标准格式，2-XML格式。缺省值为0。
            public byte OutInterface;		//输出接口：0－RS232，1－RS485，2－RJ45,3- Wiegand26,4- Wiegand34。缺省值为0。
            public byte WiegandWidth;		//Weigand脉冲宽度值。
            public byte WiegandInterval;	//Weigand脉冲间隔值。
            public byte ID_Start;			//输出卡号的起始位，取值0～4。
            public byte IDPosition;			//卡号在电子标签上的存放地址。
            public byte Report_Interval;	//通知间隔：单位秒s。缺省值为1。每隔设定时间主动通知上位机一次。
            public byte Report_Condition;	//通知条件：缺省值为1。0-被动通知，1-定时通知，2-增加新标签，3-减少标签，4-标签数变化	
            public byte Report_Output;		//通知输出端
            public byte Antenna;			//天线选择。1-ant1,2-ant2,4-ant4,8-ant8
            public byte TriggerMode;	    //触发方式(缺省值为0): 0-低电平 1-高电平
            public byte HostIP1;			//主机IP地址
            public byte HostIP2;			//
            public byte HostIP3;			//
            public byte HostIP4;			//
            public byte Port1;				//主机端口
            public byte Port2;				//
            public byte Reserve24;			//
            public byte Reserve25;			//
            public byte Reserve26;			//
            public byte Reserve27;			//
            public byte Reserve28;			//
            public byte Reserve29;			//
            public byte Alarm;				//0-不报警，1-报警。在定时和触发方式下是否检测报警。
            public byte Reserve31;		    //
            public byte EnableRelay;		//自动工作模式是否控制继电器1:控制 0:不控制
        }

        public struct ReaderDebugState
        {
            public byte Test;				//0－工作状态；1－调试状态。
            public byte Reserve2;			//保留
            public byte Reserve3;			//保留
            public byte Reserve4;			//保留
        }

        public struct tagReaderFreq
        {
            public String chFreq;		//Chuỗi tần số quốc gia.

            public int iGrade;			//级数 = 50;
            public int iSkip;			//步进 = 500KHz;
            public long dwFreq;         //起始频率 = 902750;
                                        //公式：902750 + Cấp độ * Bước
        };


        //关于各个国家的频率
        //    public const tagReaderFreq[] stuctFreqCountry  =
        //{
        //    new tagReaderFreq("00---FCC(American)", 63, 400, 902600),							//(0),{"00---FCC(American)", 50, 500, 902750},
        //    {"01---ETSI EN 300-220(Europe300-220)", 11, 200, 865500},			//(1),{"01---ETSI EN 300-220(Europe300-220)", -1, -1, -1},
        //    {"02---ETSI EN 302-208(Europe302-208)", 4, 600, 865700},			//(2)
        //    {"03---HK920-925(Hong Kong)", 10, 500, 920250},						//(3)
        //    {"04---TaiWan 922-928(Taiwan)", 12, 500, 922250},					//(4)
        //    {"05---Japan 952-954(Japan)", 0, 0, 0},								//(5)
        //    {"06---Japan 952-955(Japan)", 14,200, 952200},						//(6)
        //    {"07---ETSI EN 302-208(Europe)", 4, 600, 865700},					//(7)
        //    {"08---Korea 917-921(Korea)", 6, 600, 917300},						//(8)
        //    {"09---Malaysia 919-923(Malaysia)", 8, 500, 919250},				//(9)
        //    {"10--China 920-925(China)", 16, 250, 920625},						//(10)
        //    {"11--Japan 952-956(Japan)", 4, 1200, 952400},						//(11)
        //    {"12--South Africa 915-919(Poncho)", 17, 200, 915600},				//(12)
        //    {"13--Brazil 902-907/915-928(Brazil)", 35, 500, 902750},			//(13)
        //    {"14--Thailand 920-925(Thailand)", -1, -1, -1},						//(14)
        //    {"15--Singapore 920-925(Singapore)", 10, 500, 920250},				//(15)
        //    {"16--Australia 920-926(Australia)", 12, 500, 920250},				//(16)
        //    {"17--India 865-867(India)", 4, 600, 865100},						//(17)
        //    {"18--Uruguay 916-928(Uruguay)", 23, 500, 916250},					//(18)
        //    {"19--Vietnam 920-925(Vietnam)", 10, 500, 920250},					//(19)
        //    {"20--Israel 915-917", 1, 0, 916250},								//(20)
        //    {"21--Philippines 918-920(Philippines)", 4, 500, 918250},			//(21)
        //    {"22--Canada 902-928(Canada)", 42, 500, 902750},					//(22)
        //    {"23--Indonesia 923-925(Indonesia)", 4, 500, 923250},				//(23)
        //    {"24--New Zealand 921.5-928(New Zealand)", 11, 500, 922250},		//(24)
        //};



        #region Controlling command
        //Bởi vì quá nhiều, không có thời gian để viết bình luận đầy đủ, chỉ cần viết các tính năng đơn giản, có thời gian để hoàn thành nó ~


        //////////////////////////////////////////////////////////////////////////
        //Công dụng: Đặt tay cầm ứng dụng
        //Tên chức năng:	SetAppHwnd
        //Đặc trưng:		Xử lý lưu trữ.
        //Nhập tham số: HWND là xử lý ứng dụng
        //Thông số đầu ra: Không
        //Giá trị trả về: Trả lại _OK là thành công, thất bại khác
        //Lưu ý: Giao diện bên ngoài
        //////////////////////////////////////////////////////////////////////////
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern void SetAppHwnd(int hWnd);


        //////////////////////////////////////////////////////////////////////////
        //用途:		串口模式连接读写器
        //函数名:	ConnectScanner
        //功能:		进行读写器连接
        //输入参数:	PortNum 为端口号字符串, nBaudRate 为速率的下标(0对应115200,1对应57600,2对应38400,3对应19200,4对应9600) 
        //输出参数:	hScanner 为输出的通信句柄
        //返回值:	返回 _OK 为成功，其它失败
        //备注:		外部接口
        //////////////////////////////////////////////////////////////////////////
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int ConnectScanner(ref int hScanner, string PortNum, ref int nBaudRate);


        //连接读写器(485方式)
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int ConnectScanner485(ref int hScanner, string szPort, int nBaudRate, int Address);

        //////////////////////////////////////////////////////////////////////////
        //用途:		断开读写器连接
        //函数名:	DisconnectScanner
        //功能:		读写器断开
        //输入参数:	hScanner 为通信句柄
        //输出参数:	
        //返回值:	返回 _OK 为成功，其它失败
        //备注:		外部接口
        //////////////////////////////////////////////////////////////////////////
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int DisconnectScanner(int hScanner);




        //==============================设备控制命令==============================
        //设置波特率
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int SetBaudRate(int hScanner, int nBaudRate,int Address);


        //读取版本号
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int GetReaderVersion(int hScanner, ref int wHardVer, ref int wSoftVer,int Address);


        //设定输出功率
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int SetOutputPower(int hScanner, int nPower1,int Address);

        
        //设定工作频率
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int SetFrequency(int hScanner, int Min_Frequency, int Max_Frequency,int Address);

        //获得读写器工作频率范围
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int GetFrequencyRange(int hScanner, ref byte Frequency,int Address);


        //获取读写器基本工作参数
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int ReadBasicParam(int hScanner, ref ReaderBasicParam pParam,int Address);


        //设置读写器基本工作参数
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int WriteBasicParam(int hScanner, ref ReaderBasicParam pParam,int Address);


        //获取读写器自动工作参数
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int ReadAutoParam(int hScanner, ref ReaderAutoParam pParam,int Address);


        //设置读写器自动工作参数
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int WriteAutoParam(int hScanner, ref ReaderAutoParam pParam,int Address);


        //设置调制度
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int SetModDepth(int hScanner, int ModDepth,int Address);


        //获得调制度
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int GetModDepth(int hScanner, ref int ModDepth,int Address);


        //恢复读写器出厂参数
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int ReadFactoryParameter(int hScanner,int Address);


        //选择天线
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int SetAntenna(int hScanner, int Antenna,int Address);


        //复位读写器
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Reboot(int hScanner,int Address);


        //启动/停止读写器自动模式
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int AutoMode(int hScanner,int Mode,int Address);

        //ATA检测读取ID标签
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int ATA_ListTagID(int hScanner, byte[] btID, ref int nCounter,int Address);


        //清除内存
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int ClearTagMemory(int hScanner,int Address);


        //设置时间
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int SetReaderTime(int hScanner, ref ReaderDate time,int Address);


        //获得时间
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int GetReaderTime(int hScanner, ref ReaderDate time,int Address);


        //设置标签过滤器
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int SetReportFilter(int hScanner, int ptr, int len, byte []mask,int Address);


        //获得标签过滤器
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int GetReportFilter(int hScanner, ref int ptr, ref int len, byte[] mask, int Address);


        //增加名单
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int AddLableID(int hScanner, int listlen, int datalen, byte[] data);


        //删除名单
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int DelLableID(int hScanner, int listlen, int datalen, byte[] data);


        //获得名单
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int GetLableID(int hScanner, int startaddr, int listlen, ref int relistlen, ref int taglen, byte[] data);


        //获得记录
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int GetRecord(int hScanner, ref ReaderDate stime, ref ReaderDate etime, int startaddr, int listlen, ref int relistlen, ref int taglen, byte[] data);


        //删除全部记录
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int DeleteAllRecord(int hScanner);

    
        //立即通知
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int ReportNow(int hScanner,int Address);

        
        //获得标签记录
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int GetTagInfo(int hScanner, int straddr, byte Counter, byte[] len, byte[] Data,int Address);


        //获得读写器ID
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int SetReaderID(int hScanner, byte[] ReaderID,int Address);

    
        //获得读写器ID
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int GetReaderID(int hScanner, byte []ReaderID,int Address);


        //==============================网络命令==============================
        //设置读写器IP地址
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int SetReaderNetwork(int hScanner, byte[] IP_Address, int Port, byte[] Mask, byte[] Gateway,int Address);


        //获得读写器IP地址
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int GetReaderNetwork(int hScanner, byte[] IP_Address, ref int Port, byte[] Mask, byte [] Gateway,int Address);



        //设置读写器MAC地址
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int SetReaderMAC(int hScanner, byte[] MAC,int Address);


        //获得读写器MAC地址
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int GetReaderMAC(int hScanner, byte[] MAC,int Address);



        //==============================IO命令==============================
        //设置读写器继电器状态
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int SetRelay(int hScanner, int relay,int Address);

        //获得读写器继电器状态
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int GetRelay(int hScanner, ref int relay,int Address);



        //==============================ISO-6B数据读写命令==============================
        //检测标签存在
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int ISO6B_LabelPresent(int hScanner, ref int nCounter,int Address);

        //读取ISO6B标签ID号
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int ISO6B_ReadLabelID(int hScanner, byte[,] IDBuffer, ref int nCounter, int Address);

        //列出选定标签
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int ISO6B_ListSelectedID(int hScanner, int Cmd, int ptr, byte Mask, byte[] Data, byte[,] IDBuffer, ref int nCounter, int Address);

        //读一块数据
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int ISO6B_ReadByteBlock(int hScanner, byte []IDBuffer, byte ptr, byte len, byte []Data, int Address);

        //一次写4字节数据
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int ISO6B_WriteByteBlock(int hScanner, byte []IDBuffer, byte ptr, byte len, byte []Data, int Address);

        //一次写一字节数据
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int ISO6B_WriteAByte(int hScanner, byte []IDBuffer, byte ptr, byte len, byte []Data, int Address);

        //写大块数据，字节数超过16
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int ISO6B_WriteLongBlock(int hScanner, byte []IDBuffer, byte ptr, byte len, byte []Data, int Address);

        //置写保护状态
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int ISO6B_WriteProtect(int hScanner, byte []IDBuffer, byte ptr,int Address);

        //读写保护状态
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int ISO6B_ReadWriteProtect(int hScanner, byte []IDBuffer, byte ptr, ref byte Protected, int Address);

        //全部清除
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int ISO6B_ClearMemory(int hScanner, byte CardType, byte[] IDBuffer,int Address);        



        //==============================EPC C1G2数据读写命令==============================
        //读取EPC1G2标签ID号
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int EPC1G2_ReadLabelID(int hScanner, int mem, int ptr, int len, byte[] mask, byte[] IDBuffer, ref int nCounter, int Address);

        //读一块数据
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int EPC1G2_ReadWordBlock(int hScanner, byte EPC_WORD, byte [] IDBuffer, byte mem, byte ptr, byte len, byte[] Data, byte[] AccessPassword, int Address);

        //写一块数据
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int EPC1G2_WriteWordBlock(int hScanner, byte EPC_WORD, byte []IDBuffer, byte mem, byte ptr, byte len, byte [] Data, byte [] AccessPassword,int Address);

        //设置读写保护状态
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int EPC1G2_SetLock(int hScanner, byte EPC_WORD, byte [] IDBuffer, byte mem, byte Lock, byte [] AccessPassword, int Address);

        //擦除标签数据
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int EPC1G2_EraseBlock(int hScanner, byte EPC_WORD, byte [] IDBuffer, byte mem, byte ptr, byte len, int Address);

        //永久休眠标签
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int EPC1G2_KillTag(int hScanner, byte EPC_WORD, byte []IDBuffer, byte[] KillPassword,int Address);

        //写EPC
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int EPC1G2_WriteEPC(int hScanner, byte len, byte []Data, byte []AccessPassword,int Address);

        //块锁命令
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int EPC1G2_BlockLock(int hScanner, byte EPC_WORD, byte []IDBuffer, byte ptr, byte []AccessPassword,int Address);

        //EAS状态操作命令
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int EPC1G2_ChangeEas(int hScanner, byte EPC_WORD, byte []IDBuffer, byte State, byte []AccessPassword,int Address);

        //EAS报警命令
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int EPC1G2_EasAlarm(int hScanner,int Address);

        //读保护设置
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int EPC1G2_ReadProtect(int hScanner,byte []AccessPassword, byte EPC_WORD, byte []IDBuffer,int Address);

        //复位读保护设置
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int EPC1G2_RStreadProtect(int hScanner, byte []AccessPassword,int Address);

        //设置用户区数据块读保护
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int EPC1G2_BlockReadLock(int hScanner, byte EPC_WORD, byte []IDBuffer, byte Lock, byte []AccessPassword,int Address);

        //识别EPC同时读数据
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int EPC1G2_ReadEPCandData(int hScanner, byte []EPC_WORD, byte []IDBuffer, byte mem, byte ptr, byte len, byte []Data, int Address);

        //频谱校验
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int EPC1G2_Calibrate(int hScanner, byte []AccessPassword, byte Kword);

        //侦测标签
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int EPC1G2_DetectTag(int hScanner,int Address);



        //==============================Định nghĩa hàm mạng==============================
        //////////////////////////////////////////////////////////////////////////
        //sử dụng:		Trình đọc kết nối chế độ NET
        //Tên chức năng:	Net_ConnectScanner
        //Đặc trưng:		Kết nối sẵn sàng
        //Tham số đầu vào:	nTargetAddress ĐỊA CHỈ ĐƠN VỊ, nTargetPort 为读写器端口, nHostAddress Địa chỉ máy chủ, nHostPort 为主机端口
        //Tham số đầu ra:	hSocket 为输出的通信句柄
        //Trả về giá trị:	返回 _OK 为成功，其它失败
        //Ghi chú:		Giao diện bên ngoài
        //////////////////////////////////////////////////////////////////////////
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_ConnectScanner(ref int hSocket, string nTargetAddress, uint nTargetPort, string nHostAddress, uint nHostPort);



        //////////////////////////////////////////////////////////////////////////
        //sử dụng:		Ngắt kết nối đầu đọc
        //函数名:	DisconnectScanner
        //功能:		读写器断开
        //输入参数:	hScanner 为通信句柄
        //输出参数:	
        //返回值:	返回 _OK 为成功，其它失败
        //备注:		外部接口
        //////////////////////////////////////////////////////////////////////////
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_DisconnectScanner(int hScanner);




        //==============================Lệnh điều khiển thiết bị==============================
        //Đặt tốc độ baud
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_SetBaudRate(int hScanner, int nBaudRate);


        //Đọc số phiên bản
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_GetReaderVersion(int hScanner, ref int wHardVer, ref int wSoftVer);


        //Đặt nguồn đầu ra.
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_etOutputPower(int hScanner, int nPower1);


        //Đặt tần số hoạt động
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_SetFrequency(int hScanner, int Min_Frequency, int Max_Frequency);

        //Nhận dải tần số hoạt động của người đọc
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_GetFrequencyRange(int hScanner, ref byte Frequency);


        //Nhận các thông số làm việc cơ bản của Readerwork
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_ReadBasicParam(int hScanner, ref ReaderBasicParam pParam);


        //Đặt các thông số làm việc cơ bản của người đọc
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_WriteBasicParam(int hScanner, ref ReaderBasicParam pParam);


        //Nhận trình đọc thông số làm việc tự động
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_ReadAutoParam(int hScanner, ref ReaderAutoParam pParam);


        //Trình đọc thông số làm việc tự động
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_WriteAutoParam(int hScanner, ref ReaderAutoParam pParam);


        //Thiết lập điều chỉnh
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_SetModDepth(int hScanner, int ModDepth);


        //Có được điều chỉnh.
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_GetModDepth(int hScanner, ref int ModDepth);


        //Khôi phục thông số nhà máy đọc
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_ReadFactoryParameter(int hScanner);


        // Chọn ăng-ten.
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_SetAntenna(int hScanner, int Antenna);


        //Trình đọc lại
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_Reboot(int hScanner);


        //Bắt đầu / dừng chế độ tự động đọc
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_AutoMode(int hScanner, int Mode);

        //Thẻ đọc phát hiện ATA
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_ATA_ListTagID(int hScanner, byte[] btID, ref int nCounter);


        //Dọn dẹp bộ nhớ
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_ClearTagMemory(int hScanner);


        //Cài đặt thời gian
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_SetReaderTime(int hScanner, ref ReaderDate time);


        //Save thời gian
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_GetReaderTime(int hScanner, ref ReaderDate time);


        //Đặt bộ lọc nhãn
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_SetReportFilter(int hScanner, int ptr, int len, byte[] mask);


        //Nhận bộ lọc nhãn
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_GetReportFilter(int hScanner, ref int ptr, ref int len, byte[] mask);


        //Thêm vào danh sách
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_AddLableID(int hScanner, int listlen, int datalen, byte[] data);


        //XÓA DANH SÁCH
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_DelLableID(int hScanner, int listlen, int datalen, byte[] data);


        //Đạt được danh sách
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_GetLableID(int hScanner, int startaddr, int listlen, ref int relistlen, ref int taglen, byte[] data);


        //Ghi lại
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_GetRecord(int hScanner, ref ReaderDate stime, ref ReaderDate etime, int startaddr, int listlen, ref int relistlen, ref int taglen, byte[] data);


        //Xóa tất cả các hồ sơ
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_DeleteAllRecord(int hScanner);


        //Thông báo ngay bây giờ
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_ReportNow(int hScanner);


        //Nhận hồ sơ nhãn
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_GetTagInfo(int hScanner, int straddr, byte Counter, byte[] len, byte[] Data);


        //Nhận ID Reader
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_SetReaderID(int hScanner, byte[] ReaderID);


        //Nhận ID Reader
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_GetReaderID(int hScanner, byte[] ReaderID);


        //==============================Lệnh mạng.==============================
        //Đặt địa chỉ IP Reader
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_SetReaderNetwork(int hScanner, byte[] IP_Address, int Port, byte[] Mask, byte[] Gateway);


        //Nhận địa chỉ IP Reader
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_GetReaderNetwork(int hScanner, byte[] IP_Address, ref int Port, byte[] Mask, byte[] Gateway);



        //Trình đọc thiết lập Địa chỉ MAC
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_SetReaderMAC(int hScanner, byte[] MAC);


        //Nhận địa chỉ Mac Reader
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_GetReaderMAC(int hScanner, byte[] MAC);



        //==============================Lệnh IO.==============================
        //Trạng thái rơle Setup Reader
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_SetRelay(int hScanner, int relay);

        //Nhận trạng thái rơle Reader
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_GetRelay(int hScanner, ref int relay);



        //==============================Lệnh đọc và ghi dữ liệu ISO-6B==============================
        //Nhãn phát hiện
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_ISO6B_LabelPresent(int hScanner, ref int nCounter);

        // Đọc số ID thẻ ISO6B
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_ISO6B_ReadLabelID(int hSocket, byte[,] IDBuffer, ref int nCounter);

        //Liệt kê nhãn đã chọn
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_ISO6B_ListSelectedID(int hSocket, int Cmd, int ptr, byte Mask, byte[] Data, byte[,] IDBuffer, ref int nCounter);

        //Đọc một phần dữ liệu
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_ISO6B_ReadByteBlock(int hScanner, byte[] IDBuffer, byte ptr, byte len, byte[] Data);

        // Viết dữ liệu 4 byte
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_ISO6B_WriteByteBlock(int hScanner, byte[] IDBuffer, byte ptr, byte len, byte[] Data);

        //Viết một dữ liệu byte tại một thời điểm
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_ISO6B_WriteAByte(int hScanner, byte[] IDBuffer, byte ptr, byte len, byte[] Data);

        //Viết dữ liệu khối lớn, số byte vượt quá 16
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_ISO6B_WriteLongBlock(int hScanner, byte[] IDBuffer, byte ptr, byte len, byte[] Data);

        //Bảo vệ từ xa
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_ISO6B_WriteProtect(int hScanner, byte[] IDBuffer, byte ptr);

        //Đọc và viết bảo vệ
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_ISO6B_ReadWriteProtect(int hScanner, byte[] IDBuffer, byte ptr, ref byte Protected);

        //Làm sạch tất cả
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_ISO6B_ClearMemory(int hScanner, byte CardType, byte[] IDBuffer);



        //==============================EPC C1G2数据读写命令==============================
        //Đọc số ID thẻ EPC1G2
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_EPC1G2_ReadLabelID(int hSocket, int mem, int ptr, int len, byte[] mask, byte[] IDBuffer, ref int nCounter);

        //Đọc một phần dữ liệu
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_EPC1G2_ReadWordBlock(int hScanner, byte EPC_WORD, byte[] IDBuffer, byte mem, byte ptr, byte len, byte[] Data, byte[] AccessPassword);

        //Viết một đoạn dữ liệu
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_EPC1G2_WriteWordBlock(int hScanner, byte EPC_WORD, byte[] IDBuffer, byte mem, byte ptr, byte len, byte[] Data, byte[] AccessPassword);

        //Đặt trạng thái bảo vệ đọc và ghi
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_EPC1G2_SetLock(int hScanner, byte EPC_WORD, byte[] IDBuffer, byte mem, byte Lock, byte[] AccessPassword);

        //Xóa dữ liệu thẻ
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_EPC1G2_EraseBlock(int hScanner, byte EPC_WORD, byte[] IDBuffer, byte mem, byte ptr, byte len);

        //Tag ngủ vĩnh viễn
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_EPC1G2_KillTag(int hScanner, byte EPC_WORD, byte[] IDBuffer, byte[] KillPassword );

        //Viết EPC.
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_EPC1G2_WriteEPC(int hScanner, byte len, byte[] Data, byte[] AccessPassword);

        //Lệnh Block.
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_EPC1G2_BlockLock(int hScanner, byte EPC_WORD, byte[] IDBuffer, byte ptr, byte[] AccessPassword);

        //Lệnh hoạt động trạng thái EAS
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_EPC1G2_ChangeEas(int hScanner, byte EPC_WORD, byte[] IDBuffer, byte State, byte[] AccessPassword);

        //Lệnh báo động EAS
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_EPC1G2_EasAlarm(int hScanner);

        // Đọc cài đặt bảo vệ
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_EPC1G2_ReadProtect(int hScanner, byte[] AccessPassword, byte EPC_WORD, byte[] IDBuffer);

        //Đặt lại cài đặt bảo vệ đọc
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_EPC1G2_RStreadProtect(int hScanner, byte[] AccessPassword);

        //Đặt bảo vệ đọc dữ liệu khu vực người dùng
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_EPC1G2_BlockReadLock(int hScanner, byte EPC_WORD, byte[] IDBuffer, byte Lock, byte[] AccessPassword);

        //Xác định dữ liệu đọc đồng thời EPC
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_EPC1G2_ReadEPCandData(int hScanner, byte[] EPC_WORD, byte[] IDBuffer, byte mem, byte ptr, byte len, byte[] Data);

        //Xác minh phổ
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_EPC1G2_Calibrate(int hScanner, byte[] AccessPassword, byte Kword);

        //Nhãn phát hiện
        [DllImport(PUBLIC_COM_PATH, CharSet = CharSet.Ansi)]
        public static extern int Net_EPC1G2_DetectTag(int hScanner);



        

        #endregion


        #region systemAPI
        public delegate void TimerProc(IntPtr hWnder, uint nMsg, int nIDEvent, int dwTime);
        [DllImport("user32.dll")]
        public static extern int SetTimer(int hwnd, int nIDEvent, int uElapse, TimerProc CB);
        [DllImport("user32.dll")]
        public static extern int KillTimer(int hwnd, int nIDEvent);

        [DllImport("kernel32.dll")]
        public static extern bool GetCommState(
         int hFile,
         ref DCB lpDCB
       );

        [DllImport("kernel32.dll")]
        public static extern bool SetCommState(
         int hFile,  // Xử lý thiết bị truyền thông 
         ref DCB lpDCB    // Khối điều khiển thiết bị 
       );

        [DllImport("kernel32.dll")]
        public static extern bool PurgeComm(
        int hFile,  // Xử lý thiết bị truyền thông 
        uint dwFlags
        );


        //        [DllImport("kernel32.dll")]
        //        public static extern bool Beep(int frequency, int duration);

        [DllImport("user32.dll")]
        public static extern bool MessageBeep(int beepType);


        [StructLayout(LayoutKind.Sequential)]
        public struct DCB
        {
            //taken from c struct in platform sdk  
            public int DCBlength;           // sizeof(DCB)  
            public int BaudRate;            // Chỉ định tốc độ baud hiện tại current baud rate 
            // these are the c struct bit fields, bit twiddle flag to set 
            public int fBinary;          // Chỉ định có cho phép chế độ nhị phân không, Phải là chế độ nhị phân thực sự chính trong Windows 95, no EOF check  
            public int fParity;          // Chỉ định có cho phép chẵn lẻ không enable parity checking  
            public int fOutxCtsFlow;      // Chỉ định liệu CTS có được sử dụng để phát hiện điều khiển gửi, khi đúng là CTS bị tắt, việc truyền sẽ bị treo。 CTS output flow control  
            public int fOutxDsrFlow;      // 指定CTS是否用于检测发送控制 DSR output flow control  
            public int fDtrControl;       // DTR_CONTROL_DISABLE值将DTR置为OFF, DTR_CONTROL_ENABLE值将DTR置为ON, DTR_CONTROL_HANDSHAKE允许DTR"握手" DTR flow control type  
            public int fDsrSensitivity;   // 当该值为TRUE时DSR为OFF时接收的字节被忽略 DSR sensitivity  
            public int fTXContinueOnXoff; // 指定当接收缓冲区已满,并且驱动程序已经发送出XoffChar字符时发送是否停止。TRUE时，在接收缓冲区接收到缓冲区已满的字节XoffLim且驱动程序已经发送出XoffChar字符中止接收字节之后，发送继续进行。　FALSE时，在接收缓冲区接收到代表缓冲区已空的字节XonChar且驱动程序已经发送出恢复发送的XonChar之后，发送继续进行。XOFF continues Tx  
            public int fOutX;          // TRUE时，接收到XoffChar之后便停止发送接收到XonChar之后将重新开始 XON/XOFF out flow control  
            public int fInX;           // TRUE时，接收缓冲区接收到代表缓冲区满的XoffLim之后，XoffChar发送出去接收缓冲区接收到代表缓冲区空的XonLim之后，XonChar发送出去 XON/XOFF in flow control  
            public int fErrorChar;     // 该值为TRUE且fParity为TRUE时，用ErrorChar 成员指定的字符代替奇偶校验错误的接收字符 enable error replacement  
            public int fNull;          // eTRUE时，接收时去掉空（0值）字节 enable null stripping  
            public int fRtsControl;     // RTS flow control 
            /*RTS_CONTROL_DISABLE时,RTS置为OFF 
             RTS_CONTROL_ENABLE时, RTS置为ON 
           RTS_CONTROL_HANDSHAKE时, 
           当接收缓冲区小于半满时RTS为ON 
              当接收缓冲区超过四分之三满时RTS为OFF 
           RTS_CONTROL_TOGGLE时, 
           当接收缓冲区仍有剩余字节时RTS为ON ,否则缺省为OFF*/
            public int fAbortOnError;   // TRUE时,有错误发生时中止读和写操作 abort on error  
            public int fDummy2;        // 未使用 reserved  

            public uint flags;
            public ushort wReserved;          // 未使用,必须为0 not currently used  
            public ushort XonLim;             // 指定在XON字符发送这前接收缓冲区中可允许的最小字节数 transmit XON threshold  
            public ushort XoffLim;            // 指定在XOFF字符发送这前接收缓冲区中可允许的最小字节数 transmit XOFF threshold  
            public byte ByteSize;           // 指定端口当前使用的数据位   number of bits/byte, 4-8  
            public byte Parity;             // 指定端口当前使用的奇偶校验方法,可能为:EVENPARITY,MARKPARITY,NOPARITY,ODDPARITY  0-4=no,odd,even,mark,space  
            public byte StopBits;           // 指定端口当前使用的停止位数,可能为:ONESTOPBIT,ONE5STOPBITS,TWOSTOPBITS  0,1,2 = 1, 1.5, 2  
            public char XonChar;            // 指定用于发送和接收字符XON的值 Tx and Rx XON character  
            public char XoffChar;           // 指定用于发送和接收字符XOFF值 Tx and Rx XOFF character  
            public char ErrorChar;          // 本字符用来代替接收到的奇偶校验发生错误时的值 error replacement character  
            public char EofChar;            // 当没有使用二进制模式时,本字符可用来指示数据的结束 end of input character  
            public char EvtChar;            // 当接收到此字符时,会产生一个事件 received event character  
            public ushort wReserved1;         // 未使用 reserved; do not use  
        }

        #endregion


    }//end of public class DllComm

        
}
