#include "Debug.hpp"

Debug::Debug(State state):
	state_(state),
	filePath_("UniversityDB-DebugLog.txt")
{}

void Debug::operator<<(const std::string& str) const {
	switch (state_) {
	case State::Disable:
		return;
	case State::Cout:
		std::cout << str << '\n';
		return;
	case State::Cerr:
		std::cerr << str << '\n';
		return;
	case State::File:
		std::ofstream fileStream(filePath_, std::ios::app);
		fileStream << str << '\n';
		fileStream.close();
		return;
	}
}
