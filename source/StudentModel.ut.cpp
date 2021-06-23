#include "../tools/catch.hpp"
#include "StudentModel.hpp"

TEST_CASE("StudentModel Class") {
    struct Test {
        StudentModel::DataForm data;
        std::string expectancy;

        std::string log() const {
            std::string result = "{ (";
            for (const auto& value : data) {
                result += value + ", ";
            }
            if (!result.empty()) {
                result.pop_back();
            }
            result += ") -> " + expectancy + " }";
            return result;
        }
    };
    std::initializer_list<Test> tests = {
        {{}, {}},
        {{"a", "bcd", "c"}, "a bcd c"},
        {{"a", "b", "c", "d"}, "a b c d"},
        {{"", ""}, {}},
        {{"", "a", ""}, "a"},
        {{"", "a", "", "b", "c", ""}, "a b c"},
        {{"1", "2", "3", "4", "5", "6"}, "1 2 3 4 5 6"}
    };

    // --------------------------------------------------------------------------------
    WHEN("Testing Constructor") {
        REQUIRE_NOTHROW(StudentModel::DataForm());
        REQUIRE_NOTHROW(StudentModel::DataForm().size());
        REQUIRE_NOTHROW(StudentModel({}));
        REQUIRE_NOTHROW(StudentModel({}).size());
        REQUIRE_NOTHROW(StudentModel({}).nonConstDataSize());
        REQUIRE_NOTHROW(StudentModel({}).incomplete());
        
        REQUIRE(StudentModel::DataForm().size() == StudentModel({}).size());
        
        for (const auto& test : tests) {
			THEN(test.log()) {
                REQUIRE_NOTHROW(StudentModel(test.data));
                if (test.data.size() >= 2) {
                    REQUIRE_NOTHROW(StudentModel({test.data.at(0), test.data.at(1)}));
                }
            }
        }
    }
    // --------------------------------------------------------------------------------
    WHEN("Testing get() Operation") {
        for (const auto& test : tests) {
            THEN("All fields are available. " + test.log()) {
				const StudentModel student(test.data);				
                for (size_t i = 0; i < student.size() - 2; ++i) {
                    REQUIRE_NOTHROW(student.get(static_cast<StudentModel::Field>(i)));
                }
                for (size_t i = 0; i < student.nonConstDataSize(); ++i) {
                    REQUIRE_NOTHROW(student.get(static_cast<StudentModel::Field>(i)));
                }
                REQUIRE_THROWS(student.get(static_cast<StudentModel::Field>(student.size())));
                REQUIRE_THROWS(student.get(static_cast<StudentModel::Field>(-1)));
                
                REQUIRE_NOTHROW(student.get(StudentModel::Field::FirstName));
                REQUIRE_NOTHROW(student.get(StudentModel::Field::LastName));
                REQUIRE_NOTHROW(student.get(StudentModel::Field::Address));
                REQUIRE_NOTHROW(student.get(StudentModel::Field::Gender));
                REQUIRE_NOTHROW(student.get(StudentModel::Field::Index));
                REQUIRE_NOTHROW(student.get(StudentModel::Field::Pesel));
            }
			THEN("All values can be read. " + test.log()) {
				const StudentModel student(test.data);				
                for (size_t i = 0; i < student.size(); ++i) {
                    auto field = static_cast<StudentModel::Field>(i);
                    REQUIRE_NOTHROW(student.get(field));
                    if (test.data.size() <= i) {
                        break;
                    }
                    const auto& output = student.get(field);                    
                    REQUIRE(output == test.data.at(i));
                }
            }			
            THEN("Some values CAN be modified. " + test.log()) {
				StudentModel student(test.data);				
                for (size_t i = 0; i < student.nonConstDataSize(); ++i) {
                    auto field = static_cast<StudentModel::Field>(i);
                    REQUIRE_NOTHROW(student.get(field));
                    if (test.data.size() <= i) {
                        break;
                    }
                    const auto& output = student.get(field);                    
                    REQUIRE(output == test.data.at(i));

                    auto& modify = student.get(field);
                    modify += "StringAppendTest";
                    REQUIRE(modify == student.get(field));

                    modify.clear();
                    REQUIRE(student.get(field).empty());
                }
            }
            THEN("Some values CANT be modified. " + test.log()) {
				StudentModel student(test.data);				
                for (size_t i = student.nonConstDataSize(); i < student.size() + 1; ++i) {
                    REQUIRE_THROWS(student.get(static_cast<StudentModel::Field>(i)));
                }
            }
        }
    }
    // --------------------------------------------------------------------------------
    WHEN("Testing incomplete() Operation") {
        for (const auto& test : tests) {
			THEN(test.log()) {
                const StudentModel student(test.data);
                size_t count = 0;
                for (size_t i = 0; i < student.size(); ++i) {
                    if (!student.get(static_cast<StudentModel::Field>(i)).empty()) {
                        ++count;
                    }
                }
                if (test.data.size() < student.size()) {
                    REQUIRE(student.incomplete() == true);
                }
                if (student.size() == count) {
                    REQUIRE(student.incomplete() == false);
                }
                else {
                    REQUIRE(student.incomplete() == true);
                }
            }
        }
    }
    // --------------------------------------------------------------------------------
    WHEN("Testing toString() Operation") {
        for (const auto& test : tests) {
			THEN(test.log()) {
				StudentModel student(test.data);
				auto output = student.toString();
                REQUIRE(output == test.expectancy);
            }
        }
    }
}
