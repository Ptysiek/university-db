#include "StudentModel.hpp"

StudentModel::StudentModel(const std::string& index, const std::string& pesel):
	changeableData_({}),
	index_(index),
	pesel_(pesel)
{}

StudentModel::StudentModel(const DataForm& form): 
	changeableData_({form.at(0), form.at(1), form.at(2), form.at(3)}),
	index_(form.at(4)),
	pesel_(form.at(5))
{}
	
std::string StudentModel::fieldName(const size_t index) {
	return fieldName(static_cast<StudentModel::Field>(index));
}

std::string StudentModel::fieldName(const Field& field) {
	switch (field) {
	case Field::FirstName:
		return "First Name";
	case Field::LastName:
		return "Last Name";
	case Field::Address:
		return "Address";
	case Field::Gender:
		return "Gender";
	case Field::Index:
		return "Index";
	case Field::Pesel:
		return "Pesel";
	}
	throw std::out_of_range("StudentModel::get()");
} 

size_t StudentModel::size() const {
	return nonConstDataSize() + 2;
}

size_t StudentModel::nonConstDataSize() const {
	return changeableData_.size();
}
	
std::string StudentModel::constGet(const Field& field) const {
	return get(field);
}

std::string StudentModel::get(const Field& field) const {
	if (field == Field::Index) {
		return index_;
	}
	if (field == Field::Pesel) {
		return pesel_;
	}
	size_t index = static_cast<size_t>(field);
	if (index < nonConstDataSize()) {
		return changeableData_.at(index);
	}		
	throw std::out_of_range("StudentModel::get() const");
}

std::string& StudentModel::get(const Field& field) {
	size_t index = static_cast<size_t>(field);
	if (index < nonConstDataSize()) {
		return changeableData_.at(index);
	}	
	if (index < size()) {
		throw std::logic_error("StudentModel::get() => attempted to extract const data");
	}
	throw std::out_of_range("StudentModel::get()");
}

bool StudentModel::incomplete() const {
	for (size_t i = 0; i < size(); ++i) {
		if (get(static_cast<Field>(i)).empty()) {
			return true;
		}
	}
	return false;
}

std::string StudentModel::toString() const {
	std::string result;
	for (size_t i = 0; i < size(); ++i) {
		auto tmp = get(static_cast<Field>(i));
		result += (tmp.empty())? tmp : tmp + " ";
	}
	if (!result.empty()) {
		result.pop_back();
	}
	return result;
}
