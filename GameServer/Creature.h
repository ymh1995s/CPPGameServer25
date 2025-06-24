#pragma once
#include "Object.h"

// Creature 단계는 필요 없을 것 같아 상황 봐서 삭제
class Creature : public Object
{
public:
	Creature();
	virtual ~Creature();
};

