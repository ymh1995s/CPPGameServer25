// SendBuffer.cpp
#include "pch.h"
#include "SendBuffer.h"

/*----------------
	SendBuffer
-----------------*/

SendBuffer::SendBuffer(int32 bufferSize)
{
	_buffer.resize(bufferSize);
}

SendBuffer::~SendBuffer()
{
}

// 외부 데이터를 버퍼에 복사하는 함수
void SendBuffer::CopyData(void* data, int32 len)
{
	ASSERT_CRASH(Capacity() >= len); // 복사할 데이터가 버퍼 용량을 초과하지 않는지 확인
	::memcpy(_buffer.data(), data, len); // 데이터 복사
	_writeSize = len; // 현재 작성된 데이터 크기를 갱신
}

void SendBuffer::Close(uint32 writeSize)
{
	_writeSize = writeSize;
}
