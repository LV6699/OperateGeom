#ifndef OCTSTRUCT_H
#define OCTSTRUCT_H

#include "CutterLocation/Common/ClStruct.h"
#include "BRepClass3d_SolidClassifier.hxx"

namespace octr {
class AABB {
public:
    AABB() : min(ofts::Point(0,0,0)),max(ofts::Point(0,0,0)){}
    AABB(const ofts::Point& min_,const ofts::Point& max_)
        : min(min_),max(max_){}

    ofts::Point Center() const {
        return ofts::Point((min.X() + max.X()) * 0.5,(min.Y() + max.Y()) * 0.5,
                           (min.Z() + max.Z()) * 0.5);
    }
    ofts::Point Size() const {
        return ofts::Point(max.X() - min.X(),max.Y() - min.Y(),max.Z() - min.Z());
    }
    double longestEdge() const {
        auto s = Size();
        return std::max({s.X(),s.Y(),s.Z()});
    }
    bool Contains(const ofts::Point& p) const {
        return (p.X() >= min.X() && p.X() <= max.X() && p.Y() >= min.Y() &&
                p.Y() <= max.Y() && p.Z() >= min.Z() && p.Z() <= max.Z());
    }

public:
    ofts::Point min,max;
};
enum class NodeType {
    UNKNOWN,   //需要进一步判断
    EMPTY,     //完全在模型外部
    FULL,      //完全在模型内部
    PARTIAL    //与模型边界相交（叶子节点）
};
class OctrNode {
public:
    OctrNode(const AABB& b,int d) : bounds(b),depth(d),type(NodeType::UNKNOWN){
        child.resize(8,nullptr);
    }
    bool IsLeaf() const {return child[0] == nullptr;}
    //创建8个子节点
    void Subdivide(){
        auto c = bounds.Center();
        auto s = bounds.Size();
        double hx = s.X() * 0.5;
        double hy = s.Y() * 0.5;
        double hz = s.Z() * 0.5;
        //8个子节点的包围盒（按照标准八叉树顺序）
        child[0] = std::make_shared<OctrNode>(
                    AABB(ofts::Point(c.X() - hx,c.Y() - hy,c.Z() - hz),
                         ofts::Point(c.X(),c.Y(),c.Z())),depth + 1);
        child[1] = std::make_shared<OctrNode>(
                    AABB(ofts::Point(c.X(),c.Y() - hy,c.Z() - hz),
                         ofts::Point(c.X() + hx,c.Y(),c.Z())),depth + 1);
        child[2] = std::make_shared<OctrNode>(
                    AABB(ofts::Point(c.X() - hx,c.Y(),c.Z() - hz),
                         ofts::Point(c.X(),c.Y() + hy,c.Z())),depth + 1);
        child[3] = std::make_shared<OctrNode>(
                    AABB(ofts::Point(c.X(),c.Y(),c.Z() - hz),
                         ofts::Point(c.X() + hx,c.Y() + hy,c.Z())),depth + 1);
        child[4] = std::make_shared<OctrNode>(
                    AABB(ofts::Point(c.X() - hx,c.Y() - hy,c.Z()),
                         ofts::Point(c.X(),c.Y(),c.Z() + hz)),depth + 1);
        child[5] = std::make_shared<OctrNode>(
                    AABB(ofts::Point(c.X(),c.Y() - hy,c.Z()),
                         ofts::Point(c.X() + hx,c.Y(),c.Z() + hz)),depth + 1);
        child[6] = std::make_shared<OctrNode>(
                    AABB(ofts::Point(c.X() - hx,c.Y(),c.Z()),
                         ofts::Point(c.X(),c.Y() + hy,c.Z() + hz)),depth + 1);
        child[7] = std::make_shared<OctrNode>(
                    AABB(ofts::Point(c.X(),c.Y(),c.Z()),
                         ofts::Point(c.X() + hx,c.Y() + hy,c.Z() + hz)),depth + 1);
    }
public:
    int depth;
    AABB bounds;
    NodeType type;
    std::vector<std::shared_ptr<OctrNode>> child;
    std::vector<grm::Triangle> triangles;  //叶子节点存储的三角形
};
class IntersectionTests {
public:
    //AABB与三角形相交检测（使用分离轴定理）
    static bool AABBTriangleIntersect(const AABB& box,const grm::Triangle& tri){
        //先做快速的AABB-AABB检测（用三角形的包围盒）
        double triMinX = std::min({tri._p0.X(),tri._p1.X(),tri._p2.X()});
        double triMinY = std::min({tri._p0.Y(),tri._p1.Y(),tri._p2.Y()});
        double triMinZ = std::min({tri._p0.Z(),tri._p1.Z(),tri._p2.Z()});
        double triMaxX = std::max({tri._p0.X(),tri._p1.X(),tri._p2.X()});
        double triMaxY = std::max({tri._p0.Y(),tri._p1.Y(),tri._p2.Y()});
        double triMaxZ = std::max({tri._p0.Z(),tri._p1.Z(),tri._p2.Z()});
        
        if (box.max.X() < triMinX || box.min.X() > triMaxX ||
                box.max.Y() < triMinY || box.min.Y() > triMaxY ||
                box.max.Z() < triMinZ || box.min.Z() > triMaxZ){
            return false;
        }
        //使用分离轴进行精确检测
        ofts::Point bcp = box.Center();
        ofts::Point bHalf = box.Size() * 0.5;
        
        //将三角形顶点转换到以包围盒中心为原点的坐标系
        ofts::Point v0(tri._p0.X() - bcp.X(),tri._p0.Y() - bcp.Y(),tri._p0.Z() - bcp.Z());
        ofts::Point v1(tri._p1.X() - bcp.X(),tri._p1.Y() - bcp.Y(),tri._p1.Z() - bcp.Z());
        ofts::Point v2(tri._p2.X() - bcp.X(),tri._p2.Y() - bcp.Y(),tri._p2.Z() - bcp.Z());
        
        //计算三角形的边
        ofts::Point e0(v1.X() - v0.X(),v1.Y() - v0.Y(),v1.Z() - v0.Z());
        ofts::Point e1(v2.X() - v1.X(),v2.Y() - v1.Y(),v2.Z() - v1.Z());
        ofts::Point e2(v0.X() - v2.X(),v0.Y() - v2.Y(),v0.Z() - v2.Z());
        
        //对每个分离轴进行测试
        //轴1-3: 包围盒的法向量
        if (!SeparatingAxis(ofts::Point(1,0,0),v0,v1,v2,bHalf)){return false;}
        if (!SeparatingAxis(ofts::Point(0,1,0),v0,v1,v2,bHalf)){return false;}
        if (!SeparatingAxis(ofts::Point(0,0,1),v0,v1,v2,bHalf)){return false;}
        
        //轴4: 三角形法向量
        //ofts::Point triNormal = crossProduct(e0,e1);
        if (!SeparatingAxis(/*triNormal*/tri.N(),v0,v1,v2,bHalf)){return false;}
        
        //轴5-13: 边的叉积
        if (!SeparatingAxis(ofts::Point(1,0,0).Cross(e0),v0,v1,v2,bHalf)){return false;}
        if (!SeparatingAxis(ofts::Point(1,0,0).Cross(e1),v0,v1,v2,bHalf)){return false;}
        if (!SeparatingAxis(ofts::Point(1,0,0).Cross(e2),v0,v1,v2,bHalf)){return false;}
        if (!SeparatingAxis(ofts::Point(0,1,0).Cross(e0),v0,v1,v2,bHalf)){return false;}
        if (!SeparatingAxis(ofts::Point(0,1,0).Cross(e1),v0,v1,v2,bHalf)){return false;}
        if (!SeparatingAxis(ofts::Point(0,1,0).Cross(e2),v0,v1,v2,bHalf)){return false;}
        if (!SeparatingAxis(ofts::Point(0,0,1).Cross(e0),v0,v1,v2,bHalf)){return false;}
        if (!SeparatingAxis(ofts::Point(0,0,1).Cross(e1),v0,v1,v2,bHalf)){return false;}
        if (!SeparatingAxis(ofts::Point(0,0,1).Cross(e2),v0,v1,v2,bHalf)){return false;}
        
        return true;
    }
    
private:
    static bool SeparatingAxis(const ofts::Point& axis,const ofts::Point& v0,
                               const ofts::Point& v1,const ofts::Point& v2,
                               const ofts::Point& bHalf){
        //如果轴长度接近0,跳过（分离轴退化）
        double len = std::sqrt(axis.X() * axis.X() + axis.Y() * axis.Y() + axis.Z() * axis.Z());
        if (len < 1e-10) {return true;}
        ofts::Point n(axis.X() / len,axis.Y() / len,axis.Z() / len);
        
        //投影三角形到轴上
        double p0 = v0.X() * n.X() + v0.Y() * n.Y() + v0.Z() * n.Z();
        double p1 = v1.X() * n.X() + v1.Y() * n.Y() + v1.Z() * n.Z();
        double p2 = v2.X() * n.X() + v2.Y() * n.Y() + v2.Z() * n.Z();
        
        double triMin = std::min({p0,p1,p2});
        double triMax = std::max({p0,p1,p2});
        //投影包围盒到轴上
        double boxProj = std::abs(bHalf.X() * n.X()) +
                std::abs(bHalf.Y() * n.Y()) + std::abs(bHalf.Z() * n.Z());
        //检查分离
        return (triMin <= boxProj && triMax >= -boxProj);
    }
};

class ModelProc {
    ModelProc(){}
public:
    AABB GetShapeBox() const {
        Bnd_Box bbox;
        BRepBndLib::Add(shape,bbox);
        double xmin,ymin,zmin,xmax,ymax,zmax;
        bbox.Get(xmin,ymin,zmin,xmax,ymax,zmax);
        //添加小的膨胀量避免边界问题
        double err = 1e-6;
        return AABB(ofts::Point(xmin - err,ymin - err,zmin - err),
                    ofts::Point(xmax + err,ymax + err,zmax + err));
    }
    //判断点是否在模型内部
    bool IsPointInside(const ofts::Point& p) const {
        BRepClass3d_SolidClassifier classifier(shape);
        gp_Pnt p1(p.X(),p.Y(),p.Z());
        classifier.Perform(p1,1e-6);
        TopAbs_State state = classifier.State();
        return (state == TopAbs_IN);
    }
public:
    TopoDS_Shape shape;
    std::vector<grm::Triangle> tris;

};
//==================== 八叉树构建器 ====================
class OctreeBuilder {
private:
    int maxDepth;
    double minEdgeLength;
    std::shared_ptr<OctrNode> root;
    ModelProc* mProc;
    
    
public:
    OctreeBuilder(int n,double e,ModelProc* processor)
        : maxDepth(n),minEdgeLength(e),mProc(processor){}
    std::shared_ptr<OctrNode> build(){
        //创建根节点
        AABB rootBounds = mProc->GetShapeBox();
        root = std::make_shared<OctrNode>(rootBounds,0);
        BuildRecursive(root,mProc->tris);
        return root;
    }
    //获取八叉树统计信息
    void GetStatistics(const std::shared_ptr<OctrNode>& node,
                       int& totalNodes,int& leafNodes,int& maxDepthReached){
        if (!node){return;}
        totalNodes++;
        maxDepthReached = std::max(maxDepthReached,node->depth);
        if (node->IsLeaf()){leafNodes++;return;}
        for (const auto& child : node->child){
            if (child){
                GetStatistics(child,totalNodes,leafNodes,maxDepthReached);
            }
        }
    }
    
private:
    void BuildRecursive(std::shared_ptr<OctrNode> node,
                        const std::vector<grm::Triangle>& triangles){
        //检查终止条件
        if (ShouldTerminate(node)){
            //判断叶子节点类型
            DetermineNodeType(node,triangles);
            return;
        }
        //分割节点
        node->Subdivide();
        //将三角形分配到子节点
        std::vector<std::vector<grm::Triangle>> childTriangles(8);
        for (const auto& tri : triangles){
            for (int i = 0; i < 8; i++){
                if (IntersectionTests::AABBTriangleIntersect(
                            node->child[i]->bounds,tri)){
                    childTriangles[i].push_back(tri);
                }
            }
        }
        //递归处理子节点
        for (int i = 0; i < 8; i++){
            if (!childTriangles[i].empty()){
                BuildRecursive(node->child[i],childTriangles[i]);continue;
            }
            //如果没有三角形,判断是EMPTY还是FULL
            ofts::Point cp = node->child[i]->bounds.Center();
            if (mProc->IsPointInside(cp)){
                node->child[i]->type = NodeType::FULL;
            } else {
                node->child[i]->type = NodeType::EMPTY;
            }
        }
    }
    
    bool ShouldTerminate(std::shared_ptr<OctrNode> node){
        //达到最大深度
        if (node->depth >= maxDepth) return true;
        //节点边长小于最小限制
        ofts::Point Size = node->bounds.Size();
        double minSize = std::min({Size.X(),Size.Y(),Size.Z()});
        if (minSize <= minEdgeLength) return true;
        
        return false;
    }
    void DetermineNodeType(std::shared_ptr<OctrNode> node,
                           const std::vector<grm::Triangle>& triangles){
        if (triangles.empty()){
            //没有三角形,判断中心点
            ofts::Point cp = node->bounds.Center();
            if (mProc->IsPointInside(cp)){
                node->type = NodeType::FULL;
            } else {
                node->type = NodeType::EMPTY;
            }
            return;
        }
        //有三角形,是边界节点
        node->type = NodeType::PARTIAL;
        node->triangles = triangles;
    }
};

//==================== 八叉树可视化/输出 ====================
class OctreeVisualizer {
public:
    static void PrintTree(const std::shared_ptr<OctrNode>& node,int level = 0){
        if (!node) {return;}
        std::string indent(level * 2,' ');
        std::string typeStr;
        
        switch (node->type){
        case NodeType::UNKNOWN: typeStr = "UNKNOWN"; break;
        case NodeType::EMPTY: typeStr = "EMPTY"; break;
        case NodeType::FULL: typeStr = "FULL"; break;
        case NodeType::PARTIAL: typeStr = "PARTIAL"; break;
        }
        
        AABB& b = node->bounds;
        std::cout << indent << "Depth: " << node->depth
                  << " Type: " << typeStr
                  << " Bounds: [" << b.min.X() << "," << b.min.Y() << "," << b.min.Z()
                  << "] - [" << b.max.X() << "," << b.max.Y() << "," << b.max.Z() << "]";
        
        if (node->IsLeaf() && node->type == NodeType::PARTIAL){
            std::cout << " (Triangles: " << node->triangles.size() << ")";
        }
        std::cout << std::endl;
        if (!node->IsLeaf()){
            for (const auto& child : node->child){
                PrintTree(child,level + 1);
            }
        }
    }
    //导出为可用于渲染的数据
    static void ExportNodeBoundaries(const std::shared_ptr<OctrNode>& node,
                                     std::vector<AABB>& leafBoxes){
        if (!node) return;
        if (node->IsLeaf() && (node->type == NodeType::PARTIAL ||
                               node->type == NodeType::FULL)){
            leafBoxes.push_back(node->bounds);
        } else if (!node->IsLeaf()){
            for (const auto& child : node->child){
                ExportNodeBoundaries(child,leafBoxes);
            }
        }
    }
};



}  //namespace oct

#endif  //OCTSTRUCT_H
