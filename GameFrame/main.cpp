#include"GameFrame.h"
#include<Windowsx.h>


CGameFrame *pGameFrame = nullptr;

// 3. ��Ϸ����
LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam){


	if(pGameFrame && pGameFrame->m_MsgMap.count(uMsg)){
		switch (pGameFrame->m_MsgMap[uMsg].msgType)
		{
		case EM_MOUSE:  //���
			{
				POINT po={0};
				po.x = GET_X_LPARAM(lParam); 
				po.y= GET_Y_LPARAM(lParam);

				(pGameFrame  ->*   (pGameFrame->m_MsgMap[uMsg].p_fun_EM_MOUSE)   )(po);

			}
			break;
		case EM_KEY:  //����
			{
				(pGameFrame->*(pGameFrame->m_MsgMap[uMsg].p_fun_EM_KEY)   )(wParam);
			}
			break;
		case EM_CHAR:
			{
				(pGameFrame->*(pGameFrame->m_MsgMap[uMsg].p_fun_EM_CHAR)   )(wParam);
			}
			break;
		case EM_WINDOW:
			{
				(pGameFrame->*(pGameFrame->m_MsgMap[uMsg].p_fun_EM_WINDOW)   )(wParam,lParam);
			}
			break;
		}

			//ֻҪ����Ϣ�����ػ棬��֤��ʱˢ�´���
		if(pGameFrame){
			pGameFrame->OnPaint();
		}
	
	}



	if(uMsg == WM_CLOSE){
		pGameFrame->ON_WM_CLOSE(wParam,lParam);
	}


	/*switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			int xPos = GET_X_LPARAM(lParam); 
			int yPos = GET_Y_LPARAM(lParam);

			string str;
			char arr1[10]={0};
			itoa(xPos,arr1,10);
			char arr2[10]={0};
			itoa(yPos,arr2,10);
			

			str += arr1;
			str +="    ";
			str += arr2;

			
			OutputDebugString(str.c_str());

			//ON_LBUTTONDOWN()
		}
		break;
	case WM_CLOSE:
		{
			OutputDebugString("WM_CLOSE");
		}
		break;
	case WM_KEYDOWN:
		{
			OutputDebugString("WM_KEYDOWN");
		}
		break;
	}*/

	return DefWindowProc(hwnd,uMsg,wParam,lParam);  //window ��Ĭ�ϻص�
}

CGameFrame * CreateObject();

extern int window_x ;
extern int window_y ;
extern int window_width ;
extern int window_height ;
extern const TCHAR * window_title ;


int main(){
	pGameFrame =CreateObject();
	pGameFrame->OnInit(window_x,window_y,window_width,window_height,window_title);

	while(!pGameFrame->GetQuit()){  //�жϳ����Ƿ��˳�

		Sleep(100);
	}

	//���򼴽��˳�
	if(pGameFrame){  //����new �Ŀռ�
		delete pGameFrame;
		pGameFrame = nullptr;
	}

	return 0;
}
