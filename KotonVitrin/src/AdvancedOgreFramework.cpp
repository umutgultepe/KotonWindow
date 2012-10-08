//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "AdvancedOgreFramework.hpp"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
using namespace Ogre;
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
template<> OgreFramework* Ogre::Singleton<OgreFramework>::ms_Singleton = 0;
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
int m_HalfHeight,m_HalfWidth;

OgreFramework::OgreFramework()
{
    m_pRoot			= 0;
    m_pRenderWnd		= 0;
    m_pViewport			= 0;
    m_pLog			= 0;
    m_pTimer			= 0;
 
    m_pInputMgr			= 0;
    m_pKeyboard			= 0;
    m_pMouse			= 0;
    m_pTrayMgr			= 0;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||

OgreFramework::~OgreFramework()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");
    if(m_pTrayMgr)              delete m_pTrayMgr;
    if(m_pInputMgr)		OIS::InputManager::destroyInputSystem(m_pInputMgr);
    if(m_pRoot)			delete m_pRoot;
}

bool OgreFramework::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
{
    Ogre::LogManager* logMgr = new Ogre::LogManager();
 
    m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
    m_pLog->setDebugOutputEnabled(true);
	
 	#ifdef _DEBUG
    m_pRoot = new Ogre::Root("plugins_d.cfg","ogre.cfg");
	#else
	m_pRoot = new Ogre::Root();
	#endif
    if(!m_pRoot->showConfigDialog())
        return false;
    m_pRenderWnd = m_pRoot->initialise(true, wndTitle);
 
    m_pViewport = m_pRenderWnd->addViewport(0);
    m_pViewport->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
  
    m_pViewport->setCamera(0);
 
    unsigned long hWnd = 0;
    OIS::ParamList paramList;
    m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);
	//--------------------------
	//m_pRenderWnd->resize(m_pRenderWnd->getHeight(),m_pRenderWnd->getHeight());
   // m_pRenderWnd->resize(1080,1920);
	
	paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));
 
    m_pInputMgr = OIS::InputManager::createInputSystem(paramList);
 
    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
    m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));
 
    m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
    m_pMouse->getMouseState().width  = m_pRenderWnd->getWidth();
 
    if(pKeyListener == 0)
        m_pKeyboard->setEventCallback(this);
    else
        m_pKeyboard->setEventCallback(pKeyListener);
 
    if(pMouseListener == 0)
        m_pMouse->setEventCallback(this);
    else
        m_pMouse->setEventCallback(pMouseListener);
 
    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
	#ifdef _DEBUG
    cf.load("resources_d.cfg");
	#else
	cf.load("resources.cfg");
	#endif
 
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
 
    m_pTrayMgr = new OgreBites::SdkTrayManager("AOFTrayMgr", m_pRenderWnd, m_pMouse, 0);
 
    m_pTimer = new Ogre::Timer();
    m_pTimer->reset();
 
	

    m_pRenderWnd->setActive(true);
 
	m_HalfWidth=OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()/2;
	m_HalfHeight= OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()/2;
	m_pTrayMgr->showFrameStats(OgreBites::TrayLocation::TL_BOTTOMLEFT);
    return true;
}

bool OgreFramework::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(m_pKeyboard->isKeyDown(OIS::KC_SYSRQ))
    {
        m_pRenderWnd->writeContentsToTimestampedFile("AOF_Screenshot_", ".jpg");
        return true;
    }
 
    if(m_pKeyboard->isKeyDown(OIS::KC_O))
    {
        if(m_pTrayMgr->isLogoVisible())
        {
            m_pTrayMgr->hideFrameStats();
            m_pTrayMgr->hideLogo();
        }
        else
        {
            m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
         //   m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
        }
    }
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool OgreFramework::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool OgreFramework::mouseMoved(const OIS::MouseEvent &evt)
{
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool OgreFramework::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool OgreFramework::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}

void OgreFramework::updateOgre(double timeSinceLastFrame)
{
}

Ogre::Real getScreenYSize(Ogre::Real relativeYSize)
{
	return relativeYSize*m_HalfHeight*2/ScreenScale;
}

Ogre::Real getScreenXSize(Ogre::Real relativeXSize)
{
	return relativeXSize*m_HalfWidth*2/ScreenScale;
}

Ogre::Real getScreenYCoordinate(Ogre::Real relativeYCoordinate)
{
	return (m_HalfHeight-relativeYCoordinate*m_HalfHeight*2)/ScreenScale;
}

Ogre::Real getScreenXCoordinate(Ogre::Real relativeXCoordinate)
{
	return (m_HalfWidth-relativeXCoordinate*m_HalfWidth*2)/ScreenScale;
}

void showDepthPanel(){

		OgreBites::SdkTrayManager* mTrayMgr=OgreFramework::getSingletonPtr()->m_pTrayMgr;
		if(!Ogre::OverlayManager::getSingleton().hasOverlayElement("DepthPanel"))
	{
		Ogre::OverlayElement* mDepthPanel = Ogre::OverlayManager::getSingleton().createOverlayElement("Panel","DepthPanel");
		mDepthPanel->setMaterialName("DepthTextureMaterial");
		mDepthPanel->setMetricsMode(Ogre::GMM_RELATIVE);
		mDepthPanel->setWidth(0.25);
		mDepthPanel->setHeight(0.25*480/640);
		mDepthPanel->setHorizontalAlignment(GHA_RIGHT);
		mDepthPanel->setVerticalAlignment(GVA_BOTTOM);
		mDepthPanel->setLeft(-mDepthPanel->getWidth());
		mDepthPanel->setTop(-mDepthPanel->getHeight());
 		mTrayMgr->getTraysLayer()->add2D((Ogre::OverlayContainer*)mDepthPanel);
		mDepthPanel->show();
	}
	else
	{
		Ogre::OverlayElement*	mDepthPanel=Ogre::OverlayManager::getSingleton().getOverlayElement("DepthPanel");
		mDepthPanel->show();
	}

}

Ogre::TextureUnitState* showPassPanel(){

		OgreBites::SdkTrayManager* mTrayMgr=OgreFramework::getSingletonPtr()->m_pTrayMgr;
		if(!Ogre::OverlayManager::getSingleton().hasOverlayElement("PassPanel"))
	{
		Ogre::OverlayElement* mPassPanel = Ogre::OverlayManager::getSingleton().createOverlayElement("Panel","PassPanel");
		mPassPanel->setColour(Ogre::ColourValue(0,0,0,0));
		Ogre::MaterialPtr passMat=Ogre::MaterialManager::getSingleton().create("pass_panel_mat",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		Ogre::TextureUnitState* texState=passMat->getTechnique(0)->getPass(0)->createTextureUnitState("Background.png");
		passMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA,Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
		texState->setAlphaOperation(LBX_DOTPRODUCT, LBS_MANUAL,LBS_TEXTURE , 0.0);
		
		mPassPanel->setMaterialName("pass_panel_mat");
		mPassPanel->setMetricsMode(Ogre::GMM_RELATIVE);
		mPassPanel->setWidth(1);
		mPassPanel->setHeight(1);
		mPassPanel->setHorizontalAlignment(GHA_CENTER);
		mPassPanel->setVerticalAlignment(GVA_CENTER);
		mPassPanel->setLeft(-0.5);
		mPassPanel->setTop(-0.5);
		
 		mTrayMgr->getTraysLayer()->add2D((Ogre::OverlayContainer*)mPassPanel);
		mPassPanel->show();
		return texState;
	}
	else
	{
		Ogre::OverlayElement*	mPassPanel=Ogre::OverlayManager::getSingleton().getOverlayElement("PassPanel");
		mPassPanel->show();

		return mPassPanel->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0);
	}
}


void hidePassPanel()
{
	Ogre::OverlayElement*	mPassPanel=Ogre::OverlayManager::getSingleton().getOverlayElement("PassPanel");
	mPassPanel->hide();

}


void createDefaultCamera( Ogre::Camera* mCamera)
{
    
    mCamera->setPosition(Vector3(0, 0, -50));
    mCamera->lookAt(Vector3(0, 0, 0));
    mCamera->setNearClipDistance(1);
	mCamera->setProjectionType(PT_ORTHOGRAPHIC);
	mCamera->setOrthoWindow(m_HalfWidth*2/ScreenScale,m_HalfHeight*2/ScreenScale);
    mCamera->setAspectRatio((float)m_HalfWidth/m_HalfHeight);
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(mCamera);
 
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();
}

std::string WStringToString(const std::wstring& s)
{
std::string temp(s.length(), ' ');
std::copy(s.begin(), s.end(), temp.begin());
return temp; 
}


std::wstring StringToWString(const std::string& s)
{
std::wstring temp(s.length(),L' ');
std::copy(s.begin(), s.end(), temp.begin());
return temp; 
}


