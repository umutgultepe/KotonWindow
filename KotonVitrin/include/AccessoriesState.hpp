//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef ACCESSORIES_STATE_HPP
#define ACCESSORIES_STATE_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "AppState.hpp"
#include "ActiveControlManager.h"
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class AccessoriesState : public AppState
{
public:
    AccessoriesState();
 
    DECLARE_APPSTATE_CLASS(AccessoriesState)
 
    void enter();
    void createScene();
    void exit();
 
    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);
 
    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
 
 
    void update(double timeSinceLastFrame,bool dataIsNew, XnPoint3D handPosition);
 
	Hikari::FlashValue onItemChange(FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue onExit(FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue onItemLike(FlashControl* caller, const Hikari::Arguments& args);
private:
    bool m_bQuit;
	Ogre::SceneNode* mParticleNode;
	Ogre::SceneNode* mRainNode;
	Ogre::ParticleSystem* mMouseParticles;
	Ogre::ParticleSystem* mKotonRain;
	AppStates targetState;
	ActiveControlManager* RegionController;
	AnimationState* mYellowLightAnimState;
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||