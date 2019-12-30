#ifndef SERIAL_H
#define SERIAL_H

class   SerialGate
{	
public:
	
	SerialGate();
	~SerialGate();	

	bool Open(int port, int baud);
	int Send(char* buff, int szBuff);
	int Recv(char* buff, int szBuff);
	void Close();
	void Clean();
private:
	int fd;
	bool state;
};
#endif
