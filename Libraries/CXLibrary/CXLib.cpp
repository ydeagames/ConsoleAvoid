#include "CXLib.h"
#include "Screen.h"
#include "ScreenManager.h"

namespace CXLib
{
	// 矩形描画関数
	void DrawBox(Vector2 boundsMin, Vector2 boundsMax, Color Color, bool FillFlag, const Matrix3& world)
	{
		// ワールド座標をコンソール座標に変換
		Matrix3 matrix = (world * ScreenToConsole);
		Vector2 c1 = boundsMin * matrix;
		Vector2 c2 = boundsMax * matrix;
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
	void DrawOval(Vector2 center, Vector2 size, Color Color, bool FillFlag, const Matrix3& world)
	{
		// ワールド座標をコンソール座標に変換
		Matrix3 matrix = (world * ScreenToConsole);
		Vector2 c = center * matrix;
		Vector2 cr = size * matrix - Vector2::zero * matrix;
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
	void DrawCircle(Vector2 center, float r, Color Color, bool FillFlag, const Matrix3& world)
	{
		// 楕円描画
		DrawOval(center, Vector2::one * r, Color, FillFlag, world);
	}

	// 線描画関数
	void DrawLine(Vector2 start, Vector2 end, Color Color, const Matrix3& world)
	{
		// ワールド座標をコンソール座標に変換
		Matrix3 matrix = (world * ScreenToConsole);
		Vector2 c1 = start * matrix;
		Vector2 c2 = end * matrix;
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
	void DrawDashedLine(Vector2 start, Vector2 end, Color Color, float length, const Matrix3& world)
	{
		// ワールド座標をコンソール座標に変換
		Matrix3 matrix = (world * ScreenToConsole);
		Vector2 c1 = start * matrix;
		Vector2 c2 = end * matrix;
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

	// ウィンドウサイズ取得
	Vector2 GetWindowSize()
	{
		static auto& context = ScreenManager::GetInstance().GetOutputContext();
		return Vector2{ context.boundsMax }*ConsoleToScreen;
	}
}