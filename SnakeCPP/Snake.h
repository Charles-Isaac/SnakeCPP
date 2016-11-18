#pragma once
#include "stdafx.h"
struct	Snake
{
	POINT* m_Position;
	int m_IdexTete;
	int m_Vitesse;


	Snake()
	{
		m_Position = new POINT[5]{ 0 };
		m_IdexTete = 0;
		m_Vitesse = 100;
	}
	Snake(int Vitesse) :Snake()
	{
		m_Vitesse = Vitesse;
	}
	Snake(int Vitesse, int Longueur)
	{
		m_Position = new POINT[Longueur]{ 0 };
		m_IdexTete = 0;
		m_Vitesse = Vitesse;

	}


};