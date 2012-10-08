//|||||||||||
//||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "AppStateManager.hpp"
#include "IncomingWall.h"
 
#define HikariPeriod 20
//|||||||||||||||||||||||||||||||||||||||||||||||
AppStateManager* gManager;
StatisticsManager* gStatistics;
bool mousePressed=false;
FlashControl* gRegionFlash;
	int screenWidth;
	int screenHeight;
	extern int m_HalfHeight,m_HalfWidth;
AppStateManager::AppStateManager(Ogre::Root* ogreRoot,OgreBites::SdkTrayManager* mTrayMgr)
{
	m_bShutdown = false;
	
	mStatistics=new StatisticsManager(ogreRoot,"KotonVitrin");
	gStatistics=mStatistics;
	mHikariMgr = new Hikari::HikariManager();
#ifdef _DEBUG
	mHikariMgr->setPath("Data\\FlashContent");
#else
	mHikariMgr->setPath("..\\debug\\Data\\FlashContent");
#endif
	mTransfer=0;
	mInfoBox=0;
	loadAllFlashes();
	gManager=this;
	mKinect=0;
	if (useKinect)
	{
		mKinect=new KinectController(false,mStatistics);	
		mKinect->createRTT(ogreRoot,mTrayMgr);
	}
//	mHikariMgr = new Hikari::HikariManager("Data\\FlashContent");

	//New Hikari!!
#ifdef _DEBUG
	mStatistics->loadItemNames("Data\\FlashContent");
	#else
	mStatistics->loadItemNames("..\\debug\\Data\\FlashContent");
#endif

	//mTransfer=0;
	screenHeight=OgreFramework::getSingleton().m_pViewport->getActualHeight();
	screenWidth=OgreFramework::getSingleton().m_pViewport->getActualWidth();
	
}
 

FlashControl* AppStateManager::getControlOverlay(Ogre::String name)

{
	FlashControl* temp=0;

	for (unsigned int i=0;i<mFlashControls.size();i++)
	{
		if (mFlashNames.at(i)==name)
		{
			temp=mFlashControls.at(i);
			break;
		}
	}
	return temp;

}


void AppStateManager::loadAllFlashes()
{

	mTransfer =new FlashControl("Transfer", OgreFramework::getSingletonPtr()->m_pViewport, 1920, 1080, Hikari::Position(Hikari::Center),0);
	mTransfer->load("menudeki kari.swf",mHikariMgr);
	mTransfer->setTransparent(false);
	mTransfer->setDraggable(false);
	mHikariMgr->setFlashControl(mTransfer);
	mTransfer->stop();
	mTransfer->hide();
	mFlashNames.push_back("Transfer");
	mFlashControls.push_back(mTransfer);


	//
	FlashControl* mMenuButtons = new FlashControl("MenuButtons", OgreFramework::getSingletonPtr()->m_pViewport,m_HalfWidth*2,m_HalfHeight*2,Hikari::Position(Hikari::Center),0);
	mMenuButtons->load("swfLoaderv0.3.swf",mHikariMgr);
	mMenuButtons->setTransparent(true,true);	
	mMenuButtons->setDraggable(false);
	mHikariMgr->setFlashControl(mMenuButtons);
	mMenuButtons->hide();
	mMenuButtons->stop();
	mFlashNames.push_back("MenuButtons");
	mFlashControls.push_back(mMenuButtons);

	//
	FlashControl* 	mItemSelector = new FlashControl("ItemSelector", OgreFramework::getSingletonPtr()->m_pViewport,1920,1080,Hikari::Position(Hikari::Center),0);
	mItemSelector->load("EnYeniler.swf",mHikariMgr);
	mItemSelector->setTransparent(true,true);
	mItemSelector->setDraggable(false);
	mHikariMgr->setFlashControl(mItemSelector);
	mItemSelector->stop();
	mItemSelector->hide();
	mFlashNames.push_back("ItemSelector");
	mFlashControls.push_back(mItemSelector);


	mItemSelector = new FlashControl("KotonBook", OgreFramework::getSingletonPtr()->m_pViewport,1920,1080,Hikari::Position(Hikari::Center),0);
	mItemSelector->load("umutmenuonemli.swf",mHikariMgr);
	mItemSelector->setTransparent(true,true);
	mItemSelector->setDraggable(false);
	mHikariMgr->setFlashControl(mItemSelector);
	mItemSelector->stop();
	mItemSelector->hide();
	mFlashNames.push_back("KotonBook");
	mFlashControls.push_back(mItemSelector);

	mItemSelector = new FlashControl("StandBy", OgreFramework::getSingletonPtr()->m_pViewport,m_HalfWidth*2,m_HalfHeight*2,Hikari::Position(Hikari::Center),0);
	mItemSelector->load("dikey1.swf",mHikariMgr);
	mItemSelector->setTransparent(true,true);
	mItemSelector->setDraggable(false);
	mHikariMgr->setFlashControl(mItemSelector);
	mItemSelector->stop();
	mItemSelector->hide();
	mFlashNames.push_back("StandBy");
	mFlashControls.push_back(mItemSelector);


	gRegionFlash = new FlashControl("RegionGuide", OgreFramework::getSingletonPtr()->m_pViewport,380,350,Hikari::Position(Hikari::BottomLeft,-380),0);
	gRegionFlash->load("Pre-comp 1.swf",mHikariMgr);
	gRegionFlash->setTransparent(true,true);
	gRegionFlash->setDraggable(false);
	mHikariMgr->setFlashControl(gRegionFlash);
	mFlashNames.push_back("RegionGuide");
	mFlashControls.push_back(gRegionFlash);

	mInfoBox = new FlashControl("EkraniSil", OgreFramework::getSingletonPtr()->m_pViewport,1080,300,Hikari::Position(Hikari::TopCenter,-1920,0),5);
	mInfoBox->load("ekrani sil min.swf",mHikariMgr);
	mInfoBox->setTransparent(true,true);
	mInfoBox->setDraggable(false);
	mInfoBox->stop();
	mInfoBox->hide();
	mHikariMgr->setFlashControl(mInfoBox);
	mFlashNames.push_back("EkraniSil");
	mFlashControls.push_back(mInfoBox);


	mItemSelector = new FlashControl("Uzat", OgreFramework::getSingletonPtr()->m_pViewport,m_HalfWidth*2,m_HalfHeight*2,Hikari::Position(Hikari::Center),0);
	mItemSelector->load("elini uzat 2.swf",mHikariMgr);
	mItemSelector->setTransparent(true,true);
	mItemSelector->setDraggable(false);
	mHikariMgr->setFlashControl(mItemSelector);
	mItemSelector->stop();
	mItemSelector->hide();
	mFlashNames.push_back("Uzat");
	mFlashControls.push_back(mItemSelector);
	
	/*mItemSelector = new FlashControl("Interest", OgreFramework::getSingletonPtr()->m_pViewport,1920,1080,Hikari::Position(Hikari::Center),0);
	mItemSelector->load("Interest.swf",mHikariMgr);
	mItemSelector->setTransparent(true,true);
	mItemSelector->setDraggable(false);
	mHikariMgr->setFlashControl(mItemSelector);
	mItemSelector->stop();
	mItemSelector->hide();
	mFlashNames.push_back("Interest");
	mFlashControls.push_back(mItemSelector);*/
	
}

//|||||||||||||||||||||||||||||||||||||||||||||||
 
AppStateManager::~AppStateManager()
{
	state_info si;
 
        while(!m_ActiveStateStack.empty())
	{
		m_ActiveStateStack.back()->exit();
		m_ActiveStateStack.pop_back();
	}
 
	while(!m_States.empty())
	{
		si = m_States.back();
                si.state->destroy();
                m_States.pop_back();
	}

	if (mStatistics) delete mStatistics;

	if(mHikariMgr)	delete mHikariMgr;	

	if (useKinect) delete mKinect;
}

void AppStateManager::startTransferAnimation()
{


	int startTime = OgreFramework::getSingletonPtr()->m_pTimer->getMilliseconds();
	int elapsedTime=0;
	int counter=0;
	Ogre::TextureUnitState* textState=showPassPanel();
	while (elapsedTime<500)
	{
		counter++;
		if (counter==10)
		{
			textState->setAlphaOperation(LBX_DOTPRODUCT, LBS_MANUAL,LBS_TEXTURE ,(float) elapsedTime/500);
			elapsedTime= OgreFramework::getSingletonPtr()->m_pTimer->getMilliseconds()-startTime;
			updateTransferAnimation();
			counter=0;
		}
	}
	hidePassPanel();
	if (mTransfer)
	{
		mTransfer->show();
		mTransfer->play();
	}
	
}

void AppStateManager::updateTransferAnimation(int timeElapsed)
{
	//Ogre::WindowEventUtilities::messagePump();
	mHikariMgr->update();
	if (useKinect)
	mKinect->addTime(timeElapsed/1000,true);
	OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
}

void AppStateManager::stopTransferAnimation()
{
	if (mTransfer)
	{
		mTransfer->stop();
		mTransfer->hide();
		mTransfer->rewind();
	}

	Ogre::TextureUnitState* textState=showPassPanel();
	int startTime = OgreFramework::getSingletonPtr()->m_pTimer->getMilliseconds();
	int elapsedTime=0;
	int counter=0;
	while (elapsedTime<500)
	{
		counter++;
		if (counter==10) 
		{
			textState->setAlphaOperation(LBX_DOTPRODUCT, LBS_MANUAL,LBS_TEXTURE , 1- (float)elapsedTime/500);
			elapsedTime= OgreFramework::getSingletonPtr()->m_pTimer->getMilliseconds()-startTime;
			updateTransferAnimation();
			counter=0;
		}
	}
	hidePassPanel();
}

KinectController* AppStateManager::getKinect()
{
	return mKinect;
}

void AppStateManager::manageAppState(Ogre::String stateName, AppState* state)
{
	try
	{
		state_info new_state_info;
		new_state_info.name = stateName;
		new_state_info.state = state;
		m_States.push_back(new_state_info);		
	}
	catch(std::exception& e)
	{
		delete state;
		throw Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR, "Error while trying to manage a new AppState\n" + Ogre::String(e.what()), "AppStateManager.cpp (39)");
	}
}

AppState* AppStateManager::findByName(Ogre::String stateName)
{
	std::vector<state_info>::iterator itr;
 
	for(itr=m_States.begin();itr!=m_States.end();itr++)
	{
		if(itr->name==stateName)
			return itr->state;
	}
 
	return 0;
}
unsigned int stateBufferTime=0;
unsigned int handBufferTime=0;
unsigned int clickBufferTime=0;
float lastYCoordinate;
XnPoint3D AppStateManager::updateHandInformation()
{

		XnPoint3D handPosition, lHandPosition, rHandPosition;
		Handstate tState;
		XnVector3D tSpeed;

		bool rightOrLeft=true;

		lHandPosition=mKinect->getRelativeHandCoordinate(XN_SKEL_LEFT_HAND);
		rHandPosition=mKinect->getRelativeHandCoordinate(XN_SKEL_RIGHT_HAND);

		//---------------Using right hand by default now
		//if (rHandPosition.Y>lHandPosition.Y)
		//{
		//	if (lHandPosition.X>0 && lHandPosition.X<1)
		//		rightOrLeft=false;
		//}
		//else if (rHandPosition.X<= 0 ||rHandPosition.X >=1)
		//	rightOrLeft=false;


		if (rightOrLeft)
		{
			handPosition=mKinect->getRelativeHandCoordinate(XN_SKEL_RIGHT_HAND);
			tState=mKinect->getHandstate(false);
			tSpeed=mKinect->getHandSpeed(XN_SKEL_RIGHT_HAND);
		}
		else
		{
			handPosition=mKinect->getRelativeHandCoordinate(XN_SKEL_LEFT_HAND);
			tState=mKinect->getHandstate(true);
			tSpeed=mKinect->getHandSpeed(XN_SKEL_LEFT_HAND);
		}


		if (handPosition.X==0)
			handPosition.X+=0.02;
		else if (handPosition.X==1)
			handPosition.X-=0.02;
		mHikariMgr->injectMouseMove(handPosition.X*(float)screenWidth,handPosition.Y*(float)screenHeight);

		if (handPosition.X>0.85)
		{
			clickBufferTime=0;
			lastYCoordinate=handPosition.Y;
		}
		else if (clickBufferTime>400)
			lastYCoordinate=0;
		else
			clickBufferTime+=33;

		if (stateBufferTime>300)
		{
			if (handBufferTime>1000)
			{
				if (handPosition.X<0.3)
				{
					mHikariMgr->injectMouseMove(0.92*(float)screenWidth,lastYCoordinate*(float)screenHeight);
					mHikariMgr->injectMouseDown(OIS::MB_Left);
					mHikariMgr->injectMouseUp(OIS::MB_Left);
					handBufferTime=0;
				}
			/*	if (tState==HAND_OPEN)
				{
					mHikariMgr->injectMouseDown(OIS::MB_Left);
					mHikariMgr->injectMouseUp(OIS::MB_Left);
					handBufferTime=0;
				}*/
			}
			else
			{
				handBufferTime+=33;
			}
		}
		else
			stateBufferTime+=33;

		return handPosition;
}


void AppStateManager::start(AppState* state)
{
	changeAppState(state);
 
	int timeSinceLastFrame = 1;
	int startTime = 0;
	int HikariTime=0;
	try{
		while(!m_bShutdown)
		{
			if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed())m_bShutdown = true;
 
			if (HikariTime>HikariPeriod)
			{
				mHikariMgr->update();
				HikariTime=HikariTime%HikariPeriod;
			}
			else
				Ogre::WindowEventUtilities::messagePump();
 
			if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
			{
				startTime = OgreFramework::getSingletonPtr()->m_pTimer->getMilliseconds();
 
				OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
				OgreFramework::getSingletonPtr()->m_pMouse->capture();

				//WHhole Update Process
				bool dataNew=false;
				if(useKinect)
				dataNew=mKinect->addTime((float)timeSinceLastFrame/1000);
				XnPoint3D handPosition;
				handPosition.X=-1;
				if (dataNew)
					handPosition=updateHandInformation();
			

				m_ActiveStateStack.back()->update(timeSinceLastFrame,dataNew,handPosition);

				OgreFramework::getSingletonPtr()->updateOgre(timeSinceLastFrame);
				OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
 
				timeSinceLastFrame = OgreFramework::getSingletonPtr()->m_pTimer->getMilliseconds() - startTime;
				HikariTime+=timeSinceLastFrame;
			}
			else
			{
	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
				Sleep(1000);
	#else
				sleep(1);
	#endif
			}
		}
	}
	catch (Ogre::Exception& e)
	{
		OgreFramework::getSingletonPtr()->m_pLog->logMessage("Exception happened");
		const char* c=e.what();
		MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}
	catch (std::exception& e)
	{
		OgreFramework::getSingletonPtr()->m_pLog->logMessage("Exception happened");
		const char* c=e.what();
		MessageBox( NULL, e.what(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit");
}

void AppStateManager::changeAppState(AppState* state)
{
	//if (state->m_Index!=GuideStateId)
	startTransferAnimation();

	if(!m_ActiveStateStack.empty())
	{
		m_ActiveStateStack.back()->exit();
		m_ActiveStateStack.pop_back();
		//if (state->m_Index!=GuideStateId)
		updateTransferAnimation();
	}
	m_ActiveStateStack.push_back(state);
	init(state);
	//if (state->m_Index!=GuideStateId)
	updateTransferAnimation();
	if(mInfoBox)
	{
		if (state->m_Index==MenuStateId)
			mInfoBox->show();
		else
			mInfoBox->hide();
	}
	

	m_ActiveStateStack.back()->enter();
	mStatistics->changedState(state->m_Index);
	//if (state->m_Index!=GuideStateId)
	stopTransferAnimation();
	stateBufferTime=0;
}

bool AppStateManager::pushAppState(AppState* state)
{
	if(!m_ActiveStateStack.empty())
	{
		if(!m_ActiveStateStack.back()->pause())
			return false;
	}
	m_ActiveStateStack.push_back(state);
	init(state);
	if(mInfoBox)
	{
		if (state->m_Index==MenuStateId)
			mInfoBox->show();
		else
			mInfoBox->hide();
	}

	m_ActiveStateStack.back()->enter();
	mStatistics->changedState(m_ActiveStateStack.back()->m_Index);
	return true;
}

void AppStateManager::popAppState()
{
	if(!m_ActiveStateStack.empty())
	{
		m_ActiveStateStack.back()->exit();
		m_ActiveStateStack.pop_back();
	}
 
	if(!m_ActiveStateStack.empty())
	{
		init(m_ActiveStateStack.back());
		m_ActiveStateStack.back()->resume();
	}
    else
		shutdown();
}

void AppStateManager::popAllAndPushAppState(AppState* state)
{
	//if (state->m_Index!=GuideStateId)
	startTransferAnimation();

    while(!m_ActiveStateStack.empty())
    {
        m_ActiveStateStack.back()->exit();
        m_ActiveStateStack.pop_back();
		//if (state->m_Index!=GuideStateId)
		updateTransferAnimation();
    }
 
    pushAppState(state);

	//if (state->m_Index!=GuideStateId)
	stopTransferAnimation();
	stateBufferTime=0;
}

void AppStateManager::pauseAppState()
{


	if(!m_ActiveStateStack.empty())
	{
		m_ActiveStateStack.back()->pause();
	}
 
	if(m_ActiveStateStack.size() > 2)
	{
		init(m_ActiveStateStack.at(m_ActiveStateStack.size() - 2));
		m_ActiveStateStack.at(m_ActiveStateStack.size() - 2)->resume();
	}
}

void AppStateManager::shutdown()
{
	m_bShutdown = true;
}

void AppStateManager::init(AppState* state)
{
    OgreFramework::getSingletonPtr()->m_pKeyboard->setEventCallback(state);
	OgreFramework::getSingletonPtr()->m_pMouse->setEventCallback(state);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(state);
	OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics();
}

std::vector<AppState*>* AppStateManager::getActiveStateStack()
{
	return &m_ActiveStateStack;
}

std::vector<AppStateManager::state_info>*	AppStateManager::getStatesInfo()
{
	return &m_States;
}