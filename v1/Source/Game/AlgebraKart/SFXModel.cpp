#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/Graphics/IndexBuffer.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <SDL/SDL_log.h>

#include "SFXModel.h"

#include <Urho3D/DebugNew.h>

//=============================================================================
//=============================================================================
#define MAX_SKID_STRIPS         120
#define VERTS_PER_STRIP         4
#define INDECES_PER_STRIP       6
#define STRIP_NORMAL_OFFSET     0.05f
#define MIN_STRIP_LEN           0.4f
#define MAX_STRIP_LEN           4.0f
#define MIN_LASTPOS_CNT         8
#define SMALL_BIT               0.99f

//=============================================================================
//=============================================================================
void SFXModel::RegisterObject(Context* context)
{
    context->RegisterFactory<SFXModel>();
}

void SFXModel::OnWorldBoundingBoxUpdate()
{
    if ( m_pParentNode )
    {
        worldBoundingBox_ = boundingBox_.Transformed(m_pParentNode->GetWorldTransform());
    }
    else
    {
        StaticModel::OnWorldBoundingBoxUpdate();
    }
}

void SFXModel::SetParentNode(Node *pParentNode)
{
    m_pParentNode = pParentNode;
}

void SFXModel::UpdateWorldPos()
{
    OnMarkedDirty(node_);
}

// **not used. instead, just use a model with (MASK_POSITION | MASK_NORMAL | MASK_COLOR | MASK_TEXCOORD1) elements
bool SFXModel::CreateVBuffer(const Color &color, float width)
{
    assert(false && "just use a model with (MASK_POSITION | MASK_NORMAL | MASK_COLOR | MASK_TEXCOORD1) elements");
    return false;
}

bool SFXModel::ValidateBufferElements() const
{
    Geometry *pGeometry = GetModel()->GetGeometry(0,0);
    VertexBuffer *pVbuffer = pGeometry->GetVertexBuffer(0);
    const unsigned uElementMask = pVbuffer->GetElementMask();
    const unsigned uRequiredMask = MASK_POSITION | MASK_NORMAL | MASK_COLOR | MASK_TEXCOORD1;
    unsigned zeroMask = ( uElementMask & ~uRequiredMask );
    return ( uElementMask & uRequiredMask ) == uRequiredMask && zeroMask == 0;
}

void SFXModel::AddStrip(const Vector3 &cpos, const Vector3 &normal)
{
    Vector3 pos = cpos + normal * STRIP_NORMAL_OFFSET; // lift the position away from the ground by NORMAL_OFFSET

    // the 1st entry of the strip
    if ( !m_firstStripPoint.valid )
    {
        m_firstStripPoint.pos    = pos;
        m_firstStripPoint.normal = normal;
        m_firstStripPoint.valid  = true;
        m_firstStripPoint.lastPosCnt = MIN_LASTPOS_CNT;
    }
    else
    {
        // calculate direction and right vectors to the previous position
        Vector3 dir = ( m_firstStripPoint.pos - pos );
        m_firstStripPoint.lastPosCnt = MIN_LASTPOS_CNT;

        // avoid creating tiny strips
        if ( dir.Length() < MIN_STRIP_LEN )
        {
            return;
        }

        // and large strips
        if ( dir.Length() > MAX_STRIP_LEN )
        {
            m_firstStripPoint.valid = false;
            m_firstStripPoint.vertsArrayValid  = false;
            return;
        }

        dir.Normalize();
        Vector3 right = normal.CrossProduct(dir).Normalized();

        SfxGeomData geomData[ VERTS_PER_STRIP ];

        geomData[0].pos = pos - right * m_fHalfWidth;
        geomData[1].pos = pos + right * m_fHalfWidth;
        geomData[2].pos = m_firstStripPoint.pos - right * m_fHalfWidth;
        geomData[3].pos = m_firstStripPoint.pos + right * m_fHalfWidth;

        // copy the last vert positions if present (don't exist on the very first strip)
        if ( m_firstStripPoint.vertsArrayValid )
        {
            geomData[2].pos = m_firstStripPoint.v[0];
            geomData[3].pos = m_firstStripPoint.v[1];
        }

        geomData[0].normal = normal;
        geomData[1].normal = normal;
        geomData[2].normal = m_firstStripPoint.normal;
        geomData[3].normal = m_firstStripPoint.normal;

        geomData[0].color = m_unsignedColor;
        geomData[1].color = m_unsignedColor;
        geomData[2].color = m_unsignedColor;
        geomData[3].color = m_unsignedColor;

        geomData[0].uv = Vector2(0,0);
        geomData[1].uv = Vector2(1,0);
        geomData[2].uv = Vector2(0,1);
        geomData[3].uv = Vector2(1,1);

        // 4 verts, 2 tris, vertex draw order - clockwise dir
        unsigned short triIdx[6] = { 0, 2, 1, 1, 2, 3 };

        // update the first strip (previous) data
        m_firstStripPoint.pos    = pos;
        m_firstStripPoint.normal = normal;
        m_firstStripPoint.vertsArrayValid = true;
        m_firstStripPoint.v[0] = geomData[0].pos;
        m_firstStripPoint.v[1] = geomData[1].pos;

        // shift vbuff elements to the right by 4
        if ( m_vSfxGeom.Size() < VERTS_PER_STRIP * MAX_SKID_STRIPS )
        {
            m_vSfxGeom.Resize(m_vSfxGeom.Size() + VERTS_PER_STRIP);
        }
        for ( int i = (int)m_vSfxGeom.Size() - 1; i >= 0; --i )
        {
            if ( i - VERTS_PER_STRIP >= 0 )
            {
                // shift
                memcpy( &m_vSfxGeom[i], &m_vSfxGeom[i - VERTS_PER_STRIP], sizeof(SfxGeomData) );

                //GeomData &geData = m_vSfxGeom[i];

                // fade alpha by a small bit every shift
                // Color::ToUInt() = (a << 24) | (b << 16) | (g << 8) | r;
                //unsigned a = (m_vSfxGeom[i].color >> 24);
                //a = ((unsigned)Clamp((int)( (float)a * SMALL_BIT ), 0, 255)) << 24;
                //a = 0xff << 24;
                //m_vSfxGeom[i].color = (m_vSfxGeom[i].color & 0x00ffffff) | a;
            }
        }

        // copy new geom
        memcpy( &m_vSfxGeom[0], geomData, sizeof(geomData) );

        // shift indexbuff to the right by 6
        if ( m_vSfxIndex.Size() < INDECES_PER_STRIP * MAX_SKID_STRIPS )
        {
            m_vSfxIndex.Resize(m_vSfxIndex.Size() + INDECES_PER_STRIP);
        }
        for ( int i = (int)m_vSfxIndex.Size() - 1; i >= 0; --i )
        {
            if ( i - INDECES_PER_STRIP >= 0 )
            {
                // need to add +4 offset(for newly added verts) to indeces being shifted
                m_vSfxIndex[i] = m_vSfxIndex[i - INDECES_PER_STRIP] + VERTS_PER_STRIP;
            }
        }

        // copy new indeces
        memcpy( &m_vSfxIndex[0], triIdx, sizeof(triIdx) );

        //=================================
        // copy to vertex/index buffers
        CopyToBuffer();
    }
}

void SFXModel::CopyToBuffer()
{
    Geometry *pGeometry = GetModel()->GetGeometry(0,0);
    VertexBuffer *pVbuffer = pGeometry->GetVertexBuffer(0);
    IndexBuffer *pIbuffer = pGeometry->GetIndexBuffer();
    const unsigned uElementMask = pVbuffer->GetElementMask();
    bool brangeChanged = false;

    if ( pVbuffer->GetVertexCount() != m_vSfxGeom.Size() )
    {
        pVbuffer->SetSize(m_vSfxGeom.Size(), uElementMask);
        brangeChanged = true;
    }
    if ( pIbuffer->GetIndexCount() != m_vSfxIndex.Size() )
    {
        pIbuffer->SetSize(m_vSfxIndex.Size(), false);
        brangeChanged = true;
    }

    void *pVertexData = (void*)pVbuffer->Lock(0, pVbuffer->GetVertexCount());
    void *pIndexData = (void*)pIbuffer->Lock(0, pIbuffer->GetIndexCount());

    if ( pVertexData && pIndexData )
    {
        memcpy( pVertexData, &m_vSfxGeom[0], sizeof(SfxGeomData) * m_vSfxGeom.Size() );
        memcpy( pIndexData, &m_vSfxIndex[0], sizeof(unsigned short) * m_vSfxIndex.Size() );

        pVbuffer->Unlock();
        pIbuffer->Unlock();

        // update draw range
        if ( brangeChanged )
        {
            pGeometry->SetDrawRange(TRIANGLE_LIST, 0, m_vSfxIndex.Size());
        }
    }
}

void SFXModel::ClearStrip()
{
    if ( --m_firstStripPoint.lastPosCnt <= 0 )
    {
        m_firstStripPoint.valid = false;
        m_firstStripPoint.vertsArrayValid  = false;
    }
}

bool SFXModel::InSkidState() const
{
    return m_firstStripPoint.valid;
}

void SFXModel::DebugRender(DebugRenderer *dbgRender, const Color &color)
{
    for ( unsigned i = 0; i < m_vSfxGeom.Size(); i += VERTS_PER_STRIP )
    {
        dbgRender->AddLine( m_vSfxGeom[i+0].pos, m_vSfxGeom[i+1].pos, color );
        dbgRender->AddLine( m_vSfxGeom[i+0].pos, m_vSfxGeom[i+2].pos, color );
        dbgRender->AddLine( m_vSfxGeom[i+1].pos, m_vSfxGeom[i+2].pos, color );
        dbgRender->AddLine( m_vSfxGeom[i+1].pos, m_vSfxGeom[i+3].pos, color );
        dbgRender->AddLine( m_vSfxGeom[i+2].pos, m_vSfxGeom[i+3].pos, color );
    }
}





