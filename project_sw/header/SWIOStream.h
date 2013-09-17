#ifndef SWIOStream_h__
#define SWIOStream_h__


class SWOutputStream
{
public:
	virtual void writeBytes(byte* b, unsigned len) = 0;
	void writeByte(byte b) { write(&b,1); };
};

class SWInputStream
{
public:
	/* return : ���� ����Ʈ ��. ������ ���ٸ� -1 */
	virtual int readBytes(byte* b, unsigned len) = 0;
	int readByte() { byte b; read(&b,1); return (int)b; };
};

#endif // SWIOStream_h__