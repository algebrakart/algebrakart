#pragma once

#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/Object.h>

using namespace Urho3D;
namespace Urho3D
{
}

//=============================================================================
//=============================================================================
//#define DBG_SKID_STRIPS

//=============================================================================
//=============================================================================
struct SkidStrip
{
    SkidStrip() : vertsArrayValid(false), lastPosCnt(0), valid(false){}

    Vector3     pos;
    Vector3     normal;

    Vector3     v[2];

    bool        vertsArrayValid;
    int         lastPosCnt;
    bool        valid;
};

struct GeomData
{
    Vector3     pos;
    Vector3     normal;
    unsigned    color;
    Vector2     uv;
};

//=============================================================================
//=============================================================================
class SFXModel : public StaticModel
{
    URHO3D_OBJECT(SFXModel, StaticModel);

public:
    static void RegisterObject(Context* context);

    SFXModel(Context *context) : StaticModel(context)
    {
        m_fWidth = 1.0f;
        m_fHalfWidth = 0.5f;
    }
    virtual ~SFXModel()
    {
        m_pParentNode = NULL;
    }

    // world pos methods
    virtual void OnWorldBoundingBoxUpdate();
    void SetParentNode(Node *pParentNode);
    void UpdateWorldPos();

    // validation and set methods
    bool CreateVBuffer(const Color &color, float width);
    bool ValidateBufferElements() const;
    void SetWidth(float width)          
    { 
        m_fWidth = width; 
        m_fHalfWidth = width * 0.5f;
    }
    void SetColor(const Color &color)   
    { 
        m_Color = color; 
        m_unsignedColor = color.ToUInt();
    }

    // skid strip
    void AddStrip(const Vector3 &pos, const Vector3 &normal);
    void ClearStrip();
    bool InSkidState() const;

    const PODVector<GeomData>& GetGeomVector() 
    {
        return m_vSkidGeom;
    }

    void DebugRender(DebugRenderer *dbgRender, const Color &color);

protected:
    void CopyToBuffer();

protected:
    WeakPtr<Node> m_pParentNode;

    // strip
    PODVector<GeomData>         m_vSkidGeom;
    PODVector<unsigned short>   m_vSkidIndex;

    SkidStrip   m_firstStripPoint;
    Color       m_Color;
    unsigned    m_unsignedColor;
    float       m_fWidth;
    float       m_fHalfWidth;
};








