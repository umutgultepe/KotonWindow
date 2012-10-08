//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "AppState.hpp"
#include "IncomingWall.h"

 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
enum status
{
	PROGRESS,
	FAILURE,
	SUCCESS,
	DEATH,
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class GameState : public AppState
{
public:
	GameState();
 
	DECLARE_APPSTATE_CLASS(GameState)
 
	void enter();
	void createScene();
	void exit();
	bool pause();
	void resume();
 
	void moveCamera();
    void buildGUI();
 
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
 
	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
 
	void onLeftPressed(const OIS::MouseEvent &evt);
 
	void update(double timeSinceLastFrame,bool dataIsNew, XnPoint3D handPosition);
 
private:
	Ogre::Vector3			m_TranslateVector;
	Ogre::Real			m_MoveSpeed;
	Ogre::Degree			m_RotateSpeed;
	float				m_MoveScale;
	Ogre::Degree			m_RotScale;
 
	bool				m_bQuit;
 	void createHoleRoom(Real roomWidth,Real roomHeight);
	bool processWall(const FrameEvent& evt,float outPerc);
	void setupContent();
	void buildPanels();
	AppStates targetState;
	Ogre::Overlay* photoOverlay;
	Ogre::OverlayElement* mDepthPanel;
	Ogre::OverlayElement* mSuccess;
	Ogre::OverlayElement* mLives[3];
	Ogre::OverlayElement* mFailure;
	Ogre::OverlayElement* mDeath;
	Ogre::OverlayElement* mStart;
	Ogre::OverlayElement* mPhoto;
	OgreBites::ParamsPanel* help;
	std::vector<Ogre::String> entityList;
	status mStatus;
	float LogoTime;
	bool				m_bLMouseDown, m_bRMouseDown;
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||