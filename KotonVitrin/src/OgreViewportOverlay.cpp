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
#include "StdAfx.h"
#include "OgreViewportOverlay.h"
using namespace Ogre;

OgreViewportOverlay::OgreViewportOverlay(const std::string& name, Viewport* _viewport, int _width, int _height, const Hikari::Position& pos, const std::string& matName, unsigned short zOrder)
{
	width = _width;
	height = _height;
	position = pos;
	isVisible = true;
	
	viewport=0;
	panel=0;
	overlay=0;

	OverlayManager& overlayManager = OverlayManager::getSingleton();
	viewport = _viewport;
	
	panel = static_cast<PanelOverlayElement*>(overlayManager.createOverlayElement("Panel", name + "Panel"));
	getPanel()->setMetricsMode((Ogre::GMM_PIXELS));
	getPanel()->setMaterialName(matName);
	getPanel()->setDimensions(width, height);
	
	overlay = overlayManager.create(name + "Overlay");
	getOverlay()->add2D(panel);
	getOverlay()->setZOrder(zOrder);
	
	resetPosition();
	
	viewport->getTarget()->addListener(this);
}

OgreViewportOverlay::~OgreViewportOverlay()
{
	getViewport()->getTarget()->removeListener(this);
	if(overlay)
	{
		getOverlay()->remove2D(panel);
		OverlayManager::getSingletonPtr()->destroyOverlayElement(getPanel());
		OverlayManager::getSingletonPtr()->destroy(getOverlay());
	}
}

void OgreViewportOverlay::move(int deltaX, int deltaY)
{
	getPanel()->setPosition(getPanel()->getLeft()+deltaX, getPanel()->getTop()+deltaY);
}

void OgreViewportOverlay::setPosition(const Hikari::Position& position)
{
	this->position = position;
	resetPosition();
}

void OgreViewportOverlay::resetPosition()
{
	int viewWidth = getViewport()->getActualWidth();
	int viewHeight = getViewport()->getActualHeight();

	if(position.usingRelative)
	{
		int left = 0 + position.data.rel.x;
		int center = (viewWidth/2)-(width/2) + position.data.rel.x;
		int right = viewWidth - width + position.data.rel.x;

		int top = 0 + position.data.rel.y;
		int middle = (viewHeight/2)-(height/2) + position.data.rel.y;
		int bottom = viewHeight-height + position.data.rel.y;

		switch(position.data.rel.position)
		{
		case Hikari::Left:
			getPanel()->setPosition(left, middle);
			break;
		case Hikari::TopLeft:
			getPanel()->setPosition(left, top);
			break;
		case Hikari::TopCenter:
			getPanel()->setPosition(center, top);
			break;
		case Hikari::TopRight:
			getPanel()->setPosition(right, top);
			break;
		case Hikari::Right:
			getPanel()->setPosition(right, middle);
			break;
		case Hikari::BottomRight:
			getPanel()->setPosition(right, bottom);
			break;
		case Hikari::BottomCenter:
			getPanel()->setPosition(center, bottom);
			break;
		case Hikari::BottomLeft:
			getPanel()->setPosition(left, bottom);
			break;
		case Hikari::Center:
			getPanel()->setPosition(center, middle);
			break;
		default:
			getPanel()->setPosition(position.data.rel.x, position.data.rel.y);
			break;
		}
	}
	else
		getPanel()->setPosition(position.data.abs.left, position.data.abs.top);
}

void OgreViewportOverlay::resize(int width, int height)
{
	this->width = width;
	this->height = height;
	getPanel()->setDimensions(width, height);
}

void OgreViewportOverlay::hide()
{
	isVisible = false;
}

void OgreViewportOverlay::show()
{
	isVisible = true;
}

int OgreViewportOverlay::getRelativeX(int absX)
{
	return absX - getViewport()->getActualLeft() - getPanel()->getLeft();
}

int OgreViewportOverlay::getRelativeY(int absY)
{
	return absY - getViewport()->getActualTop() - getPanel()->getTop();
}

bool OgreViewportOverlay::isWithinBounds(int absX, int absY)
{
	int localX = getRelativeX(absX);
	int localY = getRelativeY(absY);

	if(localX > 0 && localX < width)
		if(localY > 0 && localY < height)
			return true;

	return false;
}
////////////////////////////////////////////////////////////////////////////////////////
//Event Listener inherited methods
void OgreViewportOverlay::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
{
}

void OgreViewportOverlay::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
{
}

void OgreViewportOverlay::preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt)
{
	if(evt.source == viewport && isVisible)
		getOverlay()->show();
}

void OgreViewportOverlay::postViewportUpdate(const Ogre::RenderTargetViewportEvent& evt)
{
	getOverlay()->hide();
}

void OgreViewportOverlay::viewportAdded(const Ogre::RenderTargetViewportEvent& evt)
{
}

void OgreViewportOverlay::viewportRemoved(const Ogre::RenderTargetViewportEvent& evt)
{
}
////////////////////////////////////////////////////////////////////////////////////////

