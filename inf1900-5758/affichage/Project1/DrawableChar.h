#ifndef DRAWABLE_CHAR
#define DRAWABLE_CHAR

#include "AbstractDrawable.h"

class DrawableChar : public AbstractDrawable {
public:
	DrawableChar(const char16_t character);
	virtual ~DrawableChar() = default;
	virtual void drawOnto(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const ;
private:
	const char16_t m_character;
};

#endif