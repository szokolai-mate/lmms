/*
  ks_tab.h
	
	Copyright 2006 David Yeh <dtyeh@ccrma.stanford.edu>
	
	http://quitte.de/dsp/

	Tone Stack emulation coefficient table for lattice filter.

*/
/*
	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
	02111-1307, USA or point your web browser to http://www.gnu.org.
*/

#ifndef _KS_TAB_H_
#define _KS_TAB_H_

namespace DSP {

double ToneStackKS[] = {
  -0.999655160907038,0.980127109825519,-0.614914419461634,
  -0.999656568120978,0.980206907765841,-0.615188716843305,
  -0.999658517676675,0.980317236486758,-0.615568128036899,
  -0.999661208271173,0.98046907704722,-0.616090612857295,
  -0.999664902023365,0.980676730894581,-0.616805748794314,
  -0.999669936365576,0.980958270564762,-0.617776442867139,
  -0.999676730565233,0.981335546377295,-0.619079214371838,
  -0.999685778922586,0.981833264634174,-0.620801399623313,
  -0.999697618702198,0.982476493745153,-0.623033027158334,
  -0.999712758774516,0.983286007325851,-0.625851163451736,
  -0.99973155988367,0.984271440446054,-0.629296281256582,
  -0.999754076003002,0.98542354941219,-0.633344581005633,
  -0.999779899796788,0.986708608367483,-0.637886397587646,
  -0.999808088604521,0.988068846787513,-0.642724617436751,
  -0.999837246275279,0.989431147988487,-0.647602282731862,
  -0.999865773984939,0.990721953717012,-0.652254061004993,
  -0.999892205612632,0.991882358316473,-0.656461274207081,
  -0.999915487257858,0.992877246278541,-0.660087773487817,
  -0.999935097854328,0.993696153431228,-0.663086394317218,
  -0.999951003603908,0.994347891380557,-0.665481748921572,
  -0.999963513493909,0.994852827640871,-0.667343004256292,
  -0.999973116451418,0.995235942818388,-0.668758405977034,
  -0.999980351396071,0.995522046602486,-0.669817209794174,
  -0.99998572581668,0.995733179766024,-0.670599558951996,
  -0.999989676414453,0.995887625130919,-0.671172389008809,
  -0.99964958032362,0.979587583563023,-0.613841200571614,
  -0.99965105886229,0.979673968717358,-0.614139015327499,
  -0.99965310654599,0.979793345129979,-0.614550764884576,
  -0.999655931261197,0.979957524247093,-0.615117419836355,
  -0.999659806674729,0.980181839664558,-0.615892331666542,
  -0.99966508402413,0.980485578112938,-0.616942906326338,
  -0.999672197810492,0.980891896855617,-0.618350616306868,
  -0.999681656900824,0.981426699112155,-0.620207543319113,
  -0.999694008538874,0.982115787972737,-0.622607053883059,
  -0.999709761087518,0.982979722321469,-0.625626413960214,
  -0.999729257452251,0.984026477862534,-0.629301273863883,
  -0.999752511640623,0.985243474991385,-0.633596859040439,
  -0.999779056061162,0.98659232792421,-0.638387182556479,
  -0.999807879344237,0.988010325656004,-0.643456796374595,
  -0.999837527771197,0.989420506822037,-0.648533280922821,
  -0.999866374253003,0.990747568499188,-0.653342756744767,
  -0.999892960043297,0.991933070692341,-0.657666017256545,
  -0.999916265897791,0.992943874252478,-0.661372461014463,
  -0.999935816364322,0.993772030018927,-0.66442324414345,
  -0.999951619581897,0.994428658432513,-0.666851256235163,
  -0.999964015160231,0.994935885870011,-0.668732389522373,
  -0.99997351033002,0.995319870362579,-0.67015970170105,
  -0.999980652603298,0.995606135693424,-0.671225609758467,
  -0.999985951820482,0.995817121286393,-0.672012217707519,
  -0.999989843684551,0.995971315456801,-0.672587633075114,
  -0.999643378423064,0.978985881492609,-0.612500740824184,
  -0.999644936096075,0.979079752094198,-0.612825144699971,
  -0.99964709260153,0.979209402031235,-0.613273429777593,
  -0.999650065958286,0.979387576553966,-0.613889937311562,
  -0.999654142524417,0.97963076347191,-0.614732221861648,
  -0.99965968863373,0.97995959552602,-0.615872659651328,
  -0.999667155309705,0.980398657679706,-0.617398123146777,
  -0.999677066975389,0.980975122632538,-0.619405738241327,
  -0.999689981074653,0.98171549955349,-0.621992177692255,
  -0.999706404340016,0.982639946893981,-0.625234340260081,
  -0.999726658923252,0.983754410258249,-0.629161826247319,
  -0.999750714237359,0.985042459195097,-0.633727095084122,
  -0.999778037042399,0.986460516399503,-0.638785891235472,
  -0.999807542776699,0.9879405525597,-0.644102932677235,
  -0.999837718122031,0.989401653230246,-0.649389757433484,
  -0.999866908526907,0.990766919256306,-0.654364321488812,
  -0.999893665267437,0.991978663173791,-0.658807867477182,
  -0.999917006727609,0.993006014704747,-0.662596430819353,
  -0.999936505324055,0.993843761495685,-0.665700368324747,
  -0.999952212433419,0.994505470936323,-0.668161425658953,
  -0.999964498892951,0.995015103003389,-0.670062547781078,
  -0.999973890491138,0.995400030001749,-0.671501769244335,
  -0.99998094346516,0.995686507256172,-0.672574743431766,
  -0.9999861701193,0.995897381317078,-0.673365566060677,
  -0.999990005274817,0.996051350521398,-0.673943527635947,
  -0.999636497864097,0.978314250888013,-0.610868507178806,
  -0.999638143609638,0.978416674858248,-0.611223133119698,
  -0.999640421166529,0.978558054130414,-0.611712912678915,
  -0.999643559753541,0.97875218877719,-0.612385977311627,
  -0.999647859706568,0.979016862349941,-0.61330458074876,
  -0.999653703893894,0.97937420403329,-0.614546606917925,
  -0.999661561270341,0.979850359871836,-0.616204823882976,
  -0.999671972845482,0.980473843383251,-0.618381706324296,
  -0.99968550637702,0.981271816955761,-0.621177132648703,
  -0.999702665524369,0.982263799418809,-0.624666899863223,
  -0.999723748225852,0.983453256531511,-0.628873069633944,
  -0.999748673996069,0.984819306197574,-0.63373324602853,
  -0.99977683795445,0.98631258435924,-0.639082719486578,
  -0.999807077565093,0.987859347100283,-0.644664781960943,
  -0.999837817970182,0.989374645543676,-0.650174417628097,
  -0.999867378328761,0.990780183969698,-0.655321967566108,
  -0.999894323021703,0.992019364405147,-0.659890277895962,
  -0.999917711372098,0.993063913192047,-0.663763240841613,
  -0.999937166122597,0.993911596229206,-0.66692136936033,
  -0.999952783288071,0.994578575143569,-0.669415879976423,
  -0.999964965581412,0.995090721972347,-0.671337112690936,
  -0.999974257618683,0.995476661471914,-0.672788249221993,
  -0.999981224498276,0.995763398345879,-0.673868256393632,
  -0.999986381097967,0.995974194795217,-0.674663253284672,
  -0.999990161468935,0.996127963642263,-0.675243724730679,
  -0.999628871599216,0.977563340625174,-0.608914428430414,
  -0.999630615705312,0.977675597836713,-0.609303604497054,
  -0.999633028378567,0.977830449102962,-0.609840779838346,
  -0.999636351252157,0.978042891841267,-0.610578368059795,
  -0.999640900093703,0.978332170312435,-0.611583898461033,
  -0.999647075913584,0.978722082369623,-0.612941377384029,
  -0.999655367140044,0.979240482054698,-0.614750017497272,
  -0.999666332434044,0.979917290534043,-0.617117944354921,
  -0.999680549835182,0.98078023709284,-0.62014804531906,
  -0.999698518090521,0.981847892980184,-0.623914065384914,
  -0.999720506814374,0.98312069692929,-0.628428717414309,
  -0.999746379537056,0.9845726129935,-0.633612307993453,
  -0.999775453078932,0.986147824972144,-0.639277222192053,
  -0.999806481847782,0.987766463858197,-0.64514365014005,
  -0.999837827653051,0.989339503078404,-0.650889614066351,
  -0.999867784981983,0.990787512735525,-0.656218595909368,
  -0.999894934895781,0.992055379434954,-0.660916404977201,
  -0.999918381339343,0.993117793337578,-0.664876156724425,
  -0.999937800054862,0.993975761434163,-0.668089558173762,
  -0.999953333200393,0.99464819664376,-0.670617950121693,
  -0.999965416056239,0.995162965417149,-0.672559425593145,
  -0.999974612351636,0.995549984512892,-0.674022489490217,
  -0.999981496185304,0.995837026254415,-0.675109501099312,
  -0.999986585116092,0.996047777075045,-0.675908635241351,
  -0.999990312531913,0.996201368691033,-0.676491582768135,
  -0.999620420975775,0.976721799513297,-0.606601635017253,
  -0.999622275355999,0.97684543733367,-0.607030558569431,
  -0.999624839418341,0.977015864707194,-0.6076222077637,
  -0.999628368599035,0.97724944539608,-0.608433858852205,
  -0.999633195747874,0.977567077732237,-0.609538989890021,
  -0.999639741820149,0.977994425988304,-0.611028434870471,
  -0.999648516406429,0.978561213224713,-0.61300845746645,
  -0.999660096906448,0.979298825518745,-0.615593120493524,
  -0.999675071412884,0.980235426548642,-0.618887989909175,
  -0.999693931427662,0.981388243122174,-0.622963529987665,
  -0.999716913349402,0.982754019087567,-0.627820896963069,
  -0.999743817730573,0.984300742606826,-0.633360240188853,
  -0.999773875694897,0.985965403475005,-0.639368293929638,
  -0.999805753219434,0.987661589920051,-0.64554040334027,
  -0.999837747205987,0.989296206939276,-0.651537368871066,
  -0.999868129620919,0.990789029644207,-0.657056822841524,
  -0.999895502342668,0.992086891864035,-0.661889136757108,
  -0.999919018029778,0.993167859006729,-0.66593818034046,
  -0.999938408329249,0.994036465317501,-0.669207982776834,
  -0.999953863156006,0.994714542470667,-0.671770703919781,
  -0.999965851094207,0.995232037757674,-0.673732564308737,
  -0.999974955287586,0.995620200909204,-0.675207573989099,
  -0.999981758977838,0.995907590527414,-0.676301565758894,
  -0.999986782510092,0.996118325926393,-0.677104803288624,
  -0.999990458711745,0.996271762077857,-0.677690195455546,
  -0.999611053343317,0.975775743039613,-0.603884796825947,
  -0.999613031883562,0.975912650295233,-0.604359763835632,
  -0.999615766283421,0.976101218495051,-0.6050144497913,
  -0.999619527378512,0.976359379624739,-0.6059116832352,
  -0.999624666982958,0.976709914051601,-0.607131681527055,
  -0.999631628025413,0.977180580098539,-0.60877291769307,
  -0.999640943108211,0.977803144860867,-0.610949372866091,
  -0.999653209460591,0.978610499278102,-0.613781297332705,
  -0.999669024734094,0.979631045031147,-0.617376419553776,
  -0.99968887023538,0.980880152002991,-0.621800333065081,
  -0.999712943314506,0.982350051181377,-0.627039935806544,
  -0.999740973496636,0.984001791228723,-0.632971865481303,
  -0.999772097994155,0.985764344142541,-0.639354138465182,
  -0.999804888706456,0.987544341427899,-0.645855471952423,
  -0.999837576362628,0.989244700363806,-0.652119392113297,
  -0.999868413198854,0.990784834346404,-0.657839003408275,
  -0.99989602668777,0.992114065717222,-0.662811117640661,
  -0.99991962274449,0.993214296218098,-0.666952074995355,
  -0.999938992074364,0.994093898965587,-0.670279453607488,
  -0.999954374077029,0.994777802955718,-0.672876971494188,
  -0.999966271422507,0.995298127015559,-0.674859368596826,
  -0.999975286986133,0.995687496286005,-0.676346348241333,
  -0.999982013298656,0.99597527473925,-0.677447299868145,
  -0.99998697359497,0.996186023293375,-0.67825460983632,
  -0.999990600240577,0.99633932449768,-0.678842417373446,
  -0.999600659011736,0.974708035989579,-0.600707907614531,
  -0.999602778010854,0.974860542895886,-0.601236618584895,
  -0.999605704965998,0.975070410914491,-0.601964799208377,
  -0.999609727957629,0.975357383329951,-0.602961655657006,
  -0.999615219921094,0.975746392931787,-0.604315078325081,
  -0.999622648047905,0.976267554594276,-0.606132115041143,
  -0.999632569975118,0.976954867084367,-0.608535190817401,
  -0.999645603825713,0.977842737024011,-0.61165092590881,
  -0.999662355958282,0.978959518964467,-0.615588436744899,
  -0.999683293758011,0.980318062023894,-0.6204063845804,
  -0.999708568554593,0.981905078819314,-0.626074091503736,
  -0.999737829566184,0.983673547947443,-0.63244074347204,
  -0.99977011098019,0.985543514350578,-0.639232225924571,
  -0.99980388473604,0.987414259071501,-0.646088848833459,
  -0.999837314552659,0.989184888620582,-0.652637096558166,
  -0.999868636494754,0.990775003339118,-0.658567251678203,
  -0.999896509137097,0.992137047075131,-0.663684770297548,
  -0.999920196692532,0.993257274825996,-0.667920387749179,
  -0.999939552345391,0.99414823800866,-0.671306565983005,
  -0.999954866827104,0.994838153363176,-0.673939367784046,
  -0.999966677722521,0.995361406421881,-0.675942462717049,
  -0.999975607971746,0.995752041693706,-0.677441441950547,
  -0.99998225954412,0.996040248059161,-0.678549336835372,
  -0.99998715866607,0.996251036846146,-0.679360690995204,
  -0.9999907373361,0.996404222471088,-0.679950886641583,
  -0.999589107344937,0.973497312875968,-0.597001294754147,
  -0.999591386080871,0.973668328230434,-0.597593268842574,
  -0.99959453183983,0.973903432400324,-0.598407851047308,
  -0.999598852093593,0.974224475166901,-0.599521620766309,
  -0.999604743382784,0.974658864441676,-0.601031250906323,
  -0.999612699753767,0.97523937456658,-0.603053445209834,
  -0.99962330608542,0.97600243259764,-0.605719853677987,
  -0.999637202386439,0.976983924442916,-0.609163532904264,
  -0.999655002388357,0.978211748419904,-0.613493851909141,
  -0.999677154847819,0.979695369509032,-0.61875985885503,
  -0.99970375671817,0.981414741216281,-0.624909211111447,
  -0.999734366195928,0.983313445338257,-0.631759010565249,
  -0.999767904348015,0.985301605206536,-0.638999236073545,
  -0.99980273709838,0.987270802414969,-0.646240081512932,
  -0.999836960897071,0.989116638466047,-0.653091609034812,
  -0.999868800118624,0.990759590999243,-0.659243458364673,
  -0.999896950784494,0.992155965496688,-0.664512315028276,
  -0.999920740997713,0.993296950004936,-0.668845469230266,
  -0.999940090129518,0.994199644092504,-0.672291720044044,
  -0.99995534221587,0.994895755336471,-0.674960312547262,
  -0.999967070633483,0.995422035839287,-0.676984275473309,
  -0.999975918736602,0.9958139950097,-0.678495289077025,
  -0.99998249808615,0.996102666636638,-0.679610114082481,
  -0.999987338000502,0.996313521353591,-0.680425486697752,
  -0.999990870202646,0.996466609707338,-0.681018045054702,
  -0.999576241686749,0.972116617749095,-0.592677524712023,
  -0.99957870315305,0.97230982067096,-0.593344669475996,
  -0.999582098983777,0.972575129825707,-0.594261769514429,
  -0.999586758554847,0.972936863320769,-0.595513988772127,
  -0.999593104888038,0.97342529174051,-0.597208109144451,
  -0.999601661826924,0.974076198024372,-0.599471740073253,
  -0.999613043888958,0.974928635484012,-0.602446577711838,
  -0.999627913819039,0.97601985787081,-0.606271990784749,
  -0.999646890735495,0.977376723720224,-0.611055959085039,
  -0.999670398812179,0.979004185472482,-0.616834417946938,
  -0.999698470581005,0.980873900666029,-0.623528301373912,
  -0.999730560827483,0.982918498660264,-0.630917179784869,
  -0.999765466341961,0.985037108077224,-0.638650985611181,
  -0.999801440901162,0.987113342915257,-0.646308258106163,
  -0.999836514200741,0.989039777153926,-0.653483778621958,
  -0.99986890451577,0.990738630386345,-0.659869306066888,
  -0.999897352618159,0.992170935258235,-0.665295786916716,
  -0.999921256704574,0.993333463562061,-0.669729491261027,
  -0.999940606351128,0.994248266182805,-0.673237138510218,
  -0.999955801003116,0.994950758180636,-0.675942048171054,
  -0.999967450755683,0.99548016302305,-0.677987058060857,
  -0.999976219742873,0.995873502181361,-0.679510145713319,
  -0.999982729274098,0.99616267482996,-0.680631890943183,
  -0.999987511858705,0.996373619900609,-0.681451258613912,
  -0.999990999032129,0.996526628313089,-0.682046156011673,
  -0.99956187268433,0.970531479526377,-0.587625703122706,
  -0.999564544561613,0.970751592835626,-0.588383112630484,
  -0.999568228052843,0.971053471010611,-0.589423118267952,
  -0.99957327740021,0.971464348435104,-0.590840956059624,
  -0.999580145454236,0.972017826647422,-0.592755112731153,
  -0.999589389203159,0.972753097820726,-0.595305546336125,
  -0.999601655385005,0.973712026201297,-0.59864482915026,
  -0.999617630085655,0.97493300368958,-0.602918212146777,
  -0.99963793493872,0.976441017872316,-0.608229928386846,
  -0.999662961985651,0.978235025410733,-0.614598209635963,
  -0.999692667221772,0.980276477231159,-0.621910984287905,
  -0.999726387678802,0.982485230710179,-0.62990389083695,
  -0.999762783587337,0.984748286160729,-0.638182336737736,
  -0.999799990515491,0.986941155449722,-0.646291986463415,
  -0.9998359729422,0.988954090981809,-0.653814181774226,
  -0.999868949969855,0.990712133831788,-0.660446282379072,
  -0.999897715526312,0.992182056431122,-0.666037051039227,
  -0.999921744783971,0.99336694509957,-0.670574462569404,
  -0.999941101876438,0.994294241723863,-0.674144882521851,
  -0.999956243902449,0.995003300002107,-0.676886655565201,
  -0.999967818653287,0.99553592474197,-0.678952899992532,
  -0.999976511425167,0.995930698330922,-0.680488106036015,
  -0.999982953436508,0.996220406298199,-0.681616764634929,
  -0.999987680485395,0.996431464970127,-0.682440106138711,
  -0.999991124005049,0.996584409866817,-0.683037320515038,
  -0.999545769372832,0.968697129881683,-0.581703389413761,
  -0.999548685330482,0.968950318756053,-0.582570485078089,
  -0.999552702131166,0.969297052285561,-0.583759569216554,
  -0.999558202401447,0.969768041680857,-0.585377795072816,
  -0.999565672738884,0.970400788413801,-0.587557285815401,
  -0.99957570708997,0.971238349112145,-0.590452006519345,
  -0.999588987158165,0.972325543365669,-0.594226183799786,
  -0.999606222572019,0.973701485409508,-0.59903003585127,
  -0.999628033400377,0.97538810561202,-0.604960671497707,
  -0.999654769948043,0.977376402599708,-0.612012563238519,
  -0.999686297011813,0.979615237602125,-0.620032803668798,
  -0.999721817252862,0.982009578441646,-0.62870559790186,
  -0.999759840891314,0.984433140013485,-0.637587083446276,
  -0.999798379512054,0.986753408121981,-0.646189365889778,
  -0.999835335260595,0.988859323349258,-0.654083124451187,
  -0.999868936604779,0.990680093327157,-0.660975691075208,
  -0.999898040302297,0.992189415816235,-0.666737815961683,
  -0.99992220613798,0.99339751304594,-0.671382242820988,
  -0.999941577517616,0.994337697670144,-0.675016865803864,
  -0.999956671584689,0.995053508724011,-0.677796068374459,
  -0.999968174857054,0.995589447776921,-0.679883743341079,
  -0.999976794192393,0.995985708739766,-0.681431116570944,
  -0.999983170882486,0.996275984974083,-0.682566684542184,
  -0.999987844111017,0.996487179407059,-0.683393980689551,
  -0.999991245291239,0.996640076376014,-0.683993491478984,
  -0.999527647068028,0.966554383008803,-0.574724879596308,
  -0.999530848539975,0.966848851405444,-0.575727078537408,
  -0.99953525472567,0.967251434416085,-0.57709940504731,
  -0.999541280848263,0.967797029601554,-0.578963243130562,
  -0.999549451864263,0.968527732682652,-0.581466703366014,
  -0.999560403023074,0.969490971729293,-0.584779644783603,
  -0.999574853846751,0.970734576829317,-0.589078562311174,
  -0.999593537066634,0.972297674431688,-0.59451695807115,
  -0.999617065445341,0.974197436159595,-0.601179970724196,
  -0.999645735282156,0.97641428827226,-0.609030274548682,
  -0.999679302369117,0.978881522835187,-0.617864335613826,
  -0.99971681574429,0.981486776171256,-0.62730617857504,
  -0.999756621007015,0.98408936563692,-0.636857810951617,
  -0.999796600586004,0.986549150056454,-0.645997950543695,
  -0.99983459893934,0.988755172290888,-0.654290641249287,
  -0.999868864385614,0.990642480721143,-0.661458661540686,
  -0.999898327649026,0.992193087751127,-0.66739964572281,
  -0.999922641604418,0.9934252755721,-0.672154555174154,
  -0.999942034036786,0.994378751406568,-0.675854867355025,
  -0.999957084680973,0.995101502992542,-0.678672085713889,
  -0.999968519866798,0.995640849812384,-0.68078139550167,
  -0.999977068429707,0.996038649727251,-0.682340988976285,
  -0.999983381903268,0.996329525932602,-0.683483465015717,
  -0.999988002952587,0.996540877278988,-0.684314698518073,
  -0.99999136305069,0.996693741131828,-0.684916486550783,
  -0.999507150598248,0.964023361778569,-0.566443805631269,
  -0.999510689256673,0.964370271326711,-0.567615024684631,
  -0.999515554615236,0.964843610477965,-0.569216045936927,
  -0.999522199580942,0.965483372059577,-0.571385418568372,
  -0.999531192926903,0.966337094760156,-0.574290114034592,
  -0.999543216143162,0.967457120290353,-0.578117988726243,
  -0.999559029420708,0.968894166267862,-0.583058050119345,
  -0.999579387147137,0.970686191408446,-0.589264179491485,
  -0.999604886734295,0.972843149467847,-0.596802560696606,
  -0.999635754725745,0.975331385211999,-0.605593323611265,
  -0.999671616210287,0.978064893520965,-0.615370037541993,
  -0.999711344319248,0.980911208410932,-0.625686441819605,
  -0.999753104353326,0.983714303325128,-0.635985722329208,
  -0.999794645469085,0.986327296819486,-0.645714703351608,
  -0.999833761386482,0.988641287435787,-0.654436491473955,
  -0.999868733118323,0.990599247730616,-0.661896156590664,
  -0.999898578182872,0.992193134802953,-0.66802397047431,
  -0.99992305196103,0.993450331406395,-0.672892997532942,
  -0.999942472149362,0.994417511571143,-0.676660542838115,
  -0.999957483785679,0.995147392987777,-0.679516383603064,
  -0.999968854153534,0.995690240234068,-0.681647540650936,
  -0.999977334500213,0.99608962943699,-0.683219411520137,
  -0.999983586773378,0.99638113616817,-0.684368796864731,
  -0.999988157214811,0.996592664646218,-0.685203952213579,
  -0.999991477434319,0.996745509473735,-0.685807999622806,
  -0.999483830537178,0.96099363273767,-0.556526558132353,
  -0.999487771824971,0.961408567503047,-0.557913085833148,
  -0.99949318453032,0.961973392338345,-0.559804618374626,
  -0.999500565445448,0.962734373879405,-0.562360637123727,
  -0.99951053364941,0.963745527570854,-0.565770491848824,
  -0.999523822447645,0.965064639052157,-0.570242285359762,
  -0.999541235335364,0.96674484954275,-0.575977037252997,
  -0.999563545337433,0.968821009537295,-0.583124143295505,
  -0.999591323245939,0.971292265484483,-0.59172069161267,
  -0.999624705516342,0.974106135226471,-0.601629796724883,
  -0.999663160011883,0.977152661845667,-0.612506744011134,
  -0.999705358236516,0.980276222869867,-0.623823505106148,
  -0.999749268681416,0.983304874940093,-0.634960424679979,
  -0.999792504826671,0.98608661301415,-0.645335938947312,
  -0.999832819611289,0.988517266330941,-0.654520152023392,
  -0.999868542448472,0.990550325768389,-0.662288978786769,
  -0.999898792436992,0.992189608358322,-0.668612095924406,
  -0.999923437929053,0.993472770560181,-0.673599052648918,
  -0.999942892527405,0.994454078791728,-0.67743543482306,
  -0.999957869458855,0.995191281150468,-0.680330525251589,
  -0.999969178161553,0.995737720843995,-0.682483750055223,
  -0.999977592746505,0.996138748541774,-0.684067959405521,
  -0.999983785751949,0.996430915291374,-0.685224257694965,
  -0.999988307090768,0.996642640252181,-0.686063321051298,
  -0.999991588584531,0.996795479475092,-0.686669611188537,
  -0.999457108562801,0.957308100330452,-0.544510348254195,
  -0.999461536891738,0.957813496974774,-0.546177043093093,
  -0.999467610348739,0.958499509739162,-0.548445349392839,
  -0.99947587724496,0.959420224561283,-0.551500575623861,
  -0.999487014706649,0.96063737454455,-0.555558738396518,
  -0.999501814057202,0.962214593042568,-0.560850379360616,
  -0.999521124121701,0.964206335982036,-0.567586594383798,
  -0.999545731079106,0.966641151433838,-0.575904240780955,
  -0.99957616326672,0.969502075922418,-0.585796443590518,
  -0.999612440645105,0.97271133966764,-0.597049671898101,
  -0.999653841360903,0.97612926634827,-0.609221678375412,
  -0.999698805768044,0.979573890622481,-0.621690000135093,
  -0.999745088676003,0.98285750656369,-0.633769664809452,
  -0.999790168137297,0.985825691481262,-0.644857253738139,
  -0.999831770196654,0.988382650050448,-0.654540806884388,
  -0.999868291858835,0.990495625586984,-0.662637775338392,
  -0.999898970864313,0.99218254911945,-0.669165211711007,
  -0.999923800176717,0.993492674974405,-0.674274097203207,
  -0.999943295802393,0.99448854634705,-0.678180982015034,
  -0.999958242228691,0.995233262834796,-0.681115970328253,
  -0.999969492310313,0.995783386502816,-0.683291491360672,
  -0.9999778434921,0.996186100876788,-0.684888104075612,
  -0.999983979083579,0.996478956154855,-0.686051321225753,
  -0.999988452762854,0.996690896143604,-0.686894280318611,
  -0.999991696635762,0.996843742558989,-0.687502797675764,
  -0.99942622519677,0.95273551376525,-0.52973445342276,
  -0.999431251888513,0.953364890065364,-0.531775078248324,
  -0.999438135127102,0.954216205818819,-0.534544256478421,
  -0.999447484232229,0.955353409604379,-0.538259555819354,
  -0.999460043610956,0.956847382797287,-0.543168806764506,
  -0.999476669294355,0.958767617957035,-0.549526631433258,
  -0.999498256120107,0.961167548841825,-0.557549533893376,
  -0.999525594039106,0.96406411345337,-0.567348498477478,
  -0.99955914655047,0.967416296772765,-0.578850629984727,
  -0.999598782612736,0.971112205090269,-0.591738948908515,
  -0.999643550828683,0.974975424245401,-0.605449790438815,
  -0.999691626863873,0.978794695326964,-0.619253051210138,
  -0.999740535476891,0.982368032503499,-0.632399001290434,
  -0.999787623551576,0.985542928393217,-0.644273440690982,
  -0.999830609266859,0.988236917993668,-0.654497332961353,
  -0.999867980665878,0.990435036734623,-0.662943041651411,
  -0.999899113839826,0.992171987514358,-0.669684398813115,
  -0.99992413932228,0.993510119095966,-0.674919409982818,
  -0.999943682568059,0.994521000760791,-0.678898527582435,
  -0.999958602593663,0.995273426895692,-0.681874083329038,
  -0.999969796996008,0.995827325707849,-0.684072136980539,
  -0.999978087042701,0.996231774009489,-0.685681221614805,
  -0.999984166999591,0.996525345416597,-0.686851365701776,
  -0.999988594403447,0.996737518228668,-0.687698209735077,
  -0.99999180171517,0.996890384052486,-0.68830893987291,
  -0.999390157426463,0.946920942804295,-0.51122226313308,
  -0.999395933441802,0.947726692929812,-0.513777573628096,
  -0.999403827648663,0.948811777610072,-0.51723281658354,
  -0.999414522275239,0.950252708159809,-0.521846337183872,
  -0.999428839936587,0.952130988141452,-0.527903696028361,
  -0.999447708094207,0.954520972605718,-0.535683534220698,
  -0.999472065554197,0.957470350228495,-0.545397862094965,
  -0.999502690391198,0.960975483022857,-0.557109508368084,
  -0.999539949364454,0.964959236107952,-0.570646385943978,
  -0.999583515091451,0.969263514061787,-0.585551315111654,
  -0.999632157937203,0.973666963417248,-0.601110139814965,
  -0.999683751487941,0.977927125893571,-0.616472948214351,
  -0.999735576101922,0.981831575297115,-0.630831395552119,
  -0.999784857726637,0.985236493339534,-0.643578385785915,
  -0.999829332450153,0.988079481753197,-0.65438828186954,
  -0.999867608015024,0.990368426816476,-0.663205123565136,
  -0.999899221662683,0.992157944027575,-0.670170636093868,
  -0.999924455936773,0.99352517039157,-0.675536179250677,
  -0.999944053382784,0.994551522336392,-0.679589326684972,
  -0.999958951024577,0.995311856218264,-0.682606141144479,
  -0.999970092593324,0.995869621114249,-0.684826971680466,
  -0.999978323687495,0.996275849753412,-0.686448600346481,
  -0.999984349718736,0.996570164047781,-0.687625681500497,
  -0.99998873217569,0.996782586780245,-0.68847640106736,
  -0.999991903942978,0.996935483686355,-0.689089330549666,
  -0.999347481117801,0.939290459494334,-0.487467495756698,
  -0.999354218814845,0.940359291937983,-0.490758827061018,
  -0.999363405293717,0.94179042102576,-0.495189208987614,
  -0.99937581067759,0.943676400697521,-0.501069025782312,
  -0.999392348372638,0.946110332967342,-0.508727587294775,
  -0.999414022815505,0.949167962851339,-0.518463101634253,
  -0.999441809315305,0.952881721565543,-0.530462948663941,
  -0.999476448137069,0.957211904308363,-0.544703949914482,
  -0.999518163390315,0.962026671148328,-0.560864218500752,
  -0.999566371599233,0.9671054278198,-0.578296052160285,
  -0.999619505889223,0.972173183572955,-0.596100902764443,
  -0.999675097525905,0.976957135851488,-0.613301403710775,
  -0.999730172747202,0.981242394512128,-0.629046698844639,
  -0.999781855631441,0.984904293259831,-0.64276494225858,
  -0.999827934835346,0.987909677906738,-0.654211857218486,
  -0.999867172874756,0.990295640550794,-0.663424218297065,
  -0.999899294557703,0.992140429456527,-0.670624806055411,
  -0.999924750546524,0.99353788980571,-0.676125509396204,
  -0.999944408771816,0.994580185639227,-0.680254553289513,
  -0.999959287966352,0.99534862819584,-0.683313339914276,
  -0.999970379456712,0.995910350005688,-0.685557199449765,
  -0.999978553700145,0.996318404632234,-0.687191447715675,
  -0.99998452744805,0.996613487791505,-0.688375478023611,
  -0.999988866233908,0.996826176890439,-0.689230065027467,
  -0.999992003433048,0.996979116046436,-0.689845181360383,
  -0.99929612221311,0.92885164154965,-0.456019541351542,
  -0.999304136012349,0.930338079224207,-0.460415923605211,
  -0.999315027665755,0.932312945848239,-0.466298906238068,
  -0.99932967386815,0.934888870826995,-0.474045380275375,
  -0.999349092121395,0.938169262151671,-0.484032087617024,
  -0.999374365071002,0.942221624043521,-0.496561507953729,
  -0.999406486153652,0.947043569031187,-0.511755566566498,
  -0.999446114575041,0.952531828519665,-0.529439310857211,
  -0.999493265137843,0.95847104464828,-0.549062847224271,
  -0.999547019807701,0.964557081761264,-0.56972004770415,
  -0.999605404586501,0.970454509503382,-0.590292352063801,
  -0.999665568129688,0.975867415739172,-0.609679235305811,
  -0.999724281933513,0.980593694544417,-0.627021003888902,
  -0.999778600317221,0.984543928761683,-0.641824777683263,
  -0.999826410921627,0.987726759554179,-0.653965886787326,
  -0.999866674029357,0.990216498606079,-0.663600374094058,
  -0.999899332676593,0.992119445097779,-0.671047699874178,
  -0.999925023635529,0.99354833216856,-0.676688426942388,
  -0.99994474922943,0.994607059931934,-0.680895306350511,
  -0.999959613839686,0.995383815162353,-0.683996801246194,
  -0.999970657921832,0.99594958472018,-0.686263949735904,
  -0.999978777339898,0.996359510299634,-0.687910896533956,
  -0.999984700383703,0.996655387577793,-0.689101889949892,
  -0.999988996724424,0.996868358881836,-0.689960337531757,
  -0.999992100293264,0.997021350982018,-0.690577629107776,
  -0.999232852507766,0.913727519036899,-0.412606853125446,
  -0.999242645398387,0.915935956793934,-0.41877234970756,
  -0.999255893862801,0.918837586526089,-0.426956504255908,
  -0.999273602979233,0.922567693783475,-0.437619185024585,
  -0.999296904214589,0.927230536454782,-0.451177895602595,
  -0.999326946918226,0.932859625501621,-0.467896904643251,
  -0.999364701514127,0.939375916610792,-0.487748118687341,
  -0.999410673041913,0.946562446165056,-0.510287421562708,
  -0.999464570104078,0.95407637777001,-0.534615408125632,
  -0.999525039284739,0.961506490361475,-0.559480266158481,
  -0.999589621231223,0.968459052306018,-0.583516782465201,
  -0.999655048311424,0.97463639862061,-0.605533240844905,
  -0.999717853458056,0.979877377924312,-0.62472581872234,
  -0.999775072645767,0.984152640941173,-0.640748187551763,
  -0.999824754560624,0.987529886381108,-0.653647788844286,
  -0.999866110070261,0.990130796201362,-0.663733488567256,
  -0.999899336098702,0.992094982866248,-0.671440021774989,
  -0.999925275647391,0.993556546558814,-0.677225885975937,
  -0.999945075220809,0.994632209568016,-0.681512615422448,
  -0.999959929042703,0.99541748478404,-0.684657577866916,
  -0.999970928306749,0.995987393035966,-0.686948283109992,
  -0.99997899485255,0.996399233919776,-0.688608010654417,
  -0.999984868711673,0.996695929899694,-0.689805982917444,
  -0.999989123785848,0.996909198680171,-0.690668285388792,
  -0.99999219462584,0.997062253975925,-0.691287741435037,
  -0.999152077060728,0.889889548444139,-0.349061088286487,
  -0.999164564481812,0.893512362357241,-0.358321392850279,
  -0.999181328175061,0.89819090051193,-0.370471779013374,
  -0.999203519013022,0.904073370874581,-0.386063949868755,
  -0.999232372094478,0.911225741593758,-0.405512187944769,
  -0.999269054494269,0.919575847587998,-0.428928148710386,
  -0.999314422562479,0.928873757841218,-0.455951377537584,
  -0.999368702795487,0.938696629074492,-0.485653574382402,
  -0.999431161107827,0.948513477384418,-0.516600823762431,
  -0.999499889019962,0.957794017950755,-0.547100205403036,
  -0.999571867439423,0.966117436282565,-0.575553712340007,
  -0.999643400523047,0.973236877263828,-0.600771919790487,
  -0.999710829097846,0.979083725305741,-0.622127004150167,
  -0.999771250966691,0.983727246264749,-0.639523867161234,
  -0.999822958889432,0.987318112980124,-0.653254531680796,
  -0.999865479385908,0.990038301447359,-0.663823305665926,
  -0.999899304831475,0.992067025349649,-0.67180239279328,
  -0.999925506987289,0.993562576625782,-0.677738773058922,
  -0.999945387183711,0.994655694348136,-0.682107445763671,
  -0.999960233952359,0.995449700414856,-0.685296658764391,
  -0.999971190912995,0.996023838521735,-0.687611196422924,
  -0.999979206471127,0.996437638512654,-0.689283790136547,
  -0.999985032608414,0.996735177154657,-0.690488758695185,
  -0.999989247549769,0.996948758152623,-0.691354911475921,
  -0.999992286527841,0.997101886480435,-0.691976522006297,
  -0.999042007259475,0.846839476355646,-0.247562606935731,
  -0.999059310027056,0.853850930614133,-0.262993110426348,
  -0.99908215215834,0.862634865772264,-0.282874416523585,
  -0.999111791350179,0.873267701688246,-0.307799227855353,
  -0.999149460274721,0.885615899778428,-0.337997269244,
  -0.999196176211498,0.89928385557361,-0.373105577817442,
  -0.999252479946996,0.913630787322957,-0.412017325303364,
  -0.999318122840983,0.927874730147937,-0.452924609459096,
  -0.9993917698708,0.941254666200446,-0.493608708966554,
  -0.999470858355026,0.953184064404172,-0.531898307960971,
  -0.999551781187113,0.963334777927113,-0.566107560700536,
  -0.999630458843132,0.971634043939743,-0.595279512665676,
  -0.999703140993037,0.978200975484896,-0.619183392925497,
  -0.999767110732338,0.983264056319206,-0.638138631171118,
  -0.999821016253149,0.987090375099302,-0.652782585207141,
  -0.999864780149848,0.989938753401259,-0.663869411221801,
  -0.999899238810493,0.992035545799555,-0.672135353966679,
  -0.999925718023562,0.993566460874601,-0.678227911673286,
  -0.999945685530231,0.99467756984301,-0.682680702983936,
  -0.999960528925675,0.995480521419424,-0.685914973874208,
  -0.999971446026693,0.996058980854992,-0.688253627504821,
  -0.999979412416931,0.996474783268042,-0.689939175953683,
  -0.999985192241392,0.996773187954865,-0.69115115989633,
  -0.999989368141114,0.996987095415323,-0.692021159457422,
  -0.999992376091346,0.997140306222664,-0.692644915228308,
  -0.998865086928174,0.745924580066976,-0.0604141480564288,
  -0.998895487231205,0.764873649745473,-0.0910755754690588,
  -0.998933409668284,0.787060755596958,-0.129313938502959,
  -0.998979684019541,0.81184986561543,-0.175344827159791,
  -0.99903493644843,0.83813626350615,-0.228455101442363,
  -0.999099521125361,0.864509230253996,-0.286815983154912,
  -0.999173362695985,0.889538052977561,-0.347607059890004,
  -0.999255664139382,0.912066866492635,-0.407502232575221,
  -0.999344565973438,0.931397810612656,-0.463365245732482,
  -0.99943698944938,0.94731384445106,-0.512864721985471,
  -0.999528900858466,0.959977803907794,-0.554772924977228,
  -0.999616021217074,0.969782650772129,-0.588907534659359,
  -0.999694709613298,0.977214767105395,-0.615844974558559,
  -0.999762624036077,0.982758778214894,-0.636577066889482,
  -0.999818918115288,0.986845473409128,-0.652227863178924,
  -0.999864010307122,0.989831859802084,-0.663871226969659,
  -0.999899137899114,0.992000508059639,-0.672439368989748,
  -0.999925909089307,0.993568232917785,-0.6786940662434,
  -0.999945970648199,0.994697887686373,-0.68323323728191,
  -0.999960814301116,0.995510003466929,-0.686513398356392,
  -0.99997169391947,0.996092876111869,-0.688876459454257,
  -0.999979612900072,0.996510723831304,-0.690575054289614,
  -0.999985347769733,0.996810017409768,-0.69179407428053,
  -0.999989485678529,0.997024265113336,-0.69266791809088,
  -0.999992463403904,0.997177567482597,-0.693293810560046,
  -0.998168670041896,0.234202002930654,0.397757295018613,
  -0.998398779709349,0.385186319412462,0.309863134853804,
  -0.998576095338357,0.517988723723858,0.208214440973434,
  -0.998719357216826,0.629703569518859,0.096222948116353,
  -0.998842476527936,0.720203330951345,-0.0207601551784617,
  -0.998955556213729,0.791300079843907,-0.136348134920346,
  -0.999065378019792,0.845813571958666,-0.24445016793907,
  -0.999175567184225,0.88683389735994,-0.340477179725281,
  -0.999286737933873,0.917262868364474,-0.421953754912466,
  -0.999396947303236,0.939594279052761,-0.488436998558289,
  -0.999502626750312,0.955853286353504,-0.540978607500742,
  -0.999599838919708,0.967622793392235,-0.581462483326913,
  -0.999685441175006,0.976107385917766,-0.612050478470221,
  -0.999757759055763,0.982206390023536,-0.634821102862626,
  -0.999816654953809,0.986582054280449,-0.651585654275972,
  -0.999863167558551,0.989717294446952,-0.663828002921204,
  -0.999899001887773,0.991961866430869,-0.672714826358979,
  -0.999926080483689,0.993567921696033,-0.679137945776486,
  -0.999946242902525,0.994716695840987,-0.683765847313538,
  -0.999961090399614,0.995538198798929,-0.687092756503707,
  -0.999971934849436,0.996125577031366,-0.689480524558436,
  -0.999979808120392,0.996545512563991,-0.691192260465585,
  -0.999985499344625,0.996845717383694,-0.692418338685915,
  -0.999989600274749,0.997060318675938,-0.693296025163123,
  -0.999992548548667,0.997213721346586,-0.693924046451602
};

} /* namespace DSP */

#endif /* _KS_TAB_H_ */