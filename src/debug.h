#ifndef _DEBUG_H
#define _DEBUG_H
#include <iostream>

class CDebug{

	bool m_bIsActive;

	public:
	CDebug(){
		#ifdef DEBUG
		m_bIsActive = true;
		#else
		m_bIsActive = false;
		#endif
	}
	
	template<typename T>
	CDebug& operator<<(T &info){
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
