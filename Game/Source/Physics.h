#pragma once
#include "engineMath.h"

class CollisionComponent;
class Physics
{
public:
	Physics();
	~Physics();

	class AABB
	{
	public:
		AABB() {}
		AABB(const Vector3 min, const Vector3 max)
		{
			mMin = min;
			mMax = max;
		}
		Vector3 mMin;
		Vector3 mMax;
		void Scale(float howmuch)
		{
			float xlength = mMax.x - mMin.x;
			float xCenter = mMin.x + xlength / 2;
			xlength *= howmuch;
			mMax.x = xCenter + xlength / 2;
			mMin.x = xCenter - xlength / 2;
			float ylength = mMax.y - mMin.y;
			float yCenter = mMin.y + ylength / 2;
			ylength *= howmuch;
			mMax.y = yCenter + ylength / 2;
			mMin.y = yCenter - ylength / 2;
			float zlength = mMax.z - mMin.z;
			float zCenter = mMin.z + zlength / 2;
			zlength *= howmuch;
			mMax.z = zCenter + zlength / 2;
			mMin.z = zCenter - zlength / 2;
		}
		void Translate(Vector3 howmuch)
		{
			mMin += howmuch;
			mMax += howmuch;
		}
	};

	class Ray
	{
	public:
		Ray(Vector3 from, Vector3 to)
		{
			mFrom = from;
			mTo = to;
		}
		Vector3 mFrom;
		Vector3 mTo;
	};

	static bool Intersect(const AABB& a, const AABB& b, AABB* pOverlap = nullptr);
	static bool Intersect(const Ray& ray, const AABB& box, Vector3* pHitPoint = nullptr);
	static bool UnitTest();

	std::vector<CollisionComponent*> mObj;
	void AddObj(CollisionComponent* pObj);
	void RemoveObj(CollisionComponent* pObj);
	bool RayCast(const Ray& ray, Vector3* pHitPoint = nullptr);
};


struct TestAABB
{
	Physics::AABB a;
	Physics::AABB b;
	Physics::AABB overlap;
};

struct TestRay
{
	Physics::AABB box;
	Physics::Ray Ray;
	bool hit;
	Vector3 point;
};
