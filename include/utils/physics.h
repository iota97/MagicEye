#pragma once

#include <bullet/btBulletDynamicsCommon.h>

enum shapes {
    BOX,
    SPHERE
};

class Physics {
public:
    btDiscreteDynamicsWorld* dynamicsWorld;
    btAlignedObjectArray<btCollisionShape*> collisionShapes;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;

    Physics() {
       collisionConfiguration = new btDefaultCollisionConfiguration();
       dispatcher = new btCollisionDispatcher(collisionConfiguration);
       overlappingPairCache = new btDbvtBroadphase();
       solver = new btSequentialImpulseConstraintSolver();
       dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

       dynamicsWorld->setGravity(btVector3(0, -9.82, 0));
    }

    btRigidBody* createRigidBody(int type, glm::vec3 pos, glm::vec3 size, glm::vec3 rot, float m, float friction, float restitution) {
        btCollisionShape* cShape = NULL;
        btVector3 position = btVector3(pos.x, pos.y, pos.z);
        btQuaternion rotation;
        rotation.setEuler(rot.x, rot.y, rot.z);

        if (type == BOX) {
            btVector3 dim = btVector3(size.x, size.y, size.z);
            cShape = new btBoxShape(dim);
        } else if (type == SPHERE) {
            cShape = new btSphereShape(size.x);
        }

        collisionShapes.push_back(cShape);

        btTransform objTransform;
        objTransform.setIdentity();
        objTransform.setRotation(rotation);
        objTransform.setOrigin(position);

        btScalar mass = m;
        bool isDynamic = (mass != 0);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            cShape->calculateLocalInertia(mass, localInertia);

        btDefaultMotionState *motionState = new btDefaultMotionState(objTransform);

        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, cShape, localInertia);
        rbInfo.m_friction = friction;
        rbInfo.m_restitution = restitution;

        if (type == SPHERE) {
            rbInfo.m_angularDamping = 0.3;
            rbInfo.m_rollingFriction = 0.3;
        }

        btRigidBody *body = new btRigidBody(rbInfo);
        dynamicsWorld->addRigidBody(body);

        return body;
    }

    void Clear() {
        for (int i = dynamicsWorld->getNumCollisionObjects()-1; i >= 0; --i) {
            btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);
            if (body && body->getMotionState()) {
                delete body->getMotionState();
            }
            dynamicsWorld->removeCollisionObject(obj);
            delete obj;
        }
        delete dynamicsWorld;
        delete solver;
        delete overlappingPairCache;
        delete dispatcher;
        delete collisionConfiguration;
        collisionShapes.clear();
    }
};