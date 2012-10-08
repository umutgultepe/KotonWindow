//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "GuideState.hpp"
 
extern Ogre::ParticleSystem* gKotonRain;
//|||||||||||||||||||||||||||||||||||||||||||||||
 
using namespace Ogre;
  extern int m_HalfHeight,m_HalfWidth;
  extern StatisticsManager* gStatistics;
  extern Ogre::OverlayElement* gRegionGuide;
//|||||||||||||||||||||||||||||||||||||||||||||||
 
GuideState::GuideState()
{
    m_bQuit             = false;
    m_FrameEvent        = Ogre::FrameEvent();

}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void GuideState::enter()
{
		
	mStandBy=0;
	mExitButton=0;
	mWalkingBitch=0;
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering GuideState...");
 
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "GuideSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
 
    m_pCamera = m_pSceneMgr->createCamera("GuideCam");
   	createDefaultCamera(m_pCamera);
    //OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "BackToGameBtn", "Return to Game", 250);
	//gRegionGuide->setHorizontalAlignment(GHA_CENTER);
	//gRegionGuide->setVerticalAlignment(GVA_CENTER);
	//gRegionGuide->setLeft(-190);
	//gRegionGuide->setTop(-175);
 
    m_bQuit = false;
 
    createScene();
}

void GuideState::createScene()
{

	//mKotonRain=m_pSceneMgr->createParticleSystem("kotonRain","koton");
	//mKotonRain->setRenderQueueGroup(103);
	//mKotonRain->fastForward(5);
	//mRainNode= m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
	//mRainNode->attachObject(mKotonRain);
	//mRainNode->translate(-30,m_HalfHeight/ScreenScale*2,-40);
	//


	// 
	//Ogre::Entity* ogreHead = m_pSceneMgr->createEntity("Head", "ogrehead.mesh");

 //   Ogre::SceneNode* headNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
 //   headNode->attachObject(ogreHead);

	mStandBy=m_pParent->getControlOverlay("Interest");
	if (mStandBy)
	{
		mStandBy->rewind();
		mStandBy->play();
		mStandBy->show();
	}
	showDepthPanel();

}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void GuideState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving GuideState...");
 
    m_pSceneMgr->destroyCamera(m_pCamera);
	m_pSceneMgr->destroyAllEntities();
	m_pSceneMgr->destroyAllParticleSystems();
	m_pSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
 
    OgreFramework::getSingletonPtr()->m_pTrayMgr->clearAllTrays();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(0);

	if (mStandBy)
	{
		mStandBy->hide();
		mStandBy->stop();
	}
	//gRegionGuide->setHorizontalAlignment(GHA_RIGHT);
	//gRegionGuide->setVerticalAlignment(GVA_TOP);
	//gRegionGuide->setLeft(-380);
	//gRegionGuide->setTop(0);
}
bool GuideState::keyPressed(const OIS::KeyEvent &keyEventRef)
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
 
bool GuideState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool GuideState::mouseMoved(const OIS::MouseEvent &evt)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool GuideState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool GuideState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
    return true;
}

void GuideState::update(double timeSinceLastFrame,bool dataIsNew, XnPoint3D handPosition)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);

    if(m_bQuit == true)
    {
        popAppState();
        return;
    }
}
 
