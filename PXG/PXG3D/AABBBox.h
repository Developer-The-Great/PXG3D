#pragma once
#include "Vector3.h"
#include <memory>
#include <vector>
namespace PXG
{
	struct AABBBox
	{
		AABBBox(Vector3 position, Vector3 halfWidths)
		{
			this->position = position;
			this->halfWidths = halfWidths;
		}

		//the world position of the Box
		Vector3 position;

		//the length,height,and breadth divided by 2
		Vector3 halfWidths;

		bool CheckCollisionWithAABB(const AABBBox * otherBox)
		{
			float za = halfWidths.z;
			float zb = otherBox->halfWidths.z;
			float zd = Mathf::Abs(otherBox->position.z - position.z);

			if (za + zb > zd)
			{
				return true;
			}

			float xa = halfWidths.x;
			float xb = otherBox->halfWidths.x;
			float xd = Mathf::Abs(otherBox->position.x - position.x);

			if (xa + xb > xd)
			{
				return true;
			}

			float ya = halfWidths.y;
			float yb = otherBox->halfWidths.y;
			float yd = Mathf::Abs(otherBox->position.y - position.y);

			if (ya + yb > yd)
			{
				return true;
			}


			return false;

		}

		void OctreeBoxSplit(std::vector<std::shared_ptr<AABBBox>>& splitBoxes)
		{
			Vector3 splitHalfWidths = halfWidths * 0.5f;

			//create left up forward box
			Vector3 lufBoxPosition = position + Vector3(-splitHalfWidths.x, splitHalfWidths.y, splitHalfWidths.z);
			auto leftUpForwardBox = std::make_shared<AABBBox>(lufBoxPosition, splitHalfWidths);


			//create left down forward box
			Vector3 ldfBoxPosition = position + Vector3(-splitHalfWidths.x, -splitHalfWidths.y, splitHalfWidths.z);
			auto leftDownForwardBox = std::make_shared<AABBBox>(ldfBoxPosition, splitHalfWidths);

			//create left up back box
			Vector3 lubBoxPosition = position + Vector3(-splitHalfWidths.x, splitHalfWidths.y, -splitHalfWidths.z);
			auto leftUpBackBox = std::make_shared<AABBBox>(lubBoxPosition, splitHalfWidths);

			//create left down back box
			Vector3 ldbBoxPosition = position + Vector3(-splitHalfWidths.x, -splitHalfWidths.y, -splitHalfWidths.z);
			auto leftDownBackBox = std::make_shared<AABBBox>(ldbBoxPosition, splitHalfWidths);

			//create right up forward box
			Vector3 rufBoxPosition = position + Vector3(splitHalfWidths.x, splitHalfWidths.y, splitHalfWidths.z);
			auto rightUpForwardBox = std::make_shared<AABBBox>(rufBoxPosition, splitHalfWidths);

			//create right down forward box
			Vector3 rdfBoxPosition = position + Vector3(splitHalfWidths.x, -splitHalfWidths.y, splitHalfWidths.z);
			auto rightDownForwardBox = std::make_shared<AABBBox>(rdfBoxPosition, splitHalfWidths);

			//create right up backward box
			Vector3 rubBoxPosition = position + Vector3(splitHalfWidths.x, splitHalfWidths.y, -splitHalfWidths.z);
			auto rightUpBackBox = std::make_shared<AABBBox>(rubBoxPosition, splitHalfWidths);

			//create right down backward box
			Vector3 rdbUpPosition = position + Vector3(splitHalfWidths.x, -splitHalfWidths.y, -splitHalfWidths.z);
			auto rightDownBackBox = std::make_shared<AABBBox>(rdbUpPosition, splitHalfWidths);

			splitBoxes.push_back(leftUpForwardBox);
			splitBoxes.push_back(leftDownForwardBox);
			splitBoxes.push_back(leftUpBackBox);
			splitBoxes.push_back(leftDownBackBox);

			splitBoxes.push_back(rightUpForwardBox);
			splitBoxes.push_back(rightDownForwardBox);
			splitBoxes.push_back(rightUpBackBox);
			splitBoxes.push_back(rightDownBackBox);

		}



	};

}
