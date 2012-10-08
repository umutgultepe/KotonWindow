//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef STAND_BY_STATE_HPP
#define STAND_BY_STATE_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "AppState.hpp"
#include "ActiveControlManager.h"
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class StandByState : public AppState
{
public:
    StandByState();
 
    DECLARE_APPSTATE_CLASS(StandByState)
 
    void enter();
    void createScene();
    void exit();
 
    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);
 
    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
 
    void update(double timeSinceLastFrame,bool dataIsNew, XnPoint3D handPosition);
void SetupVideoMaterial(void);
	void convertIplToTexture(IplImage* img,TexturePtr texture);

 
private:
    bool m_bQuit;
	Ogre::SceneNode* mRainNode;
	Ogre::ParticleSystem* mKotonRain;
	FlashControl *mStandBy;
	FlashControl *mWalkingBitch;
	FlashControl *mExitButton;
	TexturePtr videoTexture;
	CvCapture* videoCapture;
	float videoWidth,
	videoHeight,
	videoFPS,
	frameDelay;
	int pix_size;
	int empty_byte;
	float passedTime;

};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||