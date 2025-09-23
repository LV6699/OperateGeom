#ifndef DISCRETEMODEL_H
#define DISCRETEMODEL_H

#include "../CommonFile/ModelStruct.h"
#pragma optimize("", off)

class DiscreteModel
{
public:
    DiscreteModel(){}

    void ModelTriangulation(const TopoDS_Shape& inshape,grm::TrianRes& result);


};

#endif // DISCRETEMODEL_H
