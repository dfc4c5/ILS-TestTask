#include <iostream>
#include "ILS_BaseLogger.h"

// BaseLogger - ���������� ���������� �������� ���������� Logger-�. 
// ������������� ������������� ������ �� ����� ������ ��� ����������� 
// ������������ ��������� ����������� ���� ��������.
//-----------------------------------------------------------------------------
// �����������
BaseLogger::BaseLogger() {
	// �� ��������� ������ �� ������� � ���������
	show_info = 0;
	// ����� ������
	bStarted = false;
	// ���� ������ ���� � �������
	bLogToConsole = false;
};
//-----------------------------------------------------------------------------
// ������� ����������
// ����������� ��������� ������ �����, ������ ������� ����� ��������:
// id  - ������������� ���������
// msg - ���� ��������� � ������� ������� printf
// ... - ����� ������ ��� ������ � ��������� �� �������� printf
// �������:
// ����������� ��������������� ���������
void BaseLogger::infOut(const Msg& msg, const LogId& id) const {
	ConsoleOut(iTitle() + msg);
}
//-----------------------------------------------------------------------------
// ������� ����������
// ����������� ��������� ������ �����, ������ ������� ����� ��������:
// id  - ������������� ���������
// msg - ���� ��������� � ������� ������� printf
// ... - ����� ������ ��� ������ � ��������� �� �������� printf
// �������:
// ����������� ��������������� ���������
void BaseLogger::logOut(const Msg& msg, const LogId& id) const {
	ConsoleOut(lTitle() + msg);
}
// ����������� �������������� (warning) � �� ��������� ������
void BaseLogger::wrnOut(const Msg& msg, const LogId& id) const {
	++warnings;
	ConsoleOut(wTitle() + msg);
}
// ����������� ��������� ������, 
// ����� ������� ���������� �������� �� ����������
void BaseLogger::errOut(const Msg& msg, const LogId& id) const {
	++errors;
	ConsoleOut(eTitle() + msg);
}

void BaseLogger::ConsoleOut(const std::string& msg) const {
	if (bLogToConsole)
		std::cout << msg << std::endl;
}

//-----------------------------------------------------------------------------
// ��������������� �������
// ��������� ����� ��� ���� ����� ��������� ������ � ����������
// �� ������ �������� ��������
std::string BaseLogger::title() const {
	std::string res;
	char s[128];
	time_t ltime;
	time(&ltime);
	struct tm* today;
	today = localtime(&ltime);
	if (show_info & 1) {
		strftime(s, 128, "%Y/%m/%d ", today);
		res += s;
	}
	if ((show_info & 2) || ((show_info & 4) && (!bStarted))) {
		strftime(s, 128, "%H:%M:%S ", today);
		res += s;
	}
	if (!bStarted) {
		start_time = std::chrono::steady_clock::now();
		bStarted = true;
	}
	else if (show_info & 4) {
		long fDurationMilli = long(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count());
		sprintf(s, "% 8.2f ", double(fDurationMilli) / 1000.0);
		res += s;
	}
	return res;
}
// ��� ������� ���� ��������� �������� ��������� �������, ������� 
// ����������� �� ������ �����
std::string BaseLogger::lTitle() const {
	return title() + "> ";
}
std::string BaseLogger::iTitle() const {
	return title() + "|INFO> ";
}
std::string BaseLogger::wTitle() const {
	return title() + "|WARNING> ";
}
std::string BaseLogger::eTitle() const {
	return title() + "|ERROR> ";
}
