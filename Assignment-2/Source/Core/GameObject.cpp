#include "GameObject.h"
#include "MultiPlatformHelper.h"
#include <btBulletDynamicsCommon.h>
#include <OgreWireBoundingBox.h>

//Add the game object to the simulator
GameObject::GameObject(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin) :
	name(nme), type(tp), sceneMgr(scnMgr), rootNode(node), geom(ent), scale(scal), motionState(ms), simulator(sim), tr(), inertia(), restitution(rest), friction(frict), kinematic(kin),
	needsUpdates(false), mass(mss) {
		inertia.setZero();
		startPos = Ogre::Vector3(rootNode->getPosition());
}

GameObject::GameObject(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Vector3 scal, bool kin) :
	name(nme), type(tp), sceneMgr(scnMgr), rootNode(node), geom(ent), vscale(scal), motionState(ms), simulator(sim), tr(), inertia(), restitution(rest), friction(frict), kinematic(kin),
	needsUpdates(false), mass(mss) {
		inertia.setZero();
		startPos = Ogre::Vector3(rootNode->getPosition());
}

void GameObject::updateTransform() {
	Ogre::Vector3 pos = rootNode->getPosition();
	tr.setOrigin(btVector3(pos.x, pos.y, pos.z));
	Ogre::Quaternion qt = rootNode->getOrientation();
	tr.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));
	motionState->updateTransform(tr);
}

void GameObject::translate(float x, float y, float z) {
	if(kinematic) {
		rootNode->translate(x,y,z);
		updateTransform();
	}
	else {
		body->translate(btVector3(x,y,z));
	}
}

void GameObject::setPosition(float x, float y, float z) {
	if(kinematic) {
		rootNode->setPosition(x,y,z);
		updateTransform();
	}
	else {
		btTransform transform = body->getCenterOfMassTransform();
		transform.setOrigin(btVector3(x, y, z));
		body->setCenterOfMassTransform(transform);
		updateTransform();
	}
}

void GameObject::setPosition(const Ogre::Vector3& pos) {
	setPosition(pos.x, pos.y, pos.z);
}

Ogre::SceneNode* GameObject::getNode() {
	return rootNode;
}

void GameObject::reset() {
	setPosition(startPos);
	body->setLinearVelocity(btVector3(0,0,0));
}

void GameObject::applyForce(float x, float y, float z) {
	body->applyCentralForce(btVector3(x, y, z));
}

void GameObject::movePaddle(OISManager* _oisManager, int height, int width) {}

void GameObject::addToSimulator() {
	// using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	 updateTransform();

	// rigidbody is dynamic if and only if mass is non zero, otherwise static
	if (mass != 0.0f) 
		shape->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
	rbInfo.m_restitution = restitution;
	rbInfo.m_friction = friction;
	body = new btRigidBody(rbInfo);
	body->setUserPointer(this);

	if (kinematic) {
		std::cout << "Kinematic\n"; 
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setActivationState(DISABLE_DEACTIVATION);
	}

	context = new CollisionContext();
	cCallBack = new BulletContactCallback(*body, *context);
	simulator->addObject(this);
}

int GameObject::getPoints(){
	return 0;
}

Ogre::String GameObject::getName(){
	return name;
}

GameObject::objectType GameObject::getType(){
	return type;
}

void GameObject::setPoints(int points){
	//overwritten in ball class
}
void GameObject::resetScore() {
	//Overridden in ball class
}

void GameObject::showColliderBox() {
	auto var = geom->getBoundingBox();

	// Bullet uses half margins for collider
	auto size = var.getSize() / 2;

	btVector3 min(0, 0, 0);
	btVector3 max(0, 0, 0);
	shape->getAabb(tr, min, max);
	Ogre::WireBoundingBox* box = new Ogre::WireBoundingBox();
	Ogre::AxisAlignedBox abb(Ogre::Vector3(-size.x*scale, -size.y*scale, -size.z*scale), Ogre::Vector3(size.x*scale, size.y*scale, size.z*scale));
	box->setupBoundingBox(abb);
	box->setVisible(true);
	box->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
	sceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(box);
}