#include "StdAfx.h"

#ifndef __StatisticsManager_h_
#define __StatisticsManager_h_
//#include "AppState.hpp"
//#include "AppStateManager.hpp"




class userStatistics
{
public:
	std::vector<float> stateTimes;
	std::vector<int>	itemOpenCount;
	std::vector<int>	likedItemCount;
	std::vector<Ogre::String> itemNames;
	int sharedPictureCount;
	unsigned int startTime;
	unsigned int timeOfLastChange;
	unsigned int endTime;
	userStatistics(float startingTime,int numOfItems);
	~userStatistics(void);
};

class StatisticsManager
{
private:
	int numberOfUsers;
	int numberOfItems;
	userStatistics* currentUser;
	Ogre::Root* mRoot;
	int activeStateID;
	bool userActive;
	Ogre::String mFilename;
	std::ofstream* myfile;
	int totalColumnCount;
	std::vector<float>* averageTimes;
	std::vector<Ogre::String> mItemNames;
public:
	StatisticsManager(Ogre::Root* ogreRoot,Ogre::String filename);
	~StatisticsManager(void);

	void newUser();
	void lostUser();
	void changedState(int stateId);
	void clickedItem(Ogre::String itemName);
	void sharedPicture();
	void outputUserData();
	int findByName(Ogre::String ItemName);                                                                                                                 
	void typeHeaders();
	void likedItem(Ogre::String itemName);
	int loadItemNames(TCHAR* dir);
};


#endif 