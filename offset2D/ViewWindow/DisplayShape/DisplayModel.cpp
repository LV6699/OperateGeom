#include "DisplayModel.h"
#include <iostream>
#if DebugModel
#pragma optimize("", off)
#endif
void DisplayModel::initialViewShape(STEPControl_Reader& reader)
{
    auto myOccView = _mainwind->get_OCCView();
    myOccView->m_viewShape = new AIS_Shape(reader.OneShape());
}
void DisplayModel::RotatePartModelAngle(string path,
                                        string asSavePath)
{/**
    if (access(path.c_str(), F_OK) != 0){
        cout<<"旋转模型文件不存在!\n";
        return;
    }*/
    Handle(AIS_Shape) transeShape;
    _mainwind->myOccView->RotationShape(/*m_vFilePath[0],*/transeShape);
    ///TopTools_HSequenceOfShape ss = transeShape->Shape();
    TopoDS_Shape shape = transeShape->Shape();
    /// 存放图形的容器
    Handle(TopTools_HSequenceOfShape) aHSequenceOfShape =
            new TopTools_HSequenceOfShape;
    aHSequenceOfShape->Clear();
    aHSequenceOfShape->Append(shape);
    IFSelect_ReturnStatus status;
    /// 保存文件
    STEPControl_StepModelType aValue = STEPControl_AsIs;
    STEPControl_Writer aWriter;
    for (int i = 1; i <= aHSequenceOfShape->Length(); i++){
        ///转换三维模型到aWriter
        status = aWriter.Transfer(aHSequenceOfShape->Value(i), aValue);
        if (status != IFSelect_RetDone)
            std::cout << "模型转化失败" << std::endl;
    }
    ///保存数据到磁盘
    Standard_CString ccString = asSavePath.c_str();
    status = aWriter.Write(ccString);///重新保存模型
    _mainwind->myOccView->myRemoveall();
    ReDisplayReadeModelFile(asSavePath);
    //cout<<"模型旋转成功\n";
}
void DisplayModel::DisplayReadeModelFile(QString fileName)
{
    auto myOccView = _mainwind->get_OCCView();
    myOccView->myRemoveall();
    static int FileIndex = 0;
    ///m_vFilePath.push_back(fileName.toStdString());
    QString var = fileName;
    if (var.right(3).toUpper() == "IGS" || var.right(4).toUpper() == "IGES")
    {
        qDebug()<<fileName<<endl;
        IGESControl_Reader reader;
        ///reader.Shape();
        auto status = reader.ReadFile(var.toStdString().c_str());
        if (status == IFSelect_RetDone)
        {
            reader.TransferRoots();
            ///initialViewShape(reader);
            Handle(AIS_Shape) anshape = new AIS_Shape(reader.OneShape());
            anshape->SetColor(Quantity_NOC_STEELBLUE);
            anshape->SetHilightMode(1);
            Graphic3d_MaterialAspect smaterial;
            smaterial.SetMaterialType(Graphic3d_MATERIAL_PHYSIC);
            smaterial.SetShininess(0.5);
            myOccView->getContext()->SetMaterial(anshape, smaterial, Standard_False);
            myOccView->getContext()->Display(anshape, Standard_True);
            Handle(Prs3d_Drawer)t_hilight_style = myOccView->getContext()->HighlightStyle(Prs3d_TypeOfHighlight_LocalSelected);
            t_hilight_style->SetMethod(Aspect_TOHM_COLOR);
            t_hilight_style->SetColor(Quantity_NOC_GREEN);
            t_hilight_style->SetDisplayMode(1);
            myOccView->getContext()->SetHighlightStyle(t_hilight_style);
            myOccView->getContext()->Activate(anshape, 4);
            //myOccView->m_viewShape->SetTransparency(0.2);
            myOccView->fitAll();
        }
    }
    else{
        QFileInfo fileInfo = QFileInfo(var);
        QString fileName = fileInfo.fileName();
        QString filePath = fileInfo.absolutePath();
        QString fileSuffix = fileInfo.suffix();
        if(fileSuffix=="step" || fileSuffix=="STEP"
                || fileSuffix=="stp"  || fileSuffix=="STP"){
            STEPControl_Reader reader;
            auto status = reader.ReadFile(var.toStdString().c_str());
            if (status == IFSelect_RetDone){
                reader.TransferRoots();
                initialViewShape(reader);
                ///Handle(AIS_Shape) anshape = new AIS_Shape(reader.OneShape());
                myOccView->m_viewShape->SetColor(Quantity_NOC_STEELBLUE);
                myOccView->m_viewShape->SetHilightMode(1);
                Graphic3d_MaterialAspect smaterial;
                smaterial.SetMaterialType(Graphic3d_MATERIAL_PHYSIC);
                smaterial.SetShininess(0.5);
                myOccView->getContext()->SetMaterial(myOccView->m_viewShape, smaterial, Standard_False);
                myOccView->getContext()->Display(myOccView->m_viewShape, Standard_True);
                Handle(Prs3d_Drawer)t_hilight_style = myOccView->getContext()->HighlightStyle(Prs3d_TypeOfHighlight_LocalSelected);
                t_hilight_style->SetMethod(Aspect_TOHM_COLOR);
                t_hilight_style->SetColor(Quantity_NOC_GREEN);
                t_hilight_style->SetDisplayMode(1);
                myOccView->getContext()->SetHighlightStyle(t_hilight_style);
                myOccView->getContext()->Activate(myOccView->m_viewShape, 4);
                //myOccView->m_viewShape->SetTransparency(0.2);
                myOccView->fitAll();
                /////旋转实体
            }
            FileIndex ++;
            qDebug()<<fileName;
            m_asSavePath = filePath.toStdString() + "/" + fileName.toStdString();
        }
        if(fileSuffix=="stl" || fileSuffix=="STL" ||
                fileSuffix=="stls" || fileSuffix=="STLS"){
            StlAPI_Reader aReader_Stl;
            TopoDS_Shape shape_Stl;
            aReader_Stl.Read(shape_Stl, var.toStdString().c_str());
            Handle(AIS_Shape) shape_ = new AIS_Shape(shape_Stl);
            shape_->SetColor(Quantity_NOC_CHOCOLATE1);
            myOccView->getContext()->Display(shape_, Standard_True);
            FileIndex ++;
            qDebug()<<fileName;
        }
    }
}
void DisplayModel::ReDisplayReadeModelFile(string path)
{
    auto myOccView = _mainwind->get_OCCView();
    myOccView->myRemoveall();
    ///emit sendIsExistModelFile(NOTEXIST_MODELFILR);
    static int FileIndex = 0;
    ///QString var = fileNames[i];
    QString var = QString::fromStdString(path);
    if (var.right(3).toUpper() == "IGS" ||
            var.right(4).toUpper() == "IGES"){
        qDebug()<<var<<endl;
        IGESControl_Reader reader;
        ///reader.Shape();
        auto status = reader.ReadFile(var.toStdString().c_str());
        if (status == IFSelect_RetDone){
            reader.TransferRoots();
            ///initialViewShape(reader);
            Handle(AIS_Shape) anshape = new AIS_Shape(reader.OneShape());
            anshape->SetColor(Quantity_NOC_STEELBLUE);
            anshape->SetHilightMode(1);
            Graphic3d_MaterialAspect smaterial;
            smaterial.SetMaterialType(Graphic3d_MATERIAL_PHYSIC);
            smaterial.SetShininess(0.5);
            myOccView->getContext()->SetMaterial(anshape, smaterial, Standard_False);
            myOccView->getContext()->Display(anshape, Standard_True);
            Handle(Prs3d_Drawer)t_hilight_style = myOccView->getContext()->HighlightStyle(Prs3d_TypeOfHighlight_LocalSelected);
            t_hilight_style->SetMethod(Aspect_TOHM_COLOR);
            t_hilight_style->SetColor(Quantity_NOC_GREEN);
            t_hilight_style->SetDisplayMode(1);
            myOccView->getContext()->SetHighlightStyle(t_hilight_style);
            myOccView->getContext()->Activate(anshape, 4);
            //myOccView->m_viewShape->SetTransparency(0.7);
            myOccView->fitAll();
        }
    }
    else{
        QFileInfo fileInfo = QFileInfo(var);
        QString fileName = fileInfo.fileName();
        QString filePath = fileInfo.absolutePath();
        QString fileSuffix = fileInfo.suffix();
        if(fileSuffix=="step" || fileSuffix=="stp" ||
                fileSuffix=="STEP" || fileSuffix=="STP"){
            STEPControl_Reader reader;
            auto status = reader.ReadFile(var.toStdString().c_str());
            if (status == IFSelect_RetDone){
                reader.TransferRoots();
                initialViewShape(reader);
                //myOccView->m_viewShape->SetTransparency(0.7);
                ///Handle(AIS_Shape) anshape = new AIS_Shape(reader.OneShape());
                myOccView->m_viewShape->SetColor(Quantity_NOC_STEELBLUE);
                myOccView->m_viewShape->SetHilightMode(1);
                Graphic3d_MaterialAspect smaterial;
                smaterial.SetMaterialType(Graphic3d_MATERIAL_PHYSIC);
                smaterial.SetShininess(0.5);
                myOccView->getContext()->SetMaterial(myOccView->m_viewShape, smaterial, Standard_False);
                myOccView->getContext()->Display(myOccView->m_viewShape, Standard_True);
                Handle(Prs3d_Drawer)t_hilight_style = myOccView->getContext()->HighlightStyle(Prs3d_TypeOfHighlight_LocalSelected);
                t_hilight_style->SetMethod(Aspect_TOHM_COLOR);
                t_hilight_style->SetColor(Quantity_NOC_GREEN);
                t_hilight_style->SetDisplayMode(1);
                myOccView->getContext()->SetHighlightStyle(t_hilight_style);
                myOccView->getContext()->Activate(myOccView->m_viewShape, 4);
                myOccView->fitAll();
                /////旋转实体
            }
            FileIndex ++;
            qDebug()<<"num:"<<FileIndex<<" Open the stp file>>>>"
                   <<"Path:"<<filePath<<","<<"FileName:"<<fileName;
            m_asSavePath = filePath.toStdString() + "/" + fileName.toStdString();
        }
        if(fileSuffix=="stl" || fileSuffix=="STL"){
            StlAPI_Reader aReader_Stl;
            TopoDS_Shape shape_Stl;
            aReader_Stl.Read(shape_Stl, var.toStdString().c_str());
            Handle(AIS_Shape) shape_ = new AIS_Shape(shape_Stl);
            shape_->SetColor(Quantity_NOC_CHOCOLATE1);
            myOccView->getContext()->Display(shape_, Standard_True);
            FileIndex ++;
            qDebug()<<"num:"<<FileIndex<<" Open the stl file>>>>"
                   <<"Path:"<<filePath<<","<<"FileName:"<<fileName;
        }
    }
    ///m_isNewOpenFile = true;
    ///emit sendIsExistModelFile(EXIST_MODELFILR);
}