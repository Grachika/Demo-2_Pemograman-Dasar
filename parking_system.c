#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// 1. Desain Struct utama
struct Kendaraan {
    char plat[50];
    char jenis[20]; // "Motor" atau "Mobil"
    int jamMasuk;
    int menitMasuk;
    int jamKeluar;
    int menitKeluar;
    int biaya;
    int status; // 1 = Masih Parkir, 0 = Sudah Keluar
};

// 2. Fungsi Pengecek Format Plat Nomor
int validasiPlat(char plat[]) {
    int panjang = strlen(plat);
    int adaAngka = 0;

    // Cek batas panjang karakter
    if (panjang < 3 || panjang > 11) {
        return 0; 
    }

    // Wajib diawali dengan huruf
    if (!isalpha(plat[0])) {
        return 0;
    }

    // Pengecekan tiap karakter
    for (int i = 0; i < panjang; i++) {
        if (isdigit(plat[i])) {
            adaAngka = 1;
        }
        
        // Menolak jika ada simbol aneh, hanya boleh huruf, angka, dan spasi
        if (!isalpha(plat[i]) && !isdigit(plat[i]) && !isspace(plat[i])) {
            return 0; 
        }
    }

    // Harus mengandung minimal satu angka
    if (adaAngka == 1) {
        return 1;
    }

    return 0;
}

// 3. Fungsi hitung biaya berdasarkan total selisih menit
void hitungBiaya(struct Kendaraan *k) {
    int totalMasuk = (k->jamMasuk * 60) + k->menitMasuk;
    int totalKeluar = (k->jamKeluar * 60) + k->menitKeluar;
    int durasiMenit = totalKeluar - totalMasuk;

    // Antisipasi jika waktu sama persis (baru masuk langsung keluar)
    if (durasiMenit <= 0) durasiMenit = 1;

    int durasiJam = durasiMenit / 60;
    
    // Pembulatan ke atas: Jika ada sisa menit, dihitung penuh 1 jam berikutnya
    if (durasiMenit % 60 > 0 || durasiJam == 0) {
        durasiJam++;
    }

    // Abaikan case-sensitive saat mengecek jenis kendaraan
    if (strcmp(k->jenis, "Mobil") == 0 || strcmp(k->jenis, "mobil") == 0) {
        k->biaya = durasiJam * 5000;
    } else {
        k->biaya = durasiJam * 2000;
    }
}

// 4. Fungsi Kendaraan Masuk (Otomatis ambil waktu lokal & validasi plat)
void inputMasuk(struct Kendaraan *arrKendaraan, int *jumlah) {
    int platValid = 0;
    printf("\n--- Input Kendaraan Masuk ---\n");
    
    // Looping keamanan agar user tidak memasukkan input plat sembarangan
    do {
        printf("Plat Nomor  : ");
        scanf(" %[^\n]", arrKendaraan[*jumlah].plat);
        
        platValid = validasiPlat(arrKendaraan[*jumlah].plat);
        
        if (platValid == 0) {
            printf("Format plat ditolak. Pastikan diawali huruf, tidak pakai simbol, dan ada angkanya.\n");
        }
    } while (platValid == 0);

    printf("Jenis (Mobil/Motor): ");
    scanf(" %s", arrKendaraan[*jumlah].jenis);

    // Menarik waktu real-time dari komputer
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    arrKendaraan[*jumlah].jamMasuk = tm.tm_hour;
    arrKendaraan[*jumlah].menitMasuk = tm.tm_min;
    arrKendaraan[*jumlah].status = 1; // Menandakan kendaraan sedang di dalam area parkir

    printf("Berhasil! Waktu Masuk: %02d:%02d\n", 
           arrKendaraan[*jumlah].jamMasuk, arrKendaraan[*jumlah].menitMasuk);
    (*jumlah)++;
}

// 5. Fungsi Kendaraan Keluar (Checkout dan Hitung tarif)
void prosesKeluar(struct Kendaraan *arrKendaraan, int jumlah) {
    char cariPlat[50];
    int ditemukan = 0;
    
    printf("\n--- Proses Kendaraan Keluar ---\n");
    printf("Masukkan Plat Nomor: ");
    scanf(" %[^\n]", cariPlat);

    for (int i = 0; i < jumlah; i++) {
        // Cek kecocokan plat DAN pastikan statusnya masih parkir
        if (strcmp(arrKendaraan[i].plat, cariPlat) == 0 && arrKendaraan[i].status == 1) {
            
            // Tarik waktu real-time lagi untuk jam keluar
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            arrKendaraan[i].jamKeluar = tm.tm_hour;
            arrKendaraan[i].menitKeluar = tm.tm_min;
            arrKendaraan[i].status = 0; // Ubah status jadi sudah keluar

            // Hitung biaya parkir via pointer
            hitungBiaya(&arrKendaraan[i]);

            printf("\nKendaraan Berhasil Checkout!\n");
            printf("Plat        : %s\n", arrKendaraan[i].plat);
            printf("Waktu Masuk : %02d:%02d\n", arrKendaraan[i].jamMasuk, arrKendaraan[i].menitMasuk);
            printf("Waktu Keluar: %02d:%02d\n", arrKendaraan[i].jamKeluar, arrKendaraan[i].menitKeluar);
            printf("Total Biaya : Rp%d\n", arrKendaraan[i].biaya);
            
            ditemukan = 1;
            break;
        }
    }
    
    if (!ditemukan) {
        printf("Plat tidak ditemukan atau kendaraan sudah diproses keluar sebelumnya.\n");
    }
}

// 6. Fungsi Tampilkan Riwayat Data
void tampilkanData(struct Kendaraan *arrKendaraan, int jumlah) {
    printf("\n--- Data Parkir Kendaraan ---\n");
    if (jumlah == 0) {
        printf("Belum ada riwayat data.\n");
        return;
    }
    for (int i = 0; i < jumlah; i++) {
        printf("%d. %s | %s | Masuk: %02d:%02d | ", 
               i+1, arrKendaraan[i].plat, arrKendaraan[i].jenis, 
               arrKendaraan[i].jamMasuk, arrKendaraan[i].menitMasuk);

        if (arrKendaraan[i].status == 1) {
            printf("Status: Masih Parkir\n");
        } else {
            printf("Keluar: %02d:%02d | Biaya: Rp%d\n", 
                   arrKendaraan[i].jamKeluar, arrKendaraan[i].menitKeluar, arrKendaraan[i].biaya);
        }
    }
}

// 7. Fungsi Sorting (Bubble Sort berdasarkan Plat)
void urutkanData(struct Kendaraan *arrKendaraan, int jumlah) {
    for (int i = 0; i < jumlah - 1; i++) {
        for (int j = 0; j < jumlah - i - 1; j++) {
            if (strcmp(arrKendaraan[j].plat, arrKendaraan[j+1].plat) > 0) {
                // Swap posisi struct
                struct Kendaraan temp = arrKendaraan[j];
                arrKendaraan[j] = arrKendaraan[j+1];
                arrKendaraan[j+1] = temp;
            }
        }
    }
    printf("\nData riwayat berhasil diurutkan berdasarkan Plat Nomor (A-Z).\n");
}

// 8. Fungsi Searching (Linear Search)
void cariData(struct Kendaraan *arrKendaraan, int jumlah, char cariPlat[]) {
    int ditemukan = 0;
    printf("\n--- Hasil Pencarian ---\n");
    for (int i = 0; i < jumlah; i++) {
        if (strcmp(arrKendaraan[i].plat, cariPlat) == 0) {
            printf("Plat: %s | Jenis: %s | Masuk: %02d:%02d ", 
                   arrKendaraan[i].plat, arrKendaraan[i].jenis, 
                   arrKendaraan[i].jamMasuk, arrKendaraan[i].menitMasuk);
            
            if (arrKendaraan[i].status == 1) {
                printf("| Status: Masih Parkir\n");
            } else {
                printf("| Keluar: %02d:%02d | Biaya: Rp%d\n", 
                       arrKendaraan[i].jamKeluar, arrKendaraan[i].menitKeluar, arrKendaraan[i].biaya);
            }
            ditemukan = 1;
            break; // Hentikan pencarian setelah ketemu
        }
    }
    if (!ditemukan) {
        printf("Kendaraan dengan plat %s tidak ditemukan dalam sistem.\n", cariPlat);
    }
}

int main() {
    struct Kendaraan dataParkir[100]; // Kapasitas penyimpanan data parkir
    int jumlahKendaraan = 0;          
    int pilihan;
    char cariPlat[50];

    do {
        printf("\n=== SISTEM PARKIR KENDARAAN ===\n");
        printf("1. Kendaraan Masuk\n");
        printf("2. Kendaraan Keluar (Hitung Biaya)\n");
        printf("3. Tampilkan Riwayat Data\n");
        printf("4. Urutkan Data (Plat Nomor)\n");
        printf("5. Cari Data Kendaraan\n");
        printf("6. Keluar\n");
        printf("Pilih menu: "); 
        scanf("%d", &pilihan);

        switch(pilihan) {
            case 1: 
                inputMasuk(dataParkir, &jumlahKendaraan); 
                break;
            case 2: 
                prosesKeluar(dataParkir, jumlahKendaraan);
                break;
            case 3: 
                tampilkanData(dataParkir, jumlahKendaraan); 
                break;
            case 4: 
                urutkanData(dataParkir, jumlahKendaraan); 
                break;
            case 5: 
                printf("Masukkan Plat Nomor yang dicari: "); 
                scanf(" %[^\n]", cariPlat);
                cariData(dataParkir, jumlahKendaraan, cariPlat); 
                break;
            case 6: 
                printf("Sistem dimatikan. Terima kasih!\n"); 
                break;
            default: 
                printf("Pilihan tidak valid!\n");
        }
    } while(pilihan != 6);

    return 0;
}