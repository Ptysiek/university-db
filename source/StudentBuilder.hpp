#pragma once
#include <sstream>
#include <vector>

#include "Debug.hpp"
#include "StudentModel.hpp"

class StudentBuilder {
protected:
	const Debug& debug_;
	bool completed_;
	StudentModel::DataForm data_;
	unsigned long index_;
	std::string indexString_;
	std::string pesel_;
	std::string lastName_;

public:
	StudentBuilder(const Debug& debug);

	StudentModel getStudent() const;
	unsigned long getIndex() const;
	std::string getPesel() const;
	std::string getLastName() const;

	bool buildStudent(const StudentModel::DataForm& form);
	bool buildStudent(const std::string& studentData);

protected:
	unsigned long to_unlong(const std::string& str) const;
	void clear();
	bool extractForm(std::stringstream&& studentData);
	bool extractFields();
	
	bool verify() const;
	bool verifyEmptyField() const;
	bool verifySpaces() const;
	bool verifyIndexFormat(const std::string& index) const;
	bool verifyPeselFormat(const std::string& pesel) const;
	std::vector<int> breakPeselIntoPieces(const std::string& pesel) const;
	int calculatePeselCheckDigit(const std::vector<int>& peselVctr) const;
};