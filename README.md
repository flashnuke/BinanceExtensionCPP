

BinanceExtensionCPP
=

# Intro 

This library is an extension for the API of Binance. It is designed for writing trading algorithms on Binance. 
<br />The design is delibaretly attempting to reduce execution time during runtime, by using methods such as CRTP and the `Params` object; no virtual classes/methods are used in this library.

# Dependencies

1. [JsonCPP](https://github.com/open-source-parsers/jsoncpp) (latest version tested: **jsoncpp 1.9.3**) 
2. [CURL](https://curl.haxx.se) (latest version tested: **curl 7.72.0**) 
3. [Boost/Beast websockets](https://github.com/boostorg/beast) (latest version tested: **boost-1.73.0**) 

These 3 must be installed in order to use the library.


# Documentation
In order to use this library, you must have all dependencies installed. Only one `#include` statement is required - `#include "include/Binance_Client.h"`, and add all content of `/src` directory should be added to the Source files. 
<br />Note that `.inl` files are included inside the main header.
<br /> <br />
<br />You must initialize a `Client` object, which is one of the following: *[SpotClient, FuturesClientUSDT, FuturesClientCoin, OpsClient]*
### Unique methods
Many endpoints are divided by category into different structs inside `Client`. In order to use them, you must instantiate a struct object first. 
- Wallet 
- FuturesWallet
- SubAccount
- MarginAccount
- Savings
- Mining
- BLVT
- BSWAP
- Fiat
- C2C

<br>They should be initialized from within other Client classes, and by passing the Client object to the constructor.					
i.e:
 >SpotClient::Wallet my_wallet{ my_client_obj }. 
 
## Exchange client
In order to initialize a client that is not public, api-key and api-secret must be passed in `std::string` format to the constructor.
> FuturesClientUSDT(api_key, api_secret)
> 
<br />Futures and Options clients may be set in testnet mode by using the method "set_testnet_mode(bool)". SpotClient has 'test_new_order' method but no testnet mode endpoints.

- #### CRTP implementation
	The CRTP is implemented as follows:
    <br />
    <br />└── Client
    <br />   ├── SpotClient
    <br />   │
    <br />   ├── FuturesClient
    <br />   │ │
    <br />   │ ├── FuturesClientUSDT
    <br />   │ └── FuturesClientCoin
    <br />   │
    <br />   └── OpsClient
    <br /> 
    <br /> 
    As was mentioned earlier, unique endpoints are located as separate structs inside Client.
	<br /> CRTP interface and implementations are separated inside `Binance_Client.cpp`. Generally speaking, implementations are marked by a `v_` prefix.
    <br /> Deeper implementations (i.e `Client` -> `FuturesClient` -> [`FuturesClientUSDT`, `FuturesClientCoin`]) include an additional underscore: `v__`.
- #### Exceptions
	The library contains several exception classes, all of which derive from base class `ClientException`.
    <br />
    <br />
    <br />└── ClientException
    <br />   ├── BadSetupSessionREST ***(error in setting up a REST session)***
    <br />   ├── BadRequestREST ***(error in sending a REST request)***
    <br />   ├── BadCleanupREST ***(error in REST client destructor)***
    <br />   ├── BadSetupHeadersREST ***(error in setting up headers)***
    <br />   │
    <br />   ├── BadStreamOpenWS ***(error in opening a websocket stream)***
    <br />   ├── BadStreamCloseWS ***(error in closing a websocket stream)***
    <br />   ├── BadStreamCallbackWS ***(error in callback from websocket stream message)***
    <br />   ├── BadSetupPathWS ***(error in setting up host / port)***
    <br />   │
    <br />   ├── BadQuery ***(error in generating query)***
    <br />   ├── MissingCredentials ***(missing keys)***
    <br />   ├── MissingEndpoint ***(missing endpoint for a specific client)***
    <br />   │
    <br />   └── CustomException ***(a custom exception. description in `what()`)***
    <br />
    <br />ClientException base class contains a `what()` method which returns a `c-string` which is a 
    description of the problem and a traceback.
    <br />Each time an exception is thrown and a function inside the library catches it, the name of the method will be
     appended to the traceback using the `__FUNCTION__` macro and later on also added to `what()` `c-string`.
    <br />
    <br />The main idea is to `catch(ClientException& e)` in order handle any exception which is one of the aforementioned,
    since they are all derived. It is also possible to `catch()` a specific exception.
    
- #### Notes
		1. No copy assignment / constructor are implemented for Client classes. Each object has its own unique Session, WS, and running streams.
		2. All unique endpoint structs require that the client object contains keys and is not a public client.
		3. `ClientException` derives from `std::exception` therefore `catch(std::exception)` would also work.

## REST client
All (except for ones that don't have mandatory parameters) REST request methods take a pointer to a `Params` object. This object holds the parameters that would be generated to a query string and sent as the request body.
<br /> Endpoints that do not require any params, have a default argument which is a `nullptr` (beware if using threads). 
<br />* Signing requests is done after generating the query, and the `Params` object remains unchanged.
- #### 'Params' object
	The `Params` object holds all parameters in an `unordered_map`, and thus allows quick access and modification during runtime. The idea here is to prepare most of the request body and have it ready at all time. (**i.e: have the side and quantity ready at all times. Price may be set on signal, using the time complexity of `unordered_map` insertion**)
	<br />You can set  or delete parameters from the object using the methods `set_param<type>()` and `delete_param()`. Using`flush_params()` method will delete all params from the object.
	<br />It is also possible to set a default `recvWindow` value that would be set again after each flush, using the `set_recv()` method.
- #### Response type
	Each REST request returns a JSON type objet, that holds the three following keys:
    1. "response" = Actual response from the exchange in std::string format
    2. "request_status" = Request status (bool; 1 success 0 failure)
    3. "parse_status" = Returns a string generated by the JSON parser, containing parsing errors (if any)
    
- #### Custom Requests
	You can send a custom request, using `custom_{request_type}_req() method`.
	<br /> This method accepts four arguments: `base_path` std::string, `endpoint`std::string, `Params` object, and `sign_request` bool (true = append signature to request body).
- #### Debugging
	You can set verbose mode for debugging, which will make all rest requests verbose. `my_client.rest_set_verbose(1);`
- #### Notes
   		 1. There are four curl handles that are alive throughout the entire life of a RestSession object, one for each request type: PUT, DELETE, POST, GET.
   		 2. In order to avoid race conditions by using the same handle at the same time for different requests, mutex is used.
		 3. Passing an `std::string` format to `set_param()` method is quicker, because conversion-to-string is required.
		 4. Passing `const char*` to `set_param()` is currently not supported, please use `std::string`, as shown in the examples.

## Websocket client
Each time a client object is created, a websocket client is also instantiated. In fact, the websocket client accepts the Client object as an argument.

<br /> The websocket client holds a map of all stream connection names and their current status. **symbol@stream_name** (i.e: btc@aggTrade). This is very crucial to know in order to be able to close a stream by using the `close_stream()` method.
<br />Not all streams accept the same arguments list, but all of them accept an std::string buffer and a functor object to use as callback.

- #### Callback functor
	All streams accepts a reference to std::string buffer and a reference to a functor object. This is implemented using templates, therefore the template type of the stream, when called, should be the type of the functor object. 
	>client_obj.stream_aggTrade<typename SomeFunctor\>(symbol, buff, functor_obj)

	<br /> It would be good practice to set the buffer as a member of the functor object.
- #### Stream Manager
	The WebsocketClient class has a `stream_manager` method, which is responsible for the stream connection. It is possible to set `reconnect_on_error` by using Client's `ws_auto_reconnect()` method, and also specify the number of attempts by using `set_max_reconnect_count()` method.
<br />The `stream_manager` method closes a stream when the stream status is set to zero by the `close_stream()` method, or if any other error was encountered (unless `reconnect_on_error` is true).
<br /> The stream manager also accepts a `bool` for whether or not to ping a listen key periodically. The ping interval is 30 minutes by default, and can be set using `set_refresh_key_interval()` method (in `Client`).
- #### Custom Streams
	Custom streams are possible by using `Client`'s `custom_stream()` method. This method accepts 4 arguments: `stream_path` std::string, `buffer` std::string, `functor` functor, and `ping_listen_key` bool (when true, pings listen key periodically).

- #### Notes
		1. Default arguments are not allowed with threads. The argument must be specified
		2. When passing a symbol as an argument to a stream starter, the symbol must be lower case.
		3. For options client streams, conversion from gzip to binary is performed when the stream is set up.
		4. For all options websocket streams, only `interval = 0` is supported. Otherwise, no stream will be established

### Compilation & Optimizations
For Microsoft compilers (Visual Studio) set the following flags for better runtime performance:
* Optimization: /O2
* Instrinsic functions: /Oi
* Favor speed: /Ot
* Frame pointers: /Oy (if available)

If compiling on Linux, make sure you have the following dependencies installed:
* sudo apt-get install libboost-all-dev
* sudo apt-get install libjsoncpp-dev
* sudo apt-get install libcurl4-openssl-dev
* sudo apt-get install libssl-dev

If compiling on MacOS, install the dependencies using Homebrew:
* brew install boost
* brew install jsoncpp
* brew install openssl

Note for compiling on MacOS, if during compilation an error of missing `openssl` files is encountered, add the following flags to the compilation line: ` -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include`

</br >and use the following command to compile: `g++ -std=c++11 src/* -o3 -lssl -lcrypto -lcurl -lpthread -ljsoncpp -o a.out` <br />
Make sure your `.src` file that includes the `main()` function is inside `/src`. <br />
If the following error is encountered: `fatal error: json/json.h: No such file or directory`, you should change `#include <json/json.h>` to `#include <jsoncpp/json/json.h>` inside `include/Binance_Client.h`.

# Examples
* `orderbook_manager.cpp` = Connecting to a symbol orderbook and fetching live data + getting initial snap via REST 
* `custom_get_request.cpp` = Sending a user custom request.
* `place_test_order_spot.cpp` = Placing a test order for SpotClient.
* `place_testnet_order_futures.cpp` = Using 'testnet_mode' to place a test order for FuturesClient.
* `client_wallet_methods.cpp` = Initializing a 'Wallet' objects (Similar for `MarginAccount`, `FuturesWallet`, `SubAccount`, `Savings`, `Mining`)
* `custom_ws_stream.cpp` = Starting a custom stream
* `fetch_ticker_rest.cpp` = Fetching a ticker once using REST
* `options_trade_stream.cpp` = Connecting to "Trades" stream in Vanilla Options
* `futures_user_stream.cpp` = Start a userstream for Futures account



# Release Notes

### Latest version release - 2.3
Please see release notes here https://github.com/adanikel/BinanceExtensionCPP/releases/

**Latest changelog updates: `2021-27-09` (Binance API Changelogs have been implemented up to this date)**


# Links
* [CRTP Method - Info](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)
* [Binance API Documentation](https://binance-docs.github.io/apidocs/)
