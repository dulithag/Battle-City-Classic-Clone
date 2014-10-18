#ifndef _DEBUG_H
#define _DEBUG_H
#include<iostream>

class CDebug{

	bool m_bIsActive;

	public:
	CDebug():m_bIsActive(true){};

	template<typename T>
	CDebug& operator<<(T info){
		if(m_bIsActive){
			std::cout<< info;
			return *this;
		}
		else{
			return *this;
		}	
	}	
};

#endif
