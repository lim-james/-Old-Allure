#include "PhysicsSystem.h"

#include "../Entity/Entity.h"	
#include "../Components/Transform/Transform.h"

#include "Events/EventsManager.h"


PhysicsSystem::PhysicsSystem()
{
	//Events::EventsManager::GetInstance()->Subscribe("BOXCOLLIDER_ACTIVE", &PhysicsSystem::BoxColliderActiveHandler, this);
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::Start() {
	Events::EventsManager::GetInstance()->Subscribe("RIGIDBODY_ACTIVE", &PhysicsSystem::RigidbodyActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("COLLIDER_ACTIVE", &PhysicsSystem::ColliderActiveHandler, this);
}

void PhysicsSystem::Update(const float& t) { }

void PhysicsSystem::FixedUpdate(const float& t) {
	UpdateVelocity(t);

	for (std::vector<Collider*>::iterator it = collider.begin(); it != collider.end(); ++it) {
		Collider *c1 = (Collider*)*it;
		if (c1->IsActive()) {
			for (std::vector<Collider*>::iterator it2 = it + 1; it2 != collider.end(); ++it2) {
				Collider *c2 = (Collider*)*it2;
				if (c2->IsActive()) {
					//if (c1->GetParent()->GetTag() == "ball" && c2->GetParent()->GetTag() == "ball") {
						if (SphereToSphereCollision(c1, c2, c1->data)) {
							if (c1->data->time <= 0)
								CollisionResponse(c1, c2);
						}
					//}
				}
			}
		}
	}
}


void PhysicsSystem::RigidbodyActiveHandler(Events::Event* event) {
	const auto component = static_cast<Events::AnyType<Rigidbody*>*>(event)->data;
	if (component->IsActive()) {
		rigidbody.push_back(component);
	} else {
		rigidbody.erase(vfind(rigidbody, component));
	}
}

void PhysicsSystem::ColliderActiveHandler(Events::Event* event) {
	const auto component = static_cast<Events::AnyType<Collider*>*>(event)->data;
	if (component->IsActive()) {
		collider.push_back(component);
	} else {
		collider.erase(vfind(collider, component));
	}
}

//void PhysicsSystem::BoxColliderActiveHandler(Events::Event* event)
//{
//	const auto component = static_cast<Events::AnyType<BoxCollider*>*>(event)->data;
//	if (component->IsActive())
//	{
//		boxcollider.push_back(component);
//	}
//}

void PhysicsSystem::UpdateVelocity(const float& t) {
	for (auto& r : rigidbody) {
		r->GetParent()->GetComponent<Transform>()->translation += r->velocity * t;
	}
}

bool PhysicsSystem::CollisionCheck(Collider* c1, Collider* c2) {

	if (c1->GetParent()->GetTag() == "ball" && c2->GetParent()->GetTag() == "ball") {
		//vec3f dist = c1->GetParent()->GetComponent<Transform>()->translation - c2->GetParent()->GetComponent<Transform>()->translation;
		//vec3f u = c1->GetParent()->GetComponent<Rigidbody>()->velocity - c2->GetParent()->GetComponent<Rigidbody>()->velocity;

		if (Math::Length(c1->GetParent()->GetComponent<Transform>()->translation - c2->GetParent()->GetComponent<Transform>()->translation) < c1->GetParent()->GetComponent<Transform>()->scale.x) {
			return true;
		}
	}
	else if (c1)

	return false;
}

void PhysicsSystem::CollisionResponse(Collider* c1, Collider* c2) {
	float m1 = c1->GetParent()->GetComponent<Rigidbody>()->mass;
	float m2 = c2->GetParent()->GetComponent<Rigidbody>()->mass;
	vec3f u1 = c1->GetParent()->GetComponent<Rigidbody>()->velocity;
	vec3f u2 = c2->GetParent()->GetComponent<Rigidbody>()->velocity;

	vec3f N = Math::Normalized(c1->GetParent()->GetComponent<Transform>()->translation - c2->GetParent()->GetComponent<Transform>()->translation);
	vec3f u1N = Math::Dot(u1, N) * N;
	vec3f u2N = Math::Dot(u2, N) * N;
					
	c1->GetParent()->GetComponent<Rigidbody>()->velocity = u1 + (2 * m2 / (m1 + m2)) * (u2N - u1N);
	c2->GetParent()->GetComponent<Rigidbody>()->velocity = u2 + (2 * m1 / (m1 + m2)) * (u1N - u2N);

	return;
}

bool PhysicsSystem::SphereToSphereCollision(Collider* c1, Collider* c2, CollisionData* data) {
	Entity *e1 = c1->GetParent();
	Entity *e2 = c2->GetParent();

	Transform* t1 = e1->GetComponent<Transform>();
	Transform* t2 = e2->GetComponent<Transform>();

	Rigidbody* r1 = e1->GetComponent<Rigidbody>();
	Rigidbody* r2 = e2->GetComponent<Rigidbody>();

	const float r = t1->scale.x;

	const vec3f p1 = t1->GetWorldTranslation();
	const vec3f p2 = t2->GetWorldTranslation();

	const vec3f& v1 = r1->velocity;
	const vec3f& v2 = r2->velocity;

	vec3f dir = p2 - p1;
	vec3f u = v1 - v2;

	if (Math::LengthSquared(u) == 0) { return false; }
	if (Math::Dot(u, dir) < 0.f) { return false; }

	const float a = Math::Dot(u, u);
	const float b = 2.f * Math::Dot(-dir, u);
	const float c = Math::Dot(dir, dir) - r * r;

	std::vector<float> roots = Math::Quadratic(a, b, c);

	if (roots.empty()) return false;

	data->time = Math::Min(roots[0], roots[1]);

	const vec3f newP1 = p1 + v1 * data->time;
	const vec3f newP2 = p2 + v2 * data->time;

	data->normal = Math::Normalized(newP1 - newP2);
	data->position = newP1 + data->normal * t1->scale.x;
	
	return true;
}
