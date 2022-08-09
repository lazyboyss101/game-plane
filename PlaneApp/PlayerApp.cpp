#include"PlayerApp.h"
#include"../GameConfig/GameConfig.h"
#include<time.h>
#include"../FoePlaneBox/FoePlaneBig.h"
#include"../FoePlaneBox/FoePlaneMid.h"
#include"../FoePlaneBox/FoePlaneSma.h"

DYNAMIC_OBJECT(CPlaneApp)
WINDOW_PARAM(WINDOW_X,WINDOW_Y,WINDOW_WIDTH,WINDOW_HEIGHT,WINDOW_TITLE)


CPlaneApp::CPlaneApp(){
	m_nScore =0;
}
CPlaneApp::~CPlaneApp(){

}

void CPlaneApp::InitGame(){
	//�����������
	srand((unsigned int)time(NULL));


	//��ʼ������
	m_back.InitBackGround();
	//��ʼ����ҷɻ�
	m_player.InitPlayer();

	m_gunBox.ShowAllGunner();


	::loadimage(&m_scoreImg,PATH_PIC(cardboard.png));



	//�趨�����ƶ��Ķ�ʱ��
	::SetTimer(m_hWnd,TIMER_BACKGROUND_MOVE_ID,TIMER_BACKGROUND_MOVE_INTERVAL,nullptr);

	::SetTimer(m_hWnd,TIMER_PLAYER_MOVE_ID,TIMER_PLAYER_MOVE_INTERVAL,nullptr);

	//�ڵ��ƶ�
	::SetTimer(m_hWnd,TIMER_GUNNER_MOVE_ID,TIMER_GUNNER_MOVE_INTERVAL,nullptr);
	//�ڵ���ʱ����
	::SetTimer(m_hWnd,TIMER_GUNNER_SEND_ID,TIMER_GUNNER_SEND_INTERVAL,nullptr);
	
	//���˷ɻ���ʱ�ƶ�
	::SetTimer(m_hWnd,TIMER_FOEBIG_MOVE_ID,TIMER_FOEBIG_MOVE_INTERVAL,nullptr);
	

	::SetTimer(m_hWnd,TIMER_FOEBIG_CREATE_ID,TIMER_FOEBIG_CREATE_INTERVAL,nullptr);
	::SetTimer(m_hWnd,TIMER_FOEMID_CREATE_ID,TIMER_FOEMID_CREATE_INTERVAL,nullptr);
	::SetTimer(m_hWnd,TIMER_FOESMA_CREATE_ID,TIMER_FOESMA_CREATE_INTERVAL,nullptr);
	
	::SetTimer(m_hWnd,TIMER_IS_HIT_ID,TIMER_IS_HIT_INTERVAL,nullptr);

	::SetTimer(m_hWnd,TIMER_CHANGE_SHOWID_ID,TIMER_CHANGE_SHOWID_INTERVAL,nullptr);
	
	
	
}
void CPlaneApp::PaintGame(){
	//��ʾ����ͼƬ
	m_back.showBackGround();

	//��ʾ��ҷɻ�
	m_player.ShowPlayer();

	m_gunBox.ShowAllGunner();

	//��ʾ�����ĵ��˷ɻ�
	m_foeBox.ShowAllFoePlane();

	//��ʾ��ը�ĵ��˷ɻ�
	m_foeBox.ShowAllBoomFoePlane();

	//��ʾ����
	ShowScore();


}
void CPlaneApp::CloseGame(){

}


#define CREATE_FOE(FOE_CLASS)\
	CFoePlane* pFoe = new FOE_CLASS;\
	pFoe->InitFoePlane();\
	m_foeBox.m_lstFoe.push_back(pFoe);


//��ʱ����Ϣ�Ĵ�����
void CPlaneApp::ON_WM_TIMER(WPARAM w,LPARAM l){

	switch (w)
	{
	case TIMER_BACKGROUND_MOVE_ID:  //�����ƶ�
		{
			m_back.MoveBackGround(	TIMER_BACKGROUND_MOVE_STEP);				  
								  
		}
		break;
	case TIMER_PLAYER_MOVE_ID:  //��ҷɻ����ƶ�
		{
			if(::GetAsyncKeyState(VK_LEFT)){   //��ʱ��ⷿ����� ���Ƿ���
				m_player.MovePlayer(VK_LEFT,TIMER_PLAYER_MOVE_STEP);
			}	
			if(::GetAsyncKeyState(VK_RIGHT)){   //��ʱ��ⷿ����� ���Ƿ���
				m_player.MovePlayer(VK_RIGHT,TIMER_PLAYER_MOVE_STEP);
			}
			if(::GetAsyncKeyState(VK_UP)){   //��ʱ��ⷿ����� ���Ƿ���
				m_player.MovePlayer(VK_UP,TIMER_PLAYER_MOVE_STEP);
			}	
			if(::GetAsyncKeyState(VK_DOWN)){   //��ʱ��ⷿ����� ���Ƿ���
				m_player.MovePlayer(VK_DOWN,TIMER_PLAYER_MOVE_STEP);
			}	

		}
		break;
	case TIMER_GUNNER_MOVE_ID:   //�ڵ��ƶ��Ķ�ʱ��
		{
			m_gunBox.MoveAllGunner(TIMER_GUNNER_MOVE_STEP);
		}
		break;

	case TIMER_GUNNER_SEND_ID:  //�ڵ���ʱ����
		{
			CGunner * pGun = m_player.SendGunner();  //�����ڵ�
			m_gunBox.m_lstGun.push_back(pGun);     //�ڵ��ŵ��ڵ�������
		
		}
		break;
	case TIMER_FOEBIG_MOVE_ID:
		{
			m_foeBox.MoveAllFoePlane();
		
		}
		break;
	case TIMER_FOEBIG_CREATE_ID:
		{
			//CFoePlane* pFoe = new CFoePlaneBig;
			//pFoe->InitFoePlane();
			//m_foeBox.m_lstFoe.push_back(pFoe);
			//�������˷ɻ�������ӵ����˷ɻ�������
			CREATE_FOE(CFoePlaneBig)
		}
		break;
	case TIMER_FOEMID_CREATE_ID:
		{
			CREATE_FOE(CFoePlaneMid)
		}
		break;
	case TIMER_FOESMA_CREATE_ID:
		{
			CREATE_FOE(CFoePlaneSma)
		}
		break;
	case TIMER_IS_HIT_ID:   //��ʱ����Ƿ�ײ�������˷ɻ�-���ڵ������˷ɻ�-����ҷɻ���
		{
			list<CFoePlane*>::iterator iteFoe =   m_foeBox.m_lstFoe.begin();
			while(iteFoe  != m_foeBox.m_lstFoe.end()){
				bool isBoom = false;


				if(  (*iteFoe) ){
					//-----------------------------
					//1. �ж��Ƿ�ײ����ҷɻ�
					if( (*iteFoe)->IsHitPlayer(m_player)  ){
						
						//GameOver
						GameOver();
						return ;
					}
					// 2. �ж��Ƿ�ײ���ڵ�
					list<CGunner*>::iterator iteGun =   m_gunBox.m_lstGun.begin();
					while(iteGun !=   m_gunBox.m_lstGun.end()){
						if( (*iteFoe)->IsHitGunner(*iteGun) ){ //�Ƿ�ײ�����ڵ�
						
							delete (*iteGun);  //ɾ���ڵ�
							(*iteGun) = nullptr;

							//ɾ���ڵ�
							iteGun = m_gunBox.m_lstGun.erase(iteGun);

							//��Ѫ
							(*iteFoe)->m_blood-=DEF_GUNNER_HURT;
							if( (*iteFoe)->m_blood <=0){
								m_foeBox.m_lstBoomFoe.push_back(*iteFoe);  //��ӵ���ը������

								iteFoe = m_foeBox.m_lstFoe.erase(iteFoe);
								isBoom = true;

								m_nScore+=3;


								break;
							}


							continue;
						}
						
						iteGun++;
					}

					//-----------------------------
				}
				if(!isBoom)
					iteFoe++;
			}
		}
		break;

	case TIMER_CHANGE_SHOWID_ID:
		{
			list<CFoePlane*>::iterator iteBoom =  m_foeBox.m_lstBoomFoe.begin();
			while(iteBoom !=  m_foeBox.m_lstBoomFoe.end()){
				
				(*iteBoom)->m_showId--;
				if((*iteBoom)->m_showId <0 ){
					delete (*iteBoom);
					(*iteBoom) = nullptr;

				
					iteBoom = m_foeBox.m_lstBoomFoe.erase(iteBoom);
					continue;
				}

				iteBoom++;
			}
		}
		break;
	}

}

//���̰�����Ϣ����
void CPlaneApp::ON_WM_KEYDOWN(WPARAM w){
	//m_player.MovePlayer(w,TIMER_PLAYER_MOVE_STEP);
}

void CPlaneApp::GameOver(){
	//ֹͣ���ж�ʱ��
	::KillTimer(m_hWnd,TIMER_BACKGROUND_MOVE_ID);

	::KillTimer(m_hWnd,TIMER_PLAYER_MOVE_ID);

	::KillTimer(m_hWnd,TIMER_GUNNER_MOVE_ID);
	::KillTimer(m_hWnd,TIMER_GUNNER_SEND_ID);
	
	::KillTimer(m_hWnd,TIMER_FOEBIG_MOVE_ID);

	::KillTimer(m_hWnd,TIMER_FOEBIG_CREATE_ID);
	::KillTimer(m_hWnd,TIMER_FOEMID_CREATE_ID);
	::KillTimer(m_hWnd,TIMER_FOESMA_CREATE_ID);
	
	::KillTimer(m_hWnd,TIMER_IS_HIT_ID);
	::KillTimer(m_hWnd,TIMER_CHANGE_SHOWID_ID);

	::MessageBox(m_hWnd,"GameOver","��Ϸ����",MB_OK);

	//�����Զ��˳��������˳�WM_CLOSE����Ϣ
	::PostMessage(m_hWnd,WM_CLOSE,0,0);
}

void CPlaneApp::ShowScore(){
	::putimage(0,0,&m_scoreImg);
	char score[5]={0};
	itoa(m_nScore,score,10);
	RECT rect={0,0,180,60};
	settextcolor(RGB(255,0,0));
	::drawtext(score,&rect,DT_CENTER);
	
}