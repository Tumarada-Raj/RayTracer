#include <rt/groups/bvh.h>
#define SAHBVH

//#include <vector>
#include <tuple>

namespace rt {

BVH::BVH()
{
    this->root = new BVHNode();
}

BVH::~BVH()
{
}

void BVH::rebuildIndex() {
    root->primitives = primitives;
    buildTree(root);
}

BBox BVH::getBounds() const {
    return this->root->boundingBox;
}

void BVH::buildTree(BVHNode *node)
{
    Primitive* primitive;
    int primitivesCount = node->primitives.size();
    if (primitivesCount <= 0)
        return;           // if no primitives return nothing
    node->boundingBox = BBox::empty();
    for (int i = 0; i < primitivesCount; i++)
        node->extend(node->primitives[i]->getBounds());   //Get bounds of all the primitives in the scene
    
    if (primitivesCount < 3)
    {
        node->isLeaf = true;
        return;                                          // If primitives count<3 no need to split i.e., BVH not required
    }


    if (primitivesCount >= 3)                           // If count > 3 BVH required
    {
        node->isLeaf = false;
        node->lChild = new BVHNode();
        node->rChild = new BVHNode();
        Vector diagonal = node->boundingBox.diagonal();             
        int longestSplitAxis = (diagonal.x > diagonal.y && diagonal.x > diagonal.z) ? 0 : (diagonal.y > diagonal.z) ? 1 : 2;  //

        //Point p = node->boundingBox.getBBoxCentroid().getCoordinate(longestSplitAxis);
        //axisSplitAlongLen = node->boundingBox.getBBoxCentroid().getCoordinate(longestSplitAxis);
        float axisSplitAlongLen = getCenterPoint(longestSplitAxis, node->boundingBox);

        if (this->isSAH)
        {
            float lCost = 0, rCost = 0, bestCost = FLT_MAX, cost = 0, kLen;
            for (int k = 1; k < 40; k++)
            {
                kLen = node->boundingBox.min.getCoordinate(longestSplitAxis) + k * (node->boundingBox.max.getCoordinate(longestSplitAxis) - node->boundingBox.min.getCoordinate(longestSplitAxis)) / 40;
                int countL = 0, countR = 0;
                BBox areaL = BBox::empty(), areaR = BBox::empty();
                for (int i = 0; i < primitivesCount; i++)
                {
                    /*/Solid* solid = dynamic_cast<Solid*>(j);
                    if (j->getBounds().getBBoxCentroid().getCoordinate(longestSplitAxis) < kLen) //assuming x is the longest dimension
                        lCost = lCost + solid->getArea();
                    else
                        rCost = rCost + solid->getArea();
                    cost = lCost * (kLen - node->boundingBox.min.getCoordinate(longestSplitAxis)) + rCost * (node->boundingBox.max.getCoordinate(longestSplitAxis) - kLen);*/
                    float primitiveSplitCenterPoint = getCenterPoint(longestSplitAxis, node->primitives[i]->getBounds());
                    if (primitiveSplitCenterPoint <= kLen) {
                        countL++;
                        areaL.extend(node->primitives[i]->getBounds());
                    }
                    else {
                        countR++;
                        areaR.extend(node->primitives[i]->getBounds());
                    }
                }
                if (countL > 0)
                    lCost = countL * areaL.area() / node->boundingBox.area();
                if (countR > 0)
                    rCost = countR * areaR.area() / node->boundingBox.area();
                cost = lCost + rCost;
                if (cost <= bestCost) {
                    bestCost = cost;
                    axisSplitAlongLen = kLen;
                }
            }
        }
        //if centroid of primitive's BBox smaller than falls on the left of world BBox's centroid, add to left subtree, else add to right
        for (int i = 0; i < primitivesCount; i++)
        {
            float primitiveSplitMidPoint = getCenterPoint(longestSplitAxis, node->primitives[i]->getBounds());
            if (primitiveSplitMidPoint <= axisSplitAlongLen)
                node->lChild->primitives.push_back(node->primitives[i]);
            else
                node->rChild->primitives.push_back(node->primitives[i]);
        }
        node->primitives.clear(); //clear parents primitives for memory reasons
        // If split causes all children to fall on one side of the tree, tree contruction algo will not terminate 
        //--> segmentation fault
        //Solution 1 - Move one node from the populated side to the empty one  ----- Discussed in one of the tutorial
        if (node->lChild->primitives.size() == 0)
        {

            node->lChild->primitives.push_back(node->rChild->primitives[0]);
            node->rChild->primitives.erase(node->rChild->primitives.begin());
        }

        else if (node->rChild->primitives.size() == 0)
        {
            node->rChild->primitives.push_back(node->lChild->primitives[0]);
            node->lChild->primitives.erase(node->lChild->primitives.begin());
        }
        buildTree(node->lChild);
        buildTree(node->rChild);
        return;
    }
}
Intersection BVH::intersect(const Ray& ray, float previousBestDistance) const {
    BVHNode* node; 
    Intersection nearestIntersection = Intersection::failure();
    std::pair<float, float> d = root->boundingBox.intersect(ray);
    float t1 = d.first;
    float t2 = d.second;
    if (t1 > t2) {
        // nope
        return nearestIntersection;
    }
    Intersection currentIntersection;
    std::vector<BVHNode*> nodes;
    float entryLeft, exitLeft, entryRight, exitRight;
    nodes.push_back(this->root);

    while (!nodes.empty())
    {
        node = nodes.back();
        nodes.pop_back();

        if (node->isLeaf)
        {
            for (auto prim : node->primitives)
            {
                currentIntersection = prim->intersect(ray, previousBestDistance);
                if (currentIntersection)
                {
                    previousBestDistance = currentIntersection.distance;
                    nearestIntersection = currentIntersection;
                }
            }
        }
        else
        {
            BBox leftBbox = node->lChild->boundingBox;
            
            std::tie(entryLeft, exitLeft) = leftBbox.intersect(ray);
            if (entryLeft <= exitLeft)
                nodes.push_back(node->lChild);

            BBox rightBbox = node->rChild->boundingBox;
            std::tie(entryRight, exitRight) = rightBbox.intersect(ray);
            if (entryRight <= exitRight)
                nodes.push_back(node->rChild);
        }
    }
    return nearestIntersection;
}

float BVH::getCenterPoint(int axis, BBox bbox) {    //centroid bbox
    if (axis == 0)
        return (bbox.min.x + bbox.max.x) / 2;
    else if (axis == 1)
        return (bbox.min.y + bbox.max.y) / 2;
    else
        return (bbox.min.z + bbox.max.z) / 2;
}

void BVH::add(Primitive* p) {
    primitives.push_back(p);
}

void BVH::setMaterial(Material* m) {
    for (auto iter = primitives.begin(); iter != primitives.end(); ++iter) {
        (*iter)->setMaterial(m);
    }
}

void BVH::setCoordMapper(CoordMapper* cm) {
    for (auto iter = primitives.begin(); iter != primitives.end(); ++iter) {
        (*iter)->setCoordMapper(cm);
    }
}

void BVH::serialize(BVH::Output& output) {
    // To implement this function:
    // - Call output.setNodeCount() with the number of nodes in the BVH
    /* TODO */
    // - Set the root node index using output.setRootId()
    /* TODO */
    // - Write each and every one of the BVH nodes to the output using output.writeNode()
    /* TODO */ NOT_IMPLEMENTED;
}
void BVH::deserialize(BVH::Input& input) {
    // To implement this function:
    // - Allocate and initialize input.getNodeCount() nodes
    /* TODO */
    // - Fill your nodes with input.readNode(index)
    /* TODO */
    // - Use the node at index input.getRootId() as the root node
    /* TODO */ NOT_IMPLEMENTED;
}

}