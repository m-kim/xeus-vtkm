#ifndef vtkm_xeus_render_h
#define vtkm_xeus_render_h
#include <xcpp/xdisplay.hpp>
#include <vtkm/cont/DataSet.h>
#include <vtkm/rendering/CanvasRayTracer.h>
#include <vtkm/rendering/MapperWireframer.h>
#include <vtkm/rendering/MapperRayTracer.h>
#include <vtkm/rendering/Scene.h>
#include <vtkm/rendering/View3D.h>
#include <VTKmXeusConvertPng.h>
#include <xeus/xguid.hpp>
#include "JSONPNGConvert.h"
#include "VTKmXeusConvertPng.h"

class VTKmXeusRender
{
public:
    VTKmXeusRender()
    {
        first = true;
        id = xeus::new_xguid();

    }

    void SetCamera(vtkm::rendering::Camera& camera,
                    const vtkm::Bounds& coordBounds,
                    const vtkm::cont::Field&)
{
        camera = vtkm::rendering::Camera();
        camera.ResetToBounds(coordBounds);
        camera.Azimuth(static_cast<vtkm::Float32>(45.0));
        camera.Elevation(static_cast<vtkm::Float32>(45.0));
    }

    template <typename ViewType>
    void Render(ViewType& view)
    {
    view.Initialize();
    view.Paint();
    }

    template<typename MapperType, 
            typename CanvasType,
            typename ViewType>
    void Render(vtkm::cont::DataSet &ds,
                MapperType &mapper,
                CanvasType &canvas,
                vtkm::cont::ColorTable &colorTable,
                std::unique_ptr<im::image> &displayImage)
    {
        vtkm::rendering::Scene scene;
        static std::string fieldNm = "pointvar";

        scene.AddActor(vtkm::rendering::Actor(
        ds.GetCellSet(), ds.GetCoordinateSystem(), ds.GetField(fieldNm), colorTable));
        vtkm::rendering::Camera camera;
        SetCamera(camera, ds.GetCoordinateSystem().GetBounds(), ds.GetField(fieldNm));
        vtkm::rendering::Color background(0.0f, 1.0f, 1.0f, 1.0f);
        vtkm::rendering::Color foreground(0.0f, 0.0f, 0.0f, 1.0f);
        ViewType view(scene, mapper, canvas, camera, background, foreground);

        // Print the title
    //     vtkm::rendering::TextAnnotationScreen* titleAnnotation =
    //     new vtkm::rendering::TextAnnotationScreen("Test Plot",
    //                                               vtkm::rendering::Color(1, 1, 1, 1),
    //                                               .075f,
    //                                               vtkm::Vec<vtkm::Float32, 2>(-.11f, .92f),
    //                                               0.f);
    //     view.AddAnnotation(titleAnnotation);

        //render over and over again to the same position on the page
        Render<ViewType>(view);
        auto png = convertPng(canvas);
        displayImage = std::make_unique<im::image>(im::image(png));
        xcpp::display(displayImage, id, !first);        

        first = false;
    }
    void Render(vtkm::cont::DataSet &ds,
                std::unique_ptr<im::image> &displayImage)
    {
        using M = vtkm::rendering::MapperRayTracer;
        using C = vtkm::rendering::CanvasRayTracer;
        using V3 = vtkm::rendering::View3D;

        vtkm::cont::ColorTable colorTable("inferno");

        M mapper;
        C canvas(512, 512);
        Render<M,C,V3>(ds, mapper, canvas, colorTable, displayImage);
    }

    template<typename ArrayType>
    void Render(ArrayType &array,int width,int height,
    std::unique_ptr<im::image> &displayImage)
    {
        std::vector<std::uint8_t> png;
        convertPng(array, width, height, png);
        displayImage = std::make_unique<im::image>(im::image(png));
        xcpp::display(*displayImage, id, !first);        

        first = false;        
    }

    bool first;
    xeus::xguid id;
};


#endif