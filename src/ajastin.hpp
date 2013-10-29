#include <string>

class ajastin {
private:
	std::string nimi;
	int aika;
	int vali;
public:
	ajastin(std::string nimi, int vali, int aika);
	bool onko_valmis(int leima);
	void aseta_aika(int aika);
};
