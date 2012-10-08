//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef NEW_PRODUCT_STATE_HPP
#define NEW_PRODUCT_STATE_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "AppState.hpp"
#include "ActiveControlManager.h"
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class NewProductState : public AppState
{
public:
    NewProductState();
 
    DECLARE_APPSTATE_CLASS(NewProductState)
 
    void enter();
    void createScene();
    void exit();
 
    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);
 
    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
 
    void update(double timeSinceLastFrame,bool dataIsNew, XnPoint3D handPosition);
 	Hikari::FlashValue onBackToProducts(FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue onItemChange(FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue onExit(FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue onItemLike(FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue onDebug(FlashControl* caller, const Hikari::Arguments& args);
private:
    bool m_bQuit;
	AppStates targetState;
	Ogre::SceneNode* mParticleNode;
	Ogre::SceneNode* mRainNode;
	Ogre::ParticleSystem* mMouseParticles;
	Ogre::ParticleSystem* mKotonRain;
	FlashControl *mItemSelector;
	FlashControl *mWalkingBitch;
	FlashControl *mExitButton;

	ActiveControlManager* RegionController;
	AnimationState* mYellowLightAnimState;
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||