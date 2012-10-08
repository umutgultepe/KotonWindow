//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "StandByState.hpp"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
using namespace Ogre;
  extern int m_HalfHeight,m_HalfWidth;
  extern StatisticsManager* gStatistics;

//|||||||||||||||||||||||||||||||||||||||||||||||
 
StandByState::StandByState()
{
    m_bQuit             = false;
    m_FrameEvent        = Ogre::FrameEvent();

}

void StandByState:: SetupVideoMaterial()
{

	videoCapture=cvCreateFileCapture("Data\\Tyler The Creator - Yonkers [SaveYouTube.com].mp4");
	PixelFormat pix_format;
	if (videoCapture)
	{
		videoWidth=cvGetCaptureProperty(videoCapture,CV_CAP_PROP_FRAME_WIDTH);	//Get video properties
		videoHeight=cvGetCaptureProperty(videoCapture,CV_CAP_PROP_FRAME_HEIGHT);
		videoFPS=cvGetCaptureProperty(videoCapture,CV_CAP_PROP_FPS);
		
		
		frameDelay=1000/videoFPS;

		IplImage* nextFrame=cvQueryFrame(videoCapture);	//Get the pixel size
		cvSetCaptureProperty(videoCapture,CV_CAP_PROP_POS_MSEC,76583);
		//char* fourcc=(char*)(&fourC);
		//nextFrame=cvQueryFrame(videoCapture);	//Get the pixel size
		 nextFrame=cvQueryFrame(videoCapture);	//Get the pixel size
		double cdd=cvGetCaptureProperty(videoCapture,CV_CAP_PROP_POS_MSEC);
		pix_size = (nextFrame->depth & 255) >> 3;
		if( nextFrame->dataOrder == 0 )
		   pix_size *= nextFrame->nChannels;
		int row_w=nextFrame->width*pix_size;		//Get the row width and empty byte count
		empty_byte=nextFrame->widthStep-row_w;
		if (pix_size==3)
		{
			pix_format=PF_BYTE_BGR;
		}
		else if (pix_size==4)
		{
			pix_format=PF_BYTE_BGRA;
		}
		



	}	

	// Create the texture
	if (!TextureManager::getSingleton().resourceExists("VideoTexture"))
	{
	videoTexture = TextureManager::getSingleton().createManual(
			"VideoTexture", // name
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			TEX_TYPE_2D,      // type
			videoWidth, videoHeight,         // width & height
			0,                // number of mipmaps
			pix_format,     // pixel format
			TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
	}

	// Create a material using the texture
	if (!MaterialManager::getSingleton().resourceExists("VideoMaterial"))
	{
		MaterialPtr material = MaterialManager::getSingleton().create(
				"VideoMaterial", // name
				ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		material->getTechnique(0)->getPass(0)->createTextureUnitState("VideoTexture");
	}
	passedTime=0;
}


void StandByState::convertIplToTexture(IplImage* img,TexturePtr texture)
{
	HardwarePixelBufferSharedPtr pixelBuffer = texture->getBuffer();//Get the Pixel Buffer for Texture
		pixelBuffer->lock(HardwareBuffer::HBL_DISCARD);						//Lock the buffer
		const PixelBox& pixelBox = pixelBuffer->getCurrentLock();			//Get the pixel box for data pointer
		unsigned char* pDest = static_cast<unsigned char*>(pixelBox.data);
		unsigned char* videoPtr=(unsigned char*)(img->imageData);		//Get the pointer to the video frame

		for (int r=0;r<videoHeight;r++)
		{
			for(int c=0;c<videoWidth;c++)
			{
				for (int p=0;p<pix_size;p++)
					*(pDest++)=*(videoPtr++);//Copy the data
				if(pix_size==3)			//Ogre uses 4 bytes per pixel, so add an additional pass if video is RGB
					pDest++;
			}
			pDest+=empty_byte;			//If there are empty bytes at the end of the rows, add them to go to the correct location
			videoPtr+=empty_byte;
		}
		pixelBuffer->unlock();//Unlock the pixel buffer

}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void StandByState::enter()
{
	mStandBy=0;
	mExitButton=0;
	mWalkingBitch=0;
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering StandByState...");
 
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "StandBySceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
 
    m_pCamera = m_pSceneMgr->createCamera("NewProductCam");
  	createDefaultCamera(m_pCamera);

    //OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "BackToGameBtn", "Return to Game", 250);
 

    m_bQuit = false;
 
    createScene();
}

void StandByState::createScene()
{

	//mKotonRain=m_pSceneMgr->createParticleSystem("kotonRain","koton");
	//mKotonRain->setRenderQueueGroup(103);
	//mRainNode= m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
	//mRainNode->attachObject(mKotonRain);
	//mRainNode->translate(-30,m_HalfHeight/ScreenScale*2,-40);
	//mKotonRain->fastForward(5);
	//
	//mLookBook = m_pParent->mHikariMgr->createFlashOverlay("KotonBook", m_pCamera->getViewport(), 1920, 1080, Hikari::Position(Hikari::Center));
	//mLookBook->load("umutmenuonemli.swf");
	//mLookBook->setTransparent(true);
	//mLookBook->setDraggable(false);

	//Ogre::Entity* ogreHead = m_pSceneMgr->createEntity("Ninja", "ninja.mesh");
	//Ogre::SceneNode* headNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
	//headNode->attachObject(ogreHead);
	//SetupVideoMaterial();
	//Ogre::OverlayElement* mDepthPanel=Ogre::OverlayManager::getSingleton().createOverlayElement("Panel","VideoOverlay");
	//mDepthPanel->setMaterialName("VideoMaterial");
	//mDepthPanel->setMetricsMode(Ogre::GMM_PIXELS);
	//mDepthPanel->setWidth(1080);
	//mDepthPanel->setHeight(1920);
	//mDepthPanel->setHorizontalAlignment(GHA_LEFT);
	//mDepthPanel->setVerticalAlignment(GVA_TOP);
	////mDepthPanel->setLeft(-mDepthPanel->getWidth());
	////mDepthPanel->setTop(-mDepthPanel->getHeight());
 //	OgreFramework::getSingletonPtr()->m_pTrayMgr->getTraysLayer()->add2D((Ogre::OverlayContainer*)mDepthPanel);
	//mDepthPanel->show();
	//showDepthPanel();


	mStandBy=m_pParent->getControlOverlay("StandBy");
	if (mStandBy)
	{
		mStandBy->rewind();
		mStandBy->play();
		mStandBy->show();
	}

	

}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void StandByState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving StandByState...");
	

    m_pSceneMgr->destroyCamera(m_pCamera);
	m_pSceneMgr->destroyAllParticleSystems();
	m_pSceneMgr->destroyAllEntities();
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
}
bool StandByState::keyPressed(const OIS::KeyEvent &keyEventRef)
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
 
bool StandByState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool StandByState::mouseMoved(const OIS::MouseEvent &evt)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool StandByState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool StandByState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
    return true;
}

void StandByState::update(double timeSinceLastFrame,bool dataIsNew, XnPoint3D handPosition)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
	passedTime+=timeSinceLastFrame;//Add the passed time
	//Video processing ---------------------------
	//if(passedTime>=frameDelay)//If enough time has passed for the next frame
	//{
	//	IplImage* nextFrame;
	//	while (passedTime>=frameDelay)
	//	{
	//		passedTime-=frameDelay;//Update the time counter
	//		nextFrame=cvQueryFrame(videoCapture);//Get the next frame
	//		if (nextFrame==0)
	//		{
	//			cvReleaseCapture(&videoCapture);
	//			videoCapture=cvCreateFileCapture("Data\\dikey ktoton avi_arc.avi");
	//			nextFrame=cvQueryFrame(videoCapture);//Get the next frame
	//			cvSetCaptureProperty(videoCapture,CV_CAP_PROP_POS_MSEC,76000);
	//			nextFrame=cvQueryFrame(videoCapture);//Get the next frame

	//			int a=5;
	//			passedTime=0;
	//		}
	//	}
	//	convertIplToTexture(nextFrame,videoTexture);
	//}
	//Video processing ---------------------------
    if(m_bQuit == true)
    {
        popAppState();
        return;
    }
}
