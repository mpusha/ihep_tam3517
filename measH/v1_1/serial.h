#ifndef SERIAL_H
#define SERIAL_H
#include <QString>
class   SerialGate
{	
public:
	
	SerialGate();
	~SerialGate();	

    bool Open(QString port, int baud);
	int Send(char* buff, int szBuff);
	int Recv(char* buff, int szBuff);
	void Close();
	void Clean();
private:
	int fd;
	bool state;
};
#endif
