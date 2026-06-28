#include "ViewObjSolve.h"
#include"../DisplayShape/DisplayToolPath.h"
#include"../DisplayShape/DisplayModel.h"
#if DebugModel
#include<iostream>
#pragma optimize("", off)
#endif
void ViewObjSolve::UpdateUiStatus()
{
    auto myOccView = _mainwind->get_OCCView();
    myOccView->getContext()->UpdateCurrentViewer();
}