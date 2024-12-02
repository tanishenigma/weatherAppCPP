#include <iostream>
#include <curl/curl.h>
#include "json.hpp" // Using nlohmann/json

using namespace std;
using json = nlohmann::json;

// Function to handle HTTP response
size_t writeData(void *contents, size_t size, size_t nmemb, string *output)
{
	size_t totalSize = size * nmemb;
	output->append((char *)contents, totalSize);
	return totalSize;
}

// Function to fetch weather data
void getWeather(const string &apiKey, const string &city)
{
	string url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey;

	CURL *curl = curl_easy_init();
	if (!curl)
	{
		cerr << "Failed to initialize curl!" << endl;
		return;
	}

	string response;
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	CURLcode result = curl_easy_perform(curl);
	if (result != CURLE_OK)
	{
		cerr << "Error: " << curl_easy_strerror(result) << endl;
		curl_easy_cleanup(curl);
		return;
	}

	curl_easy_cleanup(curl);

	// Parse JSON response
	try
	{
		auto jsonData = json::parse(response);
		cout << "Weather in " << city << ":\n";
		cout << "Temperature: " << jsonData["main"]["temp"].get<float>() - 273.15 << "°C\n";
		cout << "Condition: " << jsonData["weather"][0]["description"].get<string>() << "\n";
	}
	catch (const json::exception &e)
	{
		cerr << "Failed to parse weather data: " << e.what() << endl;
	}
}
int main()
{
	string apiKey = "0435b5b7852e4d910cf9b5c2e66bd444"; // Replace with your API key
	string city;

	cout << "Enter the city name: ";
	cin >> city;

	getWeather(apiKey, city);

	return 0;
}
