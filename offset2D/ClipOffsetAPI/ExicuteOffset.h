#ifndef EXICUTEOFFSET_H
#define EXICUTEOFFSET_H
#include "ToolTrajectoryAlg/CommonFile/DataStructure.h"
namespace boolean_offset_api{
class ExicuteOffset
{
public:
    ExicuteOffset(){}

    void ReverseNodeDire(std::shared_ptr<ofts::OffsetNode>& node){
        if(node == nullptr){return;}
        for(auto& l : node->_loops){l.Reverse();l.SetCW(!l.IsCW());}
        for(auto& d : node->_nodes){ReverseNodeDire(d);}
    }
    void OffsetCloseRegion(std::vector<ofts::DefLoop>& loops,double oftD,bool no_zezo,
                           std::vector<std::shared_ptr<ofts::OffsetNode>>& nodes);
    void OffsetOpenLoops(const std::vector<ofts::DefLoop>& loops,double oftD,
                         std::vector<std::shared_ptr<ofts::OffsetNode>>& nodes);
    void OffsetInterface(const std::vector<ofts::DefLoop>& loops,
                         double oftD,bool no_zezo,
                         std::vector<std::shared_ptr<ofts::OffsetNode>>& close_ns,
                         std::vector<std::shared_ptr<ofts::OffsetNode>>& open_ns);



};
}
#endif // EXICUTEOFFSET_H
