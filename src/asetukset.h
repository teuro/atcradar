#include <map>
#include <string>

static class Asetukset
{
public:
	static std::map <std::string, double> asetukset;
	static double anna_asetus(std::string asetus);
	static void lataa_asetukset(std::string nimi);
};
