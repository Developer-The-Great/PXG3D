#pragma once
#include "PSGIIToAABB.h"


namespace PXG
{
	struct OctreeNode
	{
		std::shared_ptr< AABBBox> Box;
		std::vector<PSGIIToAABB> IteratorInfoToAABBCollection;

		std::vector<std::shared_ptr<OctreeNode>> Split()
		{
			std::vector<std::shared_ptr<OctreeNode>> result;
			std::vector<std::shared_ptr<AABBBox>> splitBoxes;

			Box->OctreeBoxSplit(splitBoxes);

			for (const auto& box : splitBoxes)
			{
				auto newOctreeNode = std::make_shared<OctreeNode>();
				newOctreeNode->Box = box;
				
				for (auto IteratorInfoToAABB : IteratorInfoToAABBCollection)
				{

					if (box->CheckCollisionWithAABB(IteratorInfoToAABB.AABB.get()))
					{
						newOctreeNode->IteratorInfoToAABBCollection.push_back(IteratorInfoToAABB);
					}
					
				}

				result.push_back(newOctreeNode);

			}



			return result;
		}

	};
}