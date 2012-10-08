//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "LookBookState.hpp"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
using namespace Ogre;
extern int m_HalfHeight,m_HalfWidth;
extern StatisticsManager* gStatistics;
extern std::vector<Ogre::String> StateNames;
extern XnPoint3D gHeadPosition;
//|||||||||||||||||||||||||||||||||||||||||||||||
 
LookBookState::LookBookState()
{
    m_bQuit		= false;
    m_FrameEvent= Ogre::FrameEvent();

}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void LookBookState::enter()
{
		mLookBook=0;
	mExitButton=0;
	mWalkingBitch=0;
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering LookBookState...");
 
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "LookBookSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
 
    m_pCamera = m_pSceneMgr->createCamera("LookBookCam");
	m_pCamera->setPosition(Ogre::Vector3(0,0,80));
    // Look back along -Z
    m_pCamera->lookAt(Ogre::Vector3(0,0,-300));
    m_pCamera->setNearClipDistance(5);
	   m_pCamera->setAspectRatio((float)m_HalfWidth/m_HalfHeight);
  //  createDefaultCamera(m_pCamera);
    //OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "BackToGameBtn", "Return to Game", 250);
	    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
 
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();


	targetState=LookBookStateId;
    m_bQuit = false;
 
    createScene();
}

void LookBookState::createScene()
{


	//mKotonRain=m_pSceneMgr->createParticleSystem("kotonRain","koton");
	//mKotonRain->setRenderQueueGroup(103);
	//mKotonRain->fastForward(5);
	//mRainNode= m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
	//mRainNode->attachObject(mKotonRain);
	//mRainNode->translate(-30,m_HalfHeight/ScreenScale*2,-40);
	

	Ogre::Entity* ogreHead = m_pSceneMgr->createEntity("Head", "X6.mesh");

   objectNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
    objectNode->attachObject(ogreHead);
	objectNode->pitch(-Ogre::Radian(Ogre::Math::PI/2));
	objectNode->scale(10,10,10);

	mMouseParticles=m_pSceneMgr->createParticleSystem("mouseTail","mouseParticles");
	mMouseParticles->setRenderQueueGroup(104);
	mParticleNode= m_pSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0,0,-45));
	mParticleNode->attachObject(mMouseParticles);

	
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // Create a light
    Ogre::Light* l = m_pSceneMgr->createLight("MainLight");
    l->setPosition(20,50,50);

	//RegionController=new ActiveControlManager();
	//Ogre::Real reg1[]={0.1,0.1,0.4,0.9};
	//Ogre::Real reg2[]={0.225,0.675};
	//RegionController->createItem("left",reg1,reg2,0,0,0);
	//reg1[0]=0.6;
	//reg1[2]=0.9;
	//reg2[0]=0.775;
	//RegionController->createItem("right",reg1,reg2,0,0,1);

	//mLookBook=m_pParent->getControlOverlay("LookBook");
	//if (mLookBook)
	//{
	//	mLookBook->bind(StringToWString("backToMainMenu"), Hikari::FlashDelegate(this, &LookBookState::onExit));
	//	mLookBook->bind(StringToWString("getClickedItemName"), Hikari::FlashDelegate(this, &LookBookState::onPictureClick));
	//	mLookBook->rewind();
	//	mLookBook->play();
	//	mLookBook->show();
	//}



	showDepthPanel();

}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void LookBookState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving LookBookState...");
 
    m_pSceneMgr->destroyCamera(m_pCamera);
	m_pSceneMgr->destroyAllEntities();
	m_pSceneMgr->destroyAllParticleSystems();
	m_pSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
 
    OgreFramework::getSingletonPtr()->m_pTrayMgr->clearAllTrays();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(0);

	if (mLookBook)
	{
		mLookBook->unbind(StringToWString("getClickedItemName"));
		//mLookBook->unbind(StringToWString("backToEnYeniler"));
		mLookBook->unbind(StringToWString("backToMainMenu"));
		mLookBook->hide();
		mLookBook->stop();
	}

	//if (mExitButton)
	//	m_pParent->mHikariMgr->destroyFlashControl(mExitButton);
	//if (mWalkingBitch)
	//	m_pParent->mHikariMgr->destroyFlashControl(mWalkingBitch);
	//if (mLookBook)
	//	m_pParent->mHikariMgr->destroyFlashControl(mLookBook);
}
bool LookBookState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
 	if (keyEventRef.key==OIS::KC_ESCAPE)
		targetState=MenuStateId;
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool LookBookState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool LookBookState::mouseMoved(const OIS::MouseEvent &evt)
{
	mParticleNode->setPosition((m_HalfWidth-evt.state.X.abs)/ScreenScale,(m_HalfHeight-evt.state.Y.abs)/ScreenScale,-45);

//	objectNode->setPosition((m_HalfWidth-evt.state.X.abs)/ScreenScale,(m_HalfHeight-evt.state.Y.abs)/ScreenScale,0);
	//if (RegionController->highlight(evt.state.X.abs,evt.state.Y.abs,mSelectorNode,m_HalfWidth*2,m_HalfHeight*2))
	//{
	//	//This means the hovered item has changed
	//}
	m_pParent->mHikariMgr->injectMouseMove(evt.state.X.abs,evt.state.Y.abs);

    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool LookBookState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	m_pParent->mHikariMgr->injectMouseDown(id);
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool LookBookState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	m_pParent->mHikariMgr->injectMouseUp(id);
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
    return true;
}

void LookBookState::update(double timeSinceLastFrame,bool dataIsNew, XnPoint3D handPosition)
{
	if (targetState!=LookBookStateId)
	{
		changeAppState(findByName(StateNames.at(targetState)));
		return;
	}
	
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
	
	if (dataIsNew && handPosition.X>-1)
		mParticleNode->setPosition(getScreenXCoordinate(handPosition.X),getScreenYCoordinate(handPosition.Y),-45);

	objectNode->setPosition(getScreenXCoordinate(gHeadPosition.X/640),(m_HalfHeight/ScreenScale-2*getScreenYCoordinate(gHeadPosition.Y/480)),0);
    if(m_bQuit == true)
    {
        popAppState();
        return;
    }
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void LookBookState::buttonHit(OgreBites::Button *button)
{
    /*if(button->getName() == "ExitBtn")
    {
        OgreFramework::getSingletonPtr()->m_pTrayMgr->showYesNoDialog("Sure?", "Really leave?");
        m_bQuestionActive = true;
    }*/

}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void LookBookState::yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit)
{
//    if(yesHit == true)
//        shutdown();
//    else
//        OgreFramework::getSingletonPtr()->m_pTrayMgr->closeDialog();
// 
//    m_bQuestionActive = false;
}

Hikari::FlashValue LookBookState::onExit(FlashControl* caller, const Hikari::Arguments& args)
{
	targetState=MenuStateId;
	return FLASH_VOID;
}

Hikari::FlashValue LookBookState::onPictureClick(FlashControl* caller, const Hikari::Arguments& args)
{
	Ogre::String targetPicture = WStringToString( args.at(0).getWString());
	gStatistics->clickedItem(targetPicture);
	return FLASH_VOID;
}

Hikari::FlashValue LookBookState::onPictureLike(FlashControl* caller, const Hikari::Arguments& args)
{
	Ogre::String targetPicture = WStringToString( args.at(0).getWString());
	gStatistics->likedItem(targetPicture);
	return FLASH_VOID;
}