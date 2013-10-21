class ajastin {
private:
	std::string nimi;
	int aika;
	int vali;
public:
	ajastin(std::string nimi, int vali);
	bool onko_valmis(int leima);
	void aseta_vali(int vali);
}
