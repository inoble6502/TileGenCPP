// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectStructsCPP.h"

bool FDCoords::operator==(const FDCoords& Pos) const
{
	return (this->CoordX == Pos.CoordX) && (this->CoordY == Pos.CoordY);
}
