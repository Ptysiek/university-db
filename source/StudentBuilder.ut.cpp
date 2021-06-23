#include "../tools/catch.hpp"
#include "StudentBuilder.hpp"

struct BuilderUnpacked : StudentBuilder {
	BuilderUnpacked(): StudentBuilder(Debug(Debug::State::Disable)) {}

	bool verifyPeselFormat(const std::string& pesel) const {
		return StudentBuilder::verifyPeselFormat(pesel);
	}

	bool verifyIndexFormat(const std::string& index) const {
		return StudentBuilder::verifyIndexFormat(index);
	}

	bool verifySpaces(const StudentModel::DataForm& data) {
		data_ = data;
		return StudentBuilder::verifySpaces();
	}
	
	bool verifyEmptyField(const StudentModel::DataForm& data) {
		data_ = data;
		return StudentBuilder::verifyEmptyField();
	}
};

TEST_CASE("StudentBuilder Class") {
	// --------------------------------------------------------------------------------
	WHEN("Testing Constructor") {
		THEN("Every get() Throws Exception") {
			BuilderUnpacked builder;
			REQUIRE_THROWS(builder.getStudent());
			REQUIRE_THROWS(builder.getIndex());
			REQUIRE_THROWS(builder.getPesel());
			REQUIRE_THROWS(builder.getLastName());
		}
	}

	// --------------------------------------------------------------------------------
	WHEN("Testing verifyPeselFormat() Operation") {

	}

	// --------------------------------------------------------------------------------
    WHEN("Testing verifyIndexFormat() Operation") {
		struct  Test {		
			const std::string data;
			const bool expectancy;

			std::string log() const {
				std::string expectancy = (this->expectancy)? "true" : "false";
				return "{ (" + data + ") -> " + expectancy + " }";
			}
		};
		std::initializer_list<Test> tests = {
			{{}, false},
			{"", false},
			{" ", false},
			{"a", false},
			{"abcd", false},
			{"-12", false},
			{"-1", false},
			{"0", true},
			{"123123123", true},
			{"a123123123", false},
		};
        for (const auto& test : tests) {
			BuilderUnpacked builder;
			THEN(test.log()) {
				REQUIRE(builder.verifyIndexFormat(test.data) == test.expectancy);
			}
		}
		BuilderUnpacked builder;
		unsigned long max_ul_val = 0;
		--max_ul_val;
		std::string maxVal = std::to_string(max_ul_val);
		THEN("out of range test: " + maxVal) {
			REQUIRE(builder.verifyIndexFormat(maxVal) == true);
		}

		REQUIRE(maxVal.back() != '9');
		++maxVal.back();
		THEN("out of range test: " + maxVal) {
			REQUIRE(builder.verifyIndexFormat(maxVal) == false);
		}
	}
	
    // --------------------------------------------------------------------------------
	WHEN("Testing verifySpaces() Operation") {

	}
    
	// --------------------------------------------------------------------------------
	WHEN("Testing verifyEmptyField() Operation") {
		struct  Test {		
			const StudentModel::DataForm data;
			const bool expectancy;

			std::string log() const {
				std::string expectancy = (this->expectancy)? "true" : "false";
				std::string result = "{ (";
				for (const auto& value : data) {
					result += value + ", ";
				}
				if (!result.empty()) {
					result.pop_back();
					result.pop_back();
				}
				result += ") -> " + expectancy + " }";
				return result;
			}
		};

		std::initializer_list<Test> tests = {
			{{}, false},
			{{"a", "b", "c"}, false},
			{{"a", "b", "c", "d"}, false},
			{{"", ""}, false},
			{{"", "a", ""}, false},
			{{"", "", "", "", "", ""}, false}, 
			{{"", "a", "", "b", "c", ""}, false},
			{{"1", "", "3", "4", "5", "6"}, false},
			{{"1", "2", "3", "4", "5", "6"}, true},
			{{"a", "b", "c", "d", "e", "f"}, true},
			{{" ", "a", " ", "b", "c", " "}, false},
			{{"1", " ", "3", "4", "5", "6"}, false}
		};

		/*
        for (const auto& test : emptyFieldTests) {
			BuilderUnpacked builder;

			THEN("empty field test " + test.log()) {
				REQUIRE(database.verify(test.data) == test.expectancy);				
				
				REQUIRE(database.data().empty());
				REQUIRE(database.dataPeselSorted().empty());
				REQUIRE(database.dataLastNameSorted().empty());
            }
        }
		//*/
	}

    // --------------------------------------------------------------------------------
	WHEN("Testing verify() Operation") {

	}

    // --------------------------------------------------------------------------------
    WHEN("Testing extractForm() Operation") {
	}
    
	// --------------------------------------------------------------------------------
    WHEN("Testing clear() Operation") {
	}
}
