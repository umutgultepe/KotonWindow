//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "AccessoriesState.hpp"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
using namespace Ogre;
  extern int m_HalfHeight,m_HalfWidth;
  extern StatisticsManager* gStatistics;
   extern std::vector<Ogre::String> StateNames;
//|||||||||||||||||||||||||||||||||||||||||||||||
 
AccessoriesState::AccessoriesState()
{
    m_bQuit             = false;
    m_FrameEvent        = Ogre::FrameEvent();

}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
  
void AccessoriesState::enter()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering AccessoriesState...");
 
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "AccessoriesSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
 
    m_pCamera = m_pSceneMgr->createCamera("AccessoriesCam");
    createDefaultCamera(m_pCamera);

    //OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "BackToGameBtn", "Return to Game", 250);

 
    m_bQuit = false;
	targetState=AccessoriesStateId;
    createScene();
}

void AccessoriesState::createScene()
{

	mMouseParticles=m_pSceneMgr->createParticleSystem("mouseTail","mouseParticles");
	mMouseParticles->setRenderQueueGroup(104);
	mParticleNode= m_pSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0,0,-45));
	mParticleNode->attachObject(mMouseParticles);



	//Scene Nodes


	//mKotonRain=m_pSceneMgr->createParticleSystem("kotonRain","koton");
	//mKotonRain->setRenderQueueGroup(103);
	//mRainNode= m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
	//mRainNode->attachObject(mKotonRain);
	//mRainNode->translate(-30,m_HalfHeight/ScreenScale*2,-40);
	//mKotonRain->fastForward(5);


	//RegionController=new ActiveControlManager();
	//Ogre::Real reg1[]={0.1,0.1,0.4,0.9};
	//Ogre::Real reg2[]={0.225,0.675};
	//RegionController->createItem("left",reg1,reg2,0,0,0);
	//reg1[0]=0.6;
	//reg1[2]=0.9;
	//reg2[0]=0.775;
	//RegionController->createItem("right",reg1,reg2,0,0,1);
	showDepthPanel();

}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void AccessoriesState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving AccessoriesState...");
 
    m_pSceneMgr->destroyCamera(m_pCamera);
	m_pSceneMgr->destroyAllEntities();
	m_pSceneMgr->destroyAllParticleSystems();
	m_pSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
 
    OgreFramework::getSingletonPtr()->m_pTrayMgr->clearAllTrays();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(0);

}
bool AccessoriesState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
 	if (keyEventRef.key==OIS::KC_ESCAPE)
		targetState=MenuStateId;
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool AccessoriesState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool AccessoriesState::mouseMoved(const OIS::MouseEvent &evt)
{
	mParticleNode->setPosition((m_HalfWidth-evt.state.X.abs)/ScreenScale,(m_HalfHeight-evt.state.Y.abs)/ScreenScale,-45);
	m_pParent->mHikariMgr->injectMouseMove(evt.state.X.abs,evt.state.Y.abs);

    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool AccessoriesState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	m_pParent->mHikariMgr->injectMouseDown(id);
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool AccessoriesState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	m_pParent->mHikariMgr->injectMouseUp(id);
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
    return true;
}

void AccessoriesState::update(double timeSinceLastFrame,bool dataIsNew, XnPoint3D handPosition)
{
	if (targetState!=AccessoriesStateId)
	{
		changeAppState(findByName(StateNames.at(targetState)));
		return;
	}
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);

	if (dataIsNew && handPosition.X>-1)
		mParticleNode->setPosition(getScreenXCoordinate(handPosition.X),getScreenYCoordinate(handPosition.Y),-45);

    if(m_bQuit == true)
    {
        popAppState();
        return;
    }
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||


Hikari::FlashValue AccessoriesState::onExit(FlashControl* caller, const Hikari::Arguments& args)
{
	targetState=MenuStateId;

	return FLASH_VOID;
}

Hikari::FlashValue AccessoriesState::onItemChange(FlashControl* caller, const Hikari::Arguments& args)
{
	return FLASH_VOID;
}

Hikari::FlashValue AccessoriesState::onItemLike(FlashControl* caller, const Hikari::Arguments& args)
{
	Ogre::String targetPicture = args.at(0).getString();
	gStatistics->likedItem(targetPicture);
	return FLASH_VOID;
}