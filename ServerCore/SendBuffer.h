// SendBuffer.h
#pragma once

/*----------------
	SendBuffer
-----------------*/

class SendBuffer : enable_shared_from_this<SendBuffer>
{
public:
	SendBuffer(int32 bufferSize);
	~SendBuffer();

	BYTE* Buffer() { return _buffer.data(); }  // 버퍼의 데이터를 저장할 메모리 위치를 반환
	int32 WriteSize() { return _writeSize; }  // 현재까지 사용된 데이터의 크기를 반환
	int32 Capacity() { return static_cast<int32>(_buffer.size()); } // 버퍼의 전체 용량을 반환

	void CopyData(void* data, int32 len); // 버퍼에 복사
	void Close(uint32 writeSize);

private:
	vector<BYTE>	_buffer; // 데이터를 저장할 실제 버퍼
	int32			_writeSize = 0; // 현재까지 기록된 데이터의 크기
};