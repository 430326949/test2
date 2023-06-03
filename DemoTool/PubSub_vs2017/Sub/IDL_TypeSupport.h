#ifndef _IDL_TYPESUPPORT_H_INCLUDED_ 
#define _IDL_TYPESUPPORT_H_INCLUDED_

#include <string>
using std::string;
#include "IDL_DataWriter.h"
#include "IDL_DataReader.h"
#include "DDS/TypeSupport.h"

#include "DDS/DomainParticipant.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

class DomainParticipant;

list<string> ListData;
static void initList()
{	if (ListData.size() <= 0)
	{
		ListData.push_back("struct UserDataType{");
		ListData.push_back("short x;");
		ListData.push_back("long y;");
		ListData.push_back("string 255 color;");
		ListData.push_back("};");
	}
}

class UserDataTypeTypeSupport : public TypeSupport {
public:
	static UserDataType* create_data()
	{
		return new UserDataType;
	}

	static ReturnCode_t delete_data(UserDataType* pUserDataType)
	{
		delete pUserDataType;
		pUserDataType = NULL;
		return ReturnCode_t();
	}

	static bool print_data(UserDataType* pUserDataType, int layer = 0)
	{
		auto now = std::chrono::system_clock::now();

		// ����ǰʱ���ת��Ϊtime_t��ʽ��ʱ��
		time_t time = std::chrono::system_clock::to_time_t(now);

		// ʹ��localtime_sȷ���̰߳�ȫ
		struct tm timeinfo;
		localtime_s(&timeinfo, &time);

		// ��ȡ΢����
		auto us = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;

		// ��ʽ�����ʱ��
		char output[100];
		strftime(output, sizeof(output), "%Y/%m/%d %H:%M:%S", &timeinfo);

		char *c = new char[layer];
		int i = 0;
		for (; i < layer; i++)
		{
			c[i] = '\t';
		}
		c[i] = '\0';
		std::cout << c << "UserDataType:" << std::endl;
		std::cout << c << "  x = " << pUserDataType->x << std::endl;
		std::cout << c << "  y = " << pUserDataType->y << std::endl;

		std::cout << c << "  color = " << pUserDataType->color << std::endl;
		std::cout << output << "." << std::setw(6) << std::setfill('0') << us.count() << std::endl;
		return true;
	}

	static const char* get_type_name()
	{
		return "UserDataType";
	}

	static int register_type(DomainParticipant* participant, string type_name)
	{
		if (type_name.empty())
			type_name = get_type_name();

		initList();
		return participant->register_type(type_name,ListData);
	}
};

#endif
