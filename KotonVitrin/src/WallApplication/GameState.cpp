//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "GameState.hpp"
#define LIMIT_PERC 0.25
std::vector<IncomingWall*> Walls;
extern Ogre::SceneNode* wallNode;
extern Entity* fitIndicatorTop;
extern Entity* fitIndicatorLeft;
extern Entity* fitIndicatorRight;
extern StatisticsManager* gStatistics;
bool passed=false;
float speed=START_SPEED;
extern bool checkWall;
extern float outPerc;
 	extern int screenWidth;
	extern int screenHeight;
float mod_speed=0;
int g_livesLeft=3;
int activeWall=-1;
int numberOfWalls=0;
//|||||||||||||||||||||||||||||||||||||||||||||||
 
using namespace Ogre;
std::vector<Image*> currentImages;
  extern std::vector<Ogre::String> StateNames;
//|||||||||||||||||||||||||||||||||||||||||||||||
 
GameState::GameState()
{
   LogoTime=0;
    m_bQuit             = false;
	m_bLMouseDown       = false;
    m_bRMouseDown       = false;
	   m_MoveSpeed		= 0.1f;
    m_RotateSpeed	= 0.3f;

}
 void GameState::createHoleRoom(Real roomWidth,Real roomHeight)
{
	//Create The Scene Nodes
	m_pParent->updateTransferAnimation();//Updated flash
	Ogre::SceneNode* roomNode=m_pSceneMgr->getRootSceneNode()->createChildSceneNode("roomNode");
	//Ogre::SceneNode* leftWallNode=roomNode->createChildSceneNode("LeftWallNode",Vector3(0,roomHeight/2,roomWidth/2));
	//Ogre::SceneNode* rightWallNode=roomNode->createChildSceneNode("RightWallNode",Vector3(0,roomHeight/2,-roomWidth/2));
//	Ogre::SceneNode* roofNode=roomNode->createChildSceneNode("RoofNode",Vector3(0,roomHeight,0));
	Ogre::SceneNode* pathNode=roomNode->createChildSceneNode("pathNode");
//	Ogre::SceneNode* endWallNode=roomNode->createChildSceneNode("endWallNode",Vector3(-50,roomHeight/2,0));
	m_pParent->updateTransferAnimation();//Updated flash
	//roofNode->yaw(Radian(Math::PI/2));
	//roofNode->roll(Radian(-Math::PI/2));
	//rightWallNode->pitch(Radian(Math::PI));

	//path Plane 
	if (!MeshManager::getSingleton().resourceExists("path"))
	MeshManager::getSingleton().createPlane("path", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Plane(Vector3::UNIT_Y, 0),100, IncomingWallWidth, 1, 1, true, 1, 1, 1, Vector3::UNIT_Z);
	m_pParent->updateTransferAnimation();//Updated flash
	//Wall Plane 
	//if (!MeshManager::getSingleton().resourceExists("sideWall"))
	//MeshManager::getSingleton().createPlane("sideWall", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	//	Plane(-Vector3::UNIT_Z, 0),roomHeight, 100, 1, 1, true, 1, 1, 1, Vector3::UNIT_X);
	//m_pParent->updateTransferAnimation();//Updated flash
	//if (!MeshManager::getSingleton().resourceExists("roofPlane"))
	//	MeshManager::getSingleton().createPlane("roofPlane", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	//	Plane(-Vector3::UNIT_Y, 0),roomHeight, 100, 1, 5, true, 1, 1, 5, Vector3::UNIT_Z);
	m_pParent->updateTransferAnimation();//Updated flash
	//Small Wall Plane
	//if (!MeshManager::getSingleton().resourceExists("endWall"))
	//	MeshManager::getSingleton().createPlane("endWall", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	//Plane(Vector3::UNIT_X, 0),roomWidth, roomHeight, 3, 1, true, 1, 3, 1, Vector3::UNIT_Y);
	//m_pParent->updateTransferAnimation();//Updated flash
	//if (!MeshManager::getSingleton().resourceExists("lightWall"))
	//	MeshManager::getSingleton().createPlane("lightWall", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	//Plane(Vector3::UNIT_X, 0),roomWidth/3, roomHeight, 10, 10, true, 1, 10, 10, Vector3::UNIT_Y);
	//m_pParent->updateTransferAnimation();//Updated flash		
	//Create smaller wall and floor
	Entity* path= m_pSceneMgr->createEntity("Path","path");
	path->setMaterialName("carpet_mat");
	path->setCastShadows(false);
	pathNode->attachObject(path);
	pathNode->translate(0,-IncomingWallHeight,0);
	m_pParent->updateTransferAnimation();//Updated flash
	//Entity* endWall= m_pSceneMgr->createEntity("EndWall","endWall");
	//endWall->setMaterialName("back_mat");
	//endWall->setCastShadows(false);
	//endWallNode->attachObject(endWall);
	//m_pParent->updateTransferAnimation();//Updated flash
	//Create Walls
	//Entity* leftWall=m_pSceneMgr->createEntity("LeftWall","sideWall");
	//leftWall->setMaterialName("audience_mat");
	//leftWall->setCastShadows(false);

	//Entity* rightWall=leftWall->clone("RightWall");
	//m_pParent->updateTransferAnimation();//Updated flash
	//Entity* roof=m_pSceneMgr->createEntity("Roof","roofPlane");
	//roof->setMaterialName("path_mat");
	//roof->setCastShadows(false);
	

	//m_pParent->updateTransferAnimation();//Updated flash
	//Entity* lightWall=m_pSceneMgr->createEntity("LightWall","lightWall");
	//lightWall->setMaterialName("light_mat");
	//lightWall->setCastShadows(false);
	//endWallNode->createChildSceneNode("lightWallNode",Vector3(0.005,0,0))->attachObject(lightWall);
	//m_pParent->updateTransferAnimation();//Updated flash
	//Add walls to scene
	//leftWallNode->attachObject(leftWall);
	//rightWallNode->attachObject(rightWall);
	//rightWallNode->scale(1,-1,1);
//	roofNode->attachObject(roof);

	m_pParent->updateTransferAnimation();//Updated flash
		
}

//|||||||||||||||||||||||||||||||||||||||||||||||
 
void GameState::enter()
{

	 mStatus=PROGRESS;
	 g_livesLeft=3;
	 activeWall=-1;
	 passed=false;
	 speed=START_SPEED;
	 mod_speed=0;
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering GameState...");
	m_pParent->updateTransferAnimation();//Updated flash
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "GameSceneMgr");

	m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	m_pSceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
	m_pSceneMgr->setShadowTextureSize(1024);
	m_pSceneMgr->setShadowTextureCount(1);	

    m_pCamera = m_pSceneMgr->createCamera("GameCamera");
    m_pCamera->setPosition(Ogre::Vector3(60,10,0));
    m_pCamera->setDirection(Ogre::Vector3(-1, 0, 0));
    m_pCamera->setNearClipDistance(5);
 
    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
	m_pParent->updateTransferAnimation();//Updated flash
	createScene();
	m_pParent->updateTransferAnimation();//Updated flash
	buildPanels();
	m_pParent->updateTransferAnimation();//Updated flash
	checkWall=true;
	outPerc=-1;
	m_pParent->updateTransferAnimation();//Updated flash
	targetState=GameStateId;

}
void deactivateWall(int index)
{
	if(index>=0 && index<numberOfWalls)
	{
		Walls.at(index)->getWall()->setVisible(false);
		Walls.at(index)->getLowerWall()->setVisible(false);
	}
}

void activateWall(int index)
{
	if(index>=0 && index<numberOfWalls)
	{

		Walls.at(index)->getWall()->setVisible(true);
		Walls.at(index)->getLowerWall()->setVisible(true);
		activeWall=index;
	}
}
void GameState::createScene()
{
	m_pSceneMgr->setAmbientLight(ColourValue(0.3, 0.3, 0.3));
	m_pParent->updateTransferAnimation();//Updated flash
	//m_pSceneMgr->setSkyDome(true, "NightSky", 5, 8);
	// set shadow properties
	// add a bright light above the scene
	Light* light = m_pSceneMgr->createLight();
	light->setType(Light::LT_POINT);
	light->setPosition(-10, 40, 0);
	light->setDiffuseColour(ColourValue::White);
	light->setSpecularColour(ColourValue::White);
	m_pParent->updateTransferAnimation();//Updated flash
	light = m_pSceneMgr->createLight();
	light->setType(Light::LT_POINT);
	light->setPosition(-40, 10, 0);
	light->setDiffuseColour(ColourValue::White);
	light->setSpecularColour(ColourValue::White);
	m_pParent->updateTransferAnimation();//Updated flash
	light = m_pSceneMgr->createLight();
	light->setType(Light::LT_POINT);
	light->setPosition(-30, 10, 0);
	light->setDiffuseColour(ColourValue::White);
	light->setSpecularColour(ColourValue::White);

	//MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	//Plane(Vector3::UNIT_Y, 0),100, 100, 10, 10, true, 1, 10, 10, Vector3::UNIT_Z);
	//m_pParent->updateTransferAnimation();//Updated flash
	//Entity* floor = m_pSceneMgr->createEntity("Floor", "floor");
 //   floor->setMaterialName("floor_mat");
	//floor->setCastShadows(false);
	//m_pSceneMgr->getRootSceneNode()->attachObject(floor);
	m_pParent->updateTransferAnimation();//Updated flash
	initWallResources(m_pSceneMgr,m_pParent);
	m_pParent->updateTransferAnimation();//Updated flash
	createHoleRoom(80,30);
	m_pParent->updateTransferAnimation();//Updated flash


	IncomingWall* Wall1=new IncomingWall(m_pSceneMgr,"standingDude","WallTextureMaterial1");
	m_pParent->updateTransferAnimation();//Updated flash
	numberOfWalls++;

	IncomingWall* Wall2=new IncomingWall(m_pSceneMgr,"runningDude","WallTextureMaterial6");
	m_pParent->updateTransferAnimation();//Updated flash
	numberOfWalls++;

	IncomingWall* Wall3=new IncomingWall(m_pSceneMgr,"pissingBaby","WallTextureMaterial3");
	m_pParent->updateTransferAnimation();//Updated flash
	numberOfWalls++;

	//IncomingWall* Wall4=new IncomingWall(m_pSceneMgr,"happy","WallTextureMaterial5");
	//m_pParent->updateTransferAnimation();//Updated flash
	//numberOfWalls++;

	//IncomingWall* Wall5=new IncomingWall(m_pSceneMgr,"dancingDude","WallTextureMaterial4");
	//m_pParent->updateTransferAnimation();//Updated flash
	//numberOfWalls++;

	//IncomingWall* Wall6=new IncomingWall(m_pSceneMgr,"toilet","WallTextureMaterial2");
	//numberOfWalls++;

	Walls.push_back(Wall1);
	Walls.push_back(Wall2);
	Walls.push_back(Wall3);
	//Walls.push_back(Wall4);
	//Walls.push_back(Wall5);
	//Walls.push_back(Wall6);
	activateWall(0);

	
	//
	
}

bool GameState::pause()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Pausing GameState...");
	OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();
	checkWall=false;
	//mDepthPanel->hide();
	mSuccess->hide();
	mFailure->hide();
	mPhoto->hide();
	//mStart->hide();
	mDeath->hide();

	for (int i=0;i<3;i++)
		mLives[i]->hide();
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void GameState::resume()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Resuming GameState...");
	OgreFramework::getSingletonPtr()->m_pTrayMgr->showCursor();
	checkWall=true;
	for (int i=0;i<g_livesLeft;i++)
		mLives[i]->show();
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
    m_bQuit = false;
}

void GameState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving GameState...");
    m_pSceneMgr->destroyCamera(m_pCamera);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();
	checkWall=false;
	//OgreFramework::getSingletonPtr()->m_pTrayMgr->getTraysLayer()->remove2D((Ogre::OverlayContainer*)mDepthPanel);
	//OgreFramework::getSingletonPtr()->m_pTrayMgr->getTraysLayer()->remove2D((Ogre::OverlayContainer*)mStart);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->getTraysLayer()->remove2D((Ogre::OverlayContainer*)mFailure);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->getTraysLayer()->remove2D((Ogre::OverlayContainer*)mSuccess);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->getTraysLayer()->remove2D((Ogre::OverlayContainer*)mDeath);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->getTraysLayer()->remove2D((Ogre::OverlayContainer*)mLives[0]);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->getTraysLayer()->remove2D((Ogre::OverlayContainer*)mLives[1]);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->getTraysLayer()->remove2D((Ogre::OverlayContainer*)mLives[2]);
	//Ogre::OverlayManager::getSingleton().destroyOverlayElement(mDepthPanel);
	//Ogre::OverlayManager::getSingleton().destroyOverlayElement(mStart);
	Ogre::OverlayManager::getSingleton().destroyOverlayElement(mFailure);
	Ogre::OverlayManager::getSingleton().destroyOverlayElement(mDeath);
	Ogre::OverlayManager::getSingleton().destroyOverlayElement(mSuccess);
	Ogre::OverlayManager::getSingleton().destroyOverlayElement(mLives[0]);
	Ogre::OverlayManager::getSingleton().destroyOverlayElement(mLives[1]);
	Ogre::OverlayManager::getSingleton().destroyOverlayElement(mLives[2]);
	Ogre::OverlayManager::getSingleton().destroyOverlayElement(mPhoto);
	Ogre::OverlayManager::getSingleton().destroy(photoOverlay);
	
	for (unsigned int i=0;i<Walls.size();i++)
	{
		delete Walls.at(i);
	}
	Walls.clear();
	m_pSceneMgr->destroyAllLights();
	m_pSceneMgr->destroyAllEntities();
	m_pSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
	
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);

	if (currentImages.size()>0)
	{
		for (unsigned int i=0;i<currentImages.size();i++)
		{
			delete currentImages.at(i);
			String tString("TemporaryImage"+StringConverter::toString(i+1)+".jpg");
			remove(tString.c_str());
		}
		currentImages.clear();
	}
}


bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
 
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
		targetState=MenuStateId;
        return true;
    }
 
  //  if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_I))
  //  {
		//if(help->getTrayLocation() == OgreBites::TL_NONE)
  //      {
  //          OgreFramework::getSingletonPtr()->m_pTrayMgr->moveWidgetToTray(help, OgreBites::TL_TOPLEFT, 0);
  //          help->show();
  //      }
  //      else
  //      {
  //          OgreFramework::getSingletonPtr()->m_pTrayMgr->removeWidgetFromTray(help);
  //          help->hide();
  //      }
  //  }
 
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A))
        m_TranslateVector.x = -m_MoveScale;
 
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D))
        m_TranslateVector.x = m_MoveScale;
 
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W))
        m_TranslateVector.z = -m_MoveScale;
 
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_S))
        m_TranslateVector.z = m_MoveScale;
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool GameState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}

bool GameState::mouseMoved(const OIS::MouseEvent &evt)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
 
    if(m_bRMouseDown)
    {
        m_pCamera->yaw(Degree(evt.state.X.rel * -0.1f));
        m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
    }
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool GameState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
 
    if(id == OIS::MB_Left)
    {
        onLeftPressed(evt);
        m_bLMouseDown = true;
    }
    else if(id == OIS::MB_Right)
    {
        m_bRMouseDown = true;
    }
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool GameState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
 
    if(id == OIS::MB_Left)
    {
        m_bLMouseDown = false;
    }
    else if(id == OIS::MB_Right)
    {
        m_bRMouseDown = false;
    }
 
    return true;
}

void GameState::onLeftPressed(const OIS::MouseEvent &evt)
{

}

void GameState::moveCamera()
{
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
        m_pCamera->moveRelative(m_TranslateVector);
    m_pCamera->moveRelative(m_TranslateVector / 10);
}

bool  GameState::processWall(const FrameEvent& evt,float outPerc)
{
	float time=evt.timeSinceLastFrame/1000;
	if (outPerc!=-1)//If the user is calibrated
	{
		//mStart->hide();
		if (mStatus==PROGRESS)
		{
			wallNode->translate(time*(speed+mod_speed),0,0);
			if(outPerc>LIMIT_PERC)//Check if the user hit the wall
			{//If there is a hit
				fitIndicatorTop->setMaterialName("unfit_mat");
				fitIndicatorLeft->setMaterialName("unfit_mat");
				fitIndicatorRight->setMaterialName("unfit_mat");
				mod_speed=0;
			}
			else
			{
				fitIndicatorTop->setMaterialName("fit_mat");
				fitIndicatorLeft->setMaterialName("fit_mat");
				fitIndicatorRight->setMaterialName("fit_mat");
				mod_speed+=time*50;
			}

			if (abs(wallNode->getPosition().x-50)<3)//If the user and the wall are aligned
			{
				passed=true; 
				if(outPerc>LIMIT_PERC)//Check if the user hit the wall
				{//If there is a hit
					wallNode->setPosition(-49.96,IncomingWallHeight/2,0); //Reset the wall position
					passed=false;		//Reset passed flag
					if (g_livesLeft==1)		//If this was the last life, the player died
					{
						mDeath->show();
						mStatus=DEATH;
						deactivateWall(activeWall);
						activateWall(0);	//Revert to the first wall
						speed=START_SPEED;	//Reset the speed		
						mLives[1]->show();
						mLives[2]->show();
						g_livesLeft=3;
						pushAppState(findByName("PhotoSelectionState"));
						return false;

					}						//Continue play if there are more lives
					else
					{		
						currentImages.push_back(m_pParent->getKinect()->takeSnapshot());
						currentImages.back()->save("TemporaryImage"+StringConverter::toString(currentImages.size())+".jpg");
						mPhoto->show();
						mFailure->show();
						mStatus=FAILURE;
						g_livesLeft--;
						mLives[g_livesLeft]->hide();
					}
				}
			}
			else
			{
				if(passed)//If the wall is behind the user, it means user was successful
				{
					speed+=SPEED_INCREASE;	//Increase the speed
					wallNode->setPosition(-49.96,IncomingWallHeight/2,0);//Reset the wall position
					deactivateWall(activeWall);
					if(++activeWall==Walls.size())	//Proceed to the next wall
						activeWall=0;
					activateWall(activeWall);		//
					passed=false;					//Reset the passed flag
					mSuccess->show();	//Show the success indicator
					mStatus=SUCCESS;
					currentImages.push_back(m_pParent->getKinect()->takeSnapshot());
					currentImages.back()->save("TemporaryImage"+StringConverter::toString(currentImages.size())+".jpg");
					mPhoto->show();
				}
			}
		}
		else	//If there is a logo going on
		{
			mod_speed=0;
			LogoTime+=time*1000;
			if (LogoTime>=1000)
			{
				
				mStatus=PROGRESS;
				LogoTime=0;
				mSuccess->hide();
				mFailure->hide();
				mDeath->hide();
				mPhoto->hide();
			}
		}
	}
	else//If the user is not calibrated
	{
		wallNode->setPosition(-49.96,IncomingWallHeight/2,0);//Reset the Wall position
		//mStart->show();
		mSuccess->hide();
		mFailure->hide();
		mDeath->hide();
		mStatus=PROGRESS;
		LogoTime=0;
		mod_speed=0;
	}
	return true;
}

void GameState::update(double timeSinceLastFrame,bool dataIsNew, XnPoint3D handPosition)
{
	if (targetState!=GameStateId)
	{
		changeAppState(findByName(StateNames.at(targetState)));
		return;
	}
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);

	if (dataIsNew && handPosition.X>-1)
	{
		OgreFramework::getSingletonPtr()->m_pTrayMgr->getCursorImage()->setPosition(screenWidth* handPosition.X,screenHeight* handPosition.Y);
		//OgreFramework::getSingletonPtr()->m_pTrayMgr->getCu
	}
	//mParticleNode->setPosition(getScreenXCoordinate(handPosition.X),getScreenYCoordinate(handPosition.Y),-45);
	
	if (processWall(m_FrameEvent,outPerc))//Process the incoming wall)
	{
		if(m_bQuit == true)
		{
			popAppState();
			return;
		}
 
 
		m_MoveScale = m_MoveSpeed   * timeSinceLastFrame;
		m_RotScale  = m_RotateSpeed * timeSinceLastFrame;
 
		m_TranslateVector = Vector3::ZERO;
		moveCamera();
	}
}

void GameState::buildPanels()
{
	OgreBites::SdkTrayManager* mTrayMgr=OgreFramework::getSingletonPtr()->m_pTrayMgr;
	m_pParent->updateTransferAnimation();//Updated flash

	showDepthPanel();
	mTrayMgr->showCursor();


	mLives[0] = Ogre::OverlayManager::getSingleton().createOverlayElement("Panel","Life1");
	mLives[0]->setMaterialName("life_mat");
	mLives[0]->setMetricsMode(Ogre::GMM_RELATIVE);
	mLives[0]->setWidth(0.05);
	mLives[0]->setHeight(0.05);
	mLives[0]->setHorizontalAlignment(GHA_RIGHT);
	mLives[0]->setVerticalAlignment(GVA_TOP);
	mLives[0]->setLeft(-mLives[0]->getWidth());
	m_pParent->updateTransferAnimation();//Updated flash
	mLives[1]=mLives[0]->clone("Life2");
	mLives[1]->setLeft(-mLives[0]->getWidth()*2);
	m_pParent->updateTransferAnimation();//Updated flash
	mLives[2]=mLives[0]->clone("Life3");
	mLives[2]->setLeft(-mLives[0]->getWidth()*3);

	m_pParent->updateTransferAnimation();//Updated flash
	mTrayMgr->getTraysLayer()->add2D((Ogre::OverlayContainer*)mLives[0]);
	mTrayMgr->getTraysLayer()->add2D((Ogre::OverlayContainer*)mLives[1]);
	mTrayMgr->getTraysLayer()->add2D((Ogre::OverlayContainer*)mLives[2]);
	m_pParent->updateTransferAnimation();//Updated flash
	mSuccess = Ogre::OverlayManager::getSingleton().createOverlayElement("Panel","Success");//The success and failure indicators
	mSuccess->setMaterialName("success_mat");
	mSuccess->setMetricsMode(Ogre::GMM_RELATIVE);
	mSuccess->setWidth(0.50);
	mSuccess->setHeight(0.50);
	mSuccess->setHorizontalAlignment(GHA_CENTER);
	mSuccess->setVerticalAlignment(GVA_CENTER);
	mSuccess->setLeft(-mSuccess->getWidth()/2);
	mSuccess->setTop(-mSuccess->getHeight()/2);
	mTrayMgr->getTraysLayer()->add2D((Ogre::OverlayContainer*)mSuccess);
	mSuccess->hide();
	m_pParent->updateTransferAnimation();//Updated flash
	//mStart=mSuccess->clone("Start");			//Start indicating Wifget
	//mStart->setMaterialName("start_mat");
	//mTrayMgr->getTraysLayer()->add2D((Ogre::OverlayContainer*)mStart);
	//mStart->show();
	m_pParent->updateTransferAnimation();//Updated flash
	mDeath=mSuccess->clone("Death");			//Start indicating Wifget
	mDeath->setMaterialName("death_mat");
	mTrayMgr->getTraysLayer()->add2D((Ogre::OverlayContainer*)mDeath);
	mDeath->hide();
	m_pParent->updateTransferAnimation();//Updated flash
	photoOverlay=Ogre::OverlayManager::getSingleton().create("PhotoOverlay");

	mPhoto = Ogre::OverlayManager::getSingleton().createOverlayElement("Panel","PhotoPanel");
	mPhoto->setMaterialName("RGBTextureMaterial");
	mPhoto->setMetricsMode(Ogre::GMM_PIXELS);
	mPhoto->setWidth(RGBX);
	mPhoto->setHeight(RGBY);
	mPhoto->setHorizontalAlignment(GHA_CENTER);
	mPhoto->setVerticalAlignment(GVA_CENTER);
	mPhoto->setLeft(-RGBX/2);
	mPhoto->setTop(-RGBY/2);
	photoOverlay->add2D((Ogre::OverlayContainer*)mPhoto);
	photoOverlay->setZOrder(mTrayMgr->getTraysLayer()->getZOrder()-2);
	photoOverlay->show();
	mPhoto->hide();
	m_pParent->updateTransferAnimation();//Updated flash
	mSuccess->setHorizontalAlignment(GHA_CENTER);
	mSuccess->setVerticalAlignment(GVA_BOTTOM);
	mSuccess->setWidth(0.40);
	mSuccess->setHeight(0.40);
	mSuccess->setLeft(-mSuccess->getWidth()/2);
	mSuccess->setTop(-mSuccess->getHeight());
	m_pParent->updateTransferAnimation();//Updated flash
	mFailure=mSuccess->clone("Failure");		//Failure indicating Widget
	mFailure->setMaterialName("failure_mat");
	mTrayMgr->getTraysLayer()->add2D((Ogre::OverlayContainer*)mFailure);
	mFailure->hide();
}