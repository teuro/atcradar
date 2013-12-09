atcradar
========

Toimit lähestymislennonjohtajana. Tehtäväsi on saattaa saapuvat koneet laskuun ja nousevat koneet oikealle uloslentopisteelle. Pelissä yritetään simuloida oikeaa tutkaa mahdollisimman realistisesti. Eroja toki löytyy myös paljon. Näistä saa lähettää mielellään palautetta, saadaan pelistä vielä enemmän oikeanlainen.

Pelin mukana saat lähdekoodit, joten pelin edelleen kehittäminen on mahdollista. Peli tarvitsee SDL, SDL_ttf ja SDL_gfx kirjastot mukaan. Näiden kehitysversiot lisään jossakin vaiheessa mukaan, jotta kehitys olisi helppo aloitella.

Pelin kulku:

1. Aloitusruudusta valitse peli tai lopeta
2. Täytä ATIS tiedote valitsemalla kiitotiet nousuun ja laskuun, sekä siirtopinta. Taulukon sopivasta siirtopinnasta löydät vaikkapa wikipediasta
3. Kun ATIS on täytetty oikein peli alkaa. Aluksi peliin luodaan 3 kpl koneita. Osa on luultavasti lähteviä ja osa saapuvia. 
4. Tästä eteenpäin klikkaat vektoroitavaa konetta ja valitse funktionäppäimistä sopivan
4.1 F5 anna suunta tai navigointipisteen nimi
4.2 F7 anna koneelle nopeus solmuina
4.3 F8 anna konelle korkeus 
5. Vektoroi kone +- 30 asteen kulmassa, <= 3000 jalan korkeudella ja <= 200 solmun nopeudella lähestymiseen. Koneen ollessa oikeassa asemassa anna komento ILS, joka antaa koneelle käskyn aloittaa lähestyminen. Laskuselvitystä ei anneta, vaan kone laskeutuu itse.
6. Lähteviä koneita voit vektoroida, kun koneen korkeus ylittää 1200 jalkaa. Kone nousee kentän alkunousukorkeudelle ja oletuslähtösuuntaan.
7. Lähtevän koneen ollessa yli 5000 jalkaa voit antaa komennon DCT, jolla kone alkaa hakeutumaan itsxestään kohti uloslentopistettä.
8. Koneiden ympärillä oleva ympyrä on porrastusminimi, joka ei saa alittua, mikäöi koneiden korkeusero on alle 1000 jalkaa. 
9. Pelissä saat tehdä korkeintaan 4 porrastusvirhettä, jonka jälkeen 5. aiheuttaa pelin loppumisen.
10. Pelin saa loppumaanm painamalla ESC-näppäintä milloin vain. Tällöin ruutuun tulostetaan kaikkien koneiden alueelle tuloaika ja poistumisaika, joista lasketaan alueella oloaika sekä annettujen selvitysten määrä.

