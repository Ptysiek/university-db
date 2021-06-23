#pragma once

#include "DataBase.hpp"
#include "Debug.hpp"
#include "Gui.hpp"

class Program {
	const Debug debug_;
	DataBase dataBase_;
	Gui gui_;
	bool rerender_;

public:
	Program();
	
	void execute();

private:
	void databaseOperation();
	void databaseOpen();
	void databaseAppend();
	void databaseSave();

	void addStudent();
	void removeStudent();
	void showStudents(const size_t id);

	void findByIndex();
	void findByName();
	void findByPesel();
};