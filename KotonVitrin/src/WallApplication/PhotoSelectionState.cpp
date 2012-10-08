//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "PhotoSelectionState.hpp"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 


using namespace Ogre;
extern std::vector<Image*> currentImages;
 extern int m_HalfHeight,m_HalfWidth;
 extern StatisticsManager* gStatistics;
   extern std::vector<Ogre::String> StateNames;
 int numberOfSharedPictures=0;
//|||||||||||||||||||||||||||||||||||||||||||||||
 
PhotoSelectionState::PhotoSelectionState()
{
    m_bQuit             = false;
    m_FrameEvent        = Ogre::FrameEvent();

}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void PhotoSelectionState::enter()
{
	mLookBook=0;
	mExitButton=0;
	mShareButton=0;
	mWalkingBitch=0;
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering PhotoSelectionState...");
 
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "PhotoSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
 
    m_pCamera = m_pSceneMgr->createCamera("PhotoCam");
	createDefaultCamera(m_pCamera);

    m_bQuit = false;
	targetState=PhotoSelectionId;
    createScene();
}

void PhotoSelectionState::createScene()
{

	mMouseParticles=m_pSceneMgr->createParticleSystem("mouseTail","mouseParticles");
	mMouseParticles->setRenderQueueGroup(104);

	mParticleNode= m_pSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0,0,-45));
	mParticleNode->attachObject(mMouseParticles);

	//mKotonRain=m_pSceneMgr->createParticleSystem("kotonRain","koton");
	//mKotonRain->setRenderQueueGroup(103);
	//mRainNode= m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
	//mRainNode->attachObject(mKotonRain);
	//mRainNode->translate(-30,m_HalfHeight/ScreenScale*2,-40);
	//mKotonRain->fastForward(5);




	showDepthPanel();



}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void PhotoSelectionState::exit()
{

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving PhotoSelectionState...");
 
    m_pSceneMgr->destroyCamera(m_pCamera);
	m_pSceneMgr->destroyAllEntities();
	m_pSceneMgr->destroyAllParticleSystems();
	m_pSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
 
    OgreFramework::getSingletonPtr()->m_pTrayMgr->clearAllTrays();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(0);

	for (unsigned int i=0;i<currentImages.size();i++)
	{
		delete currentImages.at(i);
		String tString("TemporaryImage"+StringConverter::toString(i+1)+".jpg");
		remove(tString.c_str());
	}
	currentImages.clear();
}
bool PhotoSelectionState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
 
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
		targetState=MenuStateId;
        return true;
    }
	else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_SPACE))
    {
		popAppState();
        return true;
    }
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool PhotoSelectionState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool PhotoSelectionState::mouseMoved(const OIS::MouseEvent &evt)
{
	mParticleNode->setPosition((m_HalfWidth-evt.state.X.abs)/ScreenScale,(m_HalfHeight-evt.state.Y.abs)/ScreenScale,-45);
	m_pParent->mHikariMgr->injectMouseMove(evt.state.X.abs,evt.state.Y.abs);

    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool PhotoSelectionState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	m_pParent->mHikariMgr->injectMouseDown(id);
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool PhotoSelectionState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	m_pParent->mHikariMgr->injectMouseUp(id);
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
    return true;
}

void PhotoSelectionState::update(double timeSinceLastFrame,bool dataIsNew, XnPoint3D handPosition)
{
	if (targetState!=PhotoSelectionId)
	{
		popAllAndPushAppState(findByName(StateNames.at(targetState)));
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
 


Hikari::FlashValue PhotoSelectionState::onExit(FlashControl* caller, const Hikari::Arguments& args)
{


	popAllAndPushAppState(findByName("MenuState"));
	return FLASH_VOID;
}

Hikari::FlashValue PhotoSelectionState::onShare(FlashControl* caller, const Hikari::Arguments& args)
{

	int targetPicture = args.at(0).getNumber();
	currentImages.at(targetPicture)->save("SharedImage"+StringConverter::toString(numberOfSharedPictures)+"jpg");
	gStatistics->sharedPicture();
	numberOfSharedPictures++;
	return FLASH_VOID;
}