# PROJEKT NA LABORATORIA Z OPTYMALIZACJI KOMBINATORYCZNEJ

  

# wymagania do uruchomienia projektu:

  

Język Programowania: C++ w wersji 20

Narzędzie do budowania projektu: CMake 3.10

Biblioteki: glog
  

# budowanie projektu

### ręczne:

budowanie projektu: ```cmake -B build```

kompilacja: ```cmake --build build```

puszczenie programu:```./build/main```


### docker:

będąc w folderze projektu:

```sh
docker compose build
docker compose run project
```

by przebudować zedytowany kod wystarczy odpalić:

```
/application/rebuild.sh
```

nie jest wymagane przebudowanie kontenera dockerowego


W pliku main.cpp znajdują się dwie funckje testSignglePickyInstance(ta wywołuje się defaultowo) is testSingleChaoticInstance, które wywołują kolejno algorytm pickyAnt i chaoticAnt, argumentem dla obu z nich jest długość generowanej sekwencji. Można też zmieniać inne parametry. Parametry związane z algorytmem mrówkowym, mogą być zmienione przez settery lub publiczne atrybuty klasy AntColonyOptimization. Parametry związane z błedami pozytywnymi czy negatywnymi są publicznymi atrybutami klasy Sequence.  Domyślne wartości parametrów to :
numOFfAnts: 50
stiopTime: 5
falsePositiveThreshold: 0.01
falseNegativeThreshold: 0.01
evaporationRate: 0.01


Program po wywołaniu wypisuje wygenerowaną sekwencje. Potem podaje w kolejnych liniach czasy wykonania kolejnych iteracji algorytmu mrówkowego. Na koniec znowu wypisuje wejściową sekwencje i pod spodem wypisuje znalezione ścieżki, solution distance to miara levensteina, a liczba przy każdej utworzonej sekwencji jest scorem jaki program przyznał temu rozwiązaniu podzielonym na 10.