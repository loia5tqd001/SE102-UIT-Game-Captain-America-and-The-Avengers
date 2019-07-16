#include "pch.h"

bool DebugDraw::isInDebugMode;
bool DebugDraw::isDrawCrt;
std::unique_ptr<Font> DebugDraw::fontDraw;

void DebugDraw::InitDefaultFont()
{
	fontDraw = std::make_unique<Font>(12, "Arial");
	isInDebugMode = false;
	isDrawCrt = false;
}

void DebugDraw::DrawSolidRect(const RectF& bbox, D3DCOLOR color)
{
	if (!isInDebugMode) return;

	static const auto& cam = Camera::Instance();
	const auto drawablePos = cam.GetPositionInViewPort( bbox.GetTopLeft() );

	static const auto bboxTexture = Textures::Get(TextureId::Bbox);
	Game::Instance().Draw(drawablePos, bboxTexture, bbox.GetOriginRect(), { 1.0f, 1.0f }, color);
}

void DebugDraw::DrawRectOutLine(const RectF& bbox, D3DCOLOR color)
{
	if (!isInDebugMode) return;

	if (bbox.left != 0.0f)
	{
		const RectF left = { bbox.left, bbox.top, bbox.left + 1, bbox.bottom  };
		DrawSolidRect( left , color );
	}

	if (bbox.top != 0.0f)
	{
		const RectF top  = { bbox.left, bbox.top, bbox.right   , bbox.top + 1 };
		DrawSolidRect( top  , color );
	}
}

void DebugDraw::ToggleDebugMode()
{
	isInDebugMode = !isInDebugMode;
}

void DebugDraw::ToggleCrtMode()
{
	isDrawCrt = !isDrawCrt;
}

bool DebugDraw::IsInDebugMode()
{
	return isInDebugMode;
}

bool DebugDraw::IsInDeepDebug()
{
	return IsInDebugMode() && !Settings::Instance().IsOpening() && Window::Instance().IsKeyPressed(VK_SHIFT);
}

void DebugDraw::DrawString(const std::string& str, const Vector2& pos, D3DCOLOR color)
{
	if (!isInDebugMode) return;

	const auto drawablePos = Camera::Instance().GetPositionInViewPort( pos );
	fontDraw->DrawString(str, drawablePos, color);
}

void DebugDraw::DrawString(const std::string& str, const Vector2& pos, D3DCOLOR color, UINT size, LPCSTR font)
{
	if (!isInDebugMode) return;

	const auto drawablePos = Camera::Instance().GetPositionInViewPort( pos );

	fontDraw->AdjustFont(size, font);
	fontDraw->DrawString(str, drawablePos, color);
}

void DebugDraw::DrawCrt()
{
	if (!isDrawCrt || isInDebugMode) return;

	static const auto  crtTexture = Textures::Get(TextureId::CrtEffect);
	static const Sprite crtSprite = { crtTexture, Window::Instance().GetWindowFrame() };

	crtSprite.Draw( { 0.0f, 0.0f }, 0, { 1.0f, 1.0f }, 100 );
}