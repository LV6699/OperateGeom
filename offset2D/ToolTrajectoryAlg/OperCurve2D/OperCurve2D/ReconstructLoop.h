#ifndef RECONSTRUCTLOOP_H
#define RECONSTRUCTLOOP_H
#include"../../CommonFile/Header.h"
#include"../../CommonFile/NumStruct.h"
class ReconstructLoop
{
public:
    ReconstructLoop();
    bool IsSameCurve(const OffsetStruct::DefElem& c1,const OffsetStruct::DefElem& c2){
        if(c1.Type() != c2.Type())
            return false;
        if(c1.Type() == OffsetEnum::LINETYPE){
            return c1._segment._segSp.IsSamePoint2D(c2._segment._segSp,PreErr_10) &&
                    c1._segment._segEp.IsSamePoint2D(c2._segment._segEp,PreErr_10);
        }else if(c1.Type() == OffsetEnum::ARCTYPE){
            return  c1._arc._isCW == c2._arc._isCW &&
                    c1._arc._arcSp.IsSamePoint2D(c2._arc._arcSp,PreErr_10) &&
                    c1._arc._arcEp.IsSamePoint2D(c2._arc._arcEp,PreErr_10) &&
                    c1._arc._arcCp.IsSamePoint2D(c2._arc._arcCp,PreErr_10);
        }else if(c1.Type() == OffsetEnum::CIRCLETYPE){
            return  std::abs(c1._circle._cirR - c2._circle._cirR) <= PreErr_10 &&
                    c1._circle._cirCp.IsSamePoint2D(c2._circle._cirCp,PreErr_10);
        }
        return false;
    }
    bool IsCoincideLoop(const OffsetStruct::DefLoop &loop1,
                        const OffsetStruct::DefLoop &loop2);
    void OperateReconstructLoop(const NumStruct::ReLoopErr& err,
                                OffsetStruct::DefLoop& loop);
    bool IsCircleStructLoop(const ofts::DefLoop& loop,
                            numst::ReLoopErr param);
};
#endif // RECONSTRUCTLOOP_H
