# Sprawozdanie nr. II z laboratoriów z optymalizacji kombinatorycznej
Kierunek: Bioinformatyka
Rok: II
Semestr: 3

Paweł Śmigielski (pawel.smigielski@student.put.poznan.pl) nr. indeksu: 155625
Natan Jabłoński (natan.jablonski@student.put.poznan.pl) nr. indeksu: 155621

# Opis Algorytmu
## Algorytm mrówkowy (Ant Colony Optimization)
Ostatecznie w celu ulepszenia algorytmu zostały wprowadzone pewne zmiany w stosunku do tego co zostało opisane w pierwszym sprawozdaniu - tzn. wciąż jest to algorytm mrówkowy, główne zasady działania się **nie zmieniły** :
* puszczane jest $n$ mrówek na graf
* mrówki zostawiają feromonowe ślady, które wyznaczają bardziej pożądaną ścieżkę dla kolejnych fal mrówek
* feromony parują z czasem
* Algorytm kończy działanie po określonej liczbie sekund (mniej więcej)
Te aspekty zostaną krótko omówione najpierw a potem zostaną omówione zmiany. Mrówki są puszczane falami. Liczba mrówek ($n$) puszczanych w danej fali jest parametrem zmiennym. Feromony są aplikowane dopiero po przejściu fali, przed wyjściem fali aplikowane jest parowanie. Ilość nałożonych feromonów przez mrówkę, jest taka sama na całej długości ścieżki, a siła feromonów jest równa $0,1 \cdot S_E$ gdzie $S_E$ uznajemy tutaj za funkcje celu, score ścieżki.
Graf po którym przechodzi algorytm jest zbudowany z wierzchołków w których każdy reprezentuje $k$-mer, podciąg z oryginalnej sekwencji, te podciągi nakładają się na siebie, występują w nich błędy pozytywne ($k$-mery, których nie ma w sekwencji). Wagi krawędzi reprezentują nałożenie na siebie podciągów, jeżeli poza pierwszym nukleotydem jednego końcówka pierwszego nakładane się cały początek drugiego poza ostatnim nukleotydem to przypisujemy wagę jeden, jeżeli poza pierwszymi dwoma nukleotydami jednego końcówka pierwszego nakłada się cały początek drugiego poza ostatnim nukleotydem to przypisujemy wagę dwa .itd. Aczkolwiek domyślnie przechowujemy jedynie wagi z zakresu od 1 do 3.
 
----
To co się zaś zmieniło to fakt, że zrezygnowaliśmy z wygładzania feromonów. Używaliśmy do tego wzoru:
$$C_{ij}=f_{0}\cdot(1+log_p \frac{f_{ij}}{f_0})$$
Nie zauważyliśmy, jednak by wygładzanie pomagało mrówkom w dużym stopniu, a generowało to błędy w postaci miejscami niecodziennymi wynikami powyższego wzoru jak np. "-nan" spowodowanymi przez np. wygładzanie feromonów o niskich wartościach (ułamkach). Problem można by obejść odpowiednio dostosowując parametry jak np. minimalną wartość feromonów i podstawę logarytmu, aczkolwiek uznaliśmy to za niewarte zachodu na tym etapie i skupiliśmy się na ulepszaniu innych części algorytmu. 

Jednym z owoców tej decyzji jest zastosowanie elityzmu, w przypadku wyboru które mrówki ostatecznie zostawiają feromony. Zostawia je jedynie n mrówek z najwyższym scorem - wpłynęło to w bardzo pożądany sposób na miarę Levenshteina odnajdywanych ścieżek. Sam system przyznawania score-u ścieżkom też uległ zmianie, właściwie całkiem drastycznej (przez score ścieżki mamy na myśli wartość funkcji celu dla tej ścieżki). Na początku system wyliczania score ścieżki (funkcja celu) wyglądała następująco: $$Si_E = \begin{cases}
\frac{\text{total path weight}  }{\text{model weight}} & \text{if model weight > total path weight  } \\
\frac{\text{model weight}}{\text{total path weight}  } & \text{in other case}
\end{cases}
$$
gdzie
$$\text{total path weight} = \Sigma_{x\in E} A_{x_1 x_2} $$
$$\text{model path weight} = {n-k}
$$

gdzie $Si_E$ ($i$ nie ma tutaj znaczenia służy tylko jako oznaczenie, można uznać za skrót od inferior) to score ścieżki w której skład wchodzą krawędzie w zbiorze $E$, $A_{ij}$ jest macierzą odległości, w której element na pozycji $ij$ oznacza wagę krawędzi od $i$-tego wierzchołka do $j$-tego, $n$ to długość sekwencji, a $k$ to długość $k$-meru. Tak jak algorytm wykorzystujący do przeszukiwania pickyAnt radził sobie z tą miarą całkiem całkiem, tak chaotic znalazł bardzo szybko sposób jak produkować wyniki z niemal perfekcyjnym scorem, ale o żałosnej miarze Levenshteina. W ostatecznej funkcji celu wprowadzona jest do $Si_E$ drobna modyfikacja, mianowicie:
$$\text{total path weight} = \Sigma_{x\in E} (A_{x_1 x_2})^2$$
Dzięki tej zmianie, algorytm dostaje większe kary za krawędzie o większych wagach. Do definicji ostatecznej funkcji celu przyda się jeszcze jeden element: 
$$Sl_E = \begin{cases}
\frac{\text{total path length}  }{\text{model path length}} & \text{if model path length > total path length  } \\
\frac{\text{model path length}}{\text{total path length}^2  } & \text{in other case}
\end{cases}
$$
gdzie
$$\text{total path length} =  |E| - 1 $$
$$\text{model path length} = {n-k+1}$$
Oznaczenia mają takie samo znaczenie jak we wzorach wyżej, bardzo istotną różnicą jednak jest "total path length" podniesione do kwadratu w drugim przypadku, służy to dodatkowym karaniem za zbyt długie i zbyt krótkie ścieżki (które na tamten moment algorytm chaoticAnt najwyraźniej uwielbiał). $l$ znowu jest tutaj tylko oznaczeniem. Ostateczna funkcja celu wygląda następująco:
$$S_E = Si_E \cdot Sl_E$$
Dzięki tej funkcji celu, algorytm jest mocno karany za złe ścieżki, stworzenie ścieżki o sumarycznych dużych wagach ale krótkiej, skutkowało małym scorem dzięki $Sl_E$. Generalnie ta funkcja celu bardzo zawęża to co algorytm jest w stanie nazwać "dobrą ścieżką". Co w połączeniu z elityzmem dało wspaniałe rezultaty. 

<div style="page-break-after: always;"></div>

## Algorytm poboczny - ruletka
Niektóre przedstawione algorytmy będą wykonywać pewne wariacje owej ruletki, aczkolwiek ogólny jej zamysł jest taki:
do ruletki ładowane jest ileś wartości typu float, przy czym każdy element ma wartość sumy wszystkich poprzednich + wartość elementu który reprezentuje. Losowana jest wartość pomiędzy 0 a ostatnią wartością w ruletce, dzięki temu kolejne wartości w ruletce są względem siebie niemalejące. Za pomocą binary searcha określa się w którym z przedziałów wyznaczonych przez elementy w ruletce zawiera się owa wartość - jeżeli wartość jest mniejsza od wartości na pozycji $i$ a większa od wartości na pozycji $i-1$ bądź 0 dla pierwszego elementu to uznane zostaje, że wartość mieści się w przedziale wyznaczanym przez element $i$ czyli została wybrana $i$-ta wartość w ruletce. Każdy indeks w ruletce odpowiada jakiejś innej wartości - indeksowi w innej strukturze czy numerowi wierzchołka.  

Celem algorytmu ruletki jest wylosowanie liczb z przedziału, z możliwością wpływania na prawdopodobieństwo z jakim dana liczba może zostać wybrana. Np. jeżeli mamy w ruletce reprezentacje 3 wartości o wagach: 1, 1.5, 1 to w ruletce będą wartości 1, 2.5, 3.5 - zatem jeżeli wylosujemy losową liczbę wymierną z przedziału od zera do 3.5 włącznie to największe szanse będą na ta liczba będzie w przedziale symbolizującym drugą wartość.

## Dwa algorytmy przechodzące graf
Jak wcześniej zostało już zasygnalizowane, udało się zaimplementować dwa algorytmy, którymi algorytm przechodzi graf, można o nich myśleć jak o dwóch rodzajach mrówek: chaoticAnt i pickyAnt. Jako pierwszy został stworzony ten wymieniony jako drugi i wydawał się z początku gorszą wersją, a ten wymieniony jako pierwszy miał być jej ulepszeniem. Trochę z braku czasu wyszło na odwrót.
### chaoticAnt
Miał być lepszą wersją pickyAnt stosującą poza normalnymi feromonami, jeszcze dwa specyficzne rodzaje:
chaosPheromones - feromony chaosu
endPheromones - feromony stopu
Oba są zaimplementowane, oba w przeciwieństwie normalnych feromonów miały się  zbierać się na wierzchołkach grafu, a nie na krawędziach. Ostatecznie jednak działają bardziej dynamicznie, nie są brane pod uwagę natężenia feromonów na wierzchołkach. Aczkolwiek spełniają swoją funkcje. 
Mrówka przechodząc graf, przechodzi od wierzchołka do wierzchołka kierując się wagą krawędzi - wybiera losowo z sąsiadów wierzchołka spośród jedynek, jeżeli tych nie ma jedynek za sąsiadów to dwójek, itd. Krawędź jest wybierana za pomocą ruletki, do wartości przez nią reprezentowaną w ruletce dodawane są feromony. Poza tym może jeszcze wykonać dwie akcje:
* zakończyć wędrówkę (endPheromones)
* skoczyć w losowe miejsce (chaosPheromones)
Szansa tych akcji jest zależna od wartości obu feromonów, przy czym wartość chaosPheromones jest zależna od endPheromones, a dokładnie jest to korelacja ujemna, kiedy  endPheromones rośnie, chaosPheromones spada - na końcu ścieżki nie ma sensu skakać. Szansa tych akcji jest zawsze wyliczana najpierw sumarycznie, z zachowaniem proporcji obu feromonów względem siebie, potem skalowane tak by na ich wybór było jedynie $x\%$ szans w ruletce, przy czym na sumarycznie inne możliwości będzie wtedy $100\% - x$ szans. endPheromones są wyliczane w następujący sposób:
$$eP = \begin{cases}
(\text{total path length}-0.9) \cdot 10 & \text{if total path length > $0,9$  } \\
0 & \text{in other case}
\end{cases}
$$
gdzie $eP$ oznacza tutaj procent szansy na zakończenie wędrówki mrówki
chaosPheromones są wyliczane w następujący sposób:
$$cP = (1-eP) \cdot \frac{minWeight}{1000}$$ gdzie minWeight oznacza krawędź o najmniejszej wadze, do której może przejść mrówka z danego wierzchołka. $cP$ zaś to szansa skoku.
Kluczową wersją dla działania tego algorytmu są także skoki. Jak widać w sposobie wyliczania szansy na skok, jest ona właściwie tym większa im większa 
jest minimalna waga do której mrówka może przejść. W przypadku gdy od wierzchołka nie można przejść dalej, wartość minPath jest ustawiana na 10. Wtedy mrówka skacze do losowo wybranego wierzchołka, oblicza jaką wagę powinna mieć krawędź do niego (może być wyższa niż 3) i dodaje są ją jako krawędź do grafu. W związku z tym, że raczej mrówka losowo wybierze wierzchołek o całkiem dużej wadze, to i szansa na skok pozostaje duża, a wraz z kolejnymi skokami zwiększa się szansa na wybranie lepszego wierzchołka. Nie jest to rozwiązanie idealne (oryginalny zamysł) ale jakoś działa. Przy czym tak w gruncie rzeczy potrzeba korzystania z takich skoków jest dość mało prawdopodobna, wymagałoby to ułożenia się obok siebie trzech lub więcej błędów negatywnych, oczywiście wraz z długością sekwencji szansa na takie wydarzenie rośnie. Mechanizm ma też małą szansę na ominięcie błędów pozytywnych, aczkolwiek relatywnie znikomą. ChaoticAnt ma największe szanse zakończenia działania im bardziej ścieżka zbliża się do pożądanej długości.
### pickyAnt
Algorytm pickyAnt jest w swoim działaniu znacznie prostszy - w gruncie rzeczy działa w dwóch etapach:
najpierw przechodzi graf, jeżeli może to idzie po krawędziach o wadze jeden, jeżeli jest ich parę to wybiera jedną z nich za pomocą ruletki ( do wartości reprezentującą ją dodawane są feromony.), jeżeli nie jest w stanie przejść krawędzią o wadze jeden to zapamiętuje odwiedzony fragment i tworzy kolejny od losowego wierzchołka. Przemieszcza się znowu po jedynkach i robi to aż przejdzie cały graf. Jeżeli okaże się z jakiegoś wierzchołka, może przejść krawędzią do wadze 1 do wierzchołka już odwiedzonego ale będącego początkiem innego fragmentu, to dwa fragmenty są łączone.
Po przejściu całego grafu, algorytm zostaje z fragmentami, znane są wszystkie wierzchołki w fragmentach ich początek i koniec. Na tym etapie można znaleźć fragmenty, które z dużym prawdopodobieństwem mogą być błędami pozytywnymi - to fragmenty, które do których koniec ma do większości innych fragmentów odległość równą długości $k$-meru (odległości pomiędzy k-merami są wyznaczane na tej samej zasadzie co przy tworzeniu grafu) i fragment ma długość 1, to jest usuwany. Reszta fragmentów jest zachowywana i zapamiętujemy, który z nich jest pierwszy - do niego będziemy dołączać kolejne fragmenty. Tworzymy ruletkę - w której umieszczamy element odpowiadający początkowi każdemu niepołączonego fragmentu. Przy czym wartości wszystkich elementów o wadze $w$ będą się sumować do $\frac{1}{w}$. Do poszczególnych wartości dodawane są feromony. Z owej ruletki wybierany jest jeden z początków fragmentów, te fragmenty są łączone w ten sposób w jedną ścieżkę. 
<div style="page-break-after: always;"></div>

# Testy
Każdy parametr testowaliśmy dla 9 różnych wartości tego parametru. Algorytm z każdą z tych 9 wartości był testowany na długościach sekwencji 200,400,600,800 i 1000. Wartość wynik testu dla danej wartości parametru jest średnią z wyników pięciu wykonań algorytmu, przy każdym wykonaniu każdy z porównywanych algorytmów dostawał tą samą sekwencje, ale zmieniała się ona z każdym wykonaniem. Parametry które testowaliśmy to:
* ilość mrówek w iteracji
* czas jaki algorytm ma na znalezienie rozwiązania
* siłę parowania feromonów
* próg błędów false negative
* próg błędów false positive
* próba kontrolna przy domyślnych parametrach

Na wykresach zostały podane wyniki dla algorytmów chaoticAnt, pickyAnt a także random, który oznacza jakość rozwiązania znalezioną przez przechodzenie grafu metodą algorytmu chaoticAnt ale bez używania metaheurystyki. Niektóre wykresy nie zawierają słupka dla tego ostatniego, ponieważ sprawdzany parametr nie ma wpływu na działanie algorytmu losowego(random). 

## Zależność pomiędzy jakością wyników algorytmów na domyślnych parametrach, a długością sekwencji

![[defaults0.png|500]]

Na domyślnych parametrach tj. :
* ilość mrówek na iteracje: 50
* Szybkość parowania feromonów: 0.01 (na iteracje)
* 5 sekund czasu działania algorytmu
* 1% szansy błędów negatywnych
* 1% szansy błędów pozytywnych
Na małych instancjach algorytmy wypadają podobnie, wraz ze wzrostem rozmiaru instancji widać jednak że algorytm pickyAnt daje najlepsze wyniki, na ogół algorytm losowy (random - bez użycia metaheurystyki) daje lepsze wyniki niż chaoticAnt, do momentu zwiększenia instancji do długości 1000.

## Zależność pomiędzy jakością wyników algorytmów metaheurystycznych a liczbą mrówek, dla różnych długości sekwencji

![[Pasted image 20240214013216.png]]

W wynikach testów można zobaczyć w znacznej większości przypadków wyższość algorytmu pickyAnt nad algorytmem chaoticAnt (jedyny test gdzie algorytm chaoticAnt pokonuje pickyAnt to dla długości sekwencji 1000 i ilości mrówek 150). Na pierwszy rzut oka, na ogół nie widać dużego wpływu jaki ilość mrówek ma na jakość otrzymywanych rozwiązań, co może być spowodowane małą ilością testów. Widać jednak dwa następujące trendy:
* Najlepsze wyniki dla pickyAnt występowały generalnie przy długości mrówek oscylującej przy 50 (ogólniej mówiąc mieściły się w prawej połowie wykresu).
* Najlepsze wyniki dla chaoticAnt zaś wydawały się wychodzić na ilości mrówek oscylującej przy 100 i były na ogół zgromadzone na środku wykresu.
Możliwe, że ilość mrówek miała mniejszy wpływ na algorytm pickyAnt niż na chaoticAnt. Można zobaczyć że większa liczba mrówek potrafiła trochę pomóc dla chaoticAnt a w przypadku pickyAnt liczba mrówek nie wpływała znacząco na wyniki, aczkolwiek wciąż średnio widać preferencje niższej liczby mrówek. 
Na długości sekwencji 800 pickyAnt odniósł najlepsze wyniki. W gruncie rzeczy widać że algorytm chaoticAnt daje coraz gorsze wyniki wraz ze wzrostem długości sekwencji, pickyAnt utrzymuje pewien pewną jakość swoich wyników, do momentu kiedy musi znaleźć ścieżkę dla sekwencji o długości 1000. 


## Zależność pomiędzy jakością wyników algorytmów metaheurystycznych a stopniem parowania, dla różnych długości sekwencji


![[Pasted image 20240214013016.png]]

Stopień parowania feromonów nie wydaje się mieć w większej mierze wpływu na algorytm pickyAnt - co dalej podpiera tezę, że nie polega on tak bardzo na feromonach, bądź może też oznaczać, że mrówki szybko znajdują dobrą ścieżkę i mocno naznaczają ją feromonami co ostatecznie niweluje efekt zwiększającego się stopnia parowania feromonów. Znaczący spadek jakości ścieżek pickyAnt widać dopiero dla sekwencji o długości 1000, wtedy widać trend że stopień parowania powyżej 0.03 wpływa niekorzystnie na pickyAnt. Co prawda efekt ten może być spowodowany małą ilością testów ponieważ widać, że z kolei wyniki dla stopnia parowania 0.06 wydają się nagle polepszać.  Dla mniejszych długości sekwencji stopień parowania wydaje się nie mieć takiego wpływu na chaoticAnt, jednak wraz ze wzrostem długości sekwencji pewne stopnie parowania wydają się być bardziej korzystne dla chaoticAnt.

## Zależność pomiędzy jakością wyników algorytmów a progiem błędów pozytywnych, dla różnych długości sekwencji

![[Pasted image 20240214012723.png]]
Ilość błędów pozytywnych nie ma wpływu na pickyAnt które utrzymuje wynik poniżej 20. Dla algorytmu chaoticAnt i losowego nie widać korelacji miedzy ilością błędów i dł. sekwencji. Ich wyniki wydają się całkowicie losowe. Może to być spowodowane losową naturą błędów pozytywnych i faktem, że na ogół statystycznie mają relatywnie małe szanse wpłynąć na generowany graf - ich wierzchołki mają sporą szanse na zostanie wierzchołkami izolowanymi, aczkolwiek szansa ta maleje wraz ze wzrostem długości sekwencji.

<div style="page-break-after: always;"></div>

## Zależność pomiędzy jakością wyników algorytmów a progiem błędów negatywnych, dla różnych długości sekwencji

![[Pasted image 20240214012859.png]]
Przy błędach negatywnych pickyAnt też wydaje się utrzymywać podobny poziom (jakość) wyników, oznacza to że najprawdopodobniej dobrze sobie radzi z błędami negatywnymi co może wynikać z natury jeg działania. Błędy negatywne w grafie wprowadzają przerwy w ścieżkach złożonych z krawędzi o wadze 1, a całą istotą działania algorytmu pickyAnt jest rozbijanie grafu na takie właśnie fragmenty i łączenie ich w jak najlepszy sposób. Wyniki algorytmu random są w zasadzie zbliżone do algorytmu pickyAnt, potem widać większy rozstrzał aczkolwiek na ogół algorytm random wciąż był w stanie osiągać lepsze wyniki od chaoticAnt poza pojedynczymi przypadkami. Świadczy to o niskiej jakości rozwiązań algorytmu chaoticAnt, jest to wywołane w większości naturą jego działania i prawdopodobieństwem losowego skoku podczas trawersowania grafu, które jest w zasadzie znikome w przypadku pojedynczego przejścia algorytmu random. Błędy negatywne także będą w miarę rozproszone co sprawia, że w gruncie rzeczy będą generować krótkie przerwy w ścieżkach złożonych z krawędzi o długości 1 w grafie, na tyle krótkie że nie będą raczej wychodziły poza próg tworzenia krawędzi dzięki czemu sposób przechodzenia grafu chaoticAnt może znaleźć dobrą ścieżkę już za pierwszym podejściem. Szansa na to w gruncie rzeczy maleje wraz z szansa na błąd negatywny a także na długość sekwencji. 

<div style="page-break-after: always;"></div>

## Zależność pomiędzy jakością wyników algorytmów metaheurystycznych a czasem na znalezienie rozwiązania, dla różnych długości sekwencji

![[Pasted image 20240214013424.png]]

Wnioskując z wykresów wygląda na to że czas nie ma wpływu na wynik dla małych rozmiarów instancji. Dla długości 800 i 1000 widać jednak że niski czas na obliczenia ma negatywny wpływ na wyniki a optymalny czas występuje bliżej środka badanego spektrum.  