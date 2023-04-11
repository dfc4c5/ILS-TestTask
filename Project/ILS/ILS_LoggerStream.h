#ifndef ILS_LoggerStreamH
#define ILS_LoggerStreamH

#include <cstring>
#include <sstream>
#include "ILS_Logger.h"

//------------------------------------------------------------------------------
/// Тривиальный класс, для возможности потокового формирования сообщения в макросе ILS_LOG.
class TLoggerStream
{
	using TLogger = ILogger;
	using TMsg = TLogger::Msg;
	using TLogId = TLogger::LogId;
	using TFuncPtr = void (TLogger::*)(const TMsg&, const TLogId&) const;
private: //Переменные-члены
	mutable std::ostringstream m_out;  // Поток для накопления вывода.
	mutable std::string m_sSectId;
	mutable TLogId m_id;
	const TLogger* m_pLogger;
	TFuncPtr m_pFunc;

public:
	/// Конструктор.
	TLoggerStream(const ILogger* pLogger, TFuncPtr pFunc);
	TLoggerStream(const ILogger* pLogger, TFuncPtr pFunc, const char* sect);
	TLoggerStream(const ILogger* pLogger, TFuncPtr pFunc, const char* sect, unsigned int ind);

	/// Функции-члены
	const TLoggerStream& operator()(const TLogId& id, const char* msg, ...) const;
	const TLoggerStream& SectBegin(const char* msg, ...) const;
	void SectCheck(const char* sect) const;
	void SectCheck(const char* sect, unsigned int ind) const;
	const TLoggerStream& SectEnd(const char* msg, ...) const;
	const char* SectId() const;
	void Flush() const;

	/// Вывод в поток.
	template<class T>
	inline const TLoggerStream& operator<<(const T& t) const {
		m_out << t;
		return *this;
	}

	/// Деструктор
	~TLoggerStream();
}; // class TLoggerStream

#endif  // ILS_LoggerStreamH
