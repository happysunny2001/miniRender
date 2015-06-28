﻿/*
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

#include "cwRunTime.h"
#include "Timer/cwTimer.h"
#include "cwStringConvert.h"
#include "Repertory/cwRepertory.h"

NS_MINI_BEGIN

cwRunTime& cwRunTime::getInstance()
{
	static cwRunTime runTime;
	return runTime;
}

cwRunTime::cwRunTime() :
m_pTimer(nullptr)
{
	
}

cwRunTime::~cwRunTime()
{

}

void cwRunTime::init()
{
	m_pTimer = new cwTimer();
	m_pTimer->reset();

	m_strWinTitle = cwStringConvert::convertToWideChar("mini render ");
}

void cwRunTime::setWindowTitle(const CWWSTRING& strTitle)
{
	m_strWinTitle = strTitle;
}

void cwRunTime::showFPS()
{
//	cwRepertory::getInstance().refreshWindowTitle(cwStringConvert::createFPSString(m_strWinTitle, m_pTimer->fps()).c_str());
}

NS_MINI_END
