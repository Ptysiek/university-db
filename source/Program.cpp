#include "Program.hpp"

Program::Program():
	debug_(Debug::State::Cout),
	dataBase_(debug_),
	gui_(),
	rerender_(true)
{}

void Program::execute() {
	while (true) {
		if (rerender_) {
			gui_.show();
		}
		rerender_ = true;
		auto input = gui_.input();
		
		if (input == '1') {
			addStudent();
		}
		else if (input == '2') {
			removeStudent();
		}
		else if (input == '3') {
			findByName();
		}
		else if (input == '4') {
			findByPesel();
		}
		else if (input == '5') {
			findByIndex();
		}
		else if (input == '6') {
			showStudents(0);
		}
		else if (input == '7') {
			showStudents(1);
		}
		else if (input == '8') {
			showStudents(2);
		}
		else if (input == '9') {
			return;
		}
		else if (input == 'o') {
			databaseOperation();
			databaseOpen();
		}
		else if (input == 'a') {
			databaseOperation();
			databaseAppend();
		}
		else if (input == 's') {
			databaseOperation();
			databaseSave();
		}
	}
}

void Program::databaseOperation() {
	rerender_ = false;
	gui_.show();
	gui_.log("");
}

void Program::databaseOpen() {
	auto value = gui_.takeCredential("Enter The Name Of THe DataBase To Open:");
	if (value == "-1") {
		return;
	}
	gui_.log("* DataBase Says...  ");
	if (dataBase_.open(value)) {
		gui_.log("* It's Cool");
	}
	else {
		gui_.log("* DataBase Actualy Don't Like The Idea");
	}	
}

void Program::databaseAppend() {
	auto value = gui_.takeCredential("Enter The Name Of THe DataBase To Append Current One:");
	if (value == "-1") {
		return;
	}
	gui_.log("* DataBase Says...  ");
	if (dataBase_.append(value)) {
		gui_.log("* It's Cool");
	}
	else {
		gui_.log("* DataBase Actualy Don't Like The Idea");
	}
}

void Program::databaseSave() {
	auto value = gui_.takeCredential("Enter New Name To Save Current DataBase:");
	if (value == "-1") {
		return;
	}
	gui_.log("* DataBase Says...  ");
	if (dataBase_.save(value)) {
		gui_.log("* It's Cool");
	}
	else {
		gui_.log("* DataBase Actualy Don't Like The Idea");
	}
}

void Program::addStudent() {
	auto studentForm = gui_.takeStudentCredentials();
	rerender_ = false;

	bool isFormEmpty = false;
	for (const auto& field : studentForm) {
		if (field.empty()) {
			isFormEmpty = true;
			break;
		}
	}
	gui_.show();
	gui_.log("");
	if (isFormEmpty) {
		gui_.log("* Operation Canceled");
		gui_.log("");
		return;
	}
	gui_.log("* Sending Student Form To DataBase");
	gui_.log("* DataBase Says...  ");
	if (dataBase_.addStudent(studentForm)) {
		gui_.log("* It's Cool");
	}
	else {
		gui_.log("* DataBase Actualy Don't Like The Idea");
	}	
}

void Program::removeStudent() {
	rerender_ = false;
	gui_.show();
	gui_.log("");
	auto value = gui_.takeCredential("Enter The Id Of The Student To Remove:");
	if (value == "-1") {
		return;
	}		
	gui_.log("* Asking DataBase If We Can Remove Student");
	gui_.log("* DataBase Says...  ");
	if (dataBase_.removeStudent(value)) {
		gui_.log("* It's Cool");
	}
	else {
		gui_.log("* DataBase Actualy Don't Like The Idea");
	}	
}

void Program::showStudents(const size_t id) {
	rerender_ = false;
	gui_.show();
	gui_.log("");
	if (dataBase_.data().empty()) {
		gui_.log("* DataBase Empty");
		return;
	}
	if (id == 0) {
		std::string size = std::to_string(dataBase_.data().size());
		gui_.log("* DataBase Name Sorted: [elements number: " + size + "]");
		for (const auto& iterator : dataBase_.dataLastNameSorted()) {
			for (const auto& student : iterator.second) {
				gui_.log(student->toString());
			}
		}
		return;
	}
	if (id == 1) {
		std::string size = std::to_string(dataBase_.dataPeselSorted().size());
		gui_.log("* DataBase Pesel Sorted: [elements number: " + size + "]");
		for (const auto& iterator : dataBase_.dataPeselSorted()) {
			gui_.log(iterator.second->toString());
		}
		return;
	}
	if (id == 2) {
		std::string size = std::to_string(dataBase_.data().size());
		gui_.log("* DataBase Index Sorted: [elements number: " + size + "]");
		for (const auto& iterator : dataBase_.data()) {
			gui_.log(iterator.second.toString());
		}
	}
}

void Program::findByName() {
	rerender_ = false;
	gui_.show();
	auto value = gui_.takeCredential("Find Student By Second Name:");
	auto it = dataBase_.dataLastNameSorted().find(value);
	if (it == dataBase_.dataLastNameSorted().end()) {
		gui_.log("* Value Not Found In The DataBase: " + value);
		return;
	}
	if (it->second.empty()) {
		gui_.log("* Value Not Found In The DataBase: " + value);
		return;
	}
	for (const auto& student : it->second) {
		gui_.log(student->toString());
	}
}

void Program::findByIndex() {
	rerender_ = false;
	gui_.show();
	auto valueStr = gui_.takeCredential("Find Student By Second Name:");
	unsigned long value;
	try {
		value = std::stoul(valueStr);
	}
	catch(...) {
		gui_.log("* Wrong Value: " + valueStr);
		return;
	}
	auto it = dataBase_.data().find(value);
	if (it == dataBase_.data().end()) {
		gui_.log("* Value Not Found In The DataBase: " + valueStr);
		return;
	}
	gui_.log(it->second.toString());
}

void Program::findByPesel() {
	rerender_ = false;
	gui_.show();
	auto value = gui_.takeCredential("Find Student By Second Name:");
	auto it = dataBase_.dataPeselSorted().find(value);
	if (it == dataBase_.dataPeselSorted().end()) {
		gui_.log("* Value Not Found In The DataBase: " + value);
		return;
	}
	gui_.log(it->second->toString());
}
