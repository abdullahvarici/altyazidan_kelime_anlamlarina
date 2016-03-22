/*
	Bu program Abdullah Varýcý tarafýndan yazýlmýþtýr.
	Benim ve program hakkýnda detaylý bilgi için:
	abdullahvarici.blogspot.com
	Bu programý kiþisel ihtiyacýnýza uygun olarak düzenleyip kullanabilirsiniz.
	
	Program sizden altyazý.srt dosyasýný alýr, içerisinden kelimeleri çeker ve 
	tekrar sayýsýyla birlikte sýralý bir liste haline getirir. Bir de sizden 
	"bildiklerim.txt" isminde anlamýný bildiðiniz kelimelerin içinde bulunduðu 
	bir nevi sizin Ýngilizce kelime haznenizi yansýtan bir dosyayý alýyor ayný 
	iþlemleri buna da uyguluyor. Daha sonra bu iki dosyadaki kelimeleri karþýlaþtýrýyor. 
	Ýki listedeki kelimeleri fark, kesiþim ve birleþim kümesi olarak farklý 
	belgelere yazýyor. Sonrasýnda altyazýda bulunan ama anlamýný bilmediðiniz 
	kelimeleri teker teker herhangi bir çekim eki aldýysa yalýn hale çeviriyor 
	(bunu hem veritabaný kullanarak hem de grammer kurallarýný kullanarak yapýyor) 
	ve bu kelimelerin anlamlarýný sözlük veritabanýndan kontrol edip yeni bir 
	liste oluþturuyor. Son haldeki listemiz dizide geçen anlamýný bilmediðimiz 
	kelimelerini, bu kelimelerin yalýn hallerini, dizide tekrar etme miktarýný ve 
	sözlükteki anlamlarýný içeriyor. Bu programý diziye baþlamadan önce çalýþtýrýp, 
	anlamýný bilmediðiniz kelimelerin bir kýsmýna çalýþtýktan sonra çalýþtýrarak 
	Ýngilizcemizi, en azýndan Ýngilizce kelime bilgimizi rahatça geliþtirebiliriz. 
	Bu programýn hedefi dizi film ve altyazý ile kýsýtlý deðil farklý amaçlar 
	için de kullanýlabilir. Mesela iki farklý Ýngilizce metni karþýlaþtýrmak, 
	kelimelerin tekrarlý listesini çýkarýp anlamlarýna ulaþmak için de rahatça kullanýlabilir.
*/

#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
using namespace std;

#define UZUNLUK 30			// Max. kelime uzunluðu

struct Kelime{				// Kelimelere ait struct yapýsý oluþturuluyor.
	string kelime_ekli;		// Yalýn olmayan, ek bulundurabilen kelime. Ör: goes, books, book;
	string kelime_yalin;	// kelime_ekli'deki kelimenin yalýn hali. Ör: go, book, book;
	string anlam;			// Kelimenin sözlükteki anlamý. Ör: f. (went, gone) 1. gitmek. 2. -e çýkmak: She´s gone... 
	int tekrar;				// Kelimenin altyazýdaki tekrar miktarý.
	int toplam_tekrar;		// Ýki metin karþýlaþtýrýldýðýndaki kelimenin toplam tekrar miktarý.
	bool kiyas_durumu;		// Ýki metini karþýlaþtýrýrken ihtiyaç duyuyoruz. 0 -> default ve ortak deðil. 1 -> Kelime iki metinde de mevcut.
	bool sozluk_durumu;		// Kelime sözlükte varsa 1, yoksa 0.
	bool cekim_durumu;		// Kelime conjuge ve çoðullar listesinde varsa 1, yoksa 0
};

void vector_oku(char adres[100], vector <Kelime>* kelimeler);	
void vector_alfabetik_sirala(vector <Kelime>* kelimeler);
void vector_ekrana_yaz_anlamsiz(vector <Kelime> kelimeler);
void vector_dosyaya_yaz_anlamsiz(char adres[100], vector <Kelime> kelimeler);
void vector_tekrari_sil(vector <Kelime>* kelimeler);
void vector_tekrara_gore_sirala(vector <Kelime>* kelimeler);
void vector_karsilastir(vector <Kelime>* kelimeler_1, vector <Kelime>* kelimeler_2, vector <Kelime>* kelimeler_3, 
						vector <Kelime>* kelimeler_4, vector <Kelime>* kelimeler_5, vector <Kelime>* kelimeler_6);
void sozluk_oku(char adres[100], vector <Kelime>* kelimeler);
void cekimli_oku(char adres[100], vector <Kelime>* kelimeler);
int anlamlari_bul(vector <Kelime> kelimeler, vector <Kelime> sozluk_kelimeler, vector <Kelime> cekimli_kelimeler, vector <Kelime>* anlamli_kelimeler);
void anlamlari_duzenle(vector <Kelime>* anlamli_kelimeler);
void vector_ekrana_yaz_anlamli(char adres[100], vector <Kelime> anlamli_kelimeler);
void vector_dosyaya_yaz_anlamli(char adres[100], vector <Kelime> anlamli_kelimeler);


int main(){
	setlocale(LC_ALL, "Turkish");
		
	vector <Kelime> sozluk_vector;
	vector <Kelime> cekimli_vector;	
	vector <Kelime> altyazi_vector;
	vector <Kelime> bildiklerim_vector;
	vector <Kelime> afb;
	vector <Kelime> bfa;
	vector <Kelime> anb;
	vector <Kelime> aub;
	vector <Kelime> anlamli_vector;
	int bulunan;
		
	cout << "Sözlükteki kelimeler taranýyor..." << endl;
	sozluk_oku("sözlük.txt", &sozluk_vector);
	cout << "Sözlükte  <" << sozluk_vector.size() << ">  adet kelime bulundu."<< endl << endl;
	
	cout << "Çekimli kelimeler listesi taranýyor..." << endl;
	cekimli_oku("çekimli kelimeler.txt", &cekimli_vector);
	cout << "Çekimli kelimeler listesinde  <" << cekimli_vector.size() << ">  adet kelime bulundu."<< endl << endl;
	
	cout << "altyazý.srt'deki kelimeler alýnýyor..." << endl;
	vector_oku("altyazý.srt", &altyazi_vector);
	cout << "altyazý.srt'den toplam  <" << altyazi_vector.size() << ">  adet kelime alýndý." << endl << endl;
	
	cout << "bildiklerim.txt'deki kelimeler alýnýyor..." << endl;
	vector_oku("bildiklerim.txt", &bildiklerim_vector);
	cout << "bildiklerim.txt'den toplam  <" << bildiklerim_vector.size() << ">  adet kelime alýndý." << endl << endl;
	
	cout << "Listeler alfabetik sýralanýyor..." << endl;
	vector_alfabetik_sirala(&altyazi_vector);
	vector_alfabetik_sirala(&bildiklerim_vector);
	
	cout << "Tekrarsýz listeler oluþturuluyor..." << endl << endl;
	vector_tekrari_sil(&altyazi_vector);
	vector_tekrari_sil(&bildiklerim_vector);
	
	cout << "altyazý.srt'de  <" << altyazi_vector.size() << ">  adet 'farklý' kelime var." << endl;
	cout << "bildiklerim.txt'de  <" << bildiklerim_vector.size() << ">  adet 'farklý' kelime var." << endl << endl; 
	
	cout << "altyazý.srt'den alfabetik liste a_alf.txt dosyasýna yazdýrýlýyor..." << endl;
	vector_dosyaya_yaz_anlamsiz("a_alf.txt", altyazi_vector);
	cout << "bildiklerim.txt'den alfabetik liste b_alf.txt dosyasýna yazdýrýlýyor..." << endl << endl;
	vector_dosyaya_yaz_anlamsiz("b_alf.txt", bildiklerim_vector);
	
	cout << "Listeler tekrara göre sýralanýyor..." << endl;
	vector_tekrara_gore_sirala(&altyazi_vector);
	vector_tekrara_gore_sirala(&bildiklerim_vector);
	
	cout << "altyazý.srt'den tekrar sýralý liste a_tek.txt dosyasýna yazdýrýlýyor..." << endl;
	vector_dosyaya_yaz_anlamsiz("a_tek.txt", altyazi_vector);
	cout << "bildiklerim.txt'den tekrar sýralý liste b_tek.txt dosyasýna yazdýrýlýyor..." << endl << endl;
	vector_dosyaya_yaz_anlamsiz("b_tek.txt", bildiklerim_vector);
	
	vector_alfabetik_sirala(&altyazi_vector);
	vector_alfabetik_sirala(&bildiklerim_vector);
	
	cout << "a_alf.txt ve b_alf.txt'deki kelimeler karþýlaþtýrýlýyor..." << endl;
	vector_karsilastir(&altyazi_vector, &bildiklerim_vector, &afb, &bfa, &anb, &aub);	
	vector_dosyaya_yaz_anlamsiz("afb.txt", afb);
	vector_dosyaya_yaz_anlamsiz("bfa.txt", bfa);
	vector_dosyaya_yaz_anlamsiz("anb.txt", anb);
	vector_dosyaya_yaz_anlamsiz("aub.txt", aub);
	cout << "Listeler karþýlaþtýrýldý ve dosyalara yazýldý." << endl;
	cout << "afb.txt=a fark b | bfa.txt=b fark a | anb.txt=a kesiþim b | aub.txt=a birleþim b" << endl;
	cout << "Son durumda a, b, afb, bfa, anb, aub listelerindeki kelime sayýlarý:" << endl;
	cout <<"a: "<<altyazi_vector.size() <<"   "<<"b: "<< bildiklerim_vector.size()<<"   "<<"afb: "<< afb.size();
	cout <<"   "<<"bfa: "<< bfa.size()<<"   "<<"anb: "<< anb.size()<<"   "<<"aub: "<< aub.size() << endl << endl;
	cout << "altyazý.srt'deki kelimelerin  %" << (float)anb.size()/altyazi_vector.size()*100 << "'ýný zaten biliyorsun." << endl <<endl;
	
	cout << "afb.txt'deki kelimelerin anlamlarý alýnýyor..." << endl;
	bulunan = anlamlari_bul(afb, sozluk_vector, cekimli_vector, &anlamli_vector);
	cout << "Sözlükten   " << bulunan << "  adet kelimenin anlamýna bakýldý."<< endl;
	cout << afb.size()-bulunan << " adet kelime sözlükte ve çekimli kelime listesinde bulunamadý. Oran: "; 
	cout << "%"<< (float)bulunan/afb.size()*100 << endl << endl;
	
	cout << "Kelimeler ve anlamlarý listesi oluþturuluyor..." << endl;
	anlamlari_duzenle(&anlamli_vector);
	vector_dosyaya_yaz_anlamli("afb_anlamlar_alf.txt", anlamli_vector);
	cout << "afb_anlamlar_alf.txt dosyasýnda alfabetik liste oluþturuldu." << endl << endl;

	cout << "Tekrara dayalý liste oluþturuluyor..." << endl;
	vector_tekrara_gore_sirala(&anlamli_vector);
	vector_dosyaya_yaz_anlamli("afb_anlamlar_tek.txt", anlamli_vector);
	cout << "afb_anlamlar_tek.txt dosyasýnda tekrara göre sýralý liste oluþturuldu." << endl << endl;
	
	cout <<endl << endl << "Burada bitti iþimiz. Ýyi çalýþmalar..." << endl;
		
	getchar();
}

// Altyazý dosyasýný açar. Ýçinden 2 ila 30 karakter büyüklüðündeki kelimeleri vector'e atar. 
// Bütün harfleri küçük harfe dönüþtürür ve ingilizcedeki 26 karakter haricindeki karakterleri almaz; 
// denk geldiðinde kelimenin o karaktere kadar olan kýsmýný alýr.
// Kelimeye ait tekrar baþlangýç deðerini 1; cekim_durumu ve sozluk_durumu deðerlerini 0 olarak atar.
void vector_oku(char adres[100], vector <Kelime>* kelimeler){		
	FILE *pRead;
	if((pRead = fopen(adres, "r")) == NULL){
		cout << adres <<" açýlamadý.";
	}
	
	int k=0;
	char harf, temp[UZUNLUK];
	Kelime tempStruct;
	while(!feof(pRead)){
		memset(temp, 0, UZUNLUK);
		fscanf(pRead, "%c", &harf);
		
		if((harf<='z' && harf>='a') || (harf<='Z' && harf>='A')){
			for(int i=0;i<UZUNLUK;i++){
				if(harf<='z' && harf>='a')
					temp[i]=harf;
				else if(harf<='Z' && harf>='A')
					temp[i]=harf+32;
				else{
					if(strlen(temp)>2){
						tempStruct.kelime_ekli = temp;
						tempStruct.tekrar = 1;
						tempStruct.cekim_durumu = 0;
						tempStruct.sozluk_durumu = 0;
						kelimeler->push_back(tempStruct);
					}
					break;
				}
				fscanf(pRead, "%c", &harf);
			}
		}
	}
	fclose(pRead);
}

// Aldýðý vector'deki kelimelerin kelime_ekli deðiþkenini baz alarak alfabetik olarak sýralama yapar. 
void vector_alfabetik_sirala(vector <Kelime>* kelimeler){
	Kelime tempStruct;
	for(int i=0; i<kelimeler->size(); i++){
		for(int j=0; j<kelimeler->size()-1; j++){
			if((*kelimeler)[j].kelime_ekli > (*kelimeler)[j+1].kelime_ekli){
				tempStruct = (*kelimeler)[j];
				(*kelimeler)[j] = (*kelimeler)[j+1];
				(*kelimeler)[j+1] = tempStruct;
			}
		}
	}
}

// Aldýðý vector'deki sýralamaya uygun olarak (alfabetik - tekrara dayalý - hiçbiri) tekrarý ve kelimeyi konsol ekranýna yazar.
void vector_ekrana_yaz_anlamsiz(vector <Kelime> kelimeler){
	for(int i=0; i<kelimeler.size(); i++)
		cout <<"("<< kelimeler[i].tekrar <<")\t"<< kelimeler[i].kelime_ekli << endl;	
}

// Aldýðý vector'deki sýralamaya uygun olarak (alfabetik - tekrara dayalý - hiçbiri) tekrarý ve kelimeyi dosyaya yazar.
void vector_dosyaya_yaz_anlamsiz(char adres[100], vector <Kelime> kelimeler){
	ofstream outFile(adres);
	for(int i=0; i<kelimeler.size(); i++)
		outFile <<"("<< kelimeler[i].tekrar <<")\t"<< kelimeler[i].kelime_ekli << endl;
	outFile.close();
}

// Aldýðý vector'deki tekrar eden kelimelerin tekrar sayýsýný hesaplar ve tekrarsýz bir liste oluþturur. 
void vector_tekrari_sil(vector <Kelime>* kelimeler){
	vector_alfabetik_sirala(kelimeler);
	for(int i=0; i<kelimeler->size()-1; i++){
		if((*kelimeler)[i].kelime_ekli == (*kelimeler)[i+1].kelime_ekli){
			(*kelimeler)[i].tekrar++;
			kelimeler->erase(kelimeler->begin()+i+1);
			i--;
		}
	}
}

// Aldýðý vector'u kelimelerin tekrar sayýsýna göre çoktan aza doðru sýralar.
// Vector'un tekrar etmeyen ve tekrar sayýsý hesaplanmýþ kelimelerden oluþtuðuna emin olunuz. 
void vector_tekrara_gore_sirala(vector <Kelime>* kelimeler){
	Kelime tempStruct;
	for(int i=0; i<kelimeler->size(); i++){
		for(int j=0; j<kelimeler->size()-1; j++){
			if((*kelimeler)[j].tekrar < (*kelimeler)[j+1].tekrar){		// "<" yerine ">" kullanarak azdan çoða doðru sýramlamýþ olursun
				tempStruct = (*kelimeler)[j];
				(*kelimeler)[j] = (*kelimeler)[j+1];
				(*kelimeler)[j+1] = tempStruct;
			}
		}
	}	
}

// Sýrasýyla a b afb bfa anb aub vector'lerini aldýðýný düþünelim. 
// afb->a fark b	bfa->b fark a	anb->a kesiþim b	aub->a birleþim b
// a ve b vector'leri herhangi iki farklý altyazýdan alýnan kelimelerden oluþmuþ ve sýralanmýþ olmalýdýr.
// a ve b vector'leri karþýlaþtýrýlýr. a'da bulunan ama b'de bulunmayan kelimelerin listesi afb'ye atanýr.
// b'de bulunan ama a'da bulunmayan kelimelerin listesi bfa'ya atanýr. Kelimelerdeki tekrar sayýlarý korunur ve atanýr.
// Hem a'da hem b'de bulunan kelimelerin listesi anb'ye atanýr ve anb'deki kelimelerin tekrar sayýsý a'daki ve b'deki tekrar sayýsýnýn toplamýna eþitlenir.
// afb, anb ve bfa vector'leri olduðu gibi birleþtirilip aub vector'üne atanýr.
void vector_karsilastir(vector <Kelime>* kelimeler_1, vector <Kelime>* kelimeler_2, vector <Kelime>* kelimeler_3, 
					vector <Kelime>* kelimeler_4, vector <Kelime>* kelimeler_5, vector <Kelime>* kelimeler_6){
	for(int i=0; i<kelimeler_1->size(); i++){
		(*kelimeler_1)[i].kiyas_durumu = 0;
	}
	for(int i=0; i<kelimeler_2->size(); i++){
		(*kelimeler_2)[i].kiyas_durumu = 0;
	}
	
	for(int i=0; i<kelimeler_1->size(); i++){
		for(int j=0; j<kelimeler_2->size(); j++){
			if((*kelimeler_1)[i].kelime_ekli == (*kelimeler_2)[j].kelime_ekli){
				(*kelimeler_1)[i].kiyas_durumu = 1;
				(*kelimeler_2)[j].kiyas_durumu = 1;
				(*kelimeler_1)[i].toplam_tekrar = (*kelimeler_1)[i].tekrar + (*kelimeler_2)[j].tekrar;
				(*kelimeler_2)[j].toplam_tekrar = (*kelimeler_1)[i].toplam_tekrar;
				break;
			}
		}
	}
	
	int k=0;
	for(int i=0; i<kelimeler_1->size(); i++){
		if((*kelimeler_1)[i].kiyas_durumu == 0){
			kelimeler_3->push_back((*kelimeler_1)[i]);
		}
		else{
			kelimeler_5->push_back((*kelimeler_1)[i]);
			(*kelimeler_5)[k].tekrar = (*kelimeler_5)[k].toplam_tekrar;
			k++;
		}
	}
		
	for(int i=0; i<kelimeler_2->size(); i++){
		if((*kelimeler_2)[i].kiyas_durumu == 0){
			kelimeler_4->push_back((*kelimeler_2)[i]);
		}
	}
	
	for(int i=0; i<kelimeler_3->size(); i++){
		kelimeler_6->push_back((*kelimeler_3)[i]);
	}
	for(int i=0; i<kelimeler_4->size(); i++){
		kelimeler_6->push_back((*kelimeler_4)[i]);
	}
	for(int i=0; i<kelimeler_5->size(); i++){
		kelimeler_6->push_back((*kelimeler_5)[i]);
	}
	
}

// Sözlük metnini açar ve kelimeleri kelime_yalin'e anlamlarýný anlam'e atar.
void sozluk_oku(char adres[100], vector <Kelime>* kelimeler){
	ifstream input(adres);
	Kelime temp;
	while(getline(input, temp.kelime_yalin)){
		getline(input, temp.anlam);
		kelimeler->push_back(temp);
	}
}

// Conjuge ve çoðullar metnini açar. Kelimelerin yalýn hallerini kelime_yalin'e, ek bulundaran halini kelime_ekli'e atar.
void cekimli_oku(char adres[100], vector <Kelime>* kelimeler){
	ifstream input(adres);
	Kelime temp;
	while(getline(input, temp.kelime_yalin)){
		getline(input, temp.kelime_ekli);
		kelimeler->push_back(temp);
	}
}

// Bu fonksiyonun ilk argümenti tarama yapacaðý alfabetik sýralý ve tekrarsýz kelimelerin bulunduðu altyazý ya da karþýlaþtýrýlmýþ vector'lerden biridir.
// ikinci argument sözlük vector'ü; üçüncüsü conjuge ve çoðullar vector'ü ve sonuncusu da sonuç vector'üdür.
// Bu fonksiyondan önce sozluk_oku ve cekimli_oku fonksiyonlarý çaðýrýlmalýdýr.
// Biraz anlamli ve karýþýk olan bu fonksiyon özet olarak önce kelimelerin kelime_ekli'lerini sözlükteki kelime_yalin'lerle karþýlaþtýrýp;
// bulduklarýný anlamlarýyla beraber sonuç vector'üne yazar. 
// Ayný zamanda sözcüklerin ek almýþ olma olasýlýða karþýn conjuge ve çoðullar listesinde arayýp tekrar sözlükte arar ve 
// yine bu kelimelere ait yalýn hallerini ve anlamlarýný sonuç vector'üne atar.
// Fonksiyon kelimelerin kaçýný sözlük veya conjuge listesinde bulduysa sonucu return eder.
int anlamlari_bul(vector <Kelime> kelimeler, vector <Kelime> sozluk_kelimeler, vector <Kelime> cekimli_kelimeler, vector <Kelime>* anlamli_kelimeler){
	Kelime temp;
	int bulunan = kelimeler.size();
	int flag = 0;
	int flag1 = 0;
	int flag2 = 0;
	int flag3 = 0;
	string kelime;
	
	for(int i=0;i<kelimeler.size(); i++){
		for(int j=0; j<sozluk_kelimeler.size(); j++){
			if(kelimeler[i].kelime_ekli == sozluk_kelimeler[j].kelime_yalin){
				temp.kelime_ekli = kelimeler[i].kelime_ekli;
				temp.kelime_yalin = kelimeler[i].kelime_ekli;
				temp.tekrar = kelimeler[i].tekrar;
				temp.anlam = sozluk_kelimeler[j].anlam;
				anlamli_kelimeler->push_back(temp);
				flag1 = 1;
				break;
			}
		}
		
		for(int k=0; k<cekimli_kelimeler.size(); k++){
			if(kelimeler[i].kelime_ekli == cekimli_kelimeler[k].kelime_ekli){
				temp.kelime_ekli = kelimeler[i].kelime_ekli;
				temp.kelime_yalin = cekimli_kelimeler[k].kelime_yalin;
				temp.tekrar = kelimeler[i].tekrar;
				
				for(int m=0; m<sozluk_kelimeler.size(); m++){
					if(temp.kelime_yalin == sozluk_kelimeler[m].kelime_yalin){
						temp.anlam = sozluk_kelimeler[m].anlam;
						flag = 1;
						flag2 = 1;
					}
				}
				
				if(flag == 0)
					temp.anlam = "??????????";
					
				flag = 0;	
				anlamli_kelimeler->push_back(temp);
				break;
			}
		}
		
		if(flag1==0 && flag2==0){
			for(int j=0; j<sozluk_kelimeler.size(); j++){
				if(kelimeler[i].kelime_ekli == sozluk_kelimeler[j].kelime_yalin+"s"){
					temp.kelime_ekli = kelimeler[i].kelime_ekli;
					temp.kelime_yalin = sozluk_kelimeler[j].kelime_yalin;
					temp.tekrar = kelimeler[i].tekrar;
					temp.anlam = sozluk_kelimeler[j].anlam;
					anlamli_kelimeler->push_back(temp);
					flag3=1;
					break;
				}
			}
			
			if(flag3==0){
				temp.kelime_ekli = kelimeler[i].kelime_ekli;
				temp.kelime_yalin = "???";
				temp.tekrar = kelimeler[i].tekrar;
				temp.anlam = "??????????";
				anlamli_kelimeler->push_back(temp);
				bulunan--;
			}
		}
		flag1=0;
		flag2=0;
		flag3=0;
	}
	return bulunan;
}

// anlamli_kelimeler vector'undeki tekrar eden ve anlamlarý ayný olan kelimelerin tekrarlarý silinir.
void anlamlari_duzenle(vector <Kelime>* anlamli_kelimeler){
	for(int i=0; i<anlamli_kelimeler->size()-1; i++){
		if(((*anlamli_kelimeler)[i].kelime_ekli == (*anlamli_kelimeler)[i+1].kelime_ekli) && ((*anlamli_kelimeler)[i].anlam == (*anlamli_kelimeler)[i+1].anlam)){
			anlamli_kelimeler->erase(anlamli_kelimeler->begin()+i+1);
		}
	}
}

// anlamli_kelimeler vector'unu sözlükten bulunmuþ anlamlarý ve altyazýdaki tekrar miktarlarýyla ekrana yazar.
void vector_ekrana_yaz_anlamli(char adres[100], vector <Kelime> anlamli_kelimeler){
	for(int i=0; i<anlamli_kelimeler.size(); i++)
		cout <<"("<< anlamli_kelimeler[i].tekrar <<")  "<< anlamli_kelimeler[i].kelime_ekli <<" -> "<< anlamli_kelimeler[i].kelime_yalin <<" = "<< anlamli_kelimeler[i].anlam << endl << endl;
}

// anlamli_kelimeler vector'unu sözlükten bulunmuþ anlamlarý ve altyazýdaki tekrar miktarlarýyla dosyaya yazar.
void vector_dosyaya_yaz_anlamli(char adres[100], vector <Kelime> anlamli_kelimeler){
	ofstream outFile(adres);
	for(int i=0; i<anlamli_kelimeler.size(); i++)
		outFile <<"("<< anlamli_kelimeler[i].tekrar <<")  "<< anlamli_kelimeler[i].kelime_ekli <<" -> "<< anlamli_kelimeler[i].kelime_yalin <<" = "<< anlamli_kelimeler[i].anlam << endl << endl;
	outFile.close();
}


