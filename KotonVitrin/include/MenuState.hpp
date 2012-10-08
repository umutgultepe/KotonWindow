//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "AppState.hpp"
#include "ActiveControlManager.h"
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class MenuState : public AppState
{
public:
    MenuState();
 
	DECLARE_APPSTATE_CLASS(MenuState)
 
	void enter();
	void createScene();
	void exit();
 
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
	Hikari::FlashValue toNewProducts(FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue toAccessories(FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue toMiniGame(FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue toLookBook(FlashControl* caller, const Hikari::Arguments& args);
	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	void changeState(int index);
 
	void update(double timeSinceLastFrame,bool dataIsNew, XnPoint3D handPosition);
private:
	bool m_bQuit;
	int activeItemIndex;
	AppStates targetState;
	Ogre::SceneNode* mParticleNode;
	Ogre::SceneNode* mRainNode;
	Ogre::ParticleSystem* mMouseParticles;
	Ogre::ParticleSystem* mKotonRain;
	FlashControl *mMenuButtons;
	XnPoint3D lastCoordinatesInMenuRegion;
	ActiveControlManager* RegionController;
	AnimationState* mYellowLightAnimState;
	RaySceneQuery* mCursorQuery;
		FlashControl *mErase;
	//For evaluation
	//OgreBites::ParamsPanel* mUserLocation;
		float passedTime;
		Real mTimeSinceLastFreeze;
	bool mWiping;
	Vector2 mBrushPos;
	Real mPlaneSize;
	bool shaked;
	float fadeTimer;
	Ogre::Entity*	mPlaneEnt;
	int textureWidth,textureHeight;
	const unsigned int SQR_BRUSH_RADIUS;
	HardwarePixelBufferSharedPtr mTexBuf;
	SceneNode* pNode;
	void updateTexture(Ogre::uint8 freezeAmount);
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||