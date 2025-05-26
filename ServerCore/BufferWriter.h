// BufferWriter.h
#pragma once

/*----------------
	BufferWriter
-----------------*/

class BufferWriter
{
public:
	BufferWriter();
	BufferWriter(BYTE* buffer, uint32 size, uint32 pos = 0);
	~BufferWriter();

	BYTE* Buffer() { return _buffer; }
	uint32			Size() { return _size; }
	uint32			WriteSize() { return _pos; }
	uint32			FreeSize() { return _size - _pos; }

	template<typename T>
	bool			Write(T* src) { return Write(src, sizeof(T)); }
	bool			Write(void* src, uint32 len);

	template<typename T>
	T* Reserve();

	/* 강의 사용 예시
		BufferWriter bw(sendBuffer->Buffer(), 4096);

		PacketHeader* header = bw.Reserve<PacketHeader>();
		// id(uint64), 체력(uint32), 공격력(uint16)
		bw << (uint64)1001 << (uint32)100 << (uint16)10;
		// bw.operator<<((uint64)1001).operator<<((uint32)100).operator<<((uint16)10);와 같다.
		bw.Write(sendData, sizeof(sendData));
	*/
	//template<typename T>
	//BufferWriter& operator<<(const T& src); // 왼값 참조

	template<typename T>
	BufferWriter& operator<<(T&& src); // 오른값 참조 

private:
	BYTE* _buffer = nullptr;
	uint32			_size = 0;
	uint32			_pos = 0;
};

template<typename T>
T* BufferWriter::Reserve()
{
	if (FreeSize() < sizeof(T))
		return nullptr;

	T* ret = reinterpret_cast<T*>(&_buffer[_pos]);
	_pos += sizeof(T);
	return ret;
}

//template<typename T>
//BufferWriter& BufferWriter::operator<<(const T& src)
//{
//	*reinterpret_cast<T*>(&_buffer[_pos]) = src;
//	_pos += sizeof(T);
//	return *this;
//}

// 얘는 보편참조인데? 다음 강의에서 오른값 참조로 바꿔줄거임
template<typename T>
BufferWriter& BufferWriter::operator<<(T&& src)
{
	*reinterpret_cast<T*>(&_buffer[_pos]) = std::move(src);
	_pos += sizeof(T);
	return *this;
}