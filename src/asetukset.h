#include <map>
#include <string>

class Asetukset
{
public:
	static std::map <std::string, double> asetukset;
	static double anna_asetus(std::string asetus);
	static void lataa_asetukset(std::string nimi);
	static void muuta_asetusta(std::string asetus, double arvo);
};
