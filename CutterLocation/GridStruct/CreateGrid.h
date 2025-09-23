#ifndef CREATEGRID_H
#define CREATEGRID_H

#pragma optimize("", off)

#include "../../CommonFile/ModelStruct.h"
namespace grm{

class CreateGrid
{
public:
    CreateGrid(){}

    void CreateModelGrid(grm::ClModel& m,
                         double step,
                         std::vector<std::vector<oft::Point>>&_clmesh);






};

}




#endif // CREATEGRID_H
