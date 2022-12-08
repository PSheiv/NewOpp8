#ifndef MYCLASSES_H
#define MYCLASSES_H
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <optional>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <gtest/gtest.h>

class Person
{
	public:
		std::string surname_;
		std::string name_;
		std::optional<std::string> patronymic_;
		friend class PhoneBook;

		Person() {}
		Person(std::string surname, std::string name, std::optional<std::string> patronymic = std::nullopt) : surname_(surname), name_(name), patronymic_(patronymic) { }
		bool operator< (const Person& p);
		bool operator== (const Person& p);
		friend std::ostream& operator<<(std::ostream& os, const Person& p);
};


class PhoneNumber
{
	public:
		int country_code_;
		int city_code_;
		std::optional<int> extension_number_;
		std::string number_;
		friend class PhoneBook;

		PhoneNumber(int country_code, int city_code, std::string number, std::optional<int> extension_number = std::nullopt) :
			country_code_(country_code), city_code_(city_code), number_(number), extension_number_(extension_number) { }
		friend std::ostream& operator<<(std::ostream& os, const PhoneNumber& p);
		bool operator< (const PhoneNumber& p);
		bool operator== (const PhoneNumber& p);
};

class PhoneBook
{
		std::vector<std::pair<Person, PhoneNumber>> data_;
	public:
		PhoneBook(std::istream& in_stream);
		void sortByName();
		void sortByPhone();
		friend std::ostream& operator<<(std::ostream& os, PhoneBook& other);
		std::pair<std::string, PhoneNumber> getPhoneNumber(std::string surname);
		bool changePhoneNumber(const Person& p, const PhoneNumber& n);
};

class TestBook: public testing::Test
{
protected:
	PhoneBook *book;

	void SetUp() override
	{
		std::stringstream src_stream;
		src_stream << "Ivanov;Daniil;Maksimovich;7;366;7508887;;\n"
				<< "Aleksandrov;Georgii;;493;7637;6114861;;\n"
				<< "Aleksandrov;Andrey;Mikhailovich;7;247;1377660;5542;\n"
				<< "Markelov;Dmitrii;Vadimovich;19;7576;5734416;2;\n";

		book = new PhoneBook(src_stream);
	}

	void TearDown() override
	{
		delete book;
	}
};

TEST_F(TestBook, GetPhoneNumber_notFound)
{
	std::string lastname = "Egorov";

	auto answer = book->getPhoneNumber(lastname);
	EXPECT_EQ(std::get<0>(answer), "not found");
}

TEST_F(TestBook, GetPhoneNumber_foundOne)
{
	std::string lastname = "Ivanov";

	auto answer = book->getPhoneNumber(lastname);
	EXPECT_EQ(std::get<0>(answer), "");
}

TEST_F(TestBook, GetPhoneNumber_foundMore)
{
	std::string lastname = "Aleksandrov";

	auto answer = book->getPhoneNumber(lastname);
	EXPECT_EQ(std::get<0>(answer), "found more than 1");
}

TEST_F(TestBook, GetPhoneNumber_empty)
{
	std::string lastname = "";

	auto answer = book->getPhoneNumber(lastname);
	EXPECT_EQ(std::get<0>(answer), "not found");
}

TEST_F(TestBook, ChangePhoneNumber)
{
	std::string lastname = "Ivanov";

	ASSERT_EQ(std::get<0>(book->getPhoneNumber(lastname)).empty(), true);

	PhoneNumber answer = std::get<1>(book->getPhoneNumber(lastname))[0];
	EXPECT_EQ(answer.number_, "7508887");

	book->changePhoneNumber(Person { "Ivanov", "Daniil", "Maksimovich" }, PhoneNumber { 7, 123, "15344458", std::nullopt });

	answer = std::get<1>(book->getPhoneNumber(lastname))[0];
	EXPECT_EQ(answer.country_code_, 7);
	EXPECT_EQ(answer.city_code_, 123);
	EXPECT_EQ(answer.number_, "15344458");
	EXPECT_EQ(answer.extension_number_, std::nullopt);
}

TEST_F(TestBook, sortByName)
{
	std::stringstream src_stream;
	src_stream << "    Aleksandrov      Andrey     Mikhailovich   +7(247)1377660 5542\n"
			<< "    Aleksandrov     Georgii                    +493(7637)6114861\n"
			<< "         Ivanov      Daniil      Maksimovich   +7(366)7508887\n"
			<< "       Markelov     Dmitrii       Vadimovich   +19(7576)5734416 2\n";

	book->sortByName();

	std::stringstream dst_stream;
	dst_stream << *book;

	while (!dst_stream.eof())
	{
		std::string str_src;
		std::string str_dst;

		src_stream >> str_src ;
		dst_stream >> str_dst;

		EXPECT_EQ(str_dst, str_src);
	}
}

TEST_F(TestBook, sortByPhone)
{
	std::stringstream src_stream;
	src_stream << "    Aleksandrov      Andrey     Mikhailovich   +7(247)1377660 5542\n"
			<< "         Ivanov      Daniil      Maksimovich   +7(366)7508887\n"
			<< "       Markelov     Dmitrii       Vadimovich   +19(7576)5734416 2\n"
			<< "    Aleksandrov     Georgii                    +493(7637)6114861\n";

	book->sortByPhone();

	std::stringstream dst_stream;
	dst_stream << *book;

	while (!dst_stream.eof())
	{
		std::string str_src;
		std::string str_dst;

		src_stream >> str_src ;
		dst_stream >> str_dst;

		EXPECT_EQ(str_dst, str_src);
	}
}
#endif
