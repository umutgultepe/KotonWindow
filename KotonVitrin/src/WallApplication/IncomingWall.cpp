#include "StdAfx.h"
#include "IncomingWall.h"
TexturePtr depthTexture;
Ogre::SceneNode* wallNode;
Ogre::SceneNode* lowerWallNode;
Entity* fitIndicatorTop;
Entity* fitIndicatorLeft;
Entity* fitIndicatorRight;
void initWallResources(SceneManager* mScene,AppStateListener* m_pParent)
{
	MeshPtr wall;
	//Incoming Wall Plane
	if ( !MeshManager::getSingleton().resourceExists("wall"))
	{
		wall= MeshManager::getSingleton().createPlane("wall", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Plane(Vector3::UNIT_X, 0),IncomingWallWidth, IncomingWallHeight, 1, 1, true, 1, 1, 1, Vector3::UNIT_Y);
		m_pParent->updateTransferAnimation();//Updated flash
		unsigned short src, dest;
		if (!wall->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
		{
			wall->buildTangentVectors(VES_TANGENT, src, dest);
		}
	}
	else
		wall=MeshManager::getSingleton().getByName("wall");
	m_pParent->updateTransferAnimation();//Updated flash


	
	if ( !MeshManager::getSingleton().resourceExists("fitIndicatorTop"))
	MeshManager::getSingleton().createPlane("fitIndicatorTop", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Plane(Vector3::UNIT_X, 0),IncomingWallWidth+3, 1.5, 1, 1, true, 1, 1, 1, Vector3::UNIT_Y);
	if ( !MeshManager::getSingleton().resourceExists("fitIndicatorSide"))
		MeshManager::getSingleton().createPlane("fitIndicatorSide", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Plane(Vector3::UNIT_X, 0),1.5, 2*IncomingWallHeight, 1, 1, true, 1, 1, 1, Vector3::UNIT_Y);
	m_pParent->updateTransferAnimation();//Updated flash

	wallNode=mScene->getRootSceneNode()->createChildSceneNode("IncomingWallNode",Vector3(0,IncomingWallHeight/2,0));
	lowerWallNode=wallNode->createChildSceneNode("LowerWallNode",Vector3(0,-IncomingWallHeight,0));
	Ogre::SceneNode* fitNode=wallNode->createChildSceneNode("FitNode",Vector3(-0.015,0.75,0));
	Ogre::SceneNode* topNode=fitNode->createChildSceneNode("FitNodeTop",Vector3(0,IncomingWallHeight/2,0));
	Ogre::SceneNode* leftNode=fitNode->createChildSceneNode("FitNodeLeft",Vector3(0,-IncomingWallHeight/2,-IncomingWallWidth/2-0.75));
	Ogre::SceneNode* rightNode=fitNode->createChildSceneNode("FitNodeRight",Vector3(0,-IncomingWallHeight/2,IncomingWallWidth/2+0.75));
	m_pParent->updateTransferAnimation();//Updated flash
	fitIndicatorTop= mScene->createEntity("FitIndicateTop","fitIndicatorTop");
	fitIndicatorLeft= mScene->createEntity("FitIndicateLeft","fitIndicatorSide");
	fitIndicatorRight= mScene->createEntity("FitIndicateRight","fitIndicatorSide");
	fitIndicatorTop->setMaterialName("unfit_mat");
	fitIndicatorTop->setCastShadows(false);
	fitIndicatorLeft->setMaterialName("unfit_mat");
	fitIndicatorLeft->setCastShadows(false);
	fitIndicatorRight->setMaterialName("unfit_mat");
	fitIndicatorRight->setCastShadows(false);
	m_pParent->updateTransferAnimation();//Updated flash
	topNode->attachObject(fitIndicatorTop);
	leftNode->attachObject(fitIndicatorLeft);
	rightNode->attachObject(fitIndicatorRight);
	fitIndicatorRight->setVisible(true);
	fitIndicatorTop->setVisible(true);
	fitIndicatorLeft->setVisible(true);
	m_pParent->updateTransferAnimation();//Updated flash
}


IncomingWall::IncomingWall(SceneManager* mScene,Ogre::String name,Ogre::String MaterialName)
{

		entityName=name;
		//Add the incoming Wall 
		mSceneManager=mScene;
		wall = mScene->createEntity(entityName,"wall");
		wall->setMaterialName(MaterialName);
		
		lowerWall=wall->clone(entityName+"low");
		lowerWall->setMaterialName(MaterialName+"b");

		String texName=wall->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(1)->getTextureName();
		wallImage=new Image();
		wallImage->load(texName,ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		wallImage->resize(640,480);

		lowerWallNode->attachObject(lowerWall);

		wallNode->attachObject(wall);
		wall->setVisible(false);
		lowerWall->setVisible(false);
}


IncomingWall::~IncomingWall(void)
{
	mSceneManager->destroyEntity(wall);
	delete wallImage;
	
}

Entity* IncomingWall::getWall()
{
	return wall;
}

Entity* IncomingWall::getLowerWall()
{
	return lowerWall;
}

Image* IncomingWall::getImage()
{
	return wallImage;
}