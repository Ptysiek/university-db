#pragma once
#include <array>
#include <exception>
#include <string>

struct StudentModel {
	using DataForm = std::array<std::string, 6>;
	enum class Field {
		FirstName,
		LastName,
		Address,
		Gender,
		Index,
		Pesel
	};

private:
	std::array<std::string, 4> changeableData_;
	const std::string index_;
	const std::string pesel_;

public:
	StudentModel(const std::string& index, const std::string& pesel);
	explicit StudentModel(const DataForm& form);
	
	static std::string fieldName(const size_t field);
	static std::string fieldName(const Field& field);
	size_t size() const;
	size_t nonConstDataSize() const;

	std::string get(const Field& field) const;
	std::string& get(const Field& field);
	std::string constGet(const Field& field) const;
	
	bool incomplete() const;
	std::string toString() const;
};
