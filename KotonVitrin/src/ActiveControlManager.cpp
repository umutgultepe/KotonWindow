#include "StdAfx.h"
#include "OgreString.h"


#include "ActiveControlManager.h"


ActiveControlManager::ActiveControlManager()
{
	numOfItems=0;
	prevId=0;
}


ActiveControlManager::~ActiveControlManager(void)
{
	for (int i=0;i<numOfItems;i++)
	{
		delete allRegions.at(i);
	}
	allRegions.clear();
}

void ActiveControlManager::createItem(String name,Ogre::Real region[4],Ogre::Real activePart[2],Ogre::Real activeWidth,Ogre::Real activeHeight,int target)
{
	
	
	Ogre::Real visSize[]={activeWidth,activeHeight};
	Active_Item* tempItem=new Active_Item(name,region,activePart,visSize,target,this);

	allRegions.push_back(tempItem);

	
	//allRegions
	numOfItems++;
		

}

Active_Item::Active_Item(String name,Ogre::Real activeCoordinates[4],Ogre::Real visibleCoordinates[2],Ogre::Real visibleSize[2],int target,ActiveControlManager* tManager)
{
	manager=tManager;
	elementName=name;
	//Ogre::OverlayElement* element;
	elementWidth=visibleSize[0];
	elementHeight=visibleSize[1];
	targetID=target;
	for(int i=0;i<4;i++)
	{
		activeRegionCoordinates[i]=activeCoordinates[i];
	}
	for (int i=0;i<2;i++)
	{
		visibleRegionCoordinates[i]=visibleCoordinates[i];

	}
}

Active_Item::~Active_Item()
{

}

Ogre::Real* Active_Item::getCoordinates()
{
	return activeRegionCoordinates;
}

int ActiveControlManager::getCurrentItem(Ogre::Real mouseXPos,Ogre::Real mouseYPos)
{
	for(unsigned int i=0;i<allRegions.size();i++)
	{
		Ogre::Real* tempCoordinates=allRegions.at(i)->getCoordinates();
		if(mouseXPos>tempCoordinates[0] && 
			mouseXPos<tempCoordinates[2]&&
			mouseYPos>tempCoordinates[1]&&
			mouseYPos<tempCoordinates[3])
			return i;
		else 
			continue;

	}

	return -1; 
}
	

int ActiveControlManager::getCurrentTarget()
{
	return allRegions.at(prevId)->targetID;
}

bool ActiveControlManager::highlight(Ogre::Real mouseXPos,Ogre::Real mouseYPos,Ogre::SceneNode* highLightNode,Ogre::Real screenWidth,Ogre::Real screenHeight)
{
	int cId=getCurrentItem(mouseXPos/screenWidth,mouseYPos/screenHeight);
	if (cId!=-1)
	{
		if (cId!=prevId)
		{
			Active_Item* curItem=allRegions.at(cId);
			highLightNode->setPosition((screenWidth/2-curItem->visibleRegionCoordinates[0]*screenWidth)/ScreenScale,
										(screenHeight/2-curItem->visibleRegionCoordinates[1]*screenHeight)/ScreenScale,-45);
			prevId=cId;
			return true;
		}
		else
			return false;
	}
	else
		return false;
	
}
bool ActiveControlManager::highlightKinect(Ogre::Real mouseXPos,Ogre::Real mouseYPos,Ogre::SceneNode* highLightNode,Ogre::Real screenWidth,Ogre::Real screenHeight)
{
	int cId=getCurrentItem(mouseXPos,mouseYPos);
	if (cId!=-1)
	{
		if (cId!=prevId)
		{
			Active_Item* curItem=allRegions.at(cId);
			highLightNode->setPosition((screenWidth/2-curItem->visibleRegionCoordinates[0]*screenWidth)/ScreenScale,
										(screenHeight/2-curItem->visibleRegionCoordinates[1]*screenHeight)/ScreenScale,-45);
			prevId=cId;
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

void ActiveControlManager::activateCurrentItem()
{
	//targetFunction(allRegions.at(prevId)->targetID);
}