#ifndef vtkm_xeus_convert_png_h
#define vtkm_xeus_convert_png_h

#include <vtkm/rendering/Canvas.h>
#include <lodepng.h>

template<typename TupleType>
void convertBuffer(std::vector<std::uint8_t> &PngBuffer,
              int width,
              int height,
              int xIndex,
              int yIndex,
              TupleType &tuple
             );
template<>
void convertBuffer(std::vector<std::uint8_t> &PngBuffer,
              int width,
              int height,
              int xIndex,
              int yIndex,
              vtkm::Int32 &tuple
             )
{
            std::size_t NewPos = (height - yIndex - 1) * (width * 4) + 4 * xIndex;
        PngBuffer[NewPos + 0] = (unsigned char)(tuple);
        PngBuffer[NewPos + 1] = (unsigned char)(tuple);
        PngBuffer[NewPos + 2] = (unsigned char)(tuple);
        PngBuffer[NewPos + 3] = 255;            

}

template<>
void convertBuffer(std::vector<std::uint8_t> &PngBuffer,
              int width,
              int height,
              int xIndex,
              int yIndex,
              vtkm::Vec<vtkm::Float32, 4> &tuple
             )
{
            std::size_t NewPos = (height - yIndex - 1) * (width * 4) + 4 * xIndex;
        PngBuffer[NewPos + 0] = (unsigned char)(tuple[0] * 255);
        PngBuffer[NewPos + 1] = (unsigned char)(tuple[1] * 255);
        PngBuffer[NewPos + 2] = (unsigned char)(tuple[2] * 255);
        PngBuffer[NewPos + 3] = 255;            

}

template<>
void convertBuffer(std::vector<std::uint8_t> &PngBuffer,
              int width,
              int height,
              int xIndex,
              int yIndex,
              vtkm::Vec<vtkm::Float32, 3> &tuple
             )
{
            std::size_t NewPos = (height - yIndex - 1) * (width * 4) + 4 * xIndex;
        PngBuffer[NewPos + 0] = (unsigned char)(tuple[0] * 255);
        PngBuffer[NewPos + 1] = (unsigned char)(tuple[1] * 255);
        PngBuffer[NewPos + 2] = (unsigned char)(tuple[2] * 255);
        PngBuffer[NewPos + 3] = 255;

}

template<typename ArrayType>
void convertPng(ArrayType &array,int width,int height, std::vector<std::uint8_t> &ImageBuffer)
{    
  std::vector<std::uint8_t> PngBuffer(width*height*4);

  for (vtkm::Id yIndex = 0; yIndex < height; yIndex++)
  {
    for (vtkm::Id xIndex = 0; xIndex < width; xIndex++)
    {
      auto tuple = array.GetPortalConstControl().Get(yIndex * width + xIndex);
      //of << (unsigned char)(tuple[0] * 255);
      //of << (unsigned char)(tuple[1] * 255);
      //of << (unsigned char)(tuple[2] * 255);
        
        convertBuffer(PngBuffer, width, height, xIndex, yIndex, tuple);
    }
  }
  lodepng::encode(ImageBuffer, PngBuffer, width, height);
}



template<typename CanvasType>
auto convertPng(CanvasType &canvas)
{
    int width = canvas.GetWidth();
    int height = canvas.GetHeight();

    auto colorArray = canvas.GetColorBuffer();
    auto depthArray = canvas.GetDepthBuffer();
    std::vector<std::uint8_t> ImageBuffer;
  convertPng(colorArray, width, height, ImageBuffer);
    return ImageBuffer;


}


#endif
