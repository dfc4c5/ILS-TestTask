#include <regex>
#include "ILS_LoggerStream.h"

namespace {
	constexpr auto EMPTY = "";
	constexpr auto WITE_SPACE = " ";

	template<typename... ARGS>
	void FormatMsg(std::ostringstream& out, const char* msg, ARGS... args) {
		auto buf = std::regex_replace(msg, std::regex("%t"), "%f");
		auto size = vsnprintf(nullptr, 0, msg, args ...) + 1;
		if (size)
		{
			std::string res(size, '\0');
			vsnprintf(&res[0], size, buf.data(), args...);
			out << res;
		}
	}
}

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


const TLoggerStream& TLoggerStream::operator()(const TLogId& id, const char* msg, ...) const {
	va_list args;
	va_start(args, msg);
	FormatMsg(m_out, msg, args);
	va_end(args);
	return *this;
} 

const TLoggerStream& TLoggerStream::SectBegin(const char* msg, ...) const {
	m_out << "SectionBegin" << WITE_SPACE << m_sSectId << WITE_SPACE;
	va_list args;
	va_start(args, msg);
	FormatMsg(m_out, msg, args);
	va_end(args);
	return *this;
}

void TLoggerStream::SectCheck(const char* sect) const {
	if (m_sSectId != sect && m_pLogger) {
		m_pLogger->errOut("Ожидается окончание секции " + m_sSectId + " вместо указанной " + sect, m_id);
	}
}

void TLoggerStream::SectCheck(const char* sect, unsigned int ind) const {
	if (m_sSectId != (sect + std::to_string(ind)) && m_pLogger) {
		m_pLogger->errOut("Ожидается окончание секции " + m_sSectId + " вместо указанной " + (sect + std::to_string(ind)), m_id);
	}
}

const TLoggerStream& TLoggerStream::SectEnd(const char* msg, ...) const {
	m_out << "SectionEnd" << WITE_SPACE << m_sSectId << WITE_SPACE;
	va_list args;
	va_start(args, msg);
	FormatMsg(m_out, msg, args);
	va_end(args);
	m_sSectId = EMPTY;
	return *this;
}

const char* TLoggerStream::SectId() const {
	return m_sSectId.c_str();
}

void TLoggerStream::Flush() const {
	std::invoke(m_pFunc, m_pLogger, m_out.str(), m_id);
	m_out.str(EMPTY);
}

TLoggerStream::~TLoggerStream() {
	if (!m_sSectId.empty()) {
		// Если m_sSectId пустая значит она не была начата, но не закончена, заканчиваем насильно
		m_out << "SectionEnd" << WITE_SPACE << m_sSectId << WITE_SPACE;
		return;
	}
	else {
		std::invoke(m_pFunc, m_pLogger, m_out.str(), m_id);
	}
}
