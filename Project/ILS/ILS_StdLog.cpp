#include <fstream>
#include <string>
#include "ILS_StdLog.h"

StdLogger::StdLogger(std::ostream& l_out /*= std::cout*/,
	std::ostream& w_out /*= std::cerr*/,
	std::ostream& e_out /*= std::cerr*/) {
	log_out = &l_out; l_del = false;
	wrn_out = &w_out; w_del = false;
	err_out = &e_out; e_del = false;
	BaseLogger::onLogStart(true, wrn_out != log_out, err_out != wrn_out && err_out != log_out);
}

StdLogger::StdLogger(std::string l_out_file,
	std::ostream& w_out /*= std::cerr*/,
	std::ostream& e_out /*= std::cerr*/,
	std::ios_base::openmode mode /*= std::ios_base::out*/) {
	using namespace std;
	log_out = NULL; l_del = false;;
	if (l_out_file != "") {
		std::ofstream* log_stream = new std::ofstream(l_out_file.c_str(), mode | std::ios_base::out);
		if (*log_stream) { log_out = log_stream; l_del = true; }
		else delete log_stream;
	}
	wrn_out = &w_out; w_del = false;
	err_out = &e_out; e_del = false;
	BaseLogger::onLogStart(true, wrn_out != log_out, err_out != wrn_out && err_out != log_out);
}

StdLogger::StdLogger(std::string l_out_file,
	std::string w_out_file,
	std::ostream& e_out /*= std::cerr*/,
	std::ios_base::openmode mode /*= std::ios_base::out*/) {
	using namespace std;
	log_out = NULL; l_del = false;;
	if (l_out_file != "") {
		std::ofstream* log_stream = new std::ofstream(l_out_file.c_str(), mode | std::ios_base::out);
		if (*log_stream) { log_out = log_stream; l_del = true; }
		else delete log_stream;
	}
	wrn_out = NULL; w_del = false;;
	if (w_out_file != "") {
		std::ofstream* wrn_stream = new std::ofstream(w_out_file.c_str(), mode | std::ios_base::out);
		if (*wrn_stream) { wrn_out = wrn_stream; w_del = true; }
		else delete wrn_stream;
	}
	err_out = &e_out; e_del = false;
	BaseLogger::onLogStart(true, wrn_out != log_out, err_out != wrn_out && err_out != log_out);
}

StdLogger::StdLogger(std::string l_out_file,
	std::string w_out_file,
	std::string e_out_file,
	std::ios_base::openmode mode /*= std::ios_base::out*/) {
	using namespace std;
	log_out = NULL; l_del = false;;
	if (l_out_file != "") {
		std::ofstream* log_stream = new std::ofstream(l_out_file.c_str(), mode | std::ios_base::out);
		if (*log_stream) { log_out = log_stream; l_del = true; }
		else delete log_stream;
	}
	wrn_out = NULL; w_del = false;;
	if (w_out_file == l_out_file && w_out_file != "") {
		wrn_out = log_out;
		w_del = false;
	}
	else if (w_out_file != "") {
		std::ofstream* wrn_stream = new std::ofstream(w_out_file.c_str(), mode | std::ios_base::out);
		if (*wrn_stream) { wrn_out = wrn_stream; w_del = true; }
		else delete wrn_stream;
	}
	err_out = NULL; e_del = false;;
	if (e_out_file == l_out_file && e_out_file != "") {
		err_out = log_out;
		e_del = false;
	}
	else if (e_out_file == w_out_file && e_out_file != "") {
		err_out = wrn_out;
		e_del = false;
	}
	else if (e_out_file != "") {
		std::ofstream* err_stream = new std::ofstream(e_out_file.c_str(), mode | std::ios_base::out);
		if (*err_stream) { err_out = err_stream; e_del = true; }
		else delete err_stream;
	}
	BaseLogger::onLogStart(true, wrn_out != log_out, err_out != wrn_out && err_out != log_out);
}

StdLogger::StdLogger(const StdLogger& src) {
	if (src.l_del) log_out = NULL; // не можем прокопировать, иначе возникнет ситуация когда объект родитель умрет и будем иметь указатель на невалидный объект
	else log_out = src.log_out;
	if (src.w_del) wrn_out = NULL; // не можем прокопировать, иначе возникнет ситуация когда объект родитель умрет и будем иметь указатель на невалидный объект
	else wrn_out = src.wrn_out;
	if (src.e_del) err_out = NULL; // не можем прокопировать, иначе возникнет ситуация когда объект родитель умрет и будем иметь указатель на невалидный объект
	else err_out = src.err_out;
	l_del = false;
	w_del = false;
	e_del = false;
	BaseLogger::onLogStart(true, wrn_out != log_out, err_out != wrn_out && err_out != log_out);
}

StdLogger::~StdLogger() {
	BaseLogger::onLogFinish(true, wrn_out != log_out, err_out != wrn_out && err_out != log_out);
	if (l_del && log_out) delete log_out;
	if (w_del && wrn_out) delete wrn_out;
	if (e_del && err_out) delete err_out;
}

//-----------------------------------------------------------------------------
void StdLogger::infOut(const Msg& msg, const LogId& id) const {
	if (log_out)
		(*log_out) << msg << std::endl;
	ConsoleOut(msg);
}

void StdLogger::logOut(const Msg& msg, const LogId& id) const {
	infOut(msg, id);
}

void StdLogger::wrnOut(const Msg& msg, const LogId& id) const {
	if (wrn_out)
		(*wrn_out) << msg << std::endl;
	ConsoleOut(msg);
}

void StdLogger::errOut(const Msg& msg, const LogId& id) const {
	if (err_out)
		(*err_out) << msg << std::endl;
	ConsoleOut(msg);
}
