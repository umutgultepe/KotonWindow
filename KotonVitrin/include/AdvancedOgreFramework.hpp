//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef OGRE_FRAMEWORK_HPP
#define OGRE_FRAMEWORK_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#ifndef ScreenScale
#define ScreenScale 10
#endif
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class OgreFramework : public Ogre::Singleton<OgreFramework>, OIS::KeyListener, OIS::MouseListener
{
public:
	OgreFramework();
	~OgreFramework();
 
	bool initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener = 0, OIS::MouseListener *pMouseListener = 0);
	void updateOgre(double timeSinceLastFrame);
	
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
 
	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
 
	Ogre::Root*				m_pRoot;
	Ogre::RenderWindow*			m_pRenderWnd;
	Ogre::Viewport*				m_pViewport;
	Ogre::Log*				m_pLog;
	Ogre::Timer*				m_pTimer;
 
	OIS::InputManager*			m_pInputMgr;
	OIS::Keyboard*				m_pKeyboard;
	OIS::Mouse*				m_pMouse;
 
    OgreBites::SdkTrayManager*	        m_pTrayMgr;
 
private:
	OgreFramework(const OgreFramework&);
	OgreFramework& operator= (const OgreFramework&);
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
Ogre::Real getScreenXSize(Ogre::Real relativeXSize);
Ogre::Real getScreenYSize(Ogre::Real relativeYSize);
Ogre::Real getScreenYCoordinate(Ogre::Real relativeYCoordinate);
Ogre::Real getScreenXCoordinate(Ogre::Real relativeXCoordinate);
void showDepthPanel();
void createDefaultCamera( Ogre::Camera* mCamera);
Ogre::TextureUnitState* showPassPanel();
void hidePassPanel();
std::wstring StringToWString(const std::string& s);
std::string WStringToString(const std::wstring& s);
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||