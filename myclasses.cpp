#include "myclasses.h"

std::optional<int> getInt(std::string& s)
{
	if(s == "")
		return std::nullopt;
	else
		return stoi(s);
}

std::optional<std::string> getStr(std::string& s)
{
	if(s == "")
		return std::nullopt;
	else
		return s;
}

bool Person::operator<(const Person& p)
{
	return tie(this->surname_, this->name_, this->patronymic_) < tie(p.surname_, p.name_, p.patronymic_);
}

bool Person::operator== (const Person& p)
{
	return tie(this->surname_, this->name_, this->patronymic_) == tie(p.surname_, p.name_, p.patronymic_);
}

std::ostream& operator<< (std::ostream& os, const Person& p)
{
	os << p.surname_ << " " << p.name_;
	if(p.patronymic_)
		os << " " << *p.patronymic_;
	return os;
}

std::ostream& operator<<(std::ostream& os, const PhoneNumber& p)
{
	os << "+" << p.country_code_ << "("<< p.city_code_ << ")" << p.number_;
	if(p.extension_number_)
		os << " " << *p.extension_number_;
	return os;
}

bool PhoneNumber::operator< (const PhoneNumber& p)
{
	return tie(this->country_code_, this->city_code_, this->number_, this->extension_number_) < tie(p.country_code_, p.city_code_, p.number_, p.extension_number_);
}
bool PhoneNumber::operator== (const PhoneNumber& p)
{
	return tie(this->country_code_, this->city_code_, this->number_, this->extension_number_) == tie(p.country_code_, p.city_code_, p.number_, p.extension_number_);
}

PhoneBook::PhoneBook(std::ifstream& file)
{
	if(!file)
	{
		std::cout << "Не удается открыть файл!" << std::endl;
		exit(-1);
	}
	if(file.peek() == file.eof())
	{
		std::cout << " Файл пустой." << std::endl;
		exit(-1);
	}

	std::string s;

	while( getline(file, s) )   // читаем строку из файла
	{
		std::vector<std::string> tmp;
		std::stringstream str(s);

		for(std::string val; getline(str, val, ';'); )   // разбиваем строку на поля
		{
			tmp.push_back(val);
		}
		Person per(tmp[0],tmp[1],getStr(tmp[2]));
		PhoneNumber ph(stoi(tmp[3]),stoi(tmp[4]),tmp[5], getInt(tmp[6]));

		std::pair<Person, PhoneNumber> tp(per,ph);
		data_.push_back(tp);
	}
}

void PhoneBook::sortByName()
{
	sort(data_.begin(), data_.end(), [](std::pair<Person, PhoneNumber> &left, std::pair<Person, PhoneNumber> &right) -> bool
	{
		return left.first < right.first;
	});
}

void PhoneBook::sortByPhone()
{
	sort(data_.begin(), data_.end(), [](std::pair<Person, PhoneNumber> &left, std::pair<Person, PhoneNumber> &right) -> bool
	{
		return left.second < right.second;
	});
}

std::ostream& operator<<(std::ostream& os, PhoneBook& phb)
{
	for(const auto& [per, tel]: phb.data_)
	{
		os << per << " " << tel << std::endl;
	}
	return os;
}

std::pair<std::string, PhoneNumber> PhoneBook::getPhoneNumber(std::string surname)
{
	int count = 0;
	PhoneNumber pn{0,0,""};
	for(auto& [per, tel]: data_)
	{
		if(per.surname_ == surname)
		{
			count++;
			pn = tel;
		}
	}
	if( count == 0)
		return {"not found" , pn };
	else if(count == 1)
		return {"" , pn };
	else
		return {"found more than 1" , pn };
}

bool PhoneBook::changePhoneNumber(const Person &p, const PhoneNumber &n)
{
	for(auto& [per, tel]: data_)
	{
		if( per == p)
		{
			tel = n;
			return true;
		}
	}
	return false;
}
