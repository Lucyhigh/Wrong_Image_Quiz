#pragma once
#include"SingletonBase.h"
#define KEY_MAX 256

class KeyManager:public SingletonBase<KeyManager>
{
private:
	bitset<KEY_MAX> _KeyUp;
	bitset<KEY_MAX> _KeyDown;
public:
	HRESULT init(void);

	bool isOnceKeyDown(int key);
	bool isOnceKeyUp(int key);
	bool isStayKeyDown(int key);
	bool isToggleKey(int key);

	bitset<KEY_MAX> getKeyUp(void) { return _KeyUp; }
	bitset<KEY_MAX> getKeyDown(void) { return _KeyDown; }

	void setKeyUp(int key, bool state) { _KeyUp.set(key, state); }
	void setKeyDown(int key, bool state) { _KeyDown.set(key, state); }

	KeyManager(){}
	~KeyManager(){}
};

