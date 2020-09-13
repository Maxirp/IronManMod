#include "ShapeHelper.h"
#include "RenderWare.h"
#include "CSprite2d.h"
#include "plugin.h"

const int MAX_CIRCLE_SIDES = 60;
const float CIRCLE_STEP = 6.0f;

float ShapeHelper::mSinTable[360];
float ShapeHelper::mCosTable[360];

void ShapeHelper::InitSinCosTable() {
    for (unsigned int i = 0; i < 360; i++) {
        mSinTable[i] = sinf(static_cast<float>(i) * 0.017453292f);
        mCosTable[i] = cosf(static_cast<float>(i) * 0.017453292f);
    }
}

void ShapeHelper::AddOneVertToBuffer(RwIm2DVertex *verts, unsigned int vertIndex, float x, float y, float z, float rhw, float u, float v, unsigned int color) {
    verts[vertIndex].emissiveColor = color;
    verts[vertIndex].rhw = rhw;
    verts[vertIndex].u = u;
    verts[vertIndex].v = v;
    verts[vertIndex].x = x;
    verts[vertIndex].y = y;
    verts[vertIndex].z = z;
}

void ShapeHelper::RotateVertices(RwIm2DVertex *verts, unsigned int num, float center_x, float center_y, float angle) {
    float l_angle = fmodf(angle, 360.0f);
    if (l_angle < 0.0f)
        l_angle += 360.0;
    l_angle = 360.0f - l_angle;
    float fCos = mCosTable[static_cast<unsigned int>(l_angle) % 360];
    float fSin = mSinTable[static_cast<unsigned int>(l_angle) % 360];
    for (unsigned int i = 0; i < num; i++) {
        float xold = verts[i].x;
        float yold = verts[i].y;
        verts[i].x = center_x + (xold - center_x) * fCos + (yold - center_y) * fSin;
        verts[i].y = center_y - (xold - center_x) * fSin + (yold - center_y) * fCos;
    }
}

void ShapeHelper::DrawFadedRectangle(CRect const& rect, CRGBA const& color, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4) {
	CSprite2d::SetVertices(rect, color, CRGBA(color.r, color.g, color.b, 0), color, CRGBA(color.r, color.g, color.b, 0), u1, v1, u2, v2, u3, v3, u4, v4);
	RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, CSprite2d::maVertices, 4);
}

void ShapeHelper::DrawTexturedRectangle(CRect const& rect, CRGBA const& color, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4) {
    CSprite2d::SetVertices(rect, color, color, color, color, u1, v1, u2, v2, u3, v3, u4, v4);
    RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, CSprite2d::maVertices, 4);
}

void ShapeHelper::DrawRotatedTexturedRectangle(CRect const& rect, float center_x, float center_y, float angle, CRGBA const& color, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4) {
    CSprite2d::SetVertices(rect, color, color, color, color, u1, v1, u2, v2, u3, v3, u4, v4);
    RotateVertices(CSprite2d::maVertices, 4, center_x, center_y, angle);
    RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, CSprite2d::maVertices, 4);
}

void ShapeHelper::DrawCircleSectorTextured(float tex_width, float tex_height, float tex_u_offset, float tex_v_offset, float tex_u_size, float tex_v_size,
    float center_x, float center_y, float width, float height, float start, float end, CRGBA color) {
    static RwIm2DVertex verts[MAX_CIRCLE_SIDES * 2 + 3];
    unsigned int vertsCounter = 0;
    AddOneVertToBuffer(verts, vertsCounter++, center_x, center_y, 0.0f, 1.0f,
        0.5f * tex_u_size / tex_width + tex_u_offset / tex_width,
        0.5f * tex_v_size / tex_height + tex_v_offset / tex_height, RWRGBALONG(color.r, color.g, color.b, color.a));
    float a = start;
    for (int i = 0; i < 60; i++) {
        float cos_x = mCosTable[static_cast<unsigned int>(a) % 360];
        float cos_y = mSinTable[static_cast<unsigned int>(a) % 360];
        float u = cos_x / 2.0f + 0.5f;
        float v = cos_y / 2.0f + 0.5f;
        AddOneVertToBuffer(verts, vertsCounter++, cos_x * width + center_x, cos_y * height + center_y, 0.0f, 1.0f,
            u * tex_u_size / tex_width + tex_u_offset / tex_width,
            v * tex_v_size / tex_height + tex_v_offset / tex_height, RWRGBALONG(color.r, color.g, color.b, color.a));
        AddOneVertToBuffer(verts, vertsCounter++, center_x, center_y, 0.0f, 1.0f,
            0.5f * tex_u_size / tex_width + tex_u_offset / tex_width,
            0.5f * tex_v_size / tex_height + tex_v_offset / tex_height, RWRGBALONG(color.r, color.g, color.b, color.a));
        a += CIRCLE_STEP;
        if (a > end) {
            cos_x = mCosTable[static_cast<unsigned int>(end) % 360];
            cos_y = mSinTable[static_cast<unsigned int>(end) % 360];
            u = cos_x / 2.0f + 0.5f;
            v = cos_y / 2.0f + 0.5f;
            AddOneVertToBuffer(verts, vertsCounter++, cos_x * width + center_x, cos_y * height + center_y, 0.0f, 1.0f,
                u * tex_u_size / tex_width + tex_u_offset / tex_width,
                v * tex_v_size / tex_height + tex_v_offset / tex_height, RWRGBALONG(color.r, color.g, color.b, color.a));
            AddOneVertToBuffer(verts, vertsCounter++, center_x, center_y, 0.0f, 1.0f,
                0.5f * tex_u_size / tex_width + tex_u_offset / tex_width,
                0.5f * tex_v_size / tex_height + tex_v_offset / tex_height, RWRGBALONG(color.r, color.g, color.b, color.a));
            break;
        }
        else if (a == end)
            break;
    }
    RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, verts, vertsCounter);
}

void ShapeHelper::DrawRotatedCircleSectorTextured(float tex_width, float tex_height, float tex_u_offset, float tex_v_offset, float tex_u_size, float tex_v_size,
	float center_x, float center_y,
	float width, float height, float start, float end, CRGBA color, float angle)
{
	

	static RwIm2DVertex verts[MAX_CIRCLE_SIDES * 2 + 3];
	unsigned int vertsCounter = 0;
	
	AddOneVertToBuffer(verts, vertsCounter++, center_x, center_y, 0.0f, 1.0f,
		0.5f * tex_u_size / tex_width + tex_u_offset / tex_width,
		0.5f * tex_v_size / tex_height + tex_v_offset / tex_height, RWRGBALONG(color.r, color.g, color.b, color.a));
	float a = start + angle;
	for (int i = 0; i < 60; i++) {
		float cos_x = mCosTable[static_cast<unsigned int>(a) % 360];
		float cos_y = mSinTable[static_cast<unsigned int>(a) % 360];
		float u = cos_x / 2.0f + 0.5f;
		float v = cos_y / 2.0f + 0.5f;
		AddOneVertToBuffer(verts, vertsCounter++, cos_x * width + center_x, cos_y * height + center_y, 0.0f, 1.0f,
			u * tex_u_size / tex_width + tex_u_offset / tex_width,
			v * tex_v_size / tex_height + tex_v_offset / tex_height, RWRGBALONG(color.r, color.g, color.b, color.a));
		AddOneVertToBuffer(verts, vertsCounter++, center_x, center_y, 0.0f, 1.0f,
			0.5f * tex_u_size / tex_width + tex_u_offset / tex_width,
			0.5f * tex_v_size / tex_height + tex_v_offset / tex_height, RWRGBALONG(color.r, color.g, color.b, color.a));
		a += CIRCLE_STEP;
		if (a > end + angle) {
			cos_x = mCosTable[static_cast<unsigned int>(end + angle) % 360];
			cos_y = mSinTable[static_cast<unsigned int>(end + angle) % 360];
			u = cos_x / 2.0f + 0.5f;
			v = cos_y / 2.0f + 0.5f;
			AddOneVertToBuffer(verts, vertsCounter++, cos_x * width + center_x, cos_y * height + center_y, 0.0f, 1.0f,
				u * tex_u_size / tex_width + tex_u_offset / tex_width,
				v * tex_v_size / tex_height + tex_v_offset / tex_height, RWRGBALONG(color.r, color.g, color.b, color.a));
			AddOneVertToBuffer(verts, vertsCounter++, center_x, center_y, 0.0f, 1.0f,
				0.5f * tex_u_size / tex_width + tex_u_offset / tex_width,
				0.5f * tex_v_size / tex_height + tex_v_offset / tex_height, RWRGBALONG(color.r, color.g, color.b, color.a));
			break;
		}
		else if (a == end + angle)
			break;
	}
	RotateVertices(verts, vertsCounter, center_x, center_y, -angle);
	
	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, verts, vertsCounter);
}

void ShapeHelper::DrawTriangleSectorTextured(CRect const& rect, CRGBA const& color) {
	static RwIm2DVertex verts[10];

	CVector2D medidas = { rect.right + 30.0f - rect.left ,rect.bottom - rect.top };
	//float resolx0 = (medidas.x) / medidas.y;
	
	CVector2D centro;
	centro.x = medidas.x / 2.0f; 
	centro.y = medidas.y / 2.0f;
	float dist = (rect.bottom - rect.top);
	float resol = dist / (rect.right + 30.0f - rect.left);

	verts[0].x = rect.left;
	verts[0].y = rect.top;
	verts[1].x = rect.right;
	verts[1].y = rect.top;
	verts[2].x = rect.left;//2
	verts[2].y = rect.bottom;//2
	verts[3].x = rect.right + 25.0f;
	verts[3].y = rect.bottom;

	verts[4].x = rect.right + 25.0f;
	verts[4].y = rect.bottom;
	verts[5].x = rect.right;
	verts[5].y = rect.top;
	verts[6].x = rect.right + 25.0f;//6
	verts[6].y = rect.top + dist / 2.0f;//6
	verts[7].x = rect.right + 30.0f;//3
	verts[7].y = rect.top + 30.0f;//3

	verts[8].x = rect.right + 25.0f;//6
	verts[8].y = rect.top + dist / 2.0f;//6
	verts[9].x = rect.right + 30.0f;//7
	verts[9].y = rect.top + dist / 2.0f;//7

	/*
	verts[4].x = rect.right + 40.0f;//5
	verts[4].y = rect.top + 40.0f;//5
	verts[5].x = rect.right + 40.0f;//7
	verts[5].y = rect.top + dist / 2.0f;//7
	verts[6].x = rect.left;//4
	verts[6].y = rect.bottom;//4

	verts[8].x = rect.right + 35.0f;//6
	verts[8].y = rect.top + dist / 2.0f;//6
	verts[9].x = rect.left;//8
	verts[9].y = rect.bottom;//8*/
	//verts[5].x = rect.left;
	//verts[5].y = rect.top;

	for (int i = 0; i < 10; i++)
	{
		verts[i].emissiveColor = RWRGBALONG(color.r, color.g, color.b, color.a);
		verts[i].rhw = 0.0f;
		verts[i].z = 0.0f;
	}
	dist = (rect.right + 30.0f - rect.left);
	verts[0].u = 0.0f;//
	verts[0].v = 0.5 - 0.5 * resol;//
	verts[1].u = 1.0 - 30.0 / dist;//
	verts[1].v = 0.5 - 0.5 * resol;//
	verts[2].u = 0.0;
	verts[2].v = 0.5 + 0.5 * resol;
	verts[3].u = 1.0 - 5.0 / dist;//
	verts[3].v = 0.5 + 0.5 * resol;//

	verts[4].u = 1.0 - 5.0 / dist;//
	verts[4].v = 0.5 + 0.5 * resol;//
	verts[5].u = 1.0 - 30.0 / dist;//
	verts[5].v = 0.5 - 0.5 * resol;//
	verts[6].u = 1.0 - 5.0 / dist;//
	verts[6].v = 0.5;//
	verts[7].u = 1.0;//
	verts[7].v = 0.5 - 0.5 * resol + 30.0 / dist;//

	verts[8].u = 1.0 - 5.0 / dist;//
	verts[8].v = 0.5;//
	//verts[4].u = 1.0;//
	//verts[4].v = 0.5 - 0.5 * resol + 40.0 / dist;//
	verts[9].u = 1.0;//
	verts[9].v = 0.5;
	/*verts[6].u = 0.0;
	verts[6].v = 0.5 + 0.5 * resol;
	//verts[5].u = 0.0f;//
	//verts[5].v = 0.5 - 0.5 * resol;//
	verts[8].u = 1.0 - 5.0 / dist;//
	verts[8].v = 0.5;//
	verts[9].u = 0.0;
	verts[9].v = 0.5 + 0.5 * resol;
	verts[10].u = 1.0 - 5.0 / dist;//
	verts[10].v = 0.5 + 0.5 * resol;//*/
	
	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, verts,10);
}

void ShapeHelper::DrawTriangleSectorBorder(CRect const& rect, CRGBA const& color) {
	
	float border = 5.0f;
	float ofset = 2.0f;
	CRect rect1 = CRect(rect.left - border,rect.top - border,rect.right + border,rect.bottom + border );
	
	static RwIm2DVertex verts[10];

	CVector2D medidas = { rect1.right + 30.0f - rect1.left ,rect1.bottom - rect1.top };
	//float resolx0 = (medidas.x) / medidas.y;

	CVector2D centro;
	centro.x = medidas.x / 2.0f;
	centro.y = medidas.y / 2.0f;
	float dist = (rect1.bottom - rect1.top);
	float resol = dist / (rect1.right + 30.0f - rect1.left);

	verts[0].x = rect1.left;
	verts[0].y = rect1.top;
	verts[1].x = rect1.right - ofset;
	verts[1].y = rect1.top;
	verts[2].x = rect1.left;//2
	verts[2].y = rect1.bottom;//2
	verts[3].x = rect1.right + 25.0f;
	verts[3].y = rect1.bottom;

	verts[4].x = rect1.right + 25.0f;
	verts[4].y = rect1.bottom;
	verts[5].x = rect1.right - ofset;
	verts[5].y = rect1.top;
	verts[6].x = rect1.right + 25.0f;//6
	verts[6].y = rect1.top + dist / 2.0f + border;//6
	verts[7].x = rect1.right + 30.0f;//3
	verts[7].y = rect1.top + 30.0f + ofset;//3

	verts[8].x = rect1.right + 25.0f;//6
	verts[8].y = rect1.top + dist / 2.0f + border;//6
	verts[9].x = rect1.right + 30.0f;//7
	verts[9].y = rect1.top + dist / 2.0f + border;//7

										/*
										verts[4].x = rect1.right + 40.0f;//5
										verts[4].y = rect1.top + 40.0f;//5
										verts[5].x = rect1.right + 40.0f;//7
										verts[5].y = rect1.top + dist / 2.0f;//7
										verts[6].x = rect1.left;//4
										verts[6].y = rect1.bottom;//4

										verts[8].x = rect1.right + 35.0f;//6
										verts[8].y = rect1.top + dist / 2.0f;//6
										verts[9].x = rect1.left;//8
										verts[9].y = rect1.bottom;//8*/
										//verts[5].x = rect1.left;
										//verts[5].y = rect1.top;

	for (int i = 0; i < 10; i++)
	{
		verts[i].emissiveColor = RWRGBALONG(color.r, color.g, color.b, color.a);
		verts[i].rhw = 0.0f;
		verts[i].z = 0.0f;
	}
	dist = (rect1.right + 30.0f - rect1.left);
	verts[0].u = 0.0f;//
	verts[0].v = 0.5 - 0.5 * resol;//
	verts[1].u = 1.0 - 30.0 / dist;//
	verts[1].v = 0.5 - 0.5 * resol;//
	verts[2].u = 0.0;
	verts[2].v = 0.5 + 0.5 * resol;
	verts[3].u = 1.0 - 5.0 / dist;//
	verts[3].v = 0.5 + 0.5 * resol;//

	verts[4].u = 1.0 - 5.0 / dist;//
	verts[4].v = 0.5 + 0.5 * resol;//
	verts[5].u = 1.0 - 30.0 / dist;//
	verts[5].v = 0.5 - 0.5 * resol;//
	verts[6].u = 1.0 - 5.0 / dist;//
	verts[6].v = 0.5;//
	verts[7].u = 1.0;//
	verts[7].v = 0.5 - 0.5 * resol + 30.0 / dist;//

	verts[8].u = 1.0 - 5.0 / dist;//
	verts[8].v = 0.5;//
					 //verts[4].u = 1.0;//
					 //verts[4].v = 0.5 - 0.5 * resol + 40.0 / dist;//
	verts[9].u = 1.0;//
	verts[9].v = 0.5;
	/*verts[6].u = 0.0;
	verts[6].v = 0.5 + 0.5 * resol;
	//verts[5].u = 0.0f;//
	//verts[5].v = 0.5 - 0.5 * resol;//
	verts[8].u = 1.0 - 5.0 / dist;//
	verts[8].v = 0.5;//
	verts[9].u = 0.0;
	verts[9].v = 0.5 + 0.5 * resol;
	verts[10].u = 1.0 - 5.0 / dist;//
	verts[10].v = 0.5 + 0.5 * resol;//*/

	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, verts, 10);
}

static int nada(CRect const& rect, CRGBA const& color) 
{
	static RwIm2DVertex verts[40];

	CVector2D medidas = { rect.right + 40.0f - rect.left ,rect.bottom - rect.top };
	//float resolx0 = (medidas.x) / medidas.y;

	CVector2D centro;
	centro.x = medidas.x / 2.0f;
	centro.y = medidas.y / 2.0f;
	float dist = (rect.bottom - rect.top);
	float resol = dist / (rect.right + 40.0f - rect.left);

	verts[0].x = rect.left;
	verts[0].y = rect.top;
	verts[1].x = rect.right;
	verts[1].y = rect.top;
	verts[2].x = rect.right + 40.0f;
	verts[2].y = rect.top + 40.0f;
	verts[3].x = rect.left;
	verts[3].y = rect.top;

	verts[4].x = rect.left;
	verts[4].y = rect.top;
	verts[5].x = rect.right + 40.0f;
	verts[5].y = rect.top + 40.0f;
	verts[6].x = rect.right + 40.0f;
	verts[6].y = rect.top + dist / 2.0f;
	verts[7].x = rect.left;
	verts[7].y = rect.top;

	verts[8].x = rect.left;
	verts[8].y = rect.top;
	verts[9].x = rect.right + 40.0f;
	verts[9].y = rect.top + dist / 2.0f;
	verts[10].x = rect.right + 35.0f;
	verts[10].y = rect.top + dist / 2.0f;
	verts[11].x = rect.left;
	verts[11].y = rect.top;

	verts[12].x = rect.left;
	verts[12].y = rect.top;
	verts[13].x = rect.right + 35.0f;
	verts[13].y = rect.top + dist / 2.0f;
	verts[14].x = rect.right + 35.0f;
	verts[14].y = rect.bottom;
	verts[15].x = rect.left;
	verts[15].y = rect.top;

	verts[16].x = rect.left;
	verts[16].y = rect.top;
	verts[17].x = rect.right + 35.0f;
	verts[17].y = rect.bottom;
	verts[18].x = rect.left;
	verts[18].y = rect.bottom;
	verts[19].x = rect.left;
	verts[19].y = rect.top;

	for (int i = 0; i < 40; i++)
	{
		verts[i].emissiveColor = RWRGBALONG(color.r, color.g, color.b, color.a);
		verts[i].rhw = 0.0f;
		verts[i].z = 0.0f;
	}
	dist = (rect.right + 40.0f - rect.left);
	verts[0].u = 0.0f;//
	verts[0].v = 0.5 - 0.5 * resol;//
	verts[1].u = 1.0 - 40.0 / dist;//
	verts[1].v = 0.5 - 0.5 * resol;//
	verts[2].u = 1.0;//
	verts[2].v = 0.5 - 0.5 * resol + 40.0 / dist;//
	verts[3].u = 0.0f;//
	verts[3].v = 0.5 - 0.5 * resol;//

	verts[4].u = 0.0;
	verts[4].v = 0.5 - 0.5 * resol;//
	verts[5].u = 1.0;
	verts[5].v = 0.5 - 0.5 * resol + 40.0 / dist;//
	verts[6].u = 1.0;//
	verts[6].v = 0.5;
	verts[7].u = 0.0;
	verts[7].v = 0.5 - 0.5 * resol;//

	verts[8].u = 0.0;
	verts[8].v = 0.5 - 0.5 * resol;//
	verts[9].u = 1.0;//
	verts[9].v = 0.5;
	verts[10].u = 1.0 - 5.0 / dist;//
	verts[10].v = 0.5;//
	verts[11].u = 0.0;
	verts[11].v = 0.5 - 0.5 * resol;//

	verts[12].u = 0.0;
	verts[12].v = 0.5 - 0.5 * resol;//
	verts[13].u = 1.0 - 5.0 / dist;//
	verts[13].v = 0.5;//
	verts[14].u = 1.0 - 5.0 / dist;//
	verts[14].v = 0.5 + 0.5 * resol;//
	verts[15].u = 0.0;
	verts[15].v = 0.5 - 0.5 * resol;//

	verts[16].u = 0.0;
	verts[16].v = 0.5 - 0.5 * resol;//
	verts[17].u = 1.0 - 5.0 / dist;//
	verts[17].v = 0.5 + 0.5 * resol;//
	verts[18].u = 0.0;
	verts[18].v = 0.5 + 0.5 * resol;
	verts[19].u = 0.0;
	verts[19].v = 0.5 - 0.5 * resol;//

	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, verts, 40);
}