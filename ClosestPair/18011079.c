#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

typedef struct {
	// 	Dosyadan okunan her bir x-y ikilisini tutmak i�in olu�turulan struct.
    int x;
    int y;
}POINT;

typedef struct {
	//	POINT'ler aras�ndaki mesafeler hesaplan�rken bulunan en yak�n nokta ikilisini tutmak i�in olu�turulan struct.
	//	distance bu noktalar aras�ndaki mesafeyi tutar.
    POINT point1;
    POINT point2;
    float distance; 
}PAIR;

void printArray(POINT * , int);
void quickSort(POINT * , int, int);
int partition(POINT * , int, int);
float bruteForce(POINT * , int, int, PAIR * );
float findPairDistance(POINT, POINT, PAIR * );
float findClosestPair(POINT * , int, int, PAIR * );
float min(float, float);


int main() {

    FILE * f = fopen("sample.txt", "r");

    if (f == NULL) {
        perror("File could not read!");
        exit(0);
    }

    	POINT * array = (POINT * ) malloc(sizeof(POINT) * 1); //Dosyadaki x-y ikililerini Point struct�na al�p tutmak i�in olu�turulan dizi.
    //	POINT array[50]; 
	//	!!!!!!!!!----> Debug yaparken realloc fonksiyonu SIGTRAP hatas� almama sebep olmaktad�r.
	// 	Fakat realloc fonksiyonu �al��ma zaman�nda hi�bir hata vermemektedir, program do�ru �ekilde �al��maktad�r.
	//	Bu y�zden debug etmek istendi�inde malloc ve realloc fonskiyonlar�n�n yorum sat�r�na al�n�p elle yer ayr�lmas� gerekmektedir.

    int counter = 0;//Dosyadan okunurken ka� nokta(sat�r) oldu�unu tutar.

    while (!feof(f)) {

        fscanf(f, "%d %d ", & array[counter].x, & array[counter].y); // x-y ikililerinin s�ras� ile array in ilgili g�zlerine atanmas�.
        counter++;
		array = (POINT * ) realloc(array, sizeof(POINT) * counter);//array boyunun dinamik �ekilde art�r�lmas�.

    }

    printf("Array has been read from file successfully:\n");
    printArray(array, counter);
	
    quickSort(array, 0, counter - 1); // x koordinatlar�na g�re point lerin s�ralanmas�.

    printf("\nArray has been sorted successfully:\n");
    printArray(array, counter);

    PAIR * pair = (PAIR * ) malloc(sizeof(PAIR));//Bir adet �iftimiz olaca�� i�in bir adet Pair allocate edilmesi.
    pair -> distance = FLT_MAX;//En yak�n nokta ikilisini bulurken distance lar� kar��la�t�raca��m�z i�in initial de�erin �ok y�ksek say� olarak atanmas�.

    float min_dist = findClosestPair(array, 0, counter, pair);
	
    printf("\nMin distance pair:\n(%d,%d)---(%d,%d)\n", pair -> point1.x, pair -> point1.y, pair -> point2.x, pair -> point2.y);//�kilinin noktalar�.
    printf("Distance between them:%f", min_dist);
	
	free(array); //Point arrayinin free edilmesi.
	
    return 0;
}

void printArray(POINT * array, int counter) {
	//Yazd�r�lmas� istenen point arrayini al�r ve counter say�s�na kadar olan elemanlar�n� yazd�r�r.Counter dizinin boyutudur.
    int i;

    for (i = 0; i < counter; i++) {
        printf("x:%d y:%d\n", array[i].x, array[i].y);
    }

}

float bruteForce(POINT * array, int left, int right, PAIR * pair) {
	//Gelen array in left-right aras�ndaki elemanlar�ndan en yak�n �ifti bulur ve aralar�ndaki mesafeyi geri g�nderir.
	
    int i;
    int j;
    float min_dist = FLT_MAX;//ilk kar��la�t�rman�n kayda ge�mesi i�in �ok y�ksek bir de�er atanmas�.
    float tmp_dist;// findPairDistance tan gelen de�eri tutmak i�in bir de�i�ken.Bu de�i�ken olmazsan fonksiyonu iki kere �a��rmal�yd�k.

    for (i = left; i < right; i++) {

        for (j = i + 1; j < right; j++) {

            tmp_dist = findPairDistance(array[i], array[j], pair);

            if (min_dist > tmp_dist) {
                min_dist = tmp_dist;
            }
        }

    }

    return min_dist;
}

float findPairDistance(POINT point1, POINT point2, PAIR * pair) {	
	//	Gelen iki nokta aras�ndaki mesafeyi hesaplar ve bu mesafe �nceki �iftin aras�ndaki mesafeden azsa pair � g�nceller.
	//  bulunan distance geri d�ner.
	
	int x_diff = point1.x - point2.x;
    x_diff *= x_diff;

    int y_diff = point1.y - point2.y;
    y_diff *= y_diff;

    float distance = sqrt( x_diff + y_diff );

    if (distance < pair -> distance) {
        pair -> point1 = point1;
        pair -> point2 = point2;
        pair -> distance = distance;
    }

    return distance;

}

void quickSort(POINT * array, int low, int high) {
	//Okudu�umuz arrayi x e g�re s�ralamak i�in kullan�lacak fonksiyon.Quicksort un POINT struct � i�in implementasyonu.
	
    if (low < high) {
        int pivot = partition(array, low, high);

        quickSort(array, low, pivot - 1);
        quickSort(array, pivot + 1, high);

    }
}

int partition(POINT * array, int low, int high) {
	//Quciksort un partition k�sm�.
	
    int i = low;
    int j = high + 1;
    POINT pivot = array[low];//ilk eleman pivot olarak se�ilir. 
    POINT tmp;

    do {
        do {
            i++;
        } while (array[i].x <= pivot.x);
        do {
            j--;
        } while (array[j].x > pivot.x);

        tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;

    } while (i < j);
	
	//	Her seferinde i<j �art�n� kontrol edip elemanlar�n yerini de�i�tirmektense
	// 	kontrols�z de�i�tirip son ad�mda gereksiz yeri de�i�tirilen elemenalr�n eski yerlerine yaz�lmas�.
	
    tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;

    array[low] = array[j];//Pivotun gelmesi gereken yerde olan eleman�n pivotun yerine ge�mesi.
    array[j] = pivot;//pivotun yerine ge�mesi

    return j;//Pivotun yerinin d�nd�r�lmesi.

}

float findClosestPair(POINT * array, int left, int right, PAIR * pair) {
	//Rek�rsif olarak �al��acak ana fonksiyonumuz.
	//Geri d�n�� olarak arraydeki left-right aras�nda belirtilen noktalar aras�ndaki min distance � g�nderir.
    
	if (right - left <= 3) return bruteForce(array, left, right, pair);

    int medyan = (left + right) / 2; 

    float left_min_dist = findClosestPair(array, left, medyan, pair);//Medyan�n sol taraf�nda bulunan min distance �n bulunmas�
    float right_min_dist = findClosestPair(array, medyan+1, right, pair);//Medyan�n sa� taraf�nda bulunan min distance �n bulunmas�

    float d = min(left_min_dist, right_min_dist);
    int i;//�evrim de�i�keni
    int j = 0;//Yeni olu�turulacak array in �evrim de�i�keni.


	//Bulundu�umuz aral�ktaki noktalar�n x koordinatlar�n�n medyan �n x noktas� ile aras�ndaki mesafenin bulunmas�.
	//e�er bu mesafe sol ve sa�daki par�alar�n min inden (d) k���kse aday nokta olarak tmp_array e at�lmas�.
	
	POINT * tmp_array = (POINT * ) malloc(sizeof(POINT) * (right - left));
	
    for (i = left; i < right ; i++) {

        if ( abs(array[i].x - array[medyan].x) <= d) {
            tmp_array[j++] = array[i];
        }

    }
	
	//tmp_array deki noktalar�n hepsinin bruteforce ile en yak�n �iftinin mesafesinin bulunmas�. d den k���kse yeni d olarak atanmas�.
    d = min(d, bruteForce(tmp_array, 0, j, pair));

    free(tmp_array);

    return d;

}

float min(float left_dist, float right_dist) {
	//Gelen iki float de�erin k���k olan�n� geri d�nd�r�r.
	
    if (left_dist >= right_dist) return right_dist;

    return left_dist;
}


