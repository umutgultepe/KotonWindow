/*
	This file is part of Hikari, a library that allows developers
	to use Flash in their Ogre3D applications.

	Copyright (C) 2008 Adam J. Simmons

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
/*********************************************************************************************
   ___ ___ .__ __                 .__ 
 /   |   \|__|  | _______ _______|__|
/    ~    \  |  |/ /\__  \\_  __ \  |
\    Y    /  |    <  / __ \|  | \/  |
 \___|_  /|__|__|_ \(____  /__|  |__|  v0.4
       \/         \/     \/          


* Zed Games PC Development Team - Jaime Crespillo Vilchez (jcrespillo@zed.com)
* Build: 0.1 - Date: 13/10/2008
* Undocked version of Hikari Lib
* Brief: RenderListener for Ogre Engine 
**********************************************************************************************/


#include "Ogre.h"
#include "OgrePanelOverlayElement.h"
#include "Position.h"
#ifndef __OgreViewportOverlay_H__
#define __OgreViewportOverlay_H__

using namespace Ogre;

class OgreViewportOverlay : public Ogre::RenderTargetListener
{
public:
	bool isVisible;
	int width, height;
	Hikari::Position position;

	Ogre::Viewport* viewport;
	Ogre::PanelOverlayElement* panel;
	Ogre::Overlay* overlay;

	OgreViewportOverlay(const std::string& name, Ogre::Viewport* viewport, int width, int height, const Hikari::Position& pos, const std::string& matName, unsigned short zOrder);
	~OgreViewportOverlay();

	inline PanelOverlayElement* getPanel(){return panel;}
	inline Viewport* getViewport(){return viewport;}
	inline Overlay* getOverlay(){return overlay;}

	inline void setViewport(Ogre::Viewport* _viewport){viewport = _viewport;};
	inline void setOverlay(Ogre::Overlay* _overlay){overlay = _overlay;};
	inline void setPanelOverlayElement(Ogre::PanelOverlayElement* _panel) {panel =_panel;};

	void move(int deltaX, int deltaY);
	void setPosition(const Hikari::Position& position);
	void resetPosition();
	
	void resize(int width, int height);

	void hide();
	void show();
	
	int getRelativeX(int absX);
	int getRelativeY(int absY);

	bool isWithinBounds(int absX, int absY);
	////////////////////////////////////////////////////////////////////////////////////////
	//Event Listener inherited methods
	void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
	void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
	void preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt);
	void postViewportUpdate(const Ogre::RenderTargetViewportEvent& evt);
	void viewportAdded(const Ogre::RenderTargetViewportEvent& evt);
	void viewportRemoved(const Ogre::RenderTargetViewportEvent& evt);
	////////////////////////////////////////////////////////////////////////////////////////
};



#endif
