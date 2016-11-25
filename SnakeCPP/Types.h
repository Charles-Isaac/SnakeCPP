#pragma once

class Point {
private://does not work
	int m_X, m_Y;

public:

	__declspec (property (put = SetFunction, get = GetFunction))
		int X;
	void SetFunction(int nInputData)
	{
		
			m_X = nInputData;
	}

	int GetFunction() const
	{
		return m_X;
	}


	__declspec (property (put = SetY, get = GetY))
		int Y;
	void SetY(int nInputData)
	{
		
			m_Y = nInputData;
	}

	int GetY() const
	{
		return m_Y;
	}



	Point::Point(int x = 0, int y = 0) {
		m_X = x;
		m_Y = y;
	}
	

};