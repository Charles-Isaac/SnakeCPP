
#include "stdafx.h"
#include "Snake.h"
/*
int Snake::LongueurDuSerpent()
{
return sizeof(m_Position) / sizeof(Point);
}*/
void Snake::Allonger(int Longueur = 1)
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
	case 'W':
		if (m_Direction != 'S')
		{
			m_Direction = DirectionDuDeplacement;
		}
		break;
	case 'A':
		if (m_Direction != 'D')
		{
			m_Direction = DirectionDuDeplacement;
		}
		break;
	case 'S':
		if (m_Direction != 'W')
		{
			m_Direction = DirectionDuDeplacement;
		}
		break;
	case 'D':
		if (m_Direction != 'A')
		{
			m_Direction = DirectionDuDeplacement;
		}
		break;
	default:
		break;
	}
	switch (m_Direction)
	{
	case 'W':
		(*(m_Position + m_IndexTete)).Y -= m_Vitesse;
		break;
	case 'A':
		(*(m_Position + m_IndexTete)).X -= m_Vitesse;
		break;
	case 'S':
		(*(m_Position + m_IndexTete)).Y += m_Vitesse;
		break;
	case 'D':
		(*(m_Position + m_IndexTete)).X += m_Vitesse;
		break;
	default:
		throw("Direction invalide");
		//break;
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
bool Snake::Update(Point WindowSize)
{
	//a = gauche, w = haut , d = droit , s = bas



	*(m_Position + ((m_IndexTete + 1) % m_LongueurDuSerpentCourant)) = *(m_Position + m_IndexTete);
	m_IndexTete = (m_IndexTete + 1) % m_LongueurDuSerpentCourant;


	switch (m_Direction)
	{
	case 'W':
		(*(m_Position + m_IndexTete)).Y -= m_Vitesse;
		break;
	case 'A':
		(*(m_Position + m_IndexTete)).X-= m_Vitesse;
		break;
	case 'S':
		(*(m_Position + m_IndexTete)).Y += m_Vitesse;
		break;
	case 'D':
		(*(m_Position + m_IndexTete)).X+= m_Vitesse;
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