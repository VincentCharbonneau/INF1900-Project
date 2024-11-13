#include "RAMMatrixProxy.h"

RAMMatrixProxy::RAMMatrixProxy(uint8_t nRows, uint8_t nColumns, char16_t defaultChar) : MatrixProxy(nRows, nColumns, defaultChar) {
	m_data = new char16_t*[nRows];

	for (uint8_t i = 0; i < nRows; ++i) {
		m_data[i] = new char16_t[nColumns];
	}

	for (uint8_t i = 0; i < nRows; ++i) {
		for (uint8_t j = 0; j < nColumns; ++j) {
			m_data[i][j] = defaultChar;
		}
	}
}

RAMMatrixProxy::~RAMMatrixProxy() {
	for (uint8_t i = 0; i < m_nRows; ++i) {
		delete[] m_data[i];
	}

	delete[] m_data;
}

char16_t RAMMatrixProxy::get(uint8_t row, uint8_t column) const {
	if (row > (m_nRows - 1) || column > (m_nColumns - 1)) {
		throw std::out_of_range("Index out of range!");
	}

	return m_data[row][column];
}

void RAMMatrixProxy::set(uint8_t row, uint8_t column, char16_t val) {
	if (row > m_nRows - 1 || column > m_nColumns - 1) {
		throw std::out_of_range("Index out of range!");
	}

	m_data[row][column] = val;
}

std::string convertChar16(char16_t ch) {
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	return convert.to_bytes(ch);
}

std::ostream& operator<<(std::ostream& os, const RAMMatrixProxy& matrix) {
	for (uint8_t i = 0; i < matrix.m_nRows; ++i) {
		for (uint8_t j = 0; j < matrix.m_nColumns; ++j) {
			os << convertChar16(matrix.m_data[i][j]);
		}
		os << std::endl;
	}
	return os;
}