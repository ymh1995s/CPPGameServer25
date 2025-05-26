#pragma once

/*----------------
	BufferReader
-----------------*/

class BufferReader
{
public:
	BufferReader();
	BufferReader(BYTE* buffer, uint32 size, uint32 pos = 0);
	~BufferReader();

	BYTE* Buffer() { return _buffer; }
	uint32			Size() { return _size; }
	uint32			ReadSize() { return _pos; }
	uint32			FreeSize() { return _size - _pos; }

	template<typename T>
	bool			Peek(T* dest) { return Peek(dest, sizeof(T)); }
	bool			Peek(void* dest, uint32 len);

	template<typename T>
	bool			Read(T* dest) { return Read(dest, sizeof(T)); }
	bool			Read(void* dest, uint32 len);

	// T& dest : 참조 형식으로 전달
	// BufferReader& : BufferReader의 참조 형식을 반환
	/* 강의 ex.
		BufferReader br(buffer, len);

		PacketHeader header;
		br >> header;
		★★★ br.operator>>(header) 이렇게 호출된다고 이해하면 됨 ★★★

		uint64 id;
		uint32 hp;
		uint16 attack;
		br >> id >> hp >> attack;
		// br.operator>>(id).operator>>(hp).operator>>(attack);

		cout << "ID: " << id << " HP : " << hp << " ATT : " << attack << endl;
	*/
	template<typename T>
	BufferReader& operator>>(OUT T& dest); // 연산자 오버로딩

private:
	BYTE* _buffer = nullptr; // unsigned char 시작 주소
	uint32			_size = 0;
	uint32			_pos = 0;
};

template<typename T>
inline BufferReader& BufferReader::operator>>(OUT T& dest)
{
	// reinterpret_cast : _buffer[_pos](BYTE 기본 자료형)을 어떤 구조체나 객체 타입으로 해석하기 위해 강제 변환
	// static_cast : 컴파일러가 타입간 관계를 이해할 수 있을 때 사용 (메모리 구조 자체를 변환할 때는 사용 불가)
	// dynamic_cast : RTTI 기반으로 virtual 등 특수한 상태에서만 사용가능하며 비용이 큼 
	// *reinterpret_cast... 역참조를 사용하여 T* 포인터가 가르키는 실제 값을 가져옴
	dest = *reinterpret_cast<T*>(&_buffer[_pos]);
	_pos += sizeof(T);
	return *this;
}