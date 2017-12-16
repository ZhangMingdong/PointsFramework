#include "Interpolater1D.h"
#include "Interpolater1DShepards.h"


Interpolater1D::Interpolater1D(std::vector<DPoint3> vecSamples, double dbP):_vecSamples(vecSamples)
,_dbP(dbP)
{
}


Interpolater1D::~Interpolater1D()
{
}
Interpolater1D* Interpolater1D::CreateInterpolater(Enum_Type type, std::vector<DPoint3> vecSamples, double dbP) {
	switch (type)
	{
	case Interpolater1D::IT_Shepards:
		return new Interpolater1DShepards(vecSamples, dbP);
		break;
	default:
		break;
	}
}