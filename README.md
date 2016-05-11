curl-asio
=========

**Here be dragons. Although this library generally works quite well, it is still being developed and has not been extensively tested.** You will probably be fine, but don't look at me when things catch fire.

This library makes use of libcurl's multi interface in order to enable easy integration into Boost.Asio applications.

* **simple interface** - Download and upload anything, synchronously or asynchronously, with just a few lines of code.
* **familiar** - If you have used libcurl in a C application before, you will feel right at home.
* **exceptions** - Libcurl errors throw exceptions. Integrates nicely with Boost.System's error_code class.
* **useful wrappers** - C++ interfaces for libcurl's easy, multi, form, share and string list containers. All setopt calls are wrapped for type safety.
* **source/sink concept** - Works nicely with Boost.Iostreams

Installation
------------
1. If not already done, install cURL and its header files
2. Clone this git repository. There are no tags or packages yet.
3. Run CMake and point it to cURL
4. `make && make install`

Example
-------

#include <curl-asio.h>
#include <iostream>
#include <fstream>
#include <curl/curlbuild.h>

std::set<curl::easy*> active_downloads;


void handle_download_completed(const asio::error_code& err, std::string url, curl::easy* easy)
{
	if (!err)
	{
		std::cout << "Download of " << url << " completed" << std::endl;
	}
	else
	{
		std::cout << "Download of " << url << " failed: " << err.message() << std::endl;
	}

	std::set<curl::easy*>::iterator itor = active_downloads.find(easy);
	if (itor != active_downloads.end())
	{
		active_downloads.erase(itor);
	}
}

bool show_process(curl::native::curl_off_t dltotal, curl::native::curl_off_t dlnow, curl::native::curl_off_t ultotal, curl::native::curl_off_t ulnow)
{
	system("cls");
	printf("%0.2g %%\n", dlnow*100.0 / dltotal);
	return true;
}

void start_download(curl::multi& multi, const std::string& url)
{
	curl::easy* easy= new curl::easy(multi);
	
	// see 'Use server-provided file names' example for a more detailed implementation of this function which receives
	// the target file name from the server using libcurl's header callbacks
	std::string file_name = url.substr(url.find_last_of('/') + 1);
	
	easy->set_url(url);
	easy->set_sink(std::make_shared<std::ofstream>(file_name.c_str(), std::ios::binary));
	easy->async_perform(std::bind(handle_download_completed, std::placeholders::_1, url, easy));
	easy->set_progress_callback(std::bind(show_process, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );

	active_downloads.insert(easy);
}

int main(int argc, char* argv[])
{
	// expect one argument
	if (argc != 2)
	{
		std::cerr << "usage: " << argv[0] << " url-list-file" << std::endl;
		return 1;
	}

	// this example program downloads all urls in the text file argv[1] to the current directory
	char* url_file_name = argv[1];
	
	// start by creating an io_service object
	asio::io_service io_service;
	
	// construct an instance of curl::multi
	curl::multi manager(io_service);
	
	// treat each line in url_file_name as url and start a download from it
	std::ifstream url_file(url_file_name);
	while (!url_file.eof())
	{
		std::string url;
		std::getline(url_file, url);
		start_download(manager, url);
	}
	
	// let Boost.Asio do its magic
	io_service.run();
	
	std::cout << "All downloads completed" << std::endl;
	
	return 0;
}



More examples, including one for curl-asio's [asynchronous interface](https://github.com/mologie/curl-asio/wiki/Asynchronous-interface), can be found in the [wiki](https://github.com/mologie/curl-asio/wiki) and the `examples` directory.

Todo
----

* Testing suite based on libcurl's tests
* API documentation, design documentation, more examples
* Support for transport schemes using UDP and incoming TCP sockets (active FTP)
* File upload streams
* string_list iterators

License
-------
Curl-asio is licensed under the same MIT/X derivate license used by libcurl.
