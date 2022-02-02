#include "GameNode.h"
struct checkBox
{
	RECT _check;
	bool _isBoxActive;
};
class MainGame : public GameNode
{
private:
	
	vector<checkBox*> _checkBox;
	vector<checkBox*>::iterator iter;
	Image* _bgStart1Image;	
	Image* _bgEnd1Image;	
	Image* _bgStart2Image;
	Image* _bgEnd2Image;
	Image* _checkImage;		
	Image* _endingImage;	
	
	int _alphaA; int _alphaB;
	int _alphaNum;

	int _count;			
	int _checkIndex;		
	int _bonusCount;	
	int _stageLevel;
	
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(HDC hdc);

	MainGame() {}
	~MainGame() {}
};