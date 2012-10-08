//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef INTEREST_STATE_HPP
#define INTEREST_STATE_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "AppState.hpp"
#include "ActiveControlManager.h"
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class InterestState : public AppState
{
public:
    InterestState();
 
    DECLARE_APPSTATE_CLASS(InterestState)
 
    void enter();
    void createScene();
    void exit();
 
    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);
	void SetupVideoMaterial(void);
	void convertIplToTexture(IplImage* img,TexturePtr texture);
 
    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
 
 
    void update(double timeSinceLastFrame,bool dataIsNew, XnPoint3D handPosition);
 

private:
    bool m_bQuit;
	Ogre::SceneNode* mRainNode;
	Ogre::ParticleSystem* mKotonRain;
	FlashControl *mErase;
	FlashControl *mPush;
	TexturePtr videoTexture;
	
		AppStates targetState;

	
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||