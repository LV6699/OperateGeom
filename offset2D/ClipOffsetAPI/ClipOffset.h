#ifndef BOOLOFTAPI_H
#define BOOLOFTAPI_H
#include <functional>
#include "ToolTrajectoryAlg/CommonFile/DataStructure.h"
namespace boolean_offset_api
{
// 接口参考Clipper2，结合实际使用到的功能进行定义
// 裁剪样式，Intersection，Union，Differents 是必须实现项
enum class ClipType {NoClip,Intersection,Union,Difference,Xor};
// 对象类型，裁剪操作A 减去B,则A 是Subject,B 是Clip
enum class PathType {Subject,Clip};
// 区域判断规则，奇偶规则EvenOdd 和整数规则Positive 必须实现
// 一般情况下，默认外轮廓是逆时针旋转，内轮廓是顺时针旋转
// EvenOdd 这个规则非常重要，忽略输入的顺逆时针，按照外内外内的规则在几何上重新调整朝向，必须实现
enum class FillRule { EvenOdd, NonZero, Positive, Negative };
// 基本的数据结构可以沿用旧的OffsetStruct::DefLoop，
// 但是其中的OffsetStruct::DefElem 必须要增加一个用于自定义数据的int32_t z
using Path = ofts::DefLoop;
using Paths = std::vector<Path>;
using PolyTree = ofts::LoopTree;
// 多边形树，遵循外轮廓包含内轮廓，内轮廓再包含外轮廓的树结构
// ZCallback 是用来满足自定义数据设置的，是个非常重要的功能，一定要实现
// 其概念是，当裁剪算法的处理到某一点输入的对象in1 和in2 相交产生了新的输出out1 和out2，
// 这时候就需要将这些对象传递到这个ZCallback 的回调函数中，使用者自行处理判断如何设置新值。
// 这个功能是用来实现各种各样的工艺上的功能的，如果没有，那每个相关的工艺的需求就会提新的需求单去实现，
// 且实现方需要保障这些需求互相之间不能有影响
typedef std::function<void(const ofts::DefElem& in1, const ofts::DefElem& in2,
                           ofts::DefElem& out1, ofts::DefElem& out2)> ZCallback;
// ZCallback callback1 = Fun1;
// ZCallback callback2 = Fun2;
// ZCallback callback3 = Fun3();
// callback1(in1,in2,out1,out2);
// callback2(in1,in2,out1,out2);
// callback3(in1,in2,out1,out2);
// 特别注意裁剪操作中，Subject 对象是可以包含开放对象的，而且Subject 对象哪怕是闭合的，
// 也可以被视为开放对象来裁剪，将闭合对象视为开放对象来裁剪这个运算非常重要
class Clipper
{
public:
    // 添加数据
    void AddPaths(const Path& path, PathType polytype, bool is_open);
    // 添加数据
    void AddPaths(const Paths& paths, PathType polytype, bool is_open);
    // ZCallback 默认为空，也可以设置
    void SetZCallback(ZCallback cb);
    // 执行裁剪计算，结果以数组形式输出
    // 输出的数据，任何情况下，计算输出的外轮廓都是是逆时针旋转，内轮廓都是是顺时针旋转
    // 若被裁剪的对象是开放线段或视为开放线段，输出的线段中的ofts::DefElem 请保持他们原来的相对顺序
    bool Execute(ClipType clip_type, FillRule fill_rule,
                 Paths& closed_paths, Paths& open_paths);
    // 执行裁剪计算，结果以树形式输出
    // 输出的数据，任何情况下，计算输出的外轮廓都是是逆时针旋转，内轮廓都是是顺时针旋转
    bool Execute(ClipType clip_type, FillRule fill_rule,
                 PolyTree& closed_paths, Paths& open_paths);
    Paths subPaths;
    Paths clipPaths;
};
// 偏置节点的样式，优先实现Round 圆形偏置和Miter 尖角偏置以及Square 方形偏置
// enum class JoinType { Square, Bevel, Round, Miter };
// Polygon 是闭合轮廓偏置，Round 是用来开放轮廓偏置，优先实现，其余都是开放偏置的样式
// enum class EndType {Polygon, Joined, Butt, Square, Round};
// 对输入的对象进行一次偏置
// 注意偏置也是可以对开放轮廓生效的，不过这个对开放轮廓进行偏置的功能可以先不做
class Offseter{
public:
    explicit Offseter(double miter_limit = 2.0) : _miter_lim(miter_limit){}
    void AddPath(const Path& loop, Ofte::JoinType jt, Ofte::EndType et){
        _loops.emplace_back(loop);
        _loops.back().SetJoinType(jt);
        _loops.back().SetEndType(et);
    }
    void AddPaths(const Paths& loops, Ofte::JoinType jt, Ofte::EndType et){
        _loops.reserve(_loops.size() + loops.size());
        for(auto d : loops){d.SetJoinType(jt);d.SetEndType(et);_loops.emplace_back(d);}
    }
    // 执行裁剪计算，结果以数组形式输出，delta 是偏置的距离正数向右，负数向左
    // 输出的数据，任何情况下，计算输出的外轮廓都是是逆时针旋转，内轮廓都是是顺时针旋转
    void Initial(){
        _bloop.SetMiterLimit(_miter_lim);
        for(auto& d : _loops){d.SetMiterLimit(_miter_lim);}
    }
    void SetNoZezo(bool d){_no_zezo = d;}

    bool NoZezo(){return _no_zezo;}
    void SetMiterLimit(double d){for(auto& d : _loops){d.SetMiterLimit(_miter_lim);}}
    void Execute(double delta, Paths& soltions);
    void Execute(double delta, PolyTree& polytree);  //结果以树形式输出
    // DeltaCallback 是用来处理变距偏置的，暂时先不实现
    // void Execute(DeltaCallback delta_cb, Path& paths);

private:
    bool _no_zezo = true;  //封闭区域填充规则,支持非零(true)和正数(false)
    double _miter_lim;     //Ofte::JoinType
    ofts::DefLoop _bloop;
    std::vector<ofts::DefLoop>_loops;
};
PolyTree NodeToPolyTree(const std::shared_ptr<ofts::OffsetNode>& n);
std::vector<ofts::DefLoop> NodesToLoops(const std::vector<std::shared_ptr<ofts::OffsetNode>>& close_ns,
                                        const std::vector<std::shared_ptr<ofts::OffsetNode>>& open_ns);
}
namespace boa = boolean_offset_api;
#endif // BOOLOFTAPI_H
