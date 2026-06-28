#ifndef CREATETOOL_H
#define CREATETOOL_H

#include "../../CommonFile/OperaParam.h"
#include "../Solution/CutLocateZ.h"

class CreateTool
{
public:
    CreateTool(){}
    
    TopoDS_Shape GetToolShape(const ofts::Point& p,const grm::DefTool& t);
    
    
};

#endif // CREATETOOL_H
