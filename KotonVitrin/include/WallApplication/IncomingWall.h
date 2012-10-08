#ifndef INCOMING_WALL_HPP
#define INCOMING_WALL_HPP

#include "StdAfx.h"
#include "AppStateManager.hpp"
#define IncomingWallWidth  30 
#define IncomingWallHeight 20
#define START_SPEED 5
#define SPEED_INCREASE 3
#define DepthMaterialName "MyDepthTexture"
#define depthWidth 640
#define depthHeight 480
using namespace Ogre;
class IncomingWall
{
private:
	Ogre::String entityName;
	Entity* wall;
	Entity* lowerWall;
	Image* wallImage;
	Ogre::SceneManager* mSceneManager;

public:
	IncomingWall(SceneManager* mScene,Ogre::String name,Ogre::String MaterialName);
	~IncomingWall(void);
	Entity* getWall();
	Entity* getLowerWall();
	Image* getImage();
	//bool compareTextures();

};

void initWallResources(SceneManager* mScene,AppStateListener* m_pParent);

#endif