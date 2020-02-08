#include "Messages.h"

//print_vector_bytes - выводит вектор байт в стандартный поток
const void Messages::print_vector_bytes()
{
	for(auto v : vector_bytes)
	{
		std::cout << v << " ";
	}
	std::cout << std::endl;
}
//print_json - выводит JSON поэлементно в стандартный поток
const void Messages::print_json()
{
	for(auto item:byte_json.items())
	{
		std::cout << item.key() << ":" << item.value() << std::endl;
	}
}

//to_hex_dec - принимает номер байта для формирования пары :decimal/hex  значения и возвращает его
std::pair<std::string, unsigned long long> Messages::to_hex_dec(int num_byte_to_hex)
{
	std::pair<std::string, unsigned long long> tmp_pair;
	std::string _tmp_str;
	std::stringstream ss;
	ss << std::hex << vector_bytes[num_byte_to_hex];
	ss >> _tmp_str;
	std::transform(_tmp_str.begin(), _tmp_str.end(), _tmp_str.begin(), toupper);
	tmp_pair.second = std::stoul(_tmp_str, nullptr, 16);
	tmp_pair.first = "0x" + _tmp_str;
	//tmp_pair.first = reinterpret_cast<unsigned short>(&_tmp_str);
	return tmp_pair;
}
//to_hex_dec - принимает номера байта от(from) до (to) для формирования пары :decimal/hex  значения и возвращает его
std::pair<std::string, unsigned long long> Messages::to_hex_dec(int from, int to)
{
	std::pair<std::string, unsigned long long> tmp_pair;
	std::string tmp;
	std::stringstream ss;
	if(from > to)
	{
		for (int i = from; i >= to; i--)
		{
			ss << std::hex << vector_bytes[i];
		}
	} else if(from != to)
	{
		for (int i = from; i <= to; i++)
		{
			ss << std::hex << vector_bytes[i];
		}
	}	
	tmp = ss.str();
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), toupper);
	tmp_pair.second = std::stoul(tmp, nullptr, 16);
	tmp_pair.first = "0x" + tmp;
	//tmp_pair.first = reinterpret_cast<unsigned short>(&tmp);
	return tmp_pair;
}

/*
 * generate_json() - генерирует JSON для сообщения сброса формата :
 * [Подпись] - hex
 * [Счетчик][Hex] - hex;
 * [Счетчик][Dec] - dec;
 * [Железо][Hex] - hex;
 * [Железо][Dec] - dec;
 * [ПО][Hex] - hex;
 * [ПО][Dec] - dec;
 * [CRC][Hex] - hex;
 * [CRC][Dec] - dec;
 */
void RevokeMessage::generate_json()
{
	//Подпись (1):
	byte_json["Подпись"] = to_hex_dec(0).first;
	//Значение счетчика (2-5) :
	auto tmp = to_hex_dec(4, 1);
	byte_json["Счетчик"]["Hex"] = tmp.first;
	byte_json["Счетчик"]["Dec"] = tmp.second;
	//	Версия железа(6):
	tmp = to_hex_dec(5);
	byte_json["Железо"]["Hex"] = tmp.first;
	byte_json["Железо"]["Dec"] = tmp.second;
	//	Версия ПО(7):
	tmp = to_hex_dec(6);
	byte_json["ПО"]["Hex"] = tmp.first;
	byte_json["ПО"]["Dec"] = tmp.second;
	// CRC (8):
	tmp = to_hex_dec(7);
	byte_json["CRC"]["Hex"] = tmp.first;
	byte_json["CRC"]["Dec"] = tmp.second;
}

/*
 * generate_json() - генерирует JSON для сообщения о ошибке формата:
 * [Подпись] - hex
 * [Ошибка] - hex;
 * [Ошибка] - dec;
 * [Железо][Hex] - hex;
 * [Железо][Dec] - dec;
 * [ПО][Hex] - hex;
 * [ПО][Dec] - dec;
 * [CRC][Hex] - hex;
 * [CRC][Dec] - dec;
 */
void ErrorMessage::generate_json()
{
	//Подпись (1):
	byte_json["Подпись"] = to_hex_dec(0).first;
	//Код Ошибки (2-5) :
	auto tmp = to_hex_dec(4, 1);
	byte_json["Ошибка"]["Hex"] = tmp.first;
	byte_json["Ошибка"]["Dec"] = tmp.second;
	//	Версия железа(6):
	tmp = to_hex_dec(5);
	byte_json["Железо"]["Hex"] = tmp.first;
	byte_json["Железо"]["Dec"] = tmp.second;
	//	Версия ПО(7):
	tmp = to_hex_dec(6);
	byte_json["ПО"]["Hex"] = tmp.first;
	byte_json["ПО"]["Dec"] = tmp.second;
	// CRC (8):
	tmp = to_hex_dec(7);
	byte_json["CRC"]["Hex"] = tmp.first;
	byte_json["CRC"]["Dec"] = tmp.second;
}

/*
 * generate_json() - генерирует JSON для ежемесячного сообщения формата:
 * [Подпись] - hex;
 * [Счетчик] - hex;
 * [Счетчик] - dec;
 * [Сообщений][Hex] - hex;
 * [Сообщений][Dec] - dec;
 * ["Метаинформация"]["Мощность"] = dec;
 * ["Метаинформация"]["Расход"] = dec;
 */
void EveryMonthMessage::generate_json()
{
	//Подпись (1):
	byte_json["Подпись"] = to_hex_dec(0).first;
	//Значение счетчика (2-5) :
	auto tmp = to_hex_dec(4, 1);
	byte_json["Счетчик"]["Hex"] = tmp.first;
	byte_json["Счетчик"]["Dec"] = tmp.second;
	//Кол-во отправленных сообщений(6-7):
	tmp = to_hex_dec(6,5);
	byte_json["Сообщений"]["Hex"] = tmp.first;
	byte_json["Сообщений"]["Dec"] = tmp.second;
	/* Метаинформация (8):
	 * Старшие 2 бита – уровень мощности сигнала
	 * Младшие 6 бит – максимально зафиксированное с момента калибровки модема кол-во импульсов per minute
	*/
	byte_json["Метаинформация"]["Мощность"] = vector_bytes[7] >> 6;
	byte_json["Метаинформация"]["Расход"] = vector_bytes[7] & 0x3F;
}

/*
 * generate_json() - генерирует JSON для ежемедневного сообщения:
 * [Подпись] - hex; (бит0)
 * [Счетчик] - dec; (бит1-бит15);
 * [Byte#][Hex] = hex; 
 * [Byte#][Dec] = dec;
 * [Расход][#] = dec; 
 * __________________
 * #- номер часа 0-23; 
 * Значения [Расход][#] - 
 * 0 - нет расхода,
 * 1 - 0.1-33,3%,
 * 2 - 33.4-66.6%,
 * 3 - 66.7-100%;
 */
void EveryDayMessage::generate_json()
{
	//Байт0 - подпись[0]=0 + 7 бит счетчика
	const unsigned int lowMask = 0x7F;
	std::string binary = std::bitset<8>(vector_bytes[0]).to_string();
	byte_json["Подпись"] = binary;
	//Байт 1: 7-14 бит счетчика (в сумме с байт0 15 бит счетчика):
	byte_json["Счетчик"] = to_hex_dec(1, 0).second >> 1;
	int count = 0;
	//Пробегаем все байты 7-2
	for(int i = 7;i>=2;i--)
	{
		byte_json["Byte" +std::to_string(i)]["Hex"] = to_hex_dec(i).first;
		auto binary =std::bitset<8>(to_hex_dec(i).second).to_string();
		byte_json["Byte" + std::to_string(i)]["Bin"] = binary;
		//Расход по часам, по 4 значения на байт (2бита на час)
		byte_json["Расход"][std::to_string(count)] = (to_hex_dec(i).second & 0xC0)>>6;
		count++;
		byte_json["Расход"][ std::to_string(count)] = (to_hex_dec(i).second & 0x30)>>4;
		count++;
		byte_json["Расход"][std::to_string(count)] = (to_hex_dec(i).second & 0xC )>>2;
		count++;
		byte_json["Расход"][std::to_string(count)] = (to_hex_dec(i).second & 0x3 );
		count++;
	}
}
