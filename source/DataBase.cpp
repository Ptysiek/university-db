#include "DataBase.hpp"

DataBase::DataBase(const Debug& debug):
	debug_(debug),
	builder_(debug),
	data_(),
	dataPeselSorted_(),
	dataLastNameSorted_()
{}

const std::map<unsigned long, StudentModel>& DataBase::data() const {
	return data_;
}

const std::map<std::string, StudentModel*>& DataBase::dataPeselSorted() const {
	return dataPeselSorted_;
}

const std::map<std::string, std::vector<StudentModel*>>& DataBase::dataLastNameSorted() const {
	return dataLastNameSorted_;
}

bool DataBase::open(const std::string& filePath) {
	close();
	return append(filePath);
}

bool DataBase::append(const std::string& filePath) {
	std::ifstream fileStream(filePath);
	if (!fileStream.is_open()) {
		debug_ << "can't open file: " + filePath;
		return false;
	}
	std::string line;
	while (std::getline(fileStream, line)) {
		if (builder_.buildStudent(line)) {	
			addStudent();
		}
		else {
			debug_ << "can't extract student data: {" + line + "}";
		}
	}
	fileStream.close();
	return true;
}

bool DataBase::save(const std::string& filePath) const {
	std::ofstream fileStream(filePath);
	if (!fileStream.is_open()) {
		debug_ << "can't open file: " + filePath;
		return false;
	}	
	for (const auto& user : data_) {
		fileStream << user.second.toString() << "\n";
	}
	fileStream.close();
	return true;
}

void DataBase::close() {
	dataLastNameSorted_.clear();
	dataPeselSorted_.clear();
	data_.clear();
}

bool DataBase::removeStudent(const std::string& id) {
	unsigned long key;
	try {
		key = std::stoul(id);
	}
	catch(...) {
		debug_ << "wrong id format: " + id;
		return false;
	}
	auto it = data_.find(key);
	if (it == data_.end()) {
		debug_ << "user with specified id does not exist: " + id;
		return false;
	}
	auto pesel = it->second.constGet(StudentModel::Field::Pesel);
	if (auto it = dataPeselSorted_.find(pesel); it != dataPeselSorted_.end()) {
		dataPeselSorted_.erase(it);
	}
	auto lastName = it->second.constGet(StudentModel::Field::LastName);
	if (auto it = dataLastNameSorted_.find(lastName); it != dataLastNameSorted_.end()) {
		for (auto& element : it->second) {
			if (element->constGet(StudentModel::Field::Index) == id) {
				element = it->second.back();
				it->second.pop_back();
				break;
			}
		}
		if (it->second.empty()) {
			dataLastNameSorted_.erase(it);
		}
	}
	data_.erase(it);
	return true;
}

bool DataBase::addStudent(const StudentModel::DataForm& form) {
	if (!builder_.buildStudent(form)) {
		return false;
	}
	return addStudent();
}

bool DataBase::addStudent() {
	const StudentModel newStudent = builder_.getStudent();
	const auto index = builder_.getIndex(); 
	const auto pesel = builder_.getPesel(); 
	auto lastName = builder_.getLastName();
	if (indexExists(index)) {
		debug_ << "student with given index exists: {" + std::to_string(index) + "}";
		return false;
	}
	if (peselExists(pesel)) {
		debug_ << "student with given pesel exists: {" + pesel + "}";
		return false;
	}
	auto it = data_.insert({index, newStudent});
	dataPeselSorted_[pesel] = &(it.first->second);
	dataLastNameSorted_[lastName].push_back(&(it.first->second));
	return true;
}

bool DataBase::indexExists(const unsigned long& index) const {
	return (data_.find(index) != data_.end());
}

bool DataBase::peselExists(const std::string& pesel) const {
	return (dataPeselSorted_.find(pesel) != dataPeselSorted_.end());
}