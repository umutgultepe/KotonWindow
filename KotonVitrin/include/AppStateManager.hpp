//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef APP_STATE_MANAGER_HPP
#define APP_STATE_MANAGER_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "AppState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||
 
class AppStateManager : public AppStateListener
{
public:
	typedef struct
	{
		Ogre::String name;
		AppState* state;
	} state_info;
 
	AppStateManager();
	AppStateManager(Ogre::Root* ogreRoot,OgreBites::SdkTrayManager* mTrayMgr);
	~AppStateManager();
 
	void manageAppState(Ogre::String stateName, AppState* state);
 
	AppState* findByName(Ogre::String stateName);
 
	void start(AppState* state);
	void changeAppState(AppState* state);
	bool pushAppState(AppState* state);
	void popAppState();
	void pauseAppState();
	void shutdown();
    void popAllAndPushAppState(AppState* state);
	void loadAllFlashes();

	void startTransferAnimation();
	void updateTransferAnimation(int timeElapsed=-1);
	void stopTransferAnimation();
	std::vector<AppState*>* getActiveStateStack();
	std::vector<state_info>*	getStatesInfo();
	KinectController* getKinect();
	FlashControl* getControlOverlay(Ogre::String name);
protected:
	void init(AppState *state);
	XnPoint3D updateHandInformation();
	FlashControl *mInfoBox;
	FlashControl* mTransfer;
	std::vector<AppState*>		m_ActiveStateStack;
	std::vector<state_info>		m_States;
	std::vector<Ogre::String> mFlashNames;
	std::vector<FlashControl*> mFlashControls;
	bool				m_bShutdown;
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||