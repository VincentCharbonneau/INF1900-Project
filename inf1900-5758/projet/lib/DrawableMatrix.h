#pragma once
#include "AbstractDrawable.h"

class DrawableMatrix :
    public AbstractDrawable
{
    public:
	DrawableMatrix(MatrixProxy& matrix);

	virtual void drawOnto(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const override;

	private:
		MatrixProxy& m_matrix;
};

