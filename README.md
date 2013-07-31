radar
=====

Virtuaalinen lennonjohtopeli Pelissä toimit tutkalennonjohtajana eri lentoasemilla. 
Tehtäväsi on saattaa kaikki koneet joko lähestymään tai ulosmenoportille riippuen onko kone 
tulossa vai lähdössä. Peliin on mahdollista luoda uusia lentoasemia melko helpolla. 

Lentoasema koostuu kahdesta tiedostosta nimi.txt ja nimi_pisteet.txt, joista ensimmäinen 
määrittelee lentoaseman paikan, nimen, korkeuden ja kiitotiet. Jälkimmäisessä määritellään 
sisään- ja uloslentopisteet. Pelin muuttujat on koottu asetukset.ini tiedostoon, 
joka on täysin muokattavissa, joten pelistä voi tehdä itselleen helpomman tai melko 
haastavankin. Tärkeimmät asetukset ovat vaadittavien käsittelyjen määrä, koneiden 
yhtäaikainen määrä ja arvontasykli.

Tiedostojen rakenne

tunnukset.txt
Tähän tiedostoon voit lisätä lentoyhtiötunnuksia. Ne ovat suurakkosin kirjoitettuja kolmen merkin pituisia.

asetukset.ini
ruutu_leveys 1100		Ruudun leveys pikseleinä
ruutu_korkeus 700		Ruudun korkeus pikseleinä
selvityskorkeus_ala 2000	Kuinka matalalle koneen voi selvittää
selvityskorkeus_yla 24000	Kuinka korkealle koneen voi selvittää
oikotie 5000			Kuinka korkealla koneelle voi antaa oikotien uloslentopisteelle
porrastus_vaaka 3		Kuika monta mailia koneiden vaakaporrastus on
porrastus_pysty 1000		Kuinka jalkaa koneiden pystyporrastus on
suunta_muutos 3			Kuinka monta astetta konee suunta muutuu sekunnissa
lahestymissuunta_muutos 6	Lähestyessä kone voi kaartaa hiukan nopeammin
korkeus_muutos 30		Kuinka monya jalkaa koneen korkeus muuttuu sekunnissa
nopeus_muutos 4			Kuinka monta solmua koneen nopeus muuttuu sekunnissa
matka_vaaka 100			Kuinka monta mailia ruudun alareuna on
selvitysnopeus_ala 160		Kuinka pienen nopeuden koneelle voi selvittää
selvitysnopeus_yla 250		Kuinka suuren nopeuden koneelle voi selvittää
koska_uusi_ala 90		Kuinka monen sekunnin kuluttua uusi kone aikaisntaan arvotaan
koska_uusi_yla 240		Kuinka monen sekunnin kuluttua uusi kone viimeistään arvotaan
vaadittavat_kasitellyt 20	Kuinka monta konetta pitää saada laskuun tai ulosmenopisteelle
maks_konemaara 10		Kuinka monta konetta alueella voi olla korkeintaan
lahestymispiste 10		kuinka monen mailin päässä on lähestymispiste
hidastuspiste 3			Kuinka monen mailin päässä on hidastuspiste
maks_porrastusvirhe 5		Kuinka monta kertaa porrastusminimi saa alittua
maks_lahestymisnopeus 200	Kuinka kovaa kone saa korkeintaan lähestyä
maks_lahestymiskorkeus 3000	Kuinka korkealta konee lähestyä
lahestymiskulma 30		Kuinka suuri saa koneen kulkusuunta erota kiitotien suunnasta
alkulahestymisnopeus 160	Kuinka kovaa koneen alkulähestymisnopeus on 
loppulahestymisnopeus 135	Kuinka paljon koneen loppulähestymisnopeus on
liukupolku 3			Millä liukukulmalla kone lähestyy kiitotietä
alkunopeus 160			Kuinka suuri on koneen kiihdytysnopeus ennen ilmaannousua
alkunousunopeus 225		Kuinka suuri on koneen kiipeämisnopeus
koska_metar 900			Kuinka monen sekunnin välein uusi metar arvotaan

