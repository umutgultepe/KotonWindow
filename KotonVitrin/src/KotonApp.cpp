//|||||||||||||||||||||||||||||||||||||||||||||||
#include "StdAfx.h"
#include "KotonApp.hpp"
 
#include "MenuState.hpp"
#include "GameState.hpp"
#include "NewProductState.hpp"
#include "AccessoriesState.hpp"
#include "LookBookState.hpp"
#include "PhotoSelectionState.hpp"
#include "InterestState.hpp"
#include "GuideState.hpp"
#include "StandByState.hpp"
std::vector<Ogre::String> StateNames;
//|||||||||||||||||||||||||||||||||||||||||||||||
 
KotonApp::KotonApp()
{
	m_pAppStateManager = 0;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
KotonApp::~KotonApp()
{
	delete m_pAppStateManager;
        delete OgreFramework::getSingletonPtr();
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void SetupDepthMaterial()
{
	// Create the texture
	Ogre::TexturePtr depthTexture = Ogre::TextureManager::getSingleton().createManual(
			"MyDepthTexture", // name
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			TEX_TYPE_2D,      // type
			m_Width, m_Height,         // width & height
			0,                // number of mipmaps
			PF_BYTE_BGRA,     // pixel format
			TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

		
	// Create a material using the texture
	MaterialPtr material = MaterialManager::getSingleton().create(
			"DepthTextureMaterial", // name
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	material->getTechnique(0)->getPass(0)->createTextureUnitState("MyDepthTexture");
	material->getTechnique(0)->getPass(0)->setSceneBlending(SBT_TRANSPARENT_ALPHA);
}

void SetupRGBMaterial()
{
	Ogre::TexturePtr depthTexture = Ogre::TextureManager::getSingleton().createManual(
			"MyRGBTexture", // name
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			TEX_TYPE_2D,      // type
			RGBX, RGBY,         // width & height
			0,                // number of mipmaps
			PF_BYTE_RGB,     // pixel format
			TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

		
	// Create a material using the texture
	MaterialPtr material = MaterialManager::getSingleton().create(
			"RGBTextureMaterial", // name
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	material->getTechnique(0)->getPass(0)->createTextureUnitState("MyRGBTexture");
}


void KotonApp::startDemo()
{
	new OgreFramework();
	if(!OgreFramework::getSingletonPtr()->initOgre("KotonVitrin", 0, 0))
		return;
 
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");
	SetupDepthMaterial();
	SetupRGBMaterial();
	
	m_pAppStateManager = new AppStateManager(OgreFramework::getSingletonPtr()->m_pRoot,OgreFramework::getSingletonPtr()->m_pTrayMgr);
		

	MenuState::create(m_pAppStateManager, "MenuState",MenuStateId);
	GameState::create(m_pAppStateManager, "GameState",GameStateId);
    NewProductState::create(m_pAppStateManager, "NewProductState",NewProductStateId);
	AccessoriesState::create(m_pAppStateManager, "AccessoriesState",AccessoriesStateId);
	LookBookState::create(m_pAppStateManager, "LookBookState",LookBookStateId);
	PhotoSelectionState::create(m_pAppStateManager, "PhotoSelectionState",PhotoSelectionId);
	InterestState::create(m_pAppStateManager, "InterestState",InterestStateId);
	GuideState::create(m_pAppStateManager, "GuideState",GuideStateId);
	StandByState::create(m_pAppStateManager, "StandByState",StandByStateId);
	StateNames.push_back("MenuState");
	StateNames.push_back("GameState");
	StateNames.push_back("NewProductState");
	StateNames.push_back("AccessoriesState");
	StateNames.push_back("LookBookState");
	StateNames.push_back("PhotoSelectionState");
	StateNames.push_back("InterestState");
	StateNames.push_back("GuideState");
	StateNames.push_back("StandByState");
	m_pAppStateManager->mStatistics->typeHeaders();
	if(useKinect)
		m_pAppStateManager->start(m_pAppStateManager->findByName("StandByState"));
	else
		m_pAppStateManager->start(m_pAppStateManager->findByName("MenuState"));
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||