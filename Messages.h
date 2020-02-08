#pragma once
#include <cstdint>
#include "json.hpp"
#include <iostream>
#include <sstream> 
#include <utility>
#include <bitset> 


using json = nlohmann::json;



/*Messages - базовый абстрактный класс для всех сообщений
 *			 print_vector_bytes()-метод вывода вектора байт в стандартный поток
 *			 print_json() - метод вывода всех элементов JSON  в стандартный поток
 *			 to_hex_dec() - перегруженная функция,возвращает пару вида hex/dec отдельного байта или слайса байт.
*/
class Messages
{
public:
	virtual void generate_json() = 0;
	const void print_vector_bytes();
	const void print_json();
	std::pair<std::string, unsigned long long> to_hex_dec(int num_byte_to_hex);
	std::pair<std::string, unsigned long long> to_hex_dec(int from,int to);
	std::vector<uint16_t> vector_bytes;
	json byte_json;
};

//RevokeMessage - класс для генерации JSON с подписью отзывы
class RevokeMessage :public Messages
{
public:
	explicit  RevokeMessage(std::vector<uint16_t> _v) { vector_bytes = _v; generate_json();};
	void generate_json() override;
};

//ErrorMessage - класс для генерации JSON с подписью ошибка
class ErrorMessage :public Messages
{
public:
	explicit ErrorMessage(std::vector<uint16_t> _v) { vector_bytes = _v; generate_json();};
	void generate_json() override;
};

//EveryMonthMessage - класс для генерации JSON ежемесячного сообщения
class EveryMonthMessage : public Messages
{
public:
	explicit EveryMonthMessage(std::vector<uint16_t> _v) { vector_bytes = _v; generate_json();};
	void generate_json() override;	
};
//EveryDayMessage - класс для генерации JSON ежедневного сообщения
class EveryDayMessage : public Messages
{
public:
	explicit EveryDayMessage(std::vector<uint16_t>_v) { vector_bytes = _v; generate_json();};
	void generate_json() override;
};
