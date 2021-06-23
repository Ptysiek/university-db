#include "StudentBuilder.hpp"

StudentBuilder::StudentBuilder(const Debug& debug): 
	debug_(debug),
	completed_(false),
	data_(),
	index_(),
	indexString_(),
	pesel_(),
	lastName_()
{}

StudentModel StudentBuilder::getStudent() const {
	if (completed_) {
		return StudentModel(data_);
	}
	throw std::logic_error("StudentBuilder::getStudent(): Uncompleted Product");
}

unsigned long StudentBuilder::getIndex() const {
	if (completed_) {
		return index_;
	}
	throw std::logic_error("StudentBuilder::getIndex(): Uncompleted Product");
}

std::string StudentBuilder::getPesel() const {
	if (completed_) {
		return pesel_;
	}
	throw std::logic_error("StudentBuilder::getPesel(): Uncompleted Product");
}

std::string StudentBuilder::getLastName() const {
	if (completed_) {
		return lastName_;
	}
	throw std::logic_error("StudentBuilder::getLastName(): Uncompleted Product");
}

bool StudentBuilder::buildStudent(const StudentModel::DataForm& form) {
	clear();
	data_ = form;
	if (!extractFields()) {
		return false;
	}
	completed_ = verify(); 
	if (completed_) {
		index_ = to_unlong(indexString_);
	}
	return completed_;
}

bool StudentBuilder::buildStudent(const std::string& studentData) {
	clear();
	if (!extractForm(std::stringstream(studentData))) {
		return false;
	}
	if (!extractFields()) {
		return false;
	}
	completed_ = verify(); 
	if (completed_) {
		index_ = to_unlong(indexString_);
	} 
	return completed_;
}

unsigned long StudentBuilder::to_unlong(const std::string& str) const {
	return std::stoul(str);
}

void StudentBuilder::clear() {
	completed_ = false;
	index_ = long();
	indexString_ = std::string();
	pesel_ = std::string();
	lastName_ = std::string();
}

bool StudentBuilder::extractForm(std::stringstream&& studentData) {
	for (auto& record : data_) {
		if (!(studentData >> record)) {
			return false;
		}
	}
	return true;
}

bool StudentBuilder::extractFields() {
	const StudentModel probableStudent(data_);
	indexString_ = probableStudent.get(StudentModel::Field::Index);
	lastName_ = probableStudent.get(StudentModel::Field::LastName);
	pesel_ = probableStudent.get(StudentModel::Field::Pesel);
	return true;
}

bool StudentBuilder::verify() const {
	if (!verifyEmptyField()) {
		return false;
	}
	if (!verifySpaces()) {
		return false;
	}
	if (!verifyIndexFormat(indexString_)) {
		return false;
	}
	if (!verifyPeselFormat(pesel_)) {
		return false;
	}
	return true;
}

bool StudentBuilder::verifyEmptyField() const {
	for (const auto& field : data_) {
		if (field.empty()) {
			debug_ << "empty field given";
			return false;
		}
	}	
	return true;	
}

bool StudentBuilder::verifySpaces() const {
	for (const auto& field : data_) {
		if (field.find(" ") != std::string::npos) {
			debug_ << "given field has space";
			return false;
		}
	}	
	return true;
}

bool StudentBuilder::verifyIndexFormat(const std::string& index) const {
	if (index.find("-") != std::string::npos) {
		debug_ << "index out of range: {" + index + "}";
		return false;
	}
	try {
		to_unlong(index);
	}
	catch(const std::invalid_argument& e) {
		debug_ << "invalid index format: {" + index + "}";
		debug_ << e.what();
		return false;
	}
	catch(const std::out_of_range& e) {
		debug_ << "index out of range: {" + index + "}";
		debug_ << e.what();
		return false;
	}
	return true;
}		

bool StudentBuilder::verifyPeselFormat(const std::string& pesel) const {
	if (pesel.length() != 11) {
		debug_ << "wrong pesel lenght: " + std::to_string(pesel.length());
		return false; 
	}
	auto peselVctr = breakPeselIntoPieces(pesel);
	if (peselVctr.empty()) {
		debug_ << "wrong pesel format: " + pesel;
		return false;
	}
	if (auto checkDigit = calculatePeselCheckDigit(peselVctr); checkDigit != 0) {
		debug_ << "wrong pesel check digit: " + checkDigit;
		return false;
	}
	return true;
}

std::vector<int> StudentBuilder::breakPeselIntoPieces(const std::string& pesel) const {
	std::vector<int> result;
	result.reserve(pesel.length());
	for (const auto character : pesel) {
		if (character < '0' || character > '9') {
			return {};
		} 			
		result.emplace_back(character - '0');
	}
	return result;
}

int StudentBuilder::calculatePeselCheckDigit(const std::vector<int>& peselVctr) const {
	int result = peselVctr.at(0) * 1;
	result += peselVctr.at(1) * 3;
	result += peselVctr.at(2) * 7;
	result += peselVctr.at(3) * 9;

	result += peselVctr.at(4) * 1;
	result += peselVctr.at(5) * 3;
	result += peselVctr.at(6) * 7;
	result += peselVctr.at(7) * 9;

	result += peselVctr.at(8) * 1;
	result += peselVctr.at(9) * 3;
	result += peselVctr.at(10) * 1;
	return result;
}