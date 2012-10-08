#pragma once
#include "StdAfx.h"
#ifndef ScreenScale
#define ScreenScale 10
#endif
 enum Region
{
	TYPE_1,  
	TYPE_2, 
	TYPE_3,
	TYPE_4
};
class ActiveControlManager;

class Active_Item
{
public:
	//Ogre::OverlayElement* element;
	String elementName;
	Ogre::Real elementWidth;
	Ogre::Real elementHeight;
	// Kordinatlar x1,y1,x2,y2 şeklinde verilmeli , x1<x2, y1<y2 
	Ogre::Real activeRegionCoordinates[4];

	// kordinatlar left,top olarak verilmeli, relative to the screen size
	Ogre::Real visibleRegionCoordinates[2];
	bool isVisible;
	ActiveControlManager* manager;
	
	int targetID;
	
	Active_Item(String name,Ogre::Real activeCoordinates[4],Ogre::Real visibleCoordinates[2],Ogre::Real visibleSize[2],int target,ActiveControlManager* tManager);
	~Active_Item();
	//Overlay elementini visible yapar
	//void setVisible(bool visible);
	////Overlay elementinin visibilitesinin döndürür
	//bool getVisible(); 
	//
	Ogre::Real* getCoordinates();
	
} ;


class ActiveControlManager
{
public:
	ActiveControlManager();
	~ActiveControlManager(void);
	int prevId;
	Ogre::RenderWindow* window;
	//Ogre::OverlayElement *item; 
	//Ogre::RenderSystem* renderSystem;
	//Ogre::RenderWindow* mRenderer;
	int numOfItems;
	std::vector<Active_Item*> allRegions;
	int getCurrentItem(Ogre::Real mouseXPos,Ogre::Real mouseYPos);
	
	bool highlight(Ogre::Real mouseXPos,Ogre::Real mouseYPos,Ogre::SceneNode* highLightNode,Ogre::Real screenWidth,Ogre::Real screenHeight);
	bool highlightKinect(Ogre::Real handXPos,Ogre::Real handYPos,Ogre::SceneNode* highLightNode,Ogre::Real screenWidth,Ogre::Real screenHeight);
	void createItem(String name,Ogre::Real region[4],Ogre::Real activePart[2],Ogre::Real activeWidth,Ogre::Real activeHeight,int target);
	void setItemVisibility(int itemIndex);
	void activateTargetItem(int targetIndex);	
	int getCurrentTarget();
	//void createRegion(Region regionID,int regionNumber); 
	//ActiveControl* createActiveRegion(ActiveControlManager* region,int width,int heigth,enum alignment);
	//ActiveControl* getCurrentSelection(float x,float y);
	void setAutoVisibility(bool visible);
	void activateCurrentItem();
	//Ogre::RenderWindow* getRenderWindow();
	//Ogre::RenderWindow* getRenderWindow();
};

