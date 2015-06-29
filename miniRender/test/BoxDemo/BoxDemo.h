/*
Copyright © 2015 Ziwei Wang

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the “Software”), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __TEST_BOX_DEMO_H__
#define __TEST_BOX_DEMO_H__

#include "cwMiniRender.h"
NS_USING_MINIR;

//#include "Base/cwWinMain.h"

//class BoxDemo : public cwWinMain
class BoxDemo : public cwApplication
{
public:
	BoxDemo(/*HINSTANCE hInstance, int iShowCmd*/);
	virtual ~BoxDemo();

	virtual void gameBegin() override;
	virtual void gameEnd() override;

	virtual void gameBeginBackGround() override;
	virtual void gameEndBackGround() override;

protected:
//	virtual void initAll() override;
	
	void buildEntity();
	void buildAxis();
	void buildScene();

protected:
	cwEntity* m_pEntityBox01;
	cwEntity* m_pEntityBox02;
	cwEntity* m_pEntityAxis;
	
	cwScene* m_pScene;

};

#endif
