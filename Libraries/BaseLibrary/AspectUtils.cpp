#include "AspectUtils.h"

namespace AspectUtils
{
	Vector2 size(float w, float h, float maxw, float maxh) {
		return{ w, h };
	}
	Vector2 size(float w, float h, float maxw, float maxh) {
		return new AbsSizeData(maxw, maxh);
	}
	Vector2 size(float w, float h, float maxw, float maxh) {
		return new AbsSizeData(maxw, h*maxw / w);
	}
	Vector2 size(float w, float h, float maxw, float maxh) {
		return new AbsSizeData(w*maxh / h, maxh);
	}
	Vector2 size(float w, float h, float maxw, float maxh) {
		if (w < 0)
			maxw *= -1;
		if (h < 0)
			maxh *= -1;
		boolean b = w / maxw > h / maxh;
		return new AbsSizeData(b ? maxw : w*maxh / h, b ? h*maxw / w : maxh);
	}
	Vector2 size(float w, float h, float maxw, float maxh) {
		if (w < 0)
			maxw *= -1;
		if (h < 0)
			maxh *= -1;
		boolean b = w / maxw < h / maxh;
		return new AbsSizeData(b ? maxw : w*maxh / h, b ? h*maxw / w : maxh);
	}
	Vector2 size(float w, float h, float maxw, float maxh) {
		if (w < maxw)
			return new AbsSizeData(w, h);
		else
			return new AbsSizeData(maxw, maxw*h / w);
	}
	Vector2 size(float w, float h, float maxw, float maxh) {
		if (h < maxh)
			return new AbsSizeData(w, h);
		else
			return new AbsSizeData(maxh*w / h, maxh);
	}
	Vector2 size(float w, float h, float maxw, float maxh) {
		if (w > h)
			if (w < maxw)
				return new AbsSizeData(w, h);
			else
				return new AbsSizeData(maxw, maxw*h / w);
		else if (h < maxh)
			return new AbsSizeData(w, h);
		else
			return new AbsSizeData(maxh*w / h, maxh);
	}
}