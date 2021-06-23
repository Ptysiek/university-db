#pragma once
#include <vector>
#include <iostream>
#include <stdlib.h>

#include "StudentModel.hpp"

struct Gui {
	void show() const;
	char input() const;
	void log(const std::string& log);

	std::string takeCredential(const std::string& title) const;
	StudentModel::DataForm takeStudentCredentials() const;

private:
	std::string credentialsInput() const;
	void showFillInStudentForm() const;
	void showCredentials(const StudentModel::DataForm& form, size_t index) const;
	void showMainMenu() const;
	void showInput() const;
	void clearConsole() const;
	
	std::string separator() const;
	std::string listStudentFields() const;
};