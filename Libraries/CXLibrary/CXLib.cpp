#include "CXLib.h"
#include "Screen.h"
#include "ScreenManager.h"

namespace CXLib
{
	// ��`�`��֐�
	void DrawBox(Vector2 boundsMin, Vector2 boundsMax, Color Color, bool FillFlag, const Matrix3& world)
	{
		// ���[���h���W���R���\�[�����W�ɕϊ�
		Matrix3 matrix = (world * ScreenToConsole);
		Vector2 c1 = boundsMin * matrix;
		Vector2 c2 = boundsMax * matrix;
		SHORT cx1 = c1.Xs();
		SHORT cy1 = c1.Ys();
		SHORT cx2 = c2.Xs();
		SHORT cy2 = c2.Ys();

		// Y���[�v
		SHORT ix, iy;
		for (iy = cy1; iy <= cy2; iy++)
		{
			// X���[�v
			for (ix = cx1; ix <= cx2; ix++)
			{
				// �h��Ԃ����A�܂��͉��̏ꍇ�`��
				if (FillFlag || (ix == cx1 || ix == cx2) || (iy == cy1 || iy == cy2))
					Screen::Draw({ ix, iy }, Color);
			}
		}
	}

	// �ȉ~�`��֐�
	void DrawOval(Vector2 center, Vector2 size, Color Color, bool FillFlag, const Matrix3& world)
	{
		// ���[���h���W���R���\�[�����W�ɕϊ�
		Matrix3 matrix = (world * ScreenToConsole);
		Vector2 c = center * matrix;
		Vector2 cr = size * matrix - Vector2::zero * matrix;
		SHORT cx = c.Xs();
		SHORT cy = c.Ys();
		SHORT crx = cr.Xs();
		SHORT cry = cr.Ys();

		// Y���[�v
		SHORT ix, iy;
		for (iy = -cry; iy <= cry; iy++)
		{
			// X���[�v
			for (ix = -crx; ix <= crx; ix++)
			{
				// �h��Ԃ����A�܂��͉��̏ꍇ�`��
				float p = (ix*ix) / (cr.x*cr.x) + (iy*iy) / (cr.y*cr.y) - 1;
				if (FillFlag ? p <= .08f : -.25f < p && p < .08f)
					Screen::Draw({ cx + ix, cy + iy }, Color);
			}
		}
	}

	// �~�`��֐�
	void DrawCircle(Vector2 center, float r, Color Color, bool FillFlag, const Matrix3& world)
	{
		// �ȉ~�`��
		DrawOval(center, Vector2::one * r, Color, FillFlag, world);
	}

	// ���`��֐�
	void DrawLine(Vector2 start, Vector2 end, Color Color, const Matrix3& world)
	{
		// ���[���h���W���R���\�[�����W�ɕϊ�
		Matrix3 matrix = (world * ScreenToConsole);
		Vector2 c1 = start * matrix;
		Vector2 c2 = end * matrix;
		SHORT cx1 = c1.Xs();
		SHORT cy1 = c1.Ys();
		SHORT cx2 = c2.Xs();
		SHORT cy2 = c2.Ys();

		// �c�A���A�ǂ���̔䂪�傫����
		bool steep = std::abs(cy2 - cy1) > std::abs(cx2 - cx1);
		// �c���傫��������c�����]
		if (steep)
		{
			std::swap(cx1, cy1);
			std::swap(cx2, cy2);
		}

		{
			// �����擾
			SHORT deltax = std::abs(cx2 - cx1);
			SHORT deltay = std::abs(cy2 - cy1);
			// �Y�����v�Z
			SHORT error = deltax / 2;

			// �C���N�������g
			SHORT incx = (cx1 < cx2) ? 1 : -1;
			SHORT incy = (cy1 < cy2) ? 1 : -1;

			// X���[�v
			SHORT x;
			SHORT y = cy1;
			for (x = cx1; x != cx2; x += incx)
			{
				// �`��
				Screen::Draw({ steep ? y : x, steep ? x : y }, Color);

				// �Y�����C��&�`�F�b�N
				if ((error -= deltay) < 0)
				{
					y += incy;
					error += deltax;
				}
			}
		}
	}

	// �j����`��
	void DrawDashedLine(Vector2 start, Vector2 end, Color Color, float length, const Matrix3& world)
	{
		// ���[���h���W���R���\�[�����W�ɕϊ�
		Matrix3 matrix = (world * ScreenToConsole);
		Vector2 c1 = start * matrix;
		Vector2 c2 = end * matrix;
		SHORT cx1 = c1.Xs();
		SHORT cy1 = c1.Ys();
		SHORT cx2 = c2.Xs();
		SHORT cy2 = c2.Ys();
		float dx = (end - start).Normalized().x * length;

		// �c�A���A�ǂ���̔䂪�傫����
		bool steep = std::abs(cy2 - cy1) > std::abs(cx2 - cx1);
		// �c���傫��������c�����]
		if (steep)
		{
			std::swap(cx1, cy1);
			std::swap(cx2, cy2);
		}

		{
			// �����擾
			SHORT deltax = std::abs(cx2 - cx1);
			SHORT deltay = std::abs(cy2 - cy1);
			// �Y�����v�Z
			SHORT error = deltax / 2;

			// �C���N�������g
			SHORT incx = (cx1 < cx2) ? 1 : -1;
			SHORT incy = (cy1 < cy2) ? 1 : -1;

			// X���[�v
			SHORT x;
			SHORT y = cy1;
			for (x = cx1; x != cx2; x += incx)
			{
				if (static_cast<SHORT>(x / dx) % 2 == 0)
					// �`��
					Screen::Draw({ steep ? y : x, steep ? x : y }, Color);

				// �Y�����C��&�`�F�b�N
				if ((error -= deltay) < 0)
				{
					y += incy;
					error += deltax;
				}
			}
		}
	}

	// �E�B���h�E�T�C�Y�擾
	Vector2 GetWindowSize()
	{
		static auto& context = ScreenManager::GetInstance().GetOutputContext();
		return Vector2{ context.boundsMax }*ConsoleToScreen;
	}
}