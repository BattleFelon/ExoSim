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

class Murano
{
public:

	Murano();
	virtual ~Murano();

	std::string postData(std::string cik, std::string data);
	std::string getData(std::string URL);
	std::string tryActivate(std::string serial_number, std::string product_id);

private:

	static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);

};
