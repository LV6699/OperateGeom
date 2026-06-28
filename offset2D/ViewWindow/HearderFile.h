#ifndef HEARDERFILE_H
#define HEARDERFILE_H
#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include<exception>
#if __linux__
#include <dirent.h>
#endif
#include <json.h>
#include "../offset2D/ToolTrajectoryAlg/CommonFile/Header.h"
//#include "BaseDataManager/Coordinate.h"
//#include "PartManager/StepPartInfo.h"
//#include "Operation/Rough/Rough_ModelAreaClearing.h>
//#include"../cnc-cam-core/build/include/BaseDataManager/Coordinate.h"
//#include"../cnc-cam-core/build/include/PartManager/StepPartInfo.h"
//#include"../cnc-cam-core/build/include/Operation/Rough/Rough_ModelAreaClearing.h"
#include "TestDataStruct.h"
#include "MainWindow/OccHeader.h"
#include "ManipulateWidget/ManipuWidget.h"
#include "ManipulateWidget/ModelFileCut.h"
#include "ManipulateWidget/ViewCurObject.h"
#include "ManipulateWidget/ViewObjSolve.h"
#include "ManipulateWidget/CurrentIndexType.h"
#include "ManipulateWidget/HandleFileObject.h"
#include "ManipulateWidget/HandleFileObject.h"
#include "ManipulateWidget/CreateToolPath.h"
#include "ManipulateWidget/TrajectoryCheck.h"
#include "ManipulateWidget/CreatePathSolve.h"
#include "BatchCutModel/BatchCutModel.h"
#include "BatchCutModel/BatchCutSolve.h"
#include "CutModelContour/AcquireEleLoop.h"
#include "CutModelContour/CutParts.h"
#include "CutModelContour/CutModelSolve.h"
#include "TextOrganize/DataEdit.h"
#include "TextOrganize/HandleText.h"
///#include "../OffSetAlgTest/OffsetAlg/TestData.h"
///#include "../OffSetAlgTest/OffsetAlg/CalWind.h"
#include "ParticularView/ViewData.h"
///#include "GetTestCdt.h"
#include "RegreTest.h"
#include "ToolPathJson.h"
#include "autotestdialog.h"
#include "debuginfodialog.h"
#include<vector>
#endif // HEARDERFILE_H