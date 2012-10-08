//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef APP_STATE_HPP
#define APP_STATE_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "AdvancedOgreFramework.hpp"
#include "KinectController.h" 
#define useKinect 1
class AppState;

#define NUMBER_OF_STATES 9

enum AppStates {
	
	MenuStateId			=0,
	GameStateId			=1,
	NewProductStateId	=2,
	AccessoriesStateId	=3,
	LookBookStateId		=4,
	PhotoSelectionId	=5,
	InterestStateId		=6,
	GuideStateId		=7,
	StandByStateId		=8
};



//|||||||||||||||||||||||||||||||||||||||||||||||
 
class AppStateListener
{
public:
	AppStateListener(){};
	virtual ~AppStateListener(){};
  
	KinectController*	mKinect;
	StatisticsManager* mStatistics;
	Hikari::HikariManager* mHikariMgr;
	virtual void manageAppState(Ogre::String stateName, AppState* state) = 0;
 
	virtual AppState*	findByName(Ogre::String stateName) = 0;
	virtual void		changeAppState(AppState *state) = 0;
	virtual bool		pushAppState(AppState* state) = 0;
	virtual void		popAppState() = 0;
	virtual void		pauseAppState() = 0;
	virtual void		shutdown() = 0;
    virtual void            popAllAndPushAppState(AppState* state) = 0;
	virtual KinectController* getKinect()=0;
	virtual void loadAllFlashes()=0;
	virtual void startTransferAnimation()=0;
	virtual void updateTransferAnimation(int timeElapsed=-1)=0;
	virtual void stopTransferAnimation()=0;
	virtual FlashControl* getControlOverlay(Ogre::String name)=0;

};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class AppState : public OIS::KeyListener, public OIS::MouseListener, public OgreBites::SdkTrayListener
{
public:
	static void create(AppStateListener* parent, const Ogre::String name){};
 
	void destroy(){delete this;}
	int				m_Index;
	virtual void enter() = 0;
	virtual void exit() = 0;
	virtual bool pause(){return true;}
	virtual void resume(){};
	virtual void update(double timeSinceLastFrame,bool dataIsNew, XnPoint3D handPosition) = 0;
 
protected:
	AppState(){};
 
	AppState*	findByName(Ogre::String stateName){return m_pParent->findByName(stateName);}
	void		changeAppState(AppState* state){m_pParent->changeAppState(state);}
	bool		pushAppState(AppState* state){return m_pParent->pushAppState(state);}
	void		popAppState(){m_pParent->popAppState();}
	void		shutdown(){m_pParent->shutdown();}
        void            popAllAndPushAppState(AppState* state){m_pParent->popAllAndPushAppState(state);}
 
	AppStateListener*	m_pParent;
	
	Ogre::Camera*		m_pCamera;
	Ogre::SceneManager*	m_pSceneMgr;
    Ogre::FrameEvent            m_FrameEvent;
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#define DECLARE_APPSTATE_CLASS(T)					\
static void create(AppStateListener* parent, const Ogre::String name, int index)	\
{									\
	T* myAppState = new T();					\
	myAppState->m_pParent = parent;					\
	parent->manageAppState(name, myAppState);			\
	myAppState->m_Index=index;				\
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||
