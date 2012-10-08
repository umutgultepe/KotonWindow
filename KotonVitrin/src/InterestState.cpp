//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "InterestState.hpp"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
using namespace Ogre;
  extern int m_HalfHeight,m_HalfWidth;
  extern StatisticsManager* gStatistics;
    extern std::vector<Ogre::String> StateNames;


//|||||||||||||||||||||||||||||||||||||||||||||||
 
InterestState::InterestState()
{
	m_bQuit             = false;
    m_FrameEvent        = Ogre::FrameEvent();
}


//|||||||||||||||||||||||||||||||||||||||||||||||
 
void InterestState::enter()
{
	mErase=0;
	mPush=0;

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering InterestState...");
 
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "InterestSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
 
    m_pCamera = m_pSceneMgr->createCamera("InterestCam");
   	createDefaultCamera(m_pCamera);
	targetState=InterestStateId;


	
	
 
    m_bQuit = false;
 
    createScene();
}

void InterestState::createScene()
{

	//mPush=m_pParent->getControlOverlay("Uzat");
	//if (mPush)
	//{
	//	mPush->rewind();
	//	mPush->play();
	//	mPush->show();
	//}



	//pNode->setVisible(false);

	showDepthPanel();

}
 
//|||||||||||||||||||||||||||||||||||||||||||||||







 
void InterestState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving InterestState...");
 
    m_pSceneMgr->destroyCamera(m_pCamera);
	m_pSceneMgr->destroyAllEntities();
	m_pSceneMgr->destroyAllParticleSystems();
	m_pSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
 
    OgreFramework::getSingletonPtr()->m_pTrayMgr->clearAllTrays();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(0);

	//Ogre::TextureManager::getSingleton().remove("erase");
	if (mErase)
	{
		mErase->stop();
		mErase->hide();
	}
	if (mPush)
	{
		mPush->stop();
		mPush->hide();
	}

}
bool InterestState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
 	if (keyEventRef.key==OIS::KC_ESCAPE)
     {
        m_bQuit = true;
        return true;
    }
	
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool InterestState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool InterestState::mouseMoved(const OIS::MouseEvent &evt)
{
	//if(!useKinect)
	//mBrushPos=Ogre::Vector2(textureWidth*evt.state.X.abs/m_HalfWidth/2,textureHeight*evt.state.Y.abs/m_HalfHeight/2);
	if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool InterestState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	//mWiping = true;  // wipe frost if user left clicks in the scene
	m_pParent->mHikariMgr->injectMouseDown(id);
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool InterestState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
		//mWiping = false;  // stop wiping frost if user releases LMB
	m_pParent->mHikariMgr->injectMouseUp(id);
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
    return true;
}

void InterestState::update(double timeSinceLastFrame,bool dataIsNew, XnPoint3D handPosition)
{



	if (targetState!=InterestStateId)
	{
		changeAppState(findByName(StateNames.at(targetState)));
		return;
	}




	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
	//Video processing ---------------------------

	
	//----------------------------
	//Video processing ---------------------------
    if(m_bQuit == true)
    {
        popAppState();
        return;
    }
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
