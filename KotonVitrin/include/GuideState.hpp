//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef GUIDE_STATE_HPP
#define GUIDE_STATE_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "AppState.hpp"
#include "ActiveControlManager.h"
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class GuideState : public AppState
{
public:
    GuideState();
 
    DECLARE_APPSTATE_CLASS(GuideState)
 
    void enter();
    void createScene();
    void exit();
 
    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);
 
    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
 

    void update(double timeSinceLastFrame,bool dataIsNew, XnPoint3D handPosition);
 

private:
    bool m_bQuit;

	Ogre::SceneNode* mRainNode;
	Ogre::ParticleSystem* mKotonRain;
		FlashControl *mStandBy;
	FlashControl *mWalkingBitch;
	FlashControl *mExitButton;

};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||