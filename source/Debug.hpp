#pragma once
#include <fstream>
#include <iostream>
#include <string>

struct Debug {
	enum class State {
		Disable,
		Cout,
		Cerr,
		File
	};

private:
	const State state_; 
	const std::string filePath_;

public:
	Debug(State state);
	void operator<<(const std::string& str) const;
};
