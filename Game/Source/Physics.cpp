#include "stdafx.h"
#include "Physics.h"
#include <limits>
#include <cstdlib>
#include "Components/CollisionComponent.h"

#define NUM_AABB_TESTS 13
#define NUM_RAY_BOX_TESTS 12

Physics::Physics()
{
}


Physics::~Physics()
{
}

bool Physics::Intersect(const AABB& a, const AABB& b, AABB* pOverlap)
{

	bool no = a.mMax.x < b.mMin.x ||
		a.mMax.y < b.mMin.y ||
		a.mMax.z < b.mMin.z ||
		b.mMax.x < a.mMin.x ||
		b.mMax.y < a.mMin.y ||
		b.mMax.z < a.mMin.z;

	if (no)
	{
		pOverlap->mMin = Vector3::One;
		pOverlap->mMax = Vector3::Zero;
		return false;
	}
	else
	{
		// x
		if (b.mMin.x <= a.mMin.x)
		{
			pOverlap->mMin.x = a.mMin.x;
		}
		else
		{
			pOverlap->mMin.x = b.mMin.x;
		}
		if (b.mMax.x >= a.mMax.x)
		{
			pOverlap->mMax.x = a.mMax.x;
		}
		else
		{
			pOverlap->mMax.x = b.mMax.x;
		}

		// y
		if (b.mMin.y <= a.mMin.y)
		{
			pOverlap->mMin.y = a.mMin.y;
		}
		else
		{
			pOverlap->mMin.y = b.mMin.y;
		}
		if (b.mMax.y >= a.mMax.y)
		{
			pOverlap->mMax.y = a.mMax.y;
		}
		else
		{
			pOverlap->mMax.y = b.mMax.y;
		}

		// z
		if (b.mMin.z <= a.mMin.z)
		{
			pOverlap->mMin.z = a.mMin.z;
		}
		else
		{
			pOverlap->mMin.z = b.mMin.z;
		}
		if (b.mMax.z >= a.mMax.z)
		{
			pOverlap->mMax.z = a.mMax.z;
		}
		else
		{
			pOverlap->mMax.z = b.mMax.z;
		}
		return true;
	}
}

bool Physics::Intersect(const Ray& ray, const AABB& box, Vector3* pHitPoint)
{
	*pHitPoint = Vector3(0.0f, 0.0f, 0.0f);
	float tmin = 0.0f;
	float tmax = (ray.mTo - ray.mFrom).Length();
	Vector3 origin = ray.mFrom;
	Vector3 direction = ray.mTo - ray.mFrom;
	direction.Normalize();

	// x
	if (abs(direction.x) < std::numeric_limits<float>::epsilon())
	{
		if (origin.x < box.mMin.x || origin.x > box.mMax.x)
		{
			return false;
		}
	}
	else
	{
		float ood = 1.0f / direction.x;
		float t1 = (box.mMin.x - origin.x) * ood;
		float t2 = (box.mMax.x - origin.x) * ood;
		if (t1 > t2)
		{
			float temp = t1;
			t1 = t2;
			t2 = temp;
		}
		tmin = max(tmin, t1);
		tmax = min(tmax, t2);
		if (tmin > tmax)
		{
			return false;
		}
	}

	// y
	if (abs(direction.y) < std::numeric_limits<float>::epsilon())
	{
		if (origin.y < box.mMin.y || origin.y > box.mMax.y)
		{
			return false;
		}
	}
	else
	{
		float ood = 1.0f / direction.y;
		float t1 = (box.mMin.y - origin.y) * ood;
		float t2 = (box.mMax.y - origin.y) * ood;
		if (t1 > t2)
		{
			float temp = t1;
			t1 = t2;
			t2 = temp;
		}
		tmin = max(tmin, t1);
		tmax = min(tmax, t2);
		if (tmin > tmax)
		{
			return false;
		}
	}

	// z
	if (abs(direction.z) < std::numeric_limits<float>::epsilon())
	{
		if (origin.z < box.mMin.z || origin.z > box.mMax.z)
		{
			return false;
		}
	}
	else
	{
		float ood = 1.0f / direction.z;
		float t1 = (box.mMin.z - origin.z) * ood;
		float t2 = (box.mMax.z - origin.z) * ood;
		if (t1 > t2)
		{
			float temp = t1;
			t1 = t2;
			t2 = temp;
		}
		tmin = max(tmin, t1);
		tmax = min(tmax, t2);
		if (tmin > tmax)
		{
			return false;
		}
	}

	*pHitPoint = origin + direction * tmin;
	return true;
}

// aabb tests
const static TestAABB testAABB[] =
{
	{
		Physics::AABB(Vector3(0.0f, 0.0f, 0.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::AABB(Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f)),
		Physics::AABB(Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f))
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::AABB(Vector3(-110.0f, -10.0f, -10.0f), Vector3(-90.0f, 10.0f, 10.0f)),
		Physics::AABB(Vector3(-100.0f, -10.0f, -10.0f), Vector3(-90.0f, 10.0f, 10.0f))
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::AABB(Vector3(90.0f, -10.0f, -10.0f), Vector3(110.0f, 10.0f, 10.0f)),
		Physics::AABB(Vector3(90.0f, -10.0f, -10.0f), Vector3(100.0f, 10.0f, 10.0f))
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::AABB(Vector3(-10.0f, -110.0f, -10.0f), Vector3(10.0f, -90.0f, 10.0f)),
		Physics::AABB(Vector3(-10.0f, -100.0f, -10.0f), Vector3(10.0f, -90.0f, 10.0f))
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::AABB(Vector3(-10.0f, 90.0f, -10.0f), Vector3(10.0f, 110.0f, 10.0f)),
		Physics::AABB(Vector3(-10.0f, 90.0f, -10.0f), Vector3(10.0f, 100.0f, 10.0f))
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::AABB(Vector3(-10.0f, -10.0f, -110.0f), Vector3(10.0f, 10.0f, -90.0f)),
		Physics::AABB(Vector3(-10.0f, -10.0f, -100.0f), Vector3(10.0f, 10.0f, -90.0f))
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::AABB(Vector3(-10.0f, -10.0f, 90.0f), Vector3(10.0f, 10.0f, 110.0f)),
		Physics::AABB(Vector3(-10.0f, -10.0f, 90.0f), Vector3(10.0f, 10.0f, 100.0f))
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::AABB(Vector3(-120.0f, -10.0f, -10.0f), Vector3(-110.0f, 10.0f, 10.0f)),
		Physics::AABB(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f))
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::AABB(Vector3(110.0f, -10.0f, -10.0f), Vector3(120.0f, 10.0f, 10.0f)),
		Physics::AABB(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f))
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::AABB(Vector3(-10.0f, -120.0f, -10.0f), Vector3(10.0f, -110.0f, 10.0f)),
		Physics::AABB(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f))
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::AABB(Vector3(-10.0f, 110.0f, -10.0f), Vector3(10.0f, 120.0f, 10.0f)),
		Physics::AABB(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f))
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::AABB(Vector3(-10.0f, -10.0f, -120.0f), Vector3(10.0f, -10.0f, -110.0f)),
		Physics::AABB(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f))
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::AABB(Vector3(-10.0f, -10.0f, 110.0f), Vector3(10.0f, 10.0f, 120.0f)),
		Physics::AABB(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f))
	}
};

// ray box tests
const TestRay testRay[] =
{
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::Ray(Vector3(-110.0f, 0.0f, 0.0f), Vector3(-90.0f, 0.0f, 0.0f)),
		true, 
		Vector3(-100.0f, 0.0f, 0.0f)
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::Ray(Vector3(0.0f, -110.0f, 0.0f), Vector3(0.0f, -90.0f, 0.0f)),
		true,
		Vector3(0.0f, -100.0f, 0.0f)
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::Ray(Vector3(0.0f, 0.0f, -110.0f), Vector3(0.0f, 0.0f, -90.0f)),
		true,
		Vector3(0.0f, 0.0f, -100.0f)
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::Ray(Vector3(110.0f, 0.0f, 0.0f), Vector3(90.0f, 0.0f, 0.0f)),
		true,
		Vector3(100.0f, 0.0f, 0.0f)
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::Ray(Vector3(0.0f, 110.0f, 0.0f), Vector3(0.0f, 90.0f, 0.0f)),
		true,
		Vector3(0.0f, 100.0f, 0.0f)
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::Ray(Vector3(0.0f, 0.0f, 110.0f), Vector3(0.0f, 0.0f, 90.0f)),
		true,
		Vector3(0.0f, 0.0f, 100.0f)
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::Ray(Vector3(-120.0f, 0.0f, 0.0f), Vector3(-110.0f, 0.0f, 0.0f)),
		false,
		Vector3(0.0f, 0.0f, 0.0f)
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::Ray(Vector3(0.0f, -120.0f, 0.0f), Vector3(0.0f, -110.0f, 0.0f)),
		false,
		Vector3(0.0f, 0.0f, 0.0f)
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::Ray(Vector3(0.0f, 0.0f, -120.0f), Vector3(0.0f, 0.0f, -110.0f)),
		false,
		Vector3(0.0f, 0.0f, 0.0f)
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::Ray(Vector3(120.0f, 0.0f, 0.0f), Vector3(110.0f, 0.0f, 0.0f)),
		false,
		Vector3(0.0f, 0.0f, 0.0f)
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::Ray(Vector3(0.0f, 120.0f, 0.0f), Vector3(0.0f, 110.0f, 0.0f)),
		false,
		Vector3(0.0f, 0.0f, 0.0f)
	},
	{
		Physics::AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
		Physics::Ray(Vector3(0.0f, 0.0f, 120.0f), Vector3(0.0f, 0.0f, 110.0f)),
		false,
		Vector3(0.0f, 0.0f, 0.0f)
	},
};


bool Physics::RayCast(const Physics::Ray& ray, Vector3* pHitPoint)
{
	std::vector<Vector3> points;
	for (CollisionComponent* component : mObj)
	{
		Physics::AABB aabb = component->GetAABB();
		Vector3 point;
		if (Intersect(ray, aabb, &point))
		{
			points.push_back(point);
		}
	}
	
	if (points.empty())
	{
		return false;
	}

	float bestdistance = FLT_MAX;
	int bestindex = -1;
	for (int i = 0; i < points.size(); i++)
	{
		Vector3 point = points[i];
		float distance = (ray.mFrom - point).Length();
		if (distance < bestdistance)
		{
			bestdistance = distance;
			bestindex = i;
		}
	}
	*pHitPoint = points[bestindex];
	return true;
}

void Physics::AddObj(CollisionComponent* pObj)
{
	mObj.push_back(pObj);
}

void Physics::RemoveObj(CollisionComponent* pObj)
{
	for (int i = 0; i < mObj.size(); i++)
	{
		if (mObj[i] == pObj)
		{
			mObj.erase(mObj.begin() + i);
			return;
		}
	}
}

bool Physics::UnitTest()
{
	for (int i = 0; i < NUM_AABB_TESTS; i++)
	{
		TestAABB test = testAABB[i];
		AABB result;
		Intersect(test.a, test.b, &result);
		if (!(test.overlap.mMax == result.mMax && test.overlap.mMin == result.mMin))
		{
			return false;
		}
	}

	for (int i = 0; i < NUM_RAY_BOX_TESTS; i++)
	{
		TestRay test = testRay[i];
		Vector3 result;
		bool hit = Intersect(test.Ray, test.box, &result);
		if (!(test.hit == hit && test.point == result))
		{
			return false;
		}
	}

	return true;
}