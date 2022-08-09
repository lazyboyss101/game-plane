#include"GunnerBox.h"
#include"../GameConfig/GameConfig.h"
CGunnerBox::CGunnerBox(){

}
CGunnerBox::~CGunnerBox(){
	list<CGunner*>::iterator ite =   m_lstGun.begin();
	while(ite !=   m_lstGun.end()){   //����ɾ�� new �ڵ�
		if((*ite)){
			delete (*ite);
			(*ite) = nullptr;
		}

		ite++;
	}
	m_lstGun.clear();  //ɾ�����нڵ�
}

void CGunnerBox::ShowAllGunner(){
	list<CGunner*>::iterator ite =   m_lstGun.begin();
	while(ite !=   m_lstGun.end()){   //����ɾ�� new �ڵ�
		if((*ite)){
			(*ite)->ShowGunner();
		}

		ite++;
	}
}
void CGunnerBox::MoveAllGunner(int step){
	list<CGunner*>::iterator ite =   m_lstGun.begin();
	while(ite !=   m_lstGun.end()){   //����ɾ�� new �ڵ�
		if((*ite)){
			(*ite)->MoveGunner(step);

			//�ж��ڵ��Ƿ���磬������ɾ��
			if(  (*ite)->m_y<= - IMG_GUNNER_HEIGHT  ){
				delete (*ite);
				(*ite) = nullptr;   //ɾ��new���ڵ�

				ite = m_lstGun.erase(ite);  //ɾ���ڵ�
				continue;
			}
		}

		ite++;
	}
}