#include "DiscreteModel.h"
using namespace grm;
#pragma optimize("", off)

void DiscreteModel::ModelTriangulation(const TopoDS_Shape& inshape,TrianRes& result)
{
    if(inshape.IsNull()){
        std::cout<<"传入模型无效,无法对模型进行离散化.";
        return;
    }
    result._vertexs.clear();
    result._triangles.clear();

    std::unordered_map<oft::Point, int, VertexHash, VertexEqual> vertex_map;
    int global_vertex_index = 0;

    /// 遍历所有面
    TopExp_Explorer face_explorer;
    for (face_explorer.Init(inshape, TopAbs_FACE); face_explorer.More(); face_explorer.Next()) {
        TopoDS_Face face = TopoDS::Face(face_explorer.Current());
        TopLoc_Location location;
        /// 获取面的三角化数据
        Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, location);
        if (triangulation.IsNull())
            continue;
        /// 获取节点数量
        int nodeCount = triangulation->NbNodes();
        if (nodeCount <= 0)
            continue;
        std::vector<int> face_vertex_indices(nodeCount + 1);
        /// 转换顶点并建立映射
        for (int i = 1; i <= nodeCount; i++) {
            gp_Pnt vertex = triangulation->Node(i).Transformed(location.Transformation());
            oft::Point tp(vertex.X(),vertex.Y(),vertex.Z());
            /// 检查顶点是否已存在
            auto it = vertex_map.find(tp);
            if (it == vertex_map.end()) {
                /// 新顶点，添加到唯一顶点列表和映射中
                vertex_map[tp] = global_vertex_index;
                face_vertex_indices[i] = global_vertex_index;
                result._vertexs.push_back(tp);
                global_vertex_index++;
            }else {
                /// 使用现有顶点索引
                face_vertex_indices[i] = it->second;
            }
        }
        /// 处理三角形
        int triangleCount = triangulation->NbTriangles();
        for (int i = 1; i <= triangleCount; i++) {
            Poly_Triangle triangle = triangulation->Triangle(i);
            int n1, n2, n3;
            triangle.Get(n1, n2, n3);
            /// 确保正确的方向（避免反向三角形）
            if (face.Orientation() == TopAbs_REVERSED) {
                std::swap(n1, n2);
            }
            /// 检查索引是否有效
            if (n1 < 1 || n1 > nodeCount || n2 < 1 || n2 > nodeCount || n3 < 1 || n3 > nodeCount) {
                std::cerr << "Warning: Invalid triangle indices: " << n1 << ", " << n2 << ", " << n3
                          << " (node count: " << nodeCount << ")" << std::endl;
                continue;
            }
            result._triangles.push_back({face_vertex_indices[n1],
                                         face_vertex_indices[n2],
                                         face_vertex_indices[n3]});
        }

    }


}



