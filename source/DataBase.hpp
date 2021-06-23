#pragma once
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

#include "Debug.hpp"
#include "StudentBuilder.hpp"
#include "StudentModel.hpp"

class DataBase {
protected:
	const Debug& debug_;
	StudentBuilder builder_;
	std::map<unsigned long, StudentModel> data_;
	std::map<std::string, StudentModel*> dataPeselSorted_;
	std::map<std::string, std::vector<StudentModel*>> dataLastNameSorted_;

public:
	DataBase(const Debug& debug);

	const std::map<unsigned long, StudentModel>& data() const;
	const std::map<std::string, StudentModel*>& dataPeselSorted() const;
	const std::map<std::string, std::vector<StudentModel*>>& dataLastNameSorted() const;

	bool open(const std::string& filePath);
	bool append(const std::string& filePath);
	bool save(const std::string& filePath) const;
	void close();
	bool removeStudent(const std::string& id);
	bool addStudent(const StudentModel::DataForm& form);

protected:
	bool addStudent();
	bool indexExists(const unsigned long& index) const;
	bool peselExists(const std::string& pesel) const;
};