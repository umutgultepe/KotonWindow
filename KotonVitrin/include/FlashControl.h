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
* Brief: Implementation of IFlashControl for Ogre Engine
**********************************************************************************************/

#ifndef __FlashControl_H__
#define __FlashControl_H__

#include "Ogre.h"
#include "IFlashControl.h"
#include "OgreViewportOverlay.h"

/**
* The Ogre-FlashControl class is an instance of the Flash Player that is rendered dynamically to a texture and
* wrapped in an optional movable overlay using the Ogre3D engine.
* NOTES: Based on original FlashControl Class, but undocked of Hikari Lib by Zed Games Development Team
*/

class FlashControl : public Hikari::IFlashControl, public Ogre::ManualResourceLoader
{
	/////////////////////////////////////////////////////////////
	friend class Hikari::Impl::FlashSite;
	friend class Hikari::Impl::FlashHandler;
	friend class Hikari::HikariManager;
public:
	/////////////////////////////////////////////////////////////

	bool usingAlphaHack;
	OgreViewportOverlay* overlay;
	size_t texWidth, texHeight, texDepth, texPitch;
	std::string materialName;
	bool isIgnoringTransparent;
	float transThreshold;
	
	//these were static in update function!!! --> causing buffers crashing when you work with many FlashControls
	int lastDirtyWidth;
	int lastDirtyHeight;

	typedef std::map<std::wstring, Hikari::FlashDelegate> DelegateMap;
	typedef DelegateMap::iterator DelegateIter;
	DelegateMap delegateMap;
	
	TexturePtr texture;
	TextureUnitState* texUnit;

	/////////////////////////////////////////////////////////////
	FlashControl(const std::string& name, Viewport* viewport, int width, int height, const Hikari::Position& position, unsigned short zOrder);
	FlashControl(const std::string& name, int width, int height);
	~FlashControl();

	/////////////////////////////////// INLINE METHODS : GETTERS/SETTERS ////////
	inline void setZOrder(unsigned short zorder){
		overlay->getOverlay()->setZOrder(zorder);
	};
	inline unsigned short getZOrder(){
		return overlay->getOverlay()->getZOrder();
	};
	inline int getRelativeX(int absX){
		return overlay->getRelativeX(absX);
	};
	inline int getRelativeY(int absY){
		return overlay->getRelativeY(absY);
	};
	// maybe we will need for keeping the access to the HIKARI object´s model
	/*inline unsigned short getPanelZOrder(){
		return overlay->getPanel()->getZOrder();
	};*/
	////////////////////////////////////// PUBLIC METHODS //////////////////////
	void createControl();
	void createMaterial();
	bool isPointOverMe(int screenX, int screenY);
	void invalidateTotally();
	void handleKeyEvent(UINT msg, WPARAM wParam, LPARAM lParam);
	void handleFlashCall(const std::wstring& xmlString);
	void update();
	/**
	* Loads a movie (a .swf file) into this FlashControl and begins playing.
	*
	* @param	movieFilename	The filename of the movie to load.
	*
	* @note	The specified movie should reside in the "assetsDirectory" that
	*		was declared when the HikariManager was instantiated.
	*/
	void load(const std::string& movieFilename, Hikari::HikariManager* hikariMgr);

	/**
	* Plays the currently-loaded movie.
	*/
	void play();

	/**
	* Stops the currently-loaded movie.
	*/
	void stop();

	/**
	* Rewinds the currently-loaded movie to the beginning.
	*/
	void rewind();

	/*JOTA*/
//	void setAViewportOverlay(AViewportOverlay* _overlay);

	/**
	* Navigates the currently-loaded movie to a certain frame and stops there.
	*/
	void gotoFrame(long frameNum);

	/**
	* Sets whether or not the currently-loaded movie should restart when
	* it reaches the end.
	*
	* @param	shouldLoop	Whether the currently-loaded movie should loop.
	*/
	void setLoop(bool shouldLoop);

	/**
	* Sets whether or not the currently-loaded movie should use a
	* transparent background instead of the default background-color.
	*
	* @param	isTransparent	Whether or not the movie should use "transparent" rendering.
	* @param	useAlphaHack	With some Flash versions, there are certain glitches with
	*							transparent rendering (usually with text and aliased geometry).
	*							Set this parameter to 'true' to use an alternative alpha-rendering
	*							hack that may mitigate these issues at the cost of some performance.
	*/
	void setTransparent(bool isTransparent, bool useAlphaHack = false);

	/**
	* Sets the Flash rendering quality for the currently-loaded movie.
	*
	* @param	renderQuality	The RenderQuality to use.
	*/
	void setQuality(short renderQuality);

	/**
	* Sets the scaling mode to use when the aspect ratio of the movie and control do not match.
	*
	* @param	scaleMode	The ScaleMode to use.
	*/
	void setScaleMode(short scaleMode);

	/**
	* Sets whether this FlashControl is draggable via the right-mouse-button,
	* this is only applicable to FlashControls created as an overlay.
	*
	* @param	isDraggable		Whether or not this FlashControl should be draggable.
	*/
	void setDraggable(bool isDraggable);

	/**
	* Sets whether or not mouse-clicks over transparent pixels should be ignored (this is on by default),
	* this is only applicable to transparent FlashControls created as an overlay.
	*
	* @param	shouldIgnore	Whether or not transparent pixels should be ignored.
	* @param	threshold	The opacity threshold (in percent, 0 to 1.0), pixels with
	*						opacities less than this amount will be ignored.
	*/
	void setIgnoreTransparentPixels(bool shouldIgnore, float threshold = 0.04);

	/**
	* Returns the name of this FlashControl.
	*/
	const std::string & getName() const;

	/**
	* Retrieves the name of the Ogre::Material used by this FlashControl.
	*/
	const std::string & getMaterialName() const;

	/**
	* Binds a local callback to a certain function name so that your Flash movie can call the function
	* from ActionScript using ExternalInterface.call('functionName').
	*
	* @param	funcName	The name to bind this callback to.
	* @param	callback	The local function to call, see below for examples of declaring a FlashDelegate.
	*
	*	\code
	*	// Example declaration of a compatible function (static function):
	*	FlashValue myStaticFunction(FlashControl* caller, const Arguments& args)
	*	{
	*		// Handle the callback here
	*		return FLASH_VOID;
	*	}
	*
	*	// Example declaration of a compatible function (member function):
	*	FlashValue MyClass::myMemberFunction(FlashControl* caller, const Arguments& args)
	*	{
	*		// Handle the callback here
	*		return "Some return value!";
	*	}
	*
	*	// FlashDelegate (member function) instantiation:
	*	FlashDelegate callback(this, &MyClass::myMemberFunction); // within a class
	*	FlashDelegate callback2(pointerToClassInstance, &MyClass::myMemberFunction);
	*
	*	// FlashDelegate (static function) instantiation:
	*	FlashDelegate callback(&myStaticFunction);
	*	\endcode
	*/
	
	void bind(const std::wstring& funcName, const Hikari::FlashDelegate& callback);

	/**
	* Un-binds the specified callback.
	*
	* @param	funcName	The name that the callback was bound to.
	*/
	void unbind(const std::wstring& funcName);

	/**
	* Attempts to call a function declared as a callback in the ActionScript of the currently-loaded movie.
	* 
	* @param	funcName	The name of the callback that was declared using 'ExternalInterface.addCallback(funcName, function)'
	*						in the ActionScript of the currently-loaded movie.
	* @param	args	The arguments to pass to the ActionScript function.
	*
	* @return	If the invocation was successful and the ActionScript function returned a value, returns a FlashValue with a non-null type.
	*
	* @note	It is highly recommended to use the 'Args(arg1)(arg2)(arg3)...' helper class to pass arguments.
	*/
	Hikari::FlashValue callFunction(const std::wstring& funcName, const Hikari::Arguments& args = Hikari::Args());

	/**
	* If this FlashControl was created as an overlay, hides the overlay.
	*/
	void hide();

	/**
	* If this FlashControl was created as an overlay, shows the overlay.
	*/
	void show();

	/**
	* Returns whether or not the FlashControl overlay is currently visible. (See FlashControl::hide and FlashControl::show)
	* If this FlashControl was not created as an overlay, always returns false.
	*/
	bool getVisibility() const;

	/**
	* Sets the opacity of this FlashControl.
	*
	* @param	opacity	The opacity as a Real value; 0 is totally transparent, 1 is totally opaque.
	*/
	void setOpacity(float opacity);

	/**
	* Gives this FlashControl keyboard focus. Additionally, if this FlashControl is an overlay, pops it to the front.
	*/
	void focus();

	/**
	* If this FlashControl was created as an overlay, moves the overlay in relative amounts.
	*
	* @param	deltaX	The amount (in pixels) to move the overlay in the X-axis.
	* @param	deltaY	The amount (in pixels) to move the overlay in the Y-axis.
	*/
	void move(int deltaX, int deltaY);

	/**
	* Retrieves the width and height that this FlashControl was created with.
	*
	* @param[out]	width	The unsigned short that will be used to store the retrieved width.
	*
	* @param[out]	height	The unsigned short that will be used to store the retrieved height.
	*/
	void getExtents(unsigned short &width, unsigned short &height) const;

	/**
	* Gets the UV scale of this FlashControl's internal texture. On certain systems we must compensate for lack of
	* NPOT-support on the videocard by using the next-highest POT texture. Normally, FlashControl overlays compensate 
	* their texture coordinates automatically however FlashControls created as pure materials will need to adjust 
	* their own by use of this function.
	*
	* @param[out]	uScale	The Ogre::Real that will be used to store the retrieved U-scale.
	* @param[out]	vScale	The Ogre::Real that will be used to store the retrieved V-scale.
	*/
	void getUVScale(float &uScale, float &vScale) const;

	/**
	* Injects a mouse-move event into this FlashControl (in the control's local coordinate-space).
	*
	* @param	xPos	The local X-coordinate.
	* @param	yPos	The local Y-coordinate.
	*/
	void injectMouseMove(int xPos, int yPos);

	/**
	* Injects a mouse-down event into this FlashControl (in the control's local coordinate-space).
	*
	* @param	xPos	The local X-coordinate.
	* @param	yPos	The local Y-coordinate.
	*/
	void injectMouseDown(int xPos, int yPos);

	/**
	* Injects a mouse-up event into this FlashControl (in the control's local coordinate-space).
	*
	* @param	xPos	The local X-coordinate.
	* @param	yPos	The local Y-coordinate.
	*/
	void injectMouseUp(int xPos, int yPos);

	/**
	* Injects a mouse-wheel event into this FlashControl (in the control's local coordinate-space).
	*
	* @param	relScroll	The relative scroll amount of the mouse-wheel.
	* @param	xPos	The local X-coordinate of the mouse.
	* @param	yPos	The local Y-coordinate of the mouse.
	*/
	void injectMouseWheel(int relScroll, int xPos, int yPos);

	void loadResource(Resource* resource);
	
};

#endif