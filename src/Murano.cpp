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

std::string Murano::postData(std::string serial_number, std::string data)
{

    CURL *curl;
    CURLcode res;

    std::string read_buffer;

    curl = curl_easy_init();
    if(curl) {
    
        //Set URL
        std::string URL = "http://m2.exosite.com/onep:v1/stack/alias?state";

        //Set Headers
        struct curl_slist *headers = NULL;
        std::string cik_header = "X-Exosite-CIK: ";
        cik_header.append(cik_map[serial_number]);
        headers = curl_slist_append(headers, cik_header.c_str());
        headers = curl_slist_append(headers, "Accept: application/x-www-form-urlencoded");
        headers = curl_slist_append(headers, "charset=utf-8");

        //Set Data to send
        std::string message = "test=";
        message.append(data);

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

    //Make file name
    std::string file_name = product_id;
    file_name.append("_cik.txt");

    std::cout << "Checking for CIK...\n";
    std::ifstream cik_file(file_name);

    if(cik_file.good()){
        std::cout << "CIK Found!\n";
        std::string cik;
        std::getline(cik_file,cik);
        cik_map[product_id] = cik;
    }
    else{
        std::cout << "CIK not found, trying activation\n";
    }

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
        message.append("&model=");
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

    if(read_buffer.find("404") == std::string::npos && read_buffer.find("HTTP") == std::string::npos){
        std::cout << read_buffer << "\n";
        std::cout << "Failed to Activate, check your product ID.\n";
        return("Failed");
    }
    else if(read_buffer.find("409") == std::string::npos && read_buffer.find("HTTP") == std::string::npos)
    {
        std::cout << read_buffer << "\n";
        std::cout << "Activation Failed and No CIK Found.\n";
        return("Failed");
    }
    else if(read_buffer.find("HTTP") == std::string::npos){
        //open new file
        std::ofstream new_cik;
        new_cik.open(file_name);
        cik_map[product_id] = read_buffer;
    }

    return("Success");
}