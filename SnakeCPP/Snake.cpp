#include "stdafx.h"
#include "Snake.h"
int Snake::LongueurDuSerpent()
{
	return sizeof(m_Position) / sizeof(POINT);
}
void Snake::Allonger(int Longueur)
{
	int LongueurDuSerpentCourant = LongueurDuSerpent();
	int i = 0;
	POINT* NewSnake = new POINT[LongueurDuSerpentCourant + 1];
	while (i + m_IdexTete < LongueurDuSerpentCourant)
	{
		NewSnake[i] = m_Position[i + m_IdexTete];
		i++;
	}
	i = 0;
	while (i < m_IdexTete)
	{
		NewSnake[i + m_IdexTete] = m_Position[i];
		i++;
	}
	NewSnake[i + m_IdexTete] = m_Position[i];
	m_IdexTete = 0;
	delete(m_Position);
	m_Position = NewSnake;
}

void Snake::Update(char DirectionDuDeplacement)
{
	//do fuck all
}