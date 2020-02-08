#include <string>
#include <iostream>
#include <vector>
#include "Messages.h"



const std::string str_revoke = "414B2FE73B0153D3";
const std::string str_error = "A1630000000230E1";
const std::string str_every_month = "696543210063D39A";
const std::string str_every_day = "3419240004F04724";

//string_to_bytes_vector - переводит полученную строку в вектор байт,по 2 байта в одном векторе
std::vector<uint16_t> string_to_bytes_vector(const std::string str)
{
	std::vector<uint16_t> _v;
	std::string _tmp_str;
	for (auto i = 0; i < str.size(); i++)
	{
		_tmp_str +=str[i];
		if (i % 2 == 1)
		{
			_v.push_back(stoul(_tmp_str, nullptr, 16));
			_tmp_str.clear();
		}
	}
	return _v;
}

int main()
{
	std::vector<uint16_t> v_rev = string_to_bytes_vector(str_revoke);
	std::vector<uint16_t> v_err = string_to_bytes_vector(str_error);
	std::vector<uint16_t> v_month = string_to_bytes_vector(str_every_month);
	std::vector<uint16_t> v_day = string_to_bytes_vector(str_every_day);

	std::cout << "Сообщение о ошибке:" << std::endl;
	ErrorMessage err_msg(v_err);
	err_msg.print_vector_bytes();
	err_msg.print_json();
	std::cout << std::endl << std::endl;
	std::cout << "Сообщение о сбросе:" << std::endl;
	RevokeMessage rev_msg(v_rev);
	rev_msg.print_vector_bytes();
	rev_msg.print_json();
	std::cout << std::endl << std::endl;
	
	std::cout << "Ежемесячное сообщение:" << std::endl;
	EveryMonthMessage mnth_msg(v_month);
	mnth_msg.print_json();
	std::cout << std::endl << std::endl;
	
	std::cout << "Ежедневное сообщение:" << std::endl;
	EveryDayMessage day_msg(v_day);
	day_msg.generate_json();
	day_msg.print_json();
	std::cout << std::endl << std::endl;
	system("pause");
}

