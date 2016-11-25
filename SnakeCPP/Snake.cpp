
#include "stdafx.h"
#include "Snake.h"
/*
int Snake::LongueurDuSerpent()
{
	return sizeof(m_Position) / sizeof(Point);
}*/
void Snake::Allonger(int Longueur = 0)
{
	
	
	
	Point *NewSnake = new Point[m_LongueurDuSerpentCourant + Longueur];
	
	NewSnake[0] = m_Position[m_IndexTete];
	Point* p = m_Position;
	for (int i = 0; i < Longueur; i++)
	{
		NewSnake[i + 1] = m_Position[(m_IndexTete + 1) % m_LongueurDuSerpentCourant];
	}
	m_Position = p;
	for (int i = 1; i < m_LongueurDuSerpentCourant; i++)
	{
		NewSnake[i + Longueur] = m_Position[(m_IndexTete + i) % m_LongueurDuSerpentCourant];

	}

	m_LongueurDuSerpentCourant += Longueur;
	
	m_IndexTete = 0;
	delete[] m_Position;
	
	m_Position = NewSnake;
	//delete[] NewSnake;
}

bool Snake::Update(char DirectionDuDeplacement, Point WindowSize)
{
	//a = gauche, w = haut , d = droit , s = bas
	
	
	
	


    *(m_Position + ((m_IndexTete + 1) % m_LongueurDuSerpentCourant)) = *(m_Position + m_IndexTete);
	m_IndexTete = (m_IndexTete + 1) % m_LongueurDuSerpentCourant;

	
	switch (DirectionDuDeplacement)
	{
	case 'w':
		(*(m_Position + m_IndexTete)).Y--;
		break;
	case 'a':
		(*(m_Position + m_IndexTete)).X--;
		break;
	case 's':
		(*(m_Position + m_IndexTete)).Y++;
		break;
	case 'd':
		(*(m_Position + m_IndexTete)).X++;
		break;
	default:
		(*(m_Position + m_IndexTete)).X = 0;
		(*(m_Position + m_IndexTete)).Y = 0;
		break;
	}


	if ((*(m_Position + m_IndexTete)).Y > WindowSize.Y || (*(m_Position + m_IndexTete)).Y < 0 || (*(m_Position + m_IndexTete)).X > WindowSize.X || (*(m_Position + m_IndexTete)).X < 0)
	{
		return true;
	}
	for (auto i = 0; i < m_LongueurDuSerpentCourant; i++)
	{
		if ((*(m_Position + m_IndexTete)).Y == (*(m_Position + i)).Y && (*(m_Position + m_IndexTete)).X == (*(m_Position + i)).X && i != m_IndexTete)
		{
			return true;
		}
	}
	return false;
}