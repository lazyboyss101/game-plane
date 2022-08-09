#include<easyx.h>
#include<string>
#include<map>
using namespace std;

//�ص�����������
LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);


#define ADD_MSG(MSG_ID,MSGTYPE)\
	m_MsgMap[MSG_ID].msgType = MSGTYPE;\
	m_MsgMap[MSG_ID].p_fun_##MSGTYPE = &CGameFrame::ON_##MSG_ID;


class CGameFrame{
	//��Ԫ  ����WindowProc�������ѣ�������WindowPro��ʹ�õ�ǰ���˽�г�Ա
	friend LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
protected:
	HWND m_hWnd;   //���ھ��
private:
	bool m_isQuit;   // true :�����˳� false �����˳�

	typedef void (CGameFrame::*P_FUN_MOUSE)(POINT );
	typedef void (CGameFrame::*P_FUN_KEY)(WPARAM );
	typedef void (CGameFrame::*P_FUN_CHAR)(WPARAM );
	typedef void (CGameFrame::*P_FUN_WINDOW)(WPARAM,LPARAM );

	//��Ϣid  ,������Ϣ���ͣ���Ϣ������
	struct MSG_FUN{
		UINT  msgType;   //��Ϣ����   EM_MOUSE;
		union{
			P_FUN_MOUSE   p_fun_EM_MOUSE;
			P_FUN_KEY     p_fun_EM_KEY;
			P_FUN_CHAR    p_fun_EM_CHAR;
			P_FUN_WINDOW  p_fun_EM_WINDOW;
		};
	};
	map<UINT,MSG_FUN>  m_MsgMap;    //��Ϣӳ���

public:
	CGameFrame(){
		m_isQuit = false;  //���˳�
	}
	bool GetQuit(){
		return m_isQuit;
	}

	virtual ~CGameFrame(){}
public:
	//1. �߼������ϵĳ�ʼ���������ҵ���߼��й�
	void OnInit(int x,int y ,int w,int h,const TCHAR * title){
		m_hWnd = ::initgraph(w,h);  //��ʼ����������
		MoveWindow(m_hWnd,x,y,w,h,true);  //�ƶ�

		SetWindowText(m_hWnd,title);   //���ô��ڱ���

		

		//��ʼ����Ϣӳ���
		InitMsgMap();

		InitGame();   //��ʼ����������Ϸ

		//�����趨���ڵĻص�����
		::SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)&WindowProc);
	}

	//2.��Ϸ�ػ�
	void OnPaint(){
		::BeginBatchDraw();  //��ʼ�����ػ�
		::cleardevice();   //����

		PaintGame();    //�ػ�������Ϸ
		::EndBatchDraw();  //����������ͼ
	}
	void InitMsgMap(){
		//m_MsgMap[WM_LBUTTONDOWN].msgType = EM_MOUSE;
		//m_MsgMap[WM_LBUTTONDOWN].p_fun_EM_MOUSE = &CGameFrame::ON_WM_LBUTTONDOWN;
		ADD_MSG(WM_LBUTTONDOWN,EM_MOUSE);

		ADD_MSG(WM_KEYDOWN,EM_KEY);

		ADD_MSG(WM_CHAR,EM_CHAR);

		ADD_MSG(WM_TIMER,EM_WINDOW);   //��Ӷ�ʱ����Ϣ


		//ADD_MSG(WM_CLOSE,EM_WINDOW);
	}

	

	//3. ��Ϸ����
	//void OnRun(){
	//	while(1){
	//		ExMessage msg;
	//		//��ȡ��Ϣ
	//		getmessage(&msg);
	//		if(msg.message == WM_LBUTTONDOWN){  //����������
	//			string str;
	//			char arr1[10]={0};
	//			itoa(msg.message,arr1,10);
	//			char arr2[10]={0};
	//			itoa(msg.x,arr2,10);
	//			char arr3[10]={0};
	//			itoa(msg.y,arr3,10);
	//			str += arr1;
	//			str += arr2;
	//			str += arr3;
	//			WM_CLOSE;
	//			WM_TIMER;
	//		
	//			OutputDebugString(str.c_str());
	//		}
	//	
	//	
	//	}
	//
	//}

	//---------------------------------------------

	virtual void InitGame()=0;
	virtual void PaintGame()=0;
	virtual void CloseGame()=0;

	/*
		�����Ϣ�������������� ON_��ϢID�ĺ꣬����ԭ�ͼ� typedef �����Ա����ָ�������
	*/
	virtual void ON_WM_LBUTTONDOWN(POINT po){};
	virtual void ON_WM_KEYDOWN(WPARAM w){};
	virtual void ON_WM_CHAR(WPARAM w){};
	virtual void ON_WM_TIMER(WPARAM w,LPARAM l){};




	//4. �رմ���-------------------------------------
	void ON_WM_CLOSE(WPARAM w,LPARAM l){
		//close �������Ϸ
		CloseGame();
		::closegraph();  //�ر� ��Ϸ�Ǵ���
		m_isQuit = true;
	}


};


#define DYNAMIC_OBJECT(CLASS_NAME)\
CGameFrame * CreateObject(){\
	return new CLASS_NAME;\
}

#define WINDOW_PARAM(X,Y,WIDHT,HEIGHT,TITLE)\
int window_x               =X;\
int window_y               =Y;\
int window_width           =WIDHT;\
int window_height          =HEIGHT;\
const TCHAR * window_title = TITLE;


/*
�Զ���һ������ ���̳���Ϸ���ࣨCGameFrame��,��д����Ĵ��麯��InitGame(),PaintGame(),CloseGame().
�������Դ�ļ��У�ճ��������DYNAMIC_OBJECT(CLASS_NAME)��WINDOW_PARAM(X,Y,WIDHT,HEIGHT,TITLE) ��
�괫�ݶ�Ӧ�Ĳ��� ����
															  
*/															  