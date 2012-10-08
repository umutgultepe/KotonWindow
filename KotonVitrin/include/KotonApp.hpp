//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef OGRE_DEMO_HPP
#define OGRE_DEMO_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h" 
#include "AdvancedOgreFramework.hpp"
#include "AppStateManager.hpp"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class KotonApp
{
public:
	KotonApp();
	~KotonApp();
 
	void startDemo();
 
private:
	AppStateManager*	m_pAppStateManager;
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||