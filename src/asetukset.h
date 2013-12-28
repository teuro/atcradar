#ifndef __ASETUKSET_H
#define __ASETUKSET_H

#include <map>
#include <string>

class IAsetukset
{
public:
	virtual double anna_asetus(std::string asetus) = 0;
	virtual void lataa_asetukset(std::string nimi) = 0;
	virtual void muuta_asetusta(std::string asetus, double arvo) = 0;
};

class Asetukset : public IAsetukset
{
public:
	Asetukset();
	double anna_asetus(std::string asetus);
	void lataa_asetukset(std::string nimi);
	void muuta_asetusta(std::string asetus, double arvo);
private:
	std::map <std::string, double> asetukset;
};

#endif