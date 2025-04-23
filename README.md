# CoreHttpServer 
This is a HTTP Server, It has full of http request controls . It can be use with https(openssl).

## Dependencies install

			sudo apt-get update
			sudo apt-get install -y build-essential make cmake qtbase5-dev qt5-qmake git libssl1.0-dev

## Compile/build and Run

comiple with anyone (Cmake/Qmake)

1. Cmake build

		git clone https://github.com/aks-arise1600/CoreHttpServer.git
		cd CoreHttpServer
		mkdir cBuild && cd cBuild
		cmake ..
		make

2. Qmake build

		git clone https://github.com/aks-arise1600/CoreHttpServer.git
		cd CoreHttpServer
		mkdir qBuild && cd qBuild
		qmake ..
		make
		
3. Run

		./CoreHttpServer
		
* Open web-browser http://localhost:8190/ or http://localhost:8190/maths?
* Open web-browser https://localhost:8193/ or https://localhost:8193/maths? 
* Open web-browser https://corehttpserver.onrender.com/ or https://corehttpserver.onrender.com/maths?MULTI=45,200,800


### To start http
Edit main.cpp & re-build
	
	obj.m_Start();
	//obj.m_StartSecure();
    	
### To start https
Edit main.cpp & re-build
	
	//obj.m_Start();
	obj.m_StartSecure();

## Home Page (HTTP)

![alt text](https://github.com/aks-arise1600/CoreHttpServer/blob/main/docs/Screenshots/Screenshot000.png?raw=true)


## ADDITION EXAMPLE

![alt text](https://github.com/aks-arise1600/CoreHttpServer/blob/main/docs/Screenshots/Screenshot001.png?raw=true)


## MULTIPLICATION EXAMPLE

![alt text](https://github.com/aks-arise1600/CoreHttpServer/blob/main/docs/Screenshots/Screenshot002.png?raw=true)


## ERROR EXAMPLE

![alt text](https://github.com/aks-arise1600/CoreHttpServer/blob/main/docs/Screenshots/Screenshot003.png?raw=true)


## Home Page (HTTPS)

![alt text](https://github.com/aks-arise1600/CoreHttpServer/blob/main/docs/Screenshots/Screenshot004.png?raw=true)



## References

* [httplib](https://github.com/aks-arise1600/CoreHttpServer/blob/main/docs/Readme_opt.md)

