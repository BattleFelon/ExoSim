// Murano Functions
#pragma once

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <string>
#include <curl/curl.h>
#include <map>
#include <vector>

class Murano
{
public:

	Murano();
	virtual ~Murano();

	std::string postData(std::string product_id, std::string data);
	std::string getData(std::string URL);
	std::string tryActivate(std::string serial_number, std::string product_id);
	std::string getCIK(std::string serial_number){return cik_map[serial_number];}

private:
	std::map<std::string,std::string> cik_map;
	static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);

};
