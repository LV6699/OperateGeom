FkfnJEIMiysb3_YumQwDFsQBI5FC
Page 1#include "LoopBoolInt.h"
#include "../GeomClipSolve.h"
#include "../../EquidisCurve/SelIntCalculate/LoopSelIntersect/LoopSelIntersect.h"
void LoopBoolInt::InputPreprocesse(vector<DefLoop>& inloops)
{
    LoopSelIntersect lsel;
    GeomClipSolve().HandleCoincideLoops(inloops);
    lsel.ArrayLoopClipPreprocesse(inloops);
}
void LoopBoolInt::LoopGeomBoolIntersection(vector<DefLoop> inloops
                                           vector<DefLoop>& rloops)
{
    InputPreprocesse(inloops);
    if(inloops.empty())
        return;
}