#include "ILS_LoggerStream.h"

TLoggerStream::TLoggerStream(const ILogger* pLogger, TFuncPtr pFunc)
	: m_pLogger(pLogger)
	, m_pFunc(pFunc)
{
}

TLoggerStream::TLoggerStream(const ILogger* pLogger, TFuncPtr pFunc, const char* sect)
	: m_pLogger(pLogger)
	, m_pFunc(pFunc)
	, m_sSectId(sect)
{
}

TLoggerStream::TLoggerStream(const ILogger* pLogger, TFuncPtr pFunc, const char* sect, unsigned int ind)
	: m_pLogger(pLogger)
	, m_pFunc(pFunc)
	, m_sSectId(sect + std::to_string(ind))
{
}

const TLoggerStream& TLoggerStream::operator()(const LogId& id, const char* msg, ...) const {
	unsigned int max_msg_size = 1024;
	char* str = new char[max_msg_size];
	char* buf = NULL; // �������������� ������, ����� ����������, � ����� ���
	try {
		va_list marker;
		// ��� ����������� ��������� ���� "�����" ������������ ����������� ���� %t, ��� ����� ������ ��������� ��� � %f
		// ���� ����� ���������� ���������� ������ msg � ��������� ������������� ������ buf
		const char* ct = strstr(msg, "%t");
		if (ct) {
			buf = new char[strlen(msg) + 1];
			strcpy(buf, msg);
			char* t = strstr(buf, "%t");
			while (t) {
				t[1] = 'f';
				t = strstr(buf, "%t");
			}
			va_start(marker, msg);
			vsnprintf(str, max_msg_size, buf, marker);
			va_end(marker);
		}
		else {
			va_start(marker, msg);
			vsnprintf(str, max_msg_size, msg, marker);
			va_end(marker);
		}
		out << str;
		//			logOut(str,id);
	}
	catch (...) {}
	delete[] str;
	if (buf != NULL) delete[] buf;
	return *this;
}

const TLoggerStream& TLoggerStream::SectBegin(const char* msg, ...) const {
	unsigned int max_msg_size = 1024;
	char* str = new char[max_msg_size];
	char* buf = NULL; // �������������� ������, ����� ����������, � ����� ���
	try {
		out << "SectionBegin " << m_sSectId << " ";
		va_list marker;
		// ��� ����������� ��������� ���� "�����" ������������ ����������� ���� %t, ��� ����� ������ ��������� ��� � %f
		// ���� ����� ���������� ���������� ������ msg � ��������� ������������� ������ buf
		const char* ct = strstr(msg, "%t");
		if (ct) {
			buf = new char[strlen(msg) + 1];
			strcpy(buf, msg);
			char* t = strstr(buf, "%t");
			while (t) {
				t[1] = 'f';
				t = strstr(buf, "%t");
			}
			va_start(marker, msg);
			vsnprintf(str, max_msg_size, buf, marker);
			va_end(marker);
		}
		else {
			va_start(marker, msg);
			vsnprintf(str, max_msg_size, msg, marker);
			va_end(marker);
		}
		out << str;
		//			logOut(str,id);
	}
	catch (...) {}
	delete[] str;
	if (buf != NULL) delete[] buf;
	return *this;
}

void TLoggerStream::SectCheck(const char* sect) const {
	if (m_sSectId != sect && m_pLogger) {
		m_pLogger->errOut("��������� ��������� ������ " + m_sSectId + " ������ ��������� " + sect, id);
	}
}

void TLoggerStream::SectCheck(const char* sect, unsigned int ind) const {
	if (m_sSectId != (sect + std::to_string(ind)) && m_pLogger) {
		m_pLogger->errOut("��������� ��������� ������ " + m_sSectId + " ������ ��������� " + (sect + std::to_string(ind)), id);
	}
}

const TLoggerStream& TLoggerStream::SectEnd(const char* msg, ...) const {
	unsigned int max_msg_size = 1024;
	char* str = new char[max_msg_size];
	char* buf = NULL; // �������������� ������, ����� ����������, � ����� ���
	try {
		out << "SectionEnd " << m_sSectId << " ";
		va_list marker;
		// ��� ����������� ��������� ���� "�����" ������������ ����������� ���� %t, ��� ����� ������ ��������� ��� � %f
		// ���� ����� ���������� ���������� ������ msg � ��������� ������������� ������ buf
		const char* ct = strstr(msg, "%t");
		if (ct) {
			buf = new char[strlen(msg) + 1];
			strcpy(buf, msg);
			char* t = strstr(buf, "%t");
			while (t) {
				t[1] = 'f';
				t = strstr(buf, "%t");
			}
			va_start(marker, msg);
			vsnprintf(str, max_msg_size, buf, marker);
			va_end(marker);
		}
		else {
			va_start(marker, msg);
			vsnprintf(str, max_msg_size, msg, marker);
			va_end(marker);
		}
		out << str;
		//			logOut(str,id);
	}
	catch (...) {}
	delete[] str;
	if (buf != NULL) delete[] buf;
	m_sSectId = "";
	return *this;
}

const char* TLoggerStream::SectId() const {
	return m_sSectId.c_str();
}

void TLoggerStream::Flush() const {
	(m_pLogger->*m_pFunc)(out.str(), id);
	out.str("");
}

TLoggerStream::~TLoggerStream() {
	if (m_sSectId != "") {
		// ���� m_sSectId!="" ������� ��� �� ���� ������, �� �� ���������, ����������� ��������
		out << "SectionEnd " << m_sSectId << " ";
	}
	else {
		(m_pLogger->*m_pFunc)(out.str(), id);
	}
}