#ifndef RAM_MATRIX_PROXY
#define RAM_MATRIX_PROXY

#include "MatrixProxy.h"
#include <stdexcept>
#include <iostream>
#include <codecvt>
#include <locale>

class RAMMatrixProxy : public MatrixProxy {
private:
	char16_t** m_data;
public:
	RAMMatrixProxy(uint8_t nRows, uint8_t nColumns, char16_t defaultChar);
	virtual ~RAMMatrixProxy();

	virtual char16_t get(uint8_t row, uint8_t column) const;
	virtual void set(uint8_t row, uint8_t column, char16_t val);

	friend std::ostream& operator<<(std::ostream& os, const RAMMatrixProxy& matrix);
};

#endif
