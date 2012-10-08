//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef LOOK_BOOK_STATE_HPP
#define LOOK_BOOK_STATE_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "AppState.hpp"
#include "ActiveControlManager.h"
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class LookBookState : public AppState
{
public:
    LookBookState();
 
    DECLARE_APPSTATE_CLASS(LookBookState)
 
    void enter();
    void createScene();
    void exit();
 
    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);
 
    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
 
    void buttonHit(OgreBites::Button* button);
    void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);
 
    void update(double timeSinceLastFrame,bool dataIsNew, XnPoint3D handPosition);
	
 
	Hikari::FlashValue onPictureClick(FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue onPictureLike(FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue onExit(FlashControl* caller, const Hikari::Arguments& args);

private:
    bool m_bQuit;
	Ogre::SceneNode* mParticleNode;
	Ogre::SceneNode* mRainNode;
	Ogre::SceneNode* objectNode;
		AppStates targetState;
	Ogre::ParticleSystem* mMouseParticles;
	Ogre::ParticleSystem* mKotonRain;
	FlashControl *mLookBook;
	FlashControl *mWalkingBitch;
	FlashControl *mExitButton;

	ActiveControlManager* RegionController;
	AnimationState* mYellowLightAnimState;
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||