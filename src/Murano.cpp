#include <Murano.h>

Murano::Murano()
{

}

Murano::~Murano()
{

}

size_t Murano::writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string Murano::postData(std::string cik, std::string data)
{
    CURL *curl;
    CURLcode res;

    std::string read_buffer;

    curl = curl_easy_init();
    if(curl) {
        
        //Set URL
        std::string URL = "https://";
        URL.append(".m2.exosite.com/provision/activate");

        //Set Headers
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        headers = curl_slist_append(headers, "charset=utf-8");

        //Set Data to send
        std::string message = "vendor=";



        curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        res = curl_easy_perform(curl);

        //Callback
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);

        if(res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));

        curl_easy_cleanup(curl);
  }

    return(read_buffer);
}

std::string Murano::getData(std::string URL)
{
	CURL *curl;
    CURLcode res;
    std::string read_buffer;


    curl = curl_easy_init();

    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com/");

        //verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        //Callback
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);

        res = curl_easy_perform(curl);

        std::cout << read_buffer << std::endl;

        /* Check for errors */
        if(res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));
     
        //always cleanup  
        curl_easy_cleanup(curl);
    }

    return(read_buffer);
}

std::string Murano::tryActivate(std::string serial_number, std::string product_id)
{
    CURL *curl;
    CURLcode res;

    std::string read_buffer;

    curl = curl_easy_init();
    if(curl) {
    
        //Set URL
        std::string URL = "https://";
        URL.append(product_id);
        URL.append(".m2.exosite.com/provision/activate");

        //Set Headers
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        headers = curl_slist_append(headers, "charset=utf-8");

        //Set Data to send
        std::string message = "vendor=";
        message.append(product_id);
        message.append("&sn=");
        message.append(serial_number);

        //verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        //Callback
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);

        //Make Call
        res = curl_easy_perform(curl);

        if(res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));

        curl_easy_cleanup(curl);
  }

  if(read_buffer.substr(0,4) == "HTTP"){
    std::cout << "Failed To Activate\n";
  }
  else
  {
    //open new file
    std::ofstream new_cik;
    new_cik.open("cik.txt");
  }

  return(read_buffer);
}