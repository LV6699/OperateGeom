//
// Created by jlc on 2024/7/2.
//
#include "slice.h"
/*
namespace slice {
int find_indices(std::vector<gp_Pnt> &arr, gp_Pnt &p1) {
    for (int i = 0; i < arr.size(); ++i) {
        if (arr[i].IsEqual(p1, 1e-4))
            return i;
    }
    return -1;
}
int exist_edge(std::vector<Edge> &edges,
               int &p1, int &p2, Edge &e) {
    if (p1 == -1 || p2 == -1)
        return -1;
    for (int i = 0; i < edges.size(); i++) {
        if (((edges[i].a == p1 && edges[i].b == p2) ||
             (edges[i].a == p2 && edges[i].b == p1)) &&
                e.midPnt.IsEqual(edges[i].midPnt, 1e-4))
            return i;
    }
    return -1;
}
void insert(int &e, int &index, gp_Pnt &p,
            std::vector<std::vector<int>> &indices,
            std::vector<gp_Pnt> &points,
            int edgeIndex) {
    if (index != -1) {
        e = index;
        indices[index].push_back(edgeIndex);
    } else {
        e = points.size();
        indices.push_back(std::vector<int>(1, edgeIndex));
        points.push_back(p);
    }
}
double getAngleOf2Vec(double v1[2], double v2[2]) {
    double dotProduct = v1[0] * v2[0] + v1[1] * v2[1];
    double magnitudeV1 = std::sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
    double magnitudeV2 = std::sqrt(v2[0] * v2[0] + v2[1] * v2[1]);
    double angle = std::acos(dotProduct / (magnitudeV1 * magnitudeV2));
    return angle * 180 / _PI_;
}
void convex(int start, int preEdge,
            std::vector<gp_Pnt> &points,
            std::vector<Edge> &edges,
            std::vector<bool> &visited,
            std::vector<std::vector<int>> &indices,
            std::vector<int> &out_edges) {
    if (preEdge != -1 && (visited[edges[preEdge].b] || start == edges[preEdge].b))
        return;
    int curPnt;
    double v1[2] = {1, 0};
    if (preEdge != -1) {
        gp_Pnt *p1 = &points[edges[preEdge].a], *p2 = &points[edges[preEdge].b];
        v1[0] = p2->X() - p1->X();
        v1[1] = p2->Y() - p1->Y();
        curPnt = edges[preEdge].b;
    } else
        curPnt = start;
    int nextEdge = preEdge, count = indices[curPnt].size();
    if (count < 2) return;
    double low_angle = 360;
    for (int i = 0; i < count; i++) {
        if (indices[curPnt][i] == preEdge) continue;
        if (edges[indices[curPnt][i]].a != curPnt) {
            std::swap(edges[indices[curPnt][i]].a, edges[indices[curPnt][i]].b);
            std::swap(edges[indices[curPnt][i]].startPnt, edges[indices[curPnt][i]].endPnt);
        }
        if (count == 2) {
            nextEdge = indices[curPnt][i];
            break;
        }
        gp_Pnt *p1 = &points[edges[indices[curPnt][i]].a], *p2 = &points[edges[indices[curPnt][i]].b];
        double v2[2] = {p2->X() - p1->X(), p2->Y() - p1->Y()};
        double angle = getAngleOf2Vec(v1, v2);
        if (angle < low_angle) {
            nextEdge = indices[curPnt][i];
            low_angle = angle;
        }
    }
    visited[edges[nextEdge].a] = true;
    out_edges.push_back(nextEdge);
    convex(start, nextEdge, points, edges, visited, indices, out_edges);
}
std::vector<std::vector<Edge>> GetCurrentLevelLoop(TopoDS_Shape shape, double level) {
    std::vector<std::vector<Edge>> out_loops;
    std::vector<gp_Pnt> points;
    std::vector<Edge> edges;
    std::vector<std::vector<int>> indices;
    TopExp_Explorer exploer(shape, TopAbs_EDGE);
    while (exploer.More()) {
        auto curve = BRepAdaptor_Curve(TopoDS::Edge(exploer.Current()));
        Edge edge;
        edge.type = curve.GetType();
        edge.edge = exploer.Current();
        curve.D0(curve.FirstParameter(), edge.startPnt);
        curve.D0(curve.LastParameter(), edge.endPnt);
        curve.D0((curve.LastParameter() + curve.FirstParameter()) / 2, edge.midPnt);
        if (edge.startPnt.IsEqual(edge.endPnt, 1e-4)) {
            out_loops.push_back(std::vector<Edge>{edge});
            exploer.Next();
            continue;
        }
        int ind1 = find_indices(points, edge.startPnt);
        int ind2 = find_indices(points, edge.endPnt);
        if (ind1 != -1 && ind1 == ind2) {
            exploer.Next();
            continue;
        }
        int edgeIndex = exist_edge(edges, ind1, ind2, edge);
        if (edgeIndex != -1) {
            exploer.Next();
            continue;
        }
        insert(edge.a, ind1, edge.startPnt, indices, points, edges.size());
        insert(edge.b, ind2, edge.endPnt, indices, points, edges.size());
        edges.push_back(edge);
        exploer.Next();
    }
    std::vector<bool> visited(indices.size(), false);
    for (int i = 0; i < indices.size(); i++) {
        if (visited[i] == true || indices[i].size() < 2) continue;
        std::vector<int> result;
        visited[i] = true;
        convex(i, -1, points, edges, visited, indices, result);
        int c = result.size();
        if (c > 1) {
            int a1 = edges[result[0]].a, b1 = edges[result[0]].b;
            int a2 = edges[result[c - 1]].a, b2 = edges[result[c - 1]].b;
            if (a1 == a2 || a1 == b2 || b1 == a2 || b1 == b2) {
                std::vector<Edge> loop;
                for (auto &k: result)
                    loop.push_back(edges[k]);
                out_loops.push_back(loop);
            }
        }
    }
    return out_loops;
}
}
*/