//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "NewProductState.hpp"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
using namespace Ogre;
  extern int m_HalfHeight,m_HalfWidth;
 extern StatisticsManager* gStatistics;
 extern std::vector<Ogre::String> StateNames;
  	extern int screenWidth;
	extern int screenHeight;

	int clickedState=-1;
	int alignment=-1;
//|||||||||||||||||||||||||||||||||||||||||||||||
 
NewProductState::NewProductState()
{
    m_bQuit             = false;
    m_FrameEvent        = Ogre::FrameEvent();

}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void NewProductState::enter()
{
	mItemSelector=0;
	mExitButton=0;
	mWalkingBitch=0;
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering NewProductState...");
 
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "NewProductSceneMgr");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
	m_pCamera = m_pSceneMgr->createCamera("NewProductCam");
	
	createDefaultCamera(m_pCamera);

    m_bQuit = false;
	targetState=NewProductStateId;
    createScene();
}

void NewProductState::createScene()
{

	mMouseParticles=m_pSceneMgr->createParticleSystem("mouseTail","mouseParticles");
	mMouseParticles->setRenderQueueGroup(104);

	//Scene Nodes
	mParticleNode= m_pSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0,0,-45));
	mParticleNode->attachObject(mMouseParticles);
	
	mItemSelector=0;
	mItemSelector=m_pParent->getControlOverlay("ItemSelector");
	if (mItemSelector)
	{
		mItemSelector->bind(StringToWString("getClickedItemName"), Hikari::FlashDelegate(this, &NewProductState::onItemChange));
		mItemSelector->bind(StringToWString("backToMainMenu"), Hikari::FlashDelegate(this, &NewProductState::onExit));
		mItemSelector->bind(StringToWString("backToEnYeniler"), Hikari::FlashDelegate(this, &NewProductState::onBackToProducts));
		mItemSelector->bind(StringToWString("Debug"), Hikari::FlashDelegate(this, &NewProductState::onBackToProducts));
		mItemSelector->rewind();
		mItemSelector->play();
		mItemSelector->show();
		mItemSelector->callFunction(StringToWString("resetFlash"));
	}
	

	showDepthPanel();

}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void NewProductState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving NewProductState...");
 
    m_pSceneMgr->destroyCamera(m_pCamera);
	m_pSceneMgr->destroyAllEntities();
	m_pSceneMgr->destroyAllParticleSystems();
	m_pSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
 
    OgreFramework::getSingletonPtr()->m_pTrayMgr->clearAllTrays();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(0);

	if (mExitButton)
		m_pParent->mHikariMgr->destroyFlashControl(mExitButton);
	if (mWalkingBitch)
		m_pParent->mHikariMgr->destroyFlashControl(mWalkingBitch);
	if (mItemSelector)
	{
		//mItemSelector->callFunction(StringToWString("resetFlash"));
		mItemSelector->unbind(StringToWString("getClickedItemName"));
		mItemSelector->unbind(StringToWString("backToEnYeniler"));
		mItemSelector->unbind(StringToWString("backToMainMenu"));
		mItemSelector->hide();
		mItemSelector->stop();
	}
}
bool NewProductState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	if (keyEventRef.key==OIS::KC_ESCAPE)
		targetState=MenuStateId;

 
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool NewProductState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool NewProductState::mouseMoved(const OIS::MouseEvent &evt)
{
	mParticleNode->setPosition((m_HalfWidth-evt.state.X.abs)/ScreenScale,(m_HalfHeight-evt.state.Y.abs)/ScreenScale,-45);
	m_pParent->mHikariMgr->injectMouseMove(evt.state.X.abs,evt.state.Y.abs);

    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool NewProductState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	m_pParent->mHikariMgr->injectMouseDown(id);
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool NewProductState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	m_pParent->mHikariMgr->injectMouseUp(id);
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
    return true;
}

void NewProductState::update(double timeSinceLastFrame,bool dataIsNew, XnPoint3D handPosition)
{
	if (targetState!=NewProductStateId)
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

Hikari::FlashValue NewProductState::onExit(FlashControl* caller, const Hikari::Arguments& args)
{
	targetState=MenuStateId;
	return FLASH_VOID;
}

Hikari::FlashValue NewProductState::onItemChange(FlashControl* caller, const Hikari::Arguments& args)
{
	Ogre::String targetPicture = WStringToString( args.at(0).getWString());
	if (useKinect)
	gStatistics->clickedItem(targetPicture);
	int tempID=(int)args.at(1).getNumber();
	if (mItemSelector)
	{
		mItemSelector->callFunction(StringToWString("setStage"),Hikari::Args((int)tempID));
		clickedState=tempID;
		alignment=(int)args.at(2).getNumber();
	}
	return FLASH_VOID;
}

Hikari::FlashValue NewProductState::onItemLike(FlashControl* caller, const Hikari::Arguments& args)
{

	return FLASH_VOID;
}

Hikari::FlashValue NewProductState::onBackToProducts(FlashControl* caller, const Hikari::Arguments& args)
{
	if (clickedState!=-1)
	{
		mItemSelector->callFunction(StringToWString("resetTab"),Hikari::Args(clickedState)(alignment));

		mItemSelector->callFunction(StringToWString("resetTab2"),Hikari::Args(clickedState)(alignment));
		clickedState=-1;
		alignment=-1;
	}
	return FLASH_VOID;
}

Hikari::FlashValue NewProductState::onDebug(FlashControl* caller, const Hikari::Arguments& args)
{
	int a =0;
	return FLASH_VOID;
}
