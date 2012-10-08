#include "StdAfx.h"
#include "StatisticsManager.hpp"
#include "AppState.hpp"
extern std::vector<Ogre::String> StateNames;

userStatistics::userStatistics(float startingTime,int numOfItems)
	:stateTimes(NUMBER_OF_STATES),
	itemOpenCount(numOfItems),
	likedItemCount(numOfItems)
{
	for(int i=0;i<NUMBER_OF_STATES;i++)
		stateTimes.at(i)=0;
	for(int i=0;i<numOfItems;i++)
		itemOpenCount.at(i)=0;
	for(int i=0;i<numOfItems;i++)
		likedItemCount.at(i)=0;
	startTime=startingTime;
	timeOfLastChange=startingTime;
	endTime=0;
	sharedPictureCount=0;
	
}

userStatistics::~userStatistics(void)	
{
	stateTimes.clear();
	likedItemCount.clear();
	itemOpenCount.clear();
}

StatisticsManager::StatisticsManager(Ogre::Root* ogreRoot,Ogre::String filename)
	:mRoot(ogreRoot),
	mFilename(filename+".csv"),
	currentUser(0)
{
	numberOfUsers=0;
	activeStateID=0;
	userActive=false;
	numberOfItems=0;	
	myfile=new std::ofstream(filename+".csv");
}



void DisplayErrorBox(LPTSTR lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and clean up

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}




int StatisticsManager::loadItemNames(TCHAR* dir)
{
	WIN32_FIND_DATA ffd;
   TCHAR szDir[MAX_PATH];
   TCHAR tDir[MAX_PATH];
   size_t length_of_arg;
   HANDLE hFind = INVALID_HANDLE_VALUE;
   DWORD dwError=0;
   
   // If the directory is not specified as a command-line argument,
   // print usage.

   // Check that the input path plus 3 is not longer than MAX_PATH.
   // Three characters are for the "\*" plus NULL appended below.

   StringCchLength(dir, MAX_PATH, &length_of_arg);

   if (length_of_arg > (MAX_PATH - 3))
   {
      _tprintf(TEXT("\nDirectory path is too long.\n"));
      return (-1);
   }

   // Prepare string for use with FindFile functions.  First, copy the
   // string to a buffer, then append '\*' to the directory name.

   StringCchCopy(szDir, MAX_PATH, dir);
   StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

   // Find the first file in the directory.

   hFind = FindFirstFile(szDir, &ffd);

   if (INVALID_HANDLE_VALUE == hFind) 
   {
      DisplayErrorBox(TEXT("FindFirstFile"));
      return dwError;
   } 
   
   // List all the files in the directory with some info about them.

   do
   {
      if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
		  Ogre::String tName(ffd.cFileName);
		  if (tName!="." && tName!="..")
		  {
		   StringCchCopy(tDir, MAX_PATH, dir);
		   StringCchCat(tDir, MAX_PATH, TEXT("\\"));
		   StringCchCat(tDir, MAX_PATH, TEXT(ffd.cFileName));
		   loadItemNames(tDir);
		  }
      }
      else
      {	  
		  mItemNames.push_back(Ogre::String(ffd.cFileName));
		  numberOfItems++;
      }
   }
   while (FindNextFile(hFind, &ffd) != 0);
 
   dwError = GetLastError();
   if (dwError != ERROR_NO_MORE_FILES) 
   {
      DisplayErrorBox(TEXT("FindFirstFile"));
   }

   FindClose(hFind);
   return dwError;
}


int StatisticsManager::findByName(Ogre::String ItemName)
{

	for (unsigned int i=0;i<mItemNames.size();i++)
	{
		if (mItemNames.at(i)==ItemName)
		{
			return i;
		}
	}
	return -1;
}

void StatisticsManager::typeHeaders()
{

	

	if (myfile->is_open())
	{
		*myfile<<"Kullanici Sayisi,";
		*myfile<<"Toplam Harcadigi Zaman,";
		*myfile<<"Paylastigi Resim Sayisi,";

		for (int j=0;j<NUMBER_OF_STATES;j++)
			*myfile<<StateNames.at(j)+"te Harcanan Zaman"+",";

		for (int j=0;j<numberOfItems;j++)
			*myfile<<mItemNames.at(j)+" Tiklanma Sayisi"+",";

		for (int j=0;j<numberOfItems;j++)
				*myfile<<mItemNames.at(j)+" Begenilme Sayisi"+",";
		
		*myfile<<"\n";

		totalColumnCount=NUMBER_OF_STATES+2*numberOfItems+1/*for total time*/+1/*for shared picture count*/;
		averageTimes=new std::vector<float>(totalColumnCount);

		for (int i=0;i<totalColumnCount;i++)
			averageTimes->at(i)=0;
		myfile->close();
	}
	
}


StatisticsManager::~StatisticsManager(void)
{
	outputUserData();
	delete myfile;
}


void StatisticsManager::newUser()
{
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Creating new statistics data");
	currentUser=new userStatistics(mRoot->getTimer()->getMilliseconds(),numberOfItems);
	numberOfUsers++;
	userActive=true;
}
void StatisticsManager::lostUser()
{

	if (currentUser!=0)
	{
		myfile->open(mFilename.c_str(),std::ios::in |std::ios::ate | std::ios::out);
		//myfile->seekp(0,std::ios::end);

		OgreFramework::getSingletonPtr()->m_pLog->logMessage("Adding the last times of the user");

		unsigned int curTime=mRoot->getTimer()->getMilliseconds();
		currentUser->endTime=curTime;
		currentUser->stateTimes.at(activeStateID)+=(curTime-currentUser->timeOfLastChange);
		userActive=false;

		if (myfile->is_open())
		{
			OgreFramework::getSingletonPtr()->m_pLog->logMessage("Saving time spent");
			*myfile<<Ogre::StringConverter::toString(numberOfUsers)+",";
			*myfile<<Ogre::StringConverter::toString((float)(currentUser->endTime-currentUser->startTime)/1000);
			averageTimes->at(0)+=(float)(currentUser->endTime-currentUser->startTime)/1000;
			OgreFramework::getSingletonPtr()->m_pLog->logMessage("Saving Shared Picture Count");
			*myfile<<",";
			*myfile<<Ogre::StringConverter::toString((float)currentUser->sharedPictureCount);
			averageTimes->at(1)+=(float)(currentUser->sharedPictureCount);
			OgreFramework::getSingletonPtr()->m_pLog->logMessage("Saving State Times");
			*myfile<<",";
			for (int j=0;j<NUMBER_OF_STATES;j++)
			{
				*myfile<<Ogre::StringConverter::toString((float)currentUser->stateTimes.at(j)/1000)+",";
				averageTimes->at(2+j)+=(float)currentUser->stateTimes.at(j)/1000;
			}
			OgreFramework::getSingletonPtr()->m_pLog->logMessage("Saving Item Numbers");
			for (int j=0;j<numberOfItems;j++)
			{
				*myfile<<Ogre::StringConverter::toString(currentUser->itemOpenCount.at(j))+",";
				averageTimes->at(2+NUMBER_OF_STATES+j)+=currentUser->itemOpenCount.at(j);
			}
			OgreFramework::getSingletonPtr()->m_pLog->logMessage("Saving Like Numbers");
			for (int j=0;j<numberOfItems;j++)
			{
				*myfile<<Ogre::StringConverter::toString(currentUser->likedItemCount.at(j))+",";
				averageTimes->at(2+NUMBER_OF_STATES+numberOfItems+j)+=currentUser->likedItemCount.at(j);
			}

			*myfile<<"\n";
			myfile->close();
		}
		OgreFramework::getSingletonPtr()->m_pLog->logMessage("Deleting the temporary data");
		delete currentUser;
		currentUser=0;

			
	}
	else
	{
		OgreFramework::getSingletonPtr()->m_pLog->logMessage("Was a phantom user, skipping record");
		numberOfUsers--;
	}


}

void StatisticsManager::changedState(int stateId)
{
	if (userActive)
	{
		unsigned int curTime=mRoot->getTimer()->getMilliseconds();
		currentUser->stateTimes.at(activeStateID)+=(curTime-currentUser->timeOfLastChange);
		currentUser->timeOfLastChange=curTime;
	}
	activeStateID=stateId;

}

void StatisticsManager::clickedItem(Ogre::String itemName)
{
	int t=findByName(itemName);
	if (t!=-1)
		currentUser->itemOpenCount.at(t)++;

}
void StatisticsManager::sharedPicture()
{
	currentUser->sharedPictureCount++;
}

void StatisticsManager::likedItem(Ogre::String itemName)
{
	int t=findByName(itemName);
	if (t!=-1)
	currentUser->likedItemCount.at(t)++;
}


using namespace std;
void StatisticsManager::outputUserData()
{
	myfile->open(mFilename.c_str(),std::ios::in |std::ios::ate | std::ios::out);
	//myfile->seekp(0,std::ios::end);
	if (myfile->is_open())
	{
		if (numberOfUsers>0)
		{
			for (int i=0;i<totalColumnCount;i++)
				averageTimes->at(i)/=numberOfUsers;
		
			*myfile<<"\n";

			*myfile<<"Ortalamalar,";
			for (int i=0;i<totalColumnCount;i++)
				*myfile<<Ogre::StringConverter::toString(averageTimes->at(i))+",";
		}
		

		myfile->close();
	}

}