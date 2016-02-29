#pragma once

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreWindowEventUtilities.h>
#include <OgreRenderTargetListener.h> 

#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISInputManager.h>

#include <unordered_map>
#include <string>
#include "SoundAdapter.h"
#include "OISManager.h"
#include "Simulator.h"
#include "Ball.h"
#include "Paddle.h"
#include "Wall.h"
#include "OgreText.h"

class Application : public Ogre::FrameListener, public Ogre::WindowEventListener, public Ogre::RenderTargetListener
{
public:
	Application();
	virtual ~Application();

	virtual void init();

	Ogre::Root * mRoot;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	Ogre::RenderWindow * mRenderWindow;
	Ogre::SceneManager * mSceneManager;
	Ogre::Camera * mCamera;
	Ogre::Camera * ballCam;
	Ogre::Timer* t1;
	SoundAdapter* sa;
	OISManager* _oisManager;
	Simulator* _simulator;
	GameObject* _theBall;
	GameObject* _thePaddle;
	OgreText* score;
	std::vector<Ogre::Camera*> cameras;

	int points;
	int width;
	int height;

	double fps = 300.0;

	bool mRunning = true;

	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt) override;
	void createRootEntity(std::string name, std::string mesh, int x, int y, int z);
	void createChildEntity(std::string name, std::string mesh, Ogre::SceneNode* sceneNode, int x, int y, int z);
	void update(const Ogre::FrameEvent &evt);
	void movePaddle();
	Ball* createBall(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim);
	Paddle* createPaddle(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim);
	Wall* createWall(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Vector3 scale, Ogre::Degree pitch, Ogre::Degree yaw, Ogre::Degree roll, Ogre::SceneManager* scnMgr, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim);
	void setupResources(void);
};
