
## Fordítás, futtatás

Ez a program kettő LMMS pluginból áll. A fordításhoz szükséges az [LMMS forrása](https://github.com/LMMS/lmms).

A letöltés után a `Diginstrument` mappát az `lmms/plugins` mappába kell másolni, majd az `lmms/plugins/CMakeLists.txt` fájlban hozzá kell adni a plugin listához.

Ezzel a kettő fő `cmake` target:
* diginstrument - a lejáttszó része a programnak
* diginstrument_spectral_analyzer - az analizáló és hangszerszerkesztő része a programnak

A futtatáshoz ajánlott mindkettő fenti targetet és az `lmms` főprogram-targetet is lefordítani, majd a kapott `lmms` futtatható állományt elindítani. Ezzel autómatikusan magábafoglalja a két plugint.

### Függőségek

Az LMMS függőségein felül szükséges még a következő könyvtárak telepítése:
* Qt5DataVisualization (platform-specifikus repositoryból)
* [nlohmann_json](https://github.com/nlohmann/json)
* fftw3 (platform-specifikus repositoryból)
* [wavelib](https://github.com/rafat/wavelib)

## A lejáttszó

A lejáttszó plugint a baloldalon lévő "Instrument Plugins" fülben találhattjuk, és innen a "Song-Editor"-ra húzással példányosíthatjuk.

A hangszerfájl betöltése után az LMMS által szolgáltatott billentyűzettel, valamint a megjelent koordináta-csúszkákkal játtszhatjuk.

## A hangszerszerkesztő

Ez a tool-plugin alapértelemzetten megnyílik, vagy a fejléc-menü "Tools" pontjában is megtalálható.

A felső részében meg kell adni az analízisparamétereket, melyek alapértelmezett értékeket vesznek fel.

A középső részben adhattjuk meg a hangszer dimenziójit. Az "Add dimension" gomb egy új dimenziót ad a listához. A dimenzióknak meg kell szabni egy nevet, valamint egy minimum és maximum értéket. A kitüntetett "pitch" dimenziót használjuk a hangmagasság leírására.

A legalsó részben adhatunk hangjegyeket a hangszerhez. A koordináták megadása után ki kell választani egy audiófájlt. A dimenziómezők zárva lesznek, amíg hangszerben analizált hangjegyek vannak.

Dimenziókat és hangjegyeket lehet törölni, az "X" gombjuk segítségével.

A hangjegy fájlba mentése a legalsó "Save instrument" gombbal tehető.