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
	Events::EventsManager::GetInstance()->Subscribe("PARTITION", &PhysicsSystem::PartitionHandler, this);
}

void PhysicsSystem::Update(const float& t) { }

void PhysicsSystem::FixedUpdate(const float& t) {
	collisionCount = 0;

	UpdateVelocity(t);
	ApplyGravity(t);
	for (std::vector<Collider*>::iterator it = collider.begin(); it != collider.end(); ++it) {
		Collider *c1 = (Collider*)*it;
		if (c1->IsActive()) {
			for (std::vector<Collider*>::iterator it2 = it + 1; it2 != collider.end(); ++it2) {
				Collider *c2 = (Collider*)*it2;
				if (c2->IsActive()) {
					if (CollisionCheck(c1, c2)) {
						if (c1->GetParent()->GetTag() == "ball" && c1->data->time <= 0)
							CollisionResponse(c1, c2);
						else if (c2->GetParent()->GetTag() == "ball" && c2->data->time <= 0)
							CollisionResponse(c2, c1);
					}
				}
			}
		}
	}

	Events::EventsManager::GetInstance()->Trigger("COLLISION_COUNT", new Events::AnyType<int>(collisionCount));
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

void PhysicsSystem::PartitionHandler(Events::Event * event) {
	partition = static_cast<Events::AnyType<bool>*>(event)->data;
}

void PhysicsSystem::ApplyGravity(const float& t) {
	for (auto& r : rigidbody) {
		if (r->hasGravity) {
			r->force = r->mass * vec3f(0, -9.81, 0);
			r->velocity += r->force / r->mass * t;
		}
	}
}

void PhysicsSystem::UpdateVelocity(const float& t) {
	for (auto& r : rigidbody) {
		r->GetParent()->GetComponent<Transform>()->translation += r->velocity * t;
	}
}

bool PhysicsSystem::CollisionCheck(Collider* c1, Collider* c2) {
	bool check = c1->GetParent()->CompareQuad(c2->GetParent()->GetQuadList());

	if (!partition)
		check = true;

	if (check) {
		if (c1->GetParent()->GetTag() == "ball" && c2->GetParent()->GetTag() == "ball") {
			++collisionCount;
			return SphereToSphereCollision(c1, c2, c1->data);
		} 
	}

	if (c1->GetParent()->GetTag() == "ball" && c2->GetParent()->GetTag() == "wall") {
		++collisionCount;
		return SphereToWallCollision(c1, c2, c1->data);
	} else if (c2->GetParent()->GetTag() == "ball" && c1->GetParent()->GetTag() == "wall") {
		++collisionCount;
		return SphereToWallCollision(c2, c1, c2->data);
	}


	return false;
}

void PhysicsSystem::CollisionResponse(Collider* c1, Collider* c2) {

	if (c1->GetParent()->GetTag() == "ball" && c2->GetParent()->GetTag() == "ball") {		
		float m1 = c1->GetParent()->GetComponent<Rigidbody>()->mass;
		float m2 = c2->GetParent()->GetComponent<Rigidbody>()->mass;
		vec3f u1 = c1->GetParent()->GetComponent<Rigidbody>()->velocity;
		vec3f u2 = c2->GetParent()->GetComponent<Rigidbody>()->velocity;

		std::cout << "U1: " << u1 << '\n';
		std::cout << "U2: " << u2 << '\n';

		vec3f N = Math::Normalized(c1->GetParent()->GetComponent<Transform>()->translation - c2->GetParent()->GetComponent<Transform>()->translation);
		std::cout << "N: " << N << '\n';
		vec3f u1N = Math::Dot(u1, N) * N;
		vec3f u2N = Math::Dot(u2, N) * N;
						
		c1->GetParent()->GetComponent<Rigidbody>()->velocity = u1 + (2 * m2 / (m1 + m2)) * (u2N - u1N);
		c2->GetParent()->GetComponent<Rigidbody>()->velocity = u2 + (2 * m1 / (m1 + m2)) * (u1N - u2N);

		std::cout << "V1: " << u1 + (2 * m2 / (m1 + m2)) * (u2N - u1N) << '\n';
		std::cout << "V2: " << u2 + (2 * m1 / (m1 + m2)) * (u1N - u2N) << '\n';
	}
	else if (c1->GetParent()->GetTag() == "ball" && c2->GetParent()->GetTag() == "wall") {
		Rigidbody* r = c1->GetParent()->GetComponent<Rigidbody>();
		
		vec3f v = r->velocity - (2 * Math::Dot(r->velocity, c2->normal)) * c2->normal;
		r->velocity = (0.8f) * v;
	}
}

bool PhysicsSystem::SphereToSphereCollision(Collider* c1, Collider* c2, CollisionData* data) {
	Entity *e1 = c1->GetParent();
	Entity *e2 = c2->GetParent();

	Transform* t1 = e1->GetComponent<Transform>();
	Transform* t2 = e2->GetComponent<Transform>();

	Rigidbody* r1 = e1->GetComponent<Rigidbody>();
	Rigidbody* r2 = e2->GetComponent<Rigidbody>();

	const float r = t1->scale.x;

	const vec3f p1 = t1->translation;
	const vec3f p2 = t2->translation;

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

	if (roots.empty()) { return false; }

	data->time = Math::Min(roots[0], roots[1]);

	const vec3f newP1 = p1 + v1 * data->time;
	const vec3f newP2 = p2 + v2 * data->time;

	data->normal = Math::Normalized(newP1 - newP2);
	data->position = newP1 + data->normal * t1->scale.x;
	
	return true;
}

bool PhysicsSystem::SphereToWallCollision(Collider* c1, Collider* c2, CollisionData* data) {
	
	Rigidbody* r = c1->GetParent()->GetComponent<Rigidbody>();

	Transform* t1 = c1->GetParent()->GetComponent<Transform>();
	Transform* t2 = c2->GetParent()->GetComponent<Transform>();

	const vec3f p1 = t1->translation;
	const vec3f p2 = t2->translation;

	vec3f N = c2->normal;

	vec3f dir = p1 - p2;
	vec3f pos = t2->translation + t1->scale.x * N;

	if (Math::Dot(N, r->velocity) >= 0) { return false; }

	data->time = Math::Dot((N - t1->translation), N) / Math::Dot(N, r->velocity);

	if (c2->bounds->WithinBounds(p1))
		return true;
}