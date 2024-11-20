#ifndef URHO3D_PIXELIMAGE_H
#define URHO3D_PIXELIMAGE_H


#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/Graphics/IndexBuffer.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Resource/Image.h>
#include <SDL/SDL_log.h>

#pragma once

using namespace Urho3D;


class PixelImage : public Image
{
    URHO3D_OBJECT(PixelImage, Image);
public:
    PixelImage(Context *context);
    ~PixelImage();

    static void RegisterObject(Context *context);

    void SetSize(int width, int height);
    void SetPoint(int x, int y, const Color& color);
    void DrawLine(int x0, int y0, int x1, int y1, const Color &color);
    void DrawRectangle(int x, int y, int width, int height, const Color &color);
    void FillRectangle(int x, int y, int width, int height, const Color &color);
    void FillRegion(int x, int y, const Color &color);
    void FillRegion(int x, int y, const Color &color, const Color &colorBound);
    void CopyImage(int x, int y, PixelImage &image);      // Those points which have transparency more than 0.5 are copied
    void DrawPolyline(const Color &color, int numPoints, int *xy);
    void DrawCircle(float x, float y, float radius, const Color &color, float step = 1.0f);
    IntVector2 GetHotSpot() const;
    void SetHotSpot(int x, int y);

private:
    PixelImage& operator=(const PixelImage&)
    {};

    void Replace4Points(int x, int y, const Color &color);
    void Replace4PointsBound(int x, int y, const Color &color);

    Color replacedColor;
    Color boundingColor;
    IntVector2 hotSpot;

};


#endif //URHO3D_PIXELIMAGE_H
