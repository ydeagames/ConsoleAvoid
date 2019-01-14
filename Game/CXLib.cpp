#include "CXLib.h"
#include "Game.h"
#include "Screen.h"
#include "ScreenManager.h"

namespace CXLib
{
	// ワールド座標をコンソール座標に変換
	float WorldToConsoleXF(float world_x, float size)
	{
		return world_x * (SCREEN_RESOLUTION_X * size);
	}

	// ワールド座標をコンソール座標に変換
	float WorldToConsoleYF(float world_y, float size)
	{
		return world_y * (SCREEN_RESOLUTION_Y * size);
	}

	// ワールド座標をコンソール座標に変換
	SHORT WorldToConsoleX(float world_x, float size)
	{
		return static_cast<SHORT>(floorf(WorldToConsoleXF(world_x, size)));
	}

	// ワールド座標をコンソール座標に変換
	SHORT WorldToConsoleY(float world_y, float size)
	{
		return static_cast<SHORT>(floorf(WorldToConsoleYF(world_y, size)));
	}

	// コンソール座標をワールド座標に変換
	float ConsoleToWorldXF(float screen_x, float size)
	{
		return screen_x / (SCREEN_RESOLUTION_X * size);
	}

	// コンソール座標をワールド座標に変換
	float ConsoleToWorldYF(float screen_y, float size)
	{
		return screen_y / (SCREEN_RESOLUTION_Y * size);
	}

	// コンソール座標をワールド座標に変換
	float ConsoleToWorldX(SHORT screen_x, float size)
	{
		return ConsoleToWorldXF(static_cast<float>(screen_x), size);
	}

	// コンソール座標をワールド座標に変換
	float ConsoleToWorldY(SHORT screen_y, float size)
	{
		return ConsoleToWorldYF(static_cast<float>(screen_y), size);
	}

	// 矩形描画関数
	void DrawBox(Vector2 boundsMin, Vector2 boundsMax, Color Color, int FillFlag)
	{
		// ワールド座標をコンソール座標に変換
		Vector2 c1 = boundsMin * ScreenToConsole;
		Vector2 c2 = boundsMax * ScreenToConsole;
		SHORT cx1 = c1.Xs();
		SHORT cy1 = c1.Ys();
		SHORT cx2 = c2.Xs();
		SHORT cy2 = c2.Ys();

		// Yループ
		SHORT ix, iy;
		for (iy = cy1; iy <= cy2; iy++)
		{
			// Xループ
			for (ix = cx1; ix <= cx2; ix++)
			{
				// 塗りつぶし時、または縁の場合描画
				if (FillFlag || (ix == cx1 || ix == cx2) || (iy == cy1 || iy == cy2))
					Screen::Draw({ ix, iy }, Color);
			}
		}
	}

	// 楕円描画関数
	void DrawOval(Vector2 center, Vector2 size, Color Color, int FillFlag)
	{
		// ワールド座標をコンソール座標に変換
		Vector2 c = center * ScreenToConsole;
		Vector2 cr = size * ScreenToConsole;
		SHORT cx = c.Xs();
		SHORT cy = c.Ys();
		SHORT crx = cr.Xs();
		SHORT cry = cr.Ys();

		// Yループ
		SHORT ix, iy;
		for (iy = -cry; iy <= cry; iy++)
		{
			// Xループ
			for (ix = -crx; ix <= crx; ix++)
			{
				// 塗りつぶし時、または縁の場合描画
				float p = (ix*ix) / (cr.x*cr.x) + (iy*iy) / (cr.y*cr.y) - 1;
				if (FillFlag ? p <= .08f : -.25f < p && p < .08f)
					Screen::Draw({ cx + ix, cy + iy }, Color);
			}
		}
	}

	// 円描画関数
	void DrawCircle(Vector2 center, float r, Color Color, int FillFlag)
	{
		// 楕円描画
		DrawOval(center, Vector2::one * r, Color, FillFlag);
	}

	// 線描画関数
	void DrawLine(Vector2 start, Vector2 end, Color Color)
	{
		// ワールド座標をコンソール座標に変換
		Vector2 c1 = start * ScreenToConsole;
		Vector2 c2 = end * ScreenToConsole;
		SHORT cx1 = c1.Xs();
		SHORT cy1 = c1.Ys();
		SHORT cx2 = c2.Xs();
		SHORT cy2 = c2.Ys();

		// 縦、横、どちらの比が大きいか
		bool steep = std::abs(cy2 - cy1) > std::abs(cx2 - cx1);
		// 縦が大きかったら縦横反転
		if (steep)
		{
			std::swap(cx1, cy1);
			std::swap(cx2, cy2);
		}

		{
			// 差を取得
			SHORT deltax = std::abs(cx2 - cx1);
			SHORT deltay = std::abs(cy2 - cy1);
			// ズレを計算
			SHORT error = deltax / 2;

			// インクリメント
			SHORT incx = (cx1 < cx2) ? 1 : -1;
			SHORT incy = (cy1 < cy2) ? 1 : -1;

			// Xループ
			SHORT x;
			SHORT y = cy1;
			for (x = cx1; x != cx2; x += incx)
			{
				// 描画
				Screen::Draw({ steep ? y : x, steep ? x : y }, Color);

				// ズレを修正&チェック
				if ((error -= deltay) < 0)
				{
					y += incy;
					error += deltax;
				}
			}
		}
	}

	// 破線を描画
	void DrawDashedLine(Vector2 start, Vector2 end, Color Color, float length)
	{
		// ワールド座標をコンソール座標に変換
		Vector2 c1 = start * ScreenToConsole;
		Vector2 c2 = end * ScreenToConsole;
		SHORT cx1 = c1.Xs();
		SHORT cy1 = c1.Ys();
		SHORT cx2 = c2.Xs();
		SHORT cy2 = c2.Ys();
		float dx = (end - start).Normalized().x * length;

		// 縦、横、どちらの比が大きいか
		bool steep = std::abs(cy2 - cy1) > std::abs(cx2 - cx1);
		// 縦が大きかったら縦横反転
		if (steep)
		{
			std::swap(cx1, cy1);
			std::swap(cx2, cy2);
		}

		{
			// 差を取得
			SHORT deltax = std::abs(cx2 - cx1);
			SHORT deltay = std::abs(cy2 - cy1);
			// ズレを計算
			SHORT error = deltax / 2;

			// インクリメント
			SHORT incx = (cx1 < cx2) ? 1 : -1;
			SHORT incy = (cy1 < cy2) ? 1 : -1;

			// Xループ
			SHORT x;
			SHORT y = cy1;
			for (x = cx1; x != cx2; x += incx)
			{
				if (static_cast<SHORT>(x / dx) % 2 == 0)
					// 描画
					Screen::Draw({ steep ? y : x, steep ? x : y }, Color);

				// ズレを修正&チェック
				if ((error -= deltay) < 0)
				{
					y += incy;
					error += deltax;
				}
			}
		}
	}
}