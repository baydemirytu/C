#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

typedef struct {
	// 	Dosyadan okunan her bir x-y ikilisini tutmak için oluþturulan struct.
    int x;
    int y;
}POINT;

typedef struct {
	//	POINT'ler arasýndaki mesafeler hesaplanýrken bulunan en yakýn nokta ikilisini tutmak için oluþturulan struct.
	//	distance bu noktalar arasýndaki mesafeyi tutar.
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

    	POINT * array = (POINT * ) malloc(sizeof(POINT) * 1); //Dosyadaki x-y ikililerini Point structýna alýp tutmak için oluþturulan dizi.
    //	POINT array[50]; 
	//	!!!!!!!!!----> Debug yaparken realloc fonksiyonu SIGTRAP hatasý almama sebep olmaktadýr.
	// 	Fakat realloc fonksiyonu çalýþma zamanýnda hiçbir hata vermemektedir, program doðru þekilde çalýþmaktadýr.
	//	Bu yüzden debug etmek istendiðinde malloc ve realloc fonskiyonlarýnýn yorum satýrýna alýnýp elle yer ayrýlmasý gerekmektedir.

    int counter = 0;//Dosyadan okunurken kaç nokta(satýr) olduðunu tutar.

    while (!feof(f)) {

        fscanf(f, "%d %d ", & array[counter].x, & array[counter].y); // x-y ikililerinin sýrasý ile array in ilgili gözlerine atanmasý.
        counter++;
		array = (POINT * ) realloc(array, sizeof(POINT) * counter);//array boyunun dinamik þekilde artýrýlmasý.

    }

    printf("Array has been read from file successfully:\n");
    printArray(array, counter);
	
    quickSort(array, 0, counter - 1); // x koordinatlarýna göre point lerin sýralanmasý.

    printf("\nArray has been sorted successfully:\n");
    printArray(array, counter);

    PAIR * pair = (PAIR * ) malloc(sizeof(PAIR));//Bir adet çiftimiz olacaðý için bir adet Pair allocate edilmesi.
    pair -> distance = FLT_MAX;//En yakýn nokta ikilisini bulurken distance larý karþýlaþtýracaðýmýz için initial deðerin çok yüksek sayý olarak atanmasý.

    float min_dist = findClosestPair(array, 0, counter, pair);
	
    printf("\nMin distance pair:\n(%d,%d)---(%d,%d)\n", pair -> point1.x, pair -> point1.y, pair -> point2.x, pair -> point2.y);//Ýkilinin noktalarý.
    printf("Distance between them:%f", min_dist);
	
	free(array); //Point arrayinin free edilmesi.
	
    return 0;
}

void printArray(POINT * array, int counter) {
	//Yazdýrýlmasý istenen point arrayini alýr ve counter sayýsýna kadar olan elemanlarýný yazdýrýr.Counter dizinin boyutudur.
    int i;

    for (i = 0; i < counter; i++) {
        printf("x:%d y:%d\n", array[i].x, array[i].y);
    }

}

float bruteForce(POINT * array, int left, int right, PAIR * pair) {
	//Gelen array in left-right arasýndaki elemanlarýndan en yakýn çifti bulur ve aralarýndaki mesafeyi geri gönderir.
	
    int i;
    int j;
    float min_dist = FLT_MAX;//ilk karþýlaþtýrmanýn kayda geçmesi için çok yüksek bir deðer atanmasý.
    float tmp_dist;// findPairDistance tan gelen deðeri tutmak için bir deðiþken.Bu deðiþken olmazsan fonksiyonu iki kere çaðýrmalýydýk.

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
	//	Gelen iki nokta arasýndaki mesafeyi hesaplar ve bu mesafe önceki çiftin arasýndaki mesafeden azsa pair ý günceller.
	//  bulunan distance geri döner.
	
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
	//Okuduðumuz arrayi x e göre sýralamak için kullanýlacak fonksiyon.Quicksort un POINT struct ý için implementasyonu.
	
    if (low < high) {
        int pivot = partition(array, low, high);

        quickSort(array, low, pivot - 1);
        quickSort(array, pivot + 1, high);

    }
}

int partition(POINT * array, int low, int high) {
	//Quciksort un partition kýsmý.
	
    int i = low;
    int j = high + 1;
    POINT pivot = array[low];//ilk eleman pivot olarak seçilir. 
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
	
	//	Her seferinde i<j þartýný kontrol edip elemanlarýn yerini deðiþtirmektense
	// 	kontrolsüz deðiþtirip son adýmda gereksiz yeri deðiþtirilen elemenalrýn eski yerlerine yazýlmasý.
	
    tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;

    array[low] = array[j];//Pivotun gelmesi gereken yerde olan elemanýn pivotun yerine geçmesi.
    array[j] = pivot;//pivotun yerine geçmesi

    return j;//Pivotun yerinin döndürülmesi.

}

float findClosestPair(POINT * array, int left, int right, PAIR * pair) {
	//Rekürsif olarak çalýþacak ana fonksiyonumuz.
	//Geri dönüþ olarak arraydeki left-right arasýnda belirtilen noktalar arasýndaki min distance ý gönderir.
    
	if (right - left <= 3) return bruteForce(array, left, right, pair);

    int medyan = (left + right) / 2; 

    float left_min_dist = findClosestPair(array, left, medyan, pair);//Medyanýn sol tarafýnda bulunan min distance ýn bulunmasý
    float right_min_dist = findClosestPair(array, medyan+1, right, pair);//Medyanýn sað tarafýnda bulunan min distance ýn bulunmasý

    float d = min(left_min_dist, right_min_dist);
    int i;//Çevrim deðiþkeni
    int j = 0;//Yeni oluþturulacak array in çevrim deðiþkeni.


	//Bulunduðumuz aralýktaki noktalarýn x koordinatlarýnýn medyan ýn x noktasý ile arasýndaki mesafenin bulunmasý.
	//eðer bu mesafe sol ve saðdaki parçalarýn min inden (d) küçükse aday nokta olarak tmp_array e atýlmasý.
	
	POINT * tmp_array = (POINT * ) malloc(sizeof(POINT) * (right - left));
	
    for (i = left; i < right ; i++) {

        if ( abs(array[i].x - array[medyan].x) <= d) {
            tmp_array[j++] = array[i];
        }

    }
	
	//tmp_array deki noktalarýn hepsinin bruteforce ile en yakýn çiftinin mesafesinin bulunmasý. d den küçükse yeni d olarak atanmasý.
    d = min(d, bruteForce(tmp_array, 0, j, pair));

    free(tmp_array);

    return d;

}

float min(float left_dist, float right_dist) {
	//Gelen iki float deðerin küçük olanýný geri döndürür.
	
    if (left_dist >= right_dist) return right_dist;

    return left_dist;
}


