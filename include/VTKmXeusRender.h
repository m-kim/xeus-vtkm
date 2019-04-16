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

	static 
    void SetCamera(vtkm::rendering::Camera& camera,
                    const vtkm::Bounds& coordBounds)
 	{
        camera = vtkm::rendering::Camera();
        camera.ResetToBounds(coordBounds);
        camera.Azimuth(static_cast<vtkm::Float32>(45.0));
        camera.Elevation(static_cast<vtkm::Float32>(45.0));

	}
	static
    void SetCamera(vtkm::rendering::Camera& camera,
                    const vtkm::Bounds& coordBounds,
                    const vtkm::cont::Field&) 
	{
		VTKmXeusRender::SetCamera(camera, coordBounds);
    }

    template <typename ViewType>
	static
    void Render(ViewType& view) 
    {
	    view.Initialize();
    	view.Paint();
    }

    template<typename MapperType, 
            typename CanvasType>
    void Render(vtkm::cont::DataSet &ds,
                MapperType &mapper,
                CanvasType &canvas,
                vtkm::cont::ColorTable &colorTable,
                std::string fieldNm)
    {
        using V3 = vtkm::rendering::View3D;

        vtkm::rendering::Scene scene;

        scene.AddActor(vtkm::rendering::Actor(
        ds.GetCellSet(), ds.GetCoordinateSystem(), ds.GetField(fieldNm), colorTable));
        vtkm::rendering::Camera camera;
        SetCamera(camera, ds.GetCoordinateSystem().GetBounds(), ds.GetField(fieldNm));
        vtkm::rendering::Color background(0.0f, 1.0f, 1.0f, 1.0f);
        vtkm::rendering::Color foreground(0.0f, 0.0f, 0.0f, 1.0f);
        V3 view(scene, mapper, canvas, camera, background, foreground);

        // Print the title
    //     vtkm::rendering::TextAnnotationScreen* titleAnnotation =
    //     new vtkm::rendering::TextAnnotationScreen("Test Plot",
    //                                               vtkm::rendering::Color(1, 1, 1, 1),
    //                                               .075f,
    //                                               vtkm::Vec<vtkm::Float32, 2>(-.11f, .92f),
    //                                               0.f);
    //     view.AddAnnotation(titleAnnotation);

        //render over and over again to the same position on the page
        Render<V3>(view);
    }
    template<typename MapperType, 
            typename CanvasType>
    void Display(vtkm::cont::DataSet &ds,
                MapperType &mapper,
                CanvasType &canvas,
                vtkm::cont::ColorTable &colorTable,
                std::string fieldNm)
    {
        Render(ds, mapper, canvas, colorTable, fieldNm);
        auto png = convertPng(canvas);
        displayImage = std::make_unique<im::image>(im::image(png));
        xcpp::display(*displayImage, id, !first);        
        first = false;
    }
    void Display(vtkm::cont::DataSet &ds,
                std::string fieldNm)
    {
        using M = vtkm::rendering::MapperRayTracer;
        using C = vtkm::rendering::CanvasRayTracer;

        vtkm::cont::ColorTable colorTable("inferno");

        M mapper;
        C canvas(512, 512);
        Display<M,C>(ds, mapper, canvas, colorTable, fieldNm);
    }

    template<typename ArrayType>
    void Display(ArrayType &array,int width,int height)
    {
        std::vector<std::uint8_t> png;
        convertPng(array, width, height, png);
        displayImage = std::make_unique<im::image>(im::image(png));
        xcpp::display(*displayImage, id, !first);        

        first = false;        
    }

	template<typename CanvasType> 
    void Display(CanvasType &canvas)
    {
	    auto png = convertPng(canvas);

    	displayImage = std::make_unique<im::image>(png);
	    xcpp::display(*displayImage, id, !first);
		first = false;

    }

    std::unique_ptr<im::image> displayImage;
    bool first;
    xeus::xguid id;
};


#endif
