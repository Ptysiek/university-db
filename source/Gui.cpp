#include "Gui.hpp"

void Gui::show() const {
	clearConsole();
	showMainMenu();
}

char Gui::input() const {
	showInput();
	std::string input;
	std::cin >> input;
	return (input.empty())? '0' : input[0];
}

void Gui::log(const std::string& log) {
	std::cout << "\n " << log;
}

std::string Gui::takeCredential(const std::string& title) const {
	show();
	std::cout << "\n" << separator()
		<< "\n " << title
		<< "\n $ ";
	return credentialsInput();
}

StudentModel::DataForm Gui::takeStudentCredentials() const {
	StudentModel::DataForm result;
	for (size_t index = 0; index < result.size(); ) {
		show();
		showFillInStudentForm();
		showCredentials(result, index);
		auto credential = credentialsInput();
		if (credential == "-1") {
			return {};
		}
		if (auto index = credential.find(" "); index != std::string::npos) {
			credential = credential.substr(0, index);
		}
		if (!credential.empty()) {
			result.at(index) = credential;
			++index;
		}
	}
	return result;
}

std::string Gui::credentialsInput() const {
	std::string input;
	std::cin >> input;
	return input;
}

void Gui::showFillInStudentForm() const {
	std::cout << "\n" << separator()
		<< "\n - Fill In Student Form:" 
		<< "\n   {" << listStudentFields() << "}"
		<< "\n - Or type \"-1\" to quit"
		<< "\n";
}

void Gui::showCredentials(const StudentModel::DataForm& form, size_t index) const {
	for (size_t i = 0; i <= index; ++i) {
		std::cout << "\n " << i + 1 << "] " 
			<< StudentModel::fieldName(i) << ": ";

		if (auto str = form.at(i); str.empty()) {
			std::cout << str;
		}
		else {
			std::cout << "\"" << str << "\"";
		}
	}
}

void Gui::showMainMenu() const {
	std::cout << separator()
		<< "\n 1] add student"
		<< "\n 2] remove student"
		<< "\n 3] find student by name"
		<< "\n 4] find student by pesel"
		<< "\n 5] find student by index"
		<< "\n 6] show students by name"
		<< "\n 7] show students by pesel"
		<< "\n 8] show students by index"
		<< "\n 9] quit"
		<< "\n"
		<< "\n o] open database from file"
		<< "\n a] append database from file"
		<< "\n s] save current database";
}

void Gui::showInput() const {
	std::cout << "\n"
		<< "\n input: [1;9]"
		<< "\n $ ";
}

void Gui::clearConsole() const {
	std::cout << "\x1B[2J\x1B[H";
}

std::string Gui::separator() const {
	return "___________________________\n";
}

std::string Gui::listStudentFields() const {
	std::string result;
	const size_t size = StudentModel::DataForm().size();
	for (size_t i = 0; i < size; ++i) {
		result += StudentModel::fieldName(i) + ", ";
	}
	if (result.size() > 1) {
		result.pop_back();
		result.pop_back();
	}
	return result;
}
