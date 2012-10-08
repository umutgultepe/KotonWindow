//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "MenuState.hpp"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
using namespace Ogre;
 float passedTime=0;
TexturePtr videoTexture;
CvCapture* videoCapture;
float videoWidth,
	videoHeight,
	videoFPS,
	frameDelay;
int pix_size;
int empty_byte;
	
//|||||||||||||||||||||||||||||||||||||||||||||||
 extern int m_HalfHeight,m_HalfWidth;
 extern StatisticsManager* gStatistics;
 extern std::vector<Ogre::String> StateNames;
extern int screenWidth;
extern int screenHeight;

MenuState::MenuState(): SQR_BRUSH_RADIUS(Math::Sqr(30))
{
    m_bQuit         = false;
    m_FrameEvent    = Ogre::FrameEvent();
	

}
int closestPowOfTwo(int num)
{
	int power=0;
	while (1)
	{
		if (pow((float)2,power)>num)
			return pow((float)2,power);
		power++;
	}
} 



void SetupVideoMaterial()
{

	videoCapture=cvCreateFileCapture("Data\\dikey ktoton avi_arc.avi");
	PixelFormat pix_format;
	if (videoCapture)
	{
		videoWidth=cvGetCaptureProperty(videoCapture,CV_CAP_PROP_FRAME_WIDTH);	//Get video properties
		videoHeight=cvGetCaptureProperty(videoCapture,CV_CAP_PROP_FRAME_HEIGHT);
		videoFPS=cvGetCaptureProperty(videoCapture,CV_CAP_PROP_FPS);
		frameDelay=1000/videoFPS;
		IplImage* nextFrame=cvQueryFrame(videoCapture);	//Get the pixel size
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

}


void convertIplToTexture(IplImage* img,TexturePtr texture)
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
 
void MenuState::enter()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering MenuState...");
	mMenuButtons=0;
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "MenuSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

    m_pCamera = m_pSceneMgr->createCamera("MenuCam");
	targetState=MenuStateId;
   	createDefaultCamera(m_pCamera);
    createScene();
	shaked=false;
	fadeTimer=1000;
			
}

void MenuState::updateTexture(Ogre::uint8 freezeAmount)
{
	mTexBuf->lock(HardwareBuffer::HBL_NORMAL);

	// get access to raw texel data
	Ogre::uint8* data = (Ogre::uint8*)mTexBuf->getCurrentLock().data;

	Ogre::uint8 temperature;
	Real sqrDistToBrush;
	float sum=0;
	// go through every texel...
	for (unsigned int y = 0; y < textureHeight; y++)
	{
		for (unsigned int x = 0; x < textureWidth; x++)
		{
			if (freezeAmount != 0)
			{
				// gradually refreeze anything that isn't completely frozen
				temperature = 0xff - *data;
				if (temperature > freezeAmount) *data += freezeAmount;
				else *data = 0xff;
			}

			if (mWiping)
			{
				// wipe frost from under the cursor
				sqrDistToBrush = Math::Sqr(x - mBrushPos.x) + Math::Sqr(y - mBrushPos.y);
				if (sqrDistToBrush <= SQR_BRUSH_RADIUS)
					*data = std::min<Ogre::uint8>(sqrDistToBrush / SQR_BRUSH_RADIUS * 0xff, *data);
			}
			sum+=*data;
			data++;
		}
	}

	sum=sum/(textureHeight*textureWidth);

	if (sum<200)
		shaked=true;

	mTexBuf->unlock();
}


void MenuState::createScene()
{
	lastCoordinatesInMenuRegion.X=0;
	lastCoordinatesInMenuRegion.Y=0;
	lastCoordinatesInMenuRegion.Z=0;
	 
	//ParticleSystems
	mMouseParticles=m_pSceneMgr->createParticleSystem("mouseTail","mouseParticles");
	mMouseParticles->setRenderQueueGroup(104);


	//Scene Nodes
	mParticleNode= m_pSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0,0,-45));
	mParticleNode->attachObject(mMouseParticles);
	//mParticleNode->setVisible(false);
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

	//Ogre::Entity* ogreHead = m_pSceneMgr->createEntity("Head", "ogrehead.mesh");

 //   Ogre::SceneNode* headNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
 //   headNode->attachObject(ogreHead);
	mMenuButtons=m_pParent->getControlOverlay("MenuButtons");

	if (mMenuButtons)
	{
		//mMenuButtons->bind(StringToWString("goToEnYeniler"), Hikari::FlashDelegate(this, &MenuState::toNewProducts));
		//mMenuButtons->bind(StringToWString("goToLookBook"), Hikari::FlashDelegate(this, &MenuState::toLookBook));
		//mMenuButtons->bind(StringToWString("goToMiniGame"), Hikari::FlashDelegate(this, &MenuState::toMiniGame));
		//mMenuButtons->bind(StringToWString("goToAccessories"), Hikari::FlashDelegate(this, &MenuState::toAccessories));
		mMenuButtons->rewind();
		mMenuButtons->play();
		mMenuButtons->show();
	}


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

	//For evaluation
	/*Ogre::StringVector items;
    items.push_back("Max Speed");
	items.push_back("Measuring");
	items.push_back("Hand State");
	mUserLocation=OgreFramework::getSingletonPtr()->m_pTrayMgr->createParamsPanel(OgreBites::TL_TOPRIGHT, "UserInfo", 200, items);*/
	




	mErase=m_pParent->getControlOverlay("EkraniSil");
	if (mErase)
	{
		mErase->rewind();
		mErase->play();
		mErase->show();
	}

	 Ogre::MeshManager::getSingleton().createPlane("ErasePlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::Plane(-Vector3::UNIT_Z, 0),
	 m_HalfWidth*2/ScreenScale, 300/ScreenScale, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);
	mPlaneEnt = m_pSceneMgr->createEntity("EraseEnt", "ErasePlane");	 
	 mPlaneEnt->setMaterialName(mErase->getMaterialName());
	 mPlaneEnt->setRenderQueueGroup(RenderQueueGroupID::RENDER_QUEUE_MAX);
	 Ogre::SceneNode* sNode=m_pSceneMgr->getRootSceneNode()->createChildSceneNode("tNode",Vector3(0,m_HalfHeight/ScreenScale-300/ScreenScale,-45));
	 sNode->attachObject(mPlaneEnt);
	 
	
	textureWidth=closestPowOfTwo(m_HalfWidth/ScreenScale*2);
	textureHeight=closestPowOfTwo(m_HalfHeight/ScreenScale*2);
	TexturePtr tex;
	if (!TextureManager::getSingleton().resourceExists("erase"))
		tex = TextureManager::getSingleton().createManual("erase", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,TEX_TYPE_2D, textureWidth,textureHeight, 0, PF_L8, TU_DYNAMIC_WRITE_ONLY);
	else
		tex = TextureManager::getSingleton().getByName("erase");
	mTexBuf = tex->getBuffer();  // save off the texture buffer
	// initialise the texture to have full luminance
	mTexBuf->lock(HardwareBuffer::HBL_DISCARD);
	memset(mTexBuf->getCurrentLock().data, 0xff, mTexBuf->getSizeInBytes());
	mTexBuf->unlock();
	
	if (!MeshManager::getSingleton().resourceExists("frostPlane"))
	MeshManager::getSingleton().createPlane("frostPlane", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Plane(-Vector3::UNIT_Z, 0),m_HalfWidth/ScreenScale*2,m_HalfHeight/ScreenScale*2, 1, 1, true, 1, 1, 1, Vector3::UNIT_Y);

	Entity* fPlane=m_pSceneMgr->createEntity("frontPlane","frostPlane");
	fPlane->setRenderQueueGroup(RenderQueueGroupID::RENDER_QUEUE_OVERLAY);
	fPlane->setMaterialName("erasableCastle");
	pNode=m_pSceneMgr->getRootSceneNode()->createChildSceneNode("planeNode",Vector3(0,0,-40));
	mWiping=false;
	pNode->attachObject(fPlane);
	mTimeSinceLastFreeze=0;


		//mErase->setOpacity(1);
		//	mErase->stop();
		//	mErase->hide();
		//	mPlaneEnt->setVisible(false);
		//	pNode->setVisible(false);



	showDepthPanel();



	

	//pNode->setVisible(false);

}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
bool measuring=false; 
float maxXSpeed=0;
void MenuState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving MenuState...");
    m_pSceneMgr->destroyCamera(m_pCamera);
	m_pSceneMgr->destroyAllEntities();
	m_pSceneMgr->destroyAllParticleSystems();
	m_pSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
 
	OgreFramework::getSingletonPtr()->m_pTrayMgr->clearAllTrays();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(0);

	if (mMenuButtons)
	{
		//mMenuButtons->unbind(StringToWString("goToEnYeniler"));
		//mMenuButtons->unbind(StringToWString("goToLookBook"));
		//mMenuButtons->unbind(StringToWString("goToMiniGame"));
		//mMenuButtons->unbind(StringToWString("goToAccessories"));
		mMenuButtons->hide();
		mMenuButtons->stop();
	}

		if (mErase)
	{
		mErase->stop();
		mErase->hide();
	}
	//cvReleaseCapture(&videoCapture);
	
	//For evaluation
//	OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyWidget(mUserLocation);
}
bool leftOrRight=true;
bool MenuState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	if (keyEventRef.key==OIS::KC_ESCAPE)
     {
        m_bQuit = true;
        return true;
    }
	else if (keyEventRef.key==OIS::KC_SPACE)
	//	leftOrRight=!leftOrRight;
	{
		if (measuring)
		{
			measuring=false;
			maxXSpeed=0;
//				mUserLocation->setParamValue(1, "false" );
		}
		else
		{
			measuring=true;
//			mUserLocation->setParamValue(1, "true" );
		}
	}

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool MenuState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 bool MenuState::mouseMoved(const OIS::MouseEvent &evt)
{
	mParticleNode->setPosition((m_HalfWidth-evt.state.X.abs)/ScreenScale,(m_HalfHeight-evt.state.Y.abs)/ScreenScale,-45);
	m_pParent->mHikariMgr->injectMouseMove(evt.state.X.abs,evt.state.Y.abs);
	if(!useKinect)
	mBrushPos=Ogre::Vector2(textureWidth*evt.state.X.abs/m_HalfWidth/2,textureHeight*evt.state.Y.abs/m_HalfHeight/2);

    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool MenuState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
		mWiping = true;  // wipe frost if user left clicks in the scene
	m_pParent->mHikariMgr->injectMouseDown(id);
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
	


    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool MenuState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
		mWiping = false;  // stop wiping frost if user releases LMB
	m_pParent->mHikariMgr->injectMouseUp(id);
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;

    return true;
}


void MenuState::update(double timeSinceLastFrame,bool dataIsNew, XnPoint3D handPosition)
{
	if (targetState!=MenuStateId)
	{
		changeAppState(findByName(StateNames.at(targetState)));
		return;
	}


    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);


	if (dataIsNew && handPosition.X>-1)
	{
	mParticleNode->setPosition(getScreenXCoordinate(handPosition.X),getScreenYCoordinate(handPosition.Y),-45);
	}


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
	//			cvSetCaptureProperty(videoCapture,CV_CAP_PROP_POS_FRAMES,0);
	//			nextFrame=cvQueryFrame(videoCapture);//Get the next frame
	//		}
	//	}
	//	convertIplToTexture(nextFrame,videoTexture);
	//}
	//Video processing ---------------------------


	//----------------------------
		// shoot a ray from the cursor to the plane

	if (useKinect)
			mWiping=true;

	if (dataIsNew && handPosition.X>-1)
	{
	mBrushPos=Ogre::Vector2(textureWidth*handPosition.X,textureHeight*handPosition.Y);
	//Vector3 torsoPos=m_pParent->mKinect->getRealCoordinate(XN_SKEL_TORSO);
	//Vector3 rightHandPos=m_pParent->mKinect->getRealCoordinate(XN_SKEL_RIGHT_HAND);
	//Vector3 leftHandPos=m_pParent->mKinect->getRealCoordinate(XN_SKEL_LEFT_HAND);
	//if ((torsoPos.z-rightHandPos.z)>550 && rightHandPos.z>100)
	//	targetState=MenuStateId;
	//else if ((torsoPos.z-leftHandPos.z)>550 && leftHandPos.z>100)
	//	targetState=MenuStateId;

	}
	if (shaked)
	{
		if (fadeTimer>0)
		{
			mErase->setOpacity(fadeTimer/1000);
			fadeTimer-=timeSinceLastFrame;
		}
		else
		{
			mErase->setOpacity(1);
			mErase->stop();
			mErase->hide();
			mPlaneEnt->setVisible(false);
			pNode->setVisible(false);
		}
		
	}
	if (fadeTimer>0)
	{
		passedTime+=timeSinceLastFrame;//Add the passed time


		Ogre::uint8 freezeAmount = 0;
		mTimeSinceLastFreeze += timeSinceLastFrame/1000;

		// find out how much to freeze the plane based on time passed
		while (mTimeSinceLastFreeze >= 0.1)
		{
			mTimeSinceLastFreeze -= 0.1;
			freezeAmount += 0x04;
		}
		updateTexture(freezeAmount);  // rebuild texture contents
		//----------------------------
	}
	

	



    if(m_bQuit == true)
    {
        shutdown();
        return;
    }
}


Hikari::FlashValue MenuState::toNewProducts(FlashControl* caller, const Hikari::Arguments& args)
{
	targetState=NewProductStateId;
	return FLASH_VOID;
}

Hikari::FlashValue MenuState::toLookBook(FlashControl* caller, const Hikari::Arguments& args)
{
	targetState=LookBookStateId;
	return FLASH_VOID;
}

Hikari::FlashValue MenuState::toMiniGame(FlashControl* caller, const Hikari::Arguments& args)
{
	targetState=GameStateId;
	return FLASH_VOID;
}

Hikari::FlashValue MenuState::toAccessories(FlashControl* caller, const Hikari::Arguments& args)
{
	targetState=AccessoriesStateId;
	return FLASH_VOID;
}